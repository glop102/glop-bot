#include "reddit_session.h"

const QString RedditSession::clientID = "f3dZ8gIetXY69Q";
const QString RedditSession::UserAgent = "pc:glop-bot:v0.0 (by /u/glop102)";
const QString RedditSession::redirectUri = "http://www.reddit.com";
//QString RedditSession::API_Token=""; //identifies that the user authorised our program
//QString RedditSession::API_Session=""; //temperary session key
//QTimer *RedditSession::Session_Time_Used = new QTimer();
QNetworkAccessManager *RedditSession::networkManager = NULL;

bool RedditSession::parseSettings(bool allow_example_firstRun){
	GlopConfig::Settings set = GlopConfig::ParseFile((QDir::homePath()+"/.redditSettings.conf") .toStdString() );
	Session_Time_Used.start();
	RequestLimit_TimeElapsed.start();
	RequestLimit_RequestsLeft = 60; // mediocre default that will be fixed on the first API use
	RequestLimit_TimeToReset = 10;

	API_Token = QString::fromStdString( set.getValueAsString("token","") );
	if(API_Token == ""){
		if(allow_example_firstRun)
			firstTimeRun_example();
		else
			printf("No Token Found - You need to do the first run code\n");
	}else{
		refreshSession();
	}
	//printf("Session token : %s\n",API_Session.toStdString().c_str());

	//testFunction();
	if(API_Session=="")
		return false;
	else
		return true;
}

void RedditSession::saveSettings(){
	GlopConfig::Settings set;
	set.values["token"] = API_Token.toStdString();
	GlopConfig::SaveToFile((QDir::homePath()+"/.redditSettings.conf") .toStdString() , set);
}

void RedditSession::firstTimeRun_example(){
	//this is an intereactive thing that runs when the settings don't seem to be all there
	// https://github.com/reddit/reddit/wiki/OAuth2
	std::string URL = firstTimeRun_authorizeURL("submit,read,history,edit,identity,privatemessages").toStdString();

	printf("This is a first time setup\n");
	printf("You need to visit the URL below and then authorize this program\n");
	printf("%s\n",URL.c_str());
	printf("\nAfter you authorize the program, it will send you to the reddit homepage\n");
	printf("\nCopy and paste the URL of the homepage to here\n");
	std::getline(std::cin,URL);
	printf("retrieving access token...\n");

	firstTimeRun_redirectedURL(QString(URL.c_str()));
}
QString RedditSession::firstTimeRun_authorizeURL(QString wanted_scopes){
	QString responseType = "code";
	QString state = "should_be_random";
	QString duration = "permanent";
	//std::string scope = "submit,read,history,edit,identity,privatemessages";

	QString URL = QString("https://www.reddit.com/api/v1/authorize?")
				  +"client_id="+clientID
				  +"&response_type="+responseType
				  +"&state="+state
				  +"&redirect_uri="+redirectUri
				  +"&duration="+duration
				  +"&scope="+wanted_scopes;
	return URL;

}
void RedditSession::firstTimeRun_redirectedURL(QString URL){
	if(URL.indexOf("error")>=0 || URL.indexOf("code=")==-1){
		printf("There was some sort of issue with the URL, try again\n");
		exit(1);
	}
	QString code;
	code = URL.right(URL.size() - URL.indexOf("code=") - QString("code=").size() ); // clean everything left of the token
	code = code.left(code.indexOf("&")); // clean everything to the right of the token
	printf("one-time code : %s\n",code.toStdString().c_str());


	QNetworkReply *reply =
	makePOSTrequest("https://www.reddit.com/api/v1/access_token",
					QString(("grant_type=authorization_code&code="+code.toStdString()+"&redirect_uri="+redirectUri.toStdString()).c_str())
					);
	reply->deleteLater();

	//temp = QString::fromUtf8(reply->readAll());
	//printf("reply : \n%s\n",temp.toStdString().c_str());
	//QJsonDocument top = QJsonDocument::fromJson(temp.toUtf8());
	QJsonDocument top = QJsonDocument::fromJson(reply->readAll());
	if(top.isObject()){
		QJsonObject obj = top.object();
		API_Session = obj["access_token"].toString();
		API_Token = obj["refresh_token"].toString();
		saveSettings();
	}else{
		printf("Error: response from server was not valid json\n");
		exit(0);
	}
}

QNetworkReply* RedditSession::makePOSTrequest(QString URL, QString postData){
	QNetworkRequest req;
	commonRequestSetup(req,URL);
	req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
	QNetworkReply *reply = networkManager->post(req,postData.toUtf8());
	commonReplyWait(reply);
	return reply;
}

QNetworkReply* RedditSession::makeGETrequest(QString URL){
	QNetworkRequest req;
	commonRequestSetup(req,URL);
	QNetworkReply *reply = networkManager->get(req);
	commonReplyWait(reply);
	return reply;
}
void RedditSession::commonRequestSetup(QNetworkRequest &req,QString URL){
	if(networkManager == NULL){
		networkManager = new QNetworkAccessManager(this);
		connect( networkManager,SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)) ,this,SLOT(sendAuthentication(QNetworkReply*,QAuthenticator*)) );
	}
	if(Session_Time_Used.elapsed() > 3570*1000) //in milliseconds -  30 seconds less than every hour, just to make sure we are always valid
		refreshSession();
	if(RequestLimit_RequestsLeft <= 0)
		waitForRequestLimitReset();
	req.setUrl(QUrl(URL));
	req.setHeader(QNetworkRequest::UserAgentHeader,UserAgent);
	if(! (API_Session == ""))
		//req.setRawHeader("Authorization", ("bearer "+API_Session).toUtf8());
		req.setRawHeader("Authorization", "bearer "+API_Session.toLocal8Bit());
}
void RedditSession::commonReplyWait(QNetworkReply *reply){
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if(reply->hasRawHeader("x-ratelimit-remaining")){
		RequestLimit_RequestsLeft = (int)reply->rawHeader("x-ratelimit-remaining").toFloat(); // oddly toInt hates when there is a period
		RequestLimit_TimeToReset = reply->rawHeader("x-ratelimit-reset").toInt();
		RequestLimit_TimeElapsed.restart();
		//printf("found header\n");
		//printf("left %s\n",reply->rawHeader("x-ratelimit-remaining").data());
		//printf("time %s\n",reply->rawHeader("x-ratelimit-reset").data());
		//printf("left %d time %d\n",RequestLimit_RequestsLeft,RequestLimit_TimeToReset);
	}
}
void RedditSession::waitForRequestLimitReset(){
	printf("Exahsted requests for the time period, now waiting for %d seconds\n",RequestLimit_TimeToReset);
	int delta = (RequestLimit_TimeToReset*1000) - RequestLimit_TimeElapsed.elapsed();
	QEventLoop loop;
	QTimer timeLeft;
	connect(&timeLeft, SIGNAL(timeout()), &loop, SLOT(quit()));
	timeLeft.start(delta);
	loop.exec();
}

void RedditSession::sendAuthentication(QNetworkReply *reply, QAuthenticator *authenticator){
	if(! (API_Session == "")){ // should never get called - is hard set in the commonRequestSetup
		authenticator->setUser("bearer");
		authenticator->setPassword(API_Session);
		printf("authenticated with session token\n");
	}else{ // only should happen on first API call while we are trying to get the session token
		authenticator->setUser(clientID);
		authenticator->setPassword("");
		printf("authenticated with client ID\n");
	}
}

void RedditSession::refreshSession(){
	printf("Getting new session token\n");
	QString req = "grant_type=refresh_token&refresh_token=" + API_Token;
	QNetworkReply* reply = makePOSTrequest("https://www.reddit.com/api/v1/access_token",req);

	QJsonDocument top = QJsonDocument::fromJson(reply->readAll());
	if(!top.isObject()){
		printf("Error: response from server was not valid json, unable to refresh session token\n");
		exit(0);
	}
	QJsonObject obj = top.object();
	API_Session = obj["access_token"].toString();

	Session_Time_Used.restart();
	//printf("\n%s\n\n",top.toJson().data());
	reply->deleteLater();
}

void RedditSession::testFunction(){
	//example json from the server when first authenticating the program to the user
	//{"access_token": "VnQBpiuRpH_otGfEqD30j7g_hZQ", "token_type": "bearer", "expires_in": 3600, "refresh_token": "65960967426-zaRlzkPzLgxbAf-0Vjw24O1H9sA", "scope": "edit history read submit"}
//	QByteArray input = "{\"access_token\": \"VnQBpiuRpH_otGfEqD30j7g_hZQ\", \"token_type\": \"bearer\", \"expires_in\": 3600, \"refresh_token\": \"65960967426-zaRlzkPzLgxbAf-0Vjw24O1H9sA\", \"scope\": \"edit history read submit\"}";
//	QJsonDocument top = QJsonDocument::fromJson(input);
//	if(top.isObject());
//		printf("is object\n");
//	QJsonObject obj=top.object();
//	printf("Session %s\n",obj["access_token"].toString().toStdString().c_str());
//	printf("Refresh %s\n",obj["refresh_token"].toString().toStdString().c_str());

	//------------------------------------------------------------------------------
	//a simple test for if the session token we get allows us to access things correctly
	//auto reply = makeGETrequest("https://oauth.reddit.com/api/v1/me");
	//QString temp = QString::fromUtf8(reply->readAll());
	//printf("person: %s\n",temp.toLatin1().data());

	//------------------------------------------------------------------------------
	// playing with reading data from a subreddit and what it returns
	auto reply = makeGETrequest("https://oauth.reddit.com/r/bottest");
	reply->deleteLater();
	QString temp = QString::fromUtf8(reply->readAll());
	printf("subreddit data:\n%s\n",temp.toLatin1().data());

	//printf("headers:\n");
	//for(auto x : reply->rawHeaderPairs()){
	//	printf("\t%s : %s\n",x.first.data(),x.second.data());
	//}

	//------------------------------------------------------------------------------
	//abuse test to test how well the "wait for request limit reset" functionality works to prevent accidental abuse
	//while(true){
	//	makeGETrequest("https://oauth.reddit.com/r/bottest");
	//	printf("%d reqs in %d seconds\n",RequestLimit_RequestsLeft,RequestLimit_TimeToReset);
	//}
}

QString RedditSession::scopesDescriptions(){
	return "\
creddits:         Spend my reddit gold creddits on giving gold to other users.\
modcontributors:  Add/remove users to approved submitter lists and ban/unban or mute/unmute users from subreddits I moderate.\
modmail:          Access and manage modmail via mod.reddit.com.\
modconfig:        Manage the configuration, sidebar, and CSS of subreddits I moderate.\
subscribe:        Manage my subreddit subscriptions. Manage 'friends' - users whose content I follow.\
structuredstyles: Edit structured styles for a subreddit I moderate.\
vote:             Submit and change my votes on comments and submissions.\
wikiedit:         Edit wiki pages on my behalf\
mysubreddits:     Access the list of subreddits I moderate, contribute to, and subscribe to.\
submit:           Submit links and comments from my account.\
modlog:           Access the moderation log in subreddits I moderate.\
modposts:         Approve, remove, mark nsfw, and distinguish content in subreddits I moderate.\
modflair:         Manage and assign flair in subreddits I moderate.\
save:             Save and unsave comments and submissions.\
modothers:        Invite or remove other moderators from subreddits I moderate.\
read:             Access posts and comments through my account.\
privatemessages:  Access my inbox and send private messages to other users.\
report:           Report content for rules violations. Hide & show individual submissions.\
identity:         Access my reddit username and signup date.\
livemanage:       Manage settings and contributors of live threads I contribute to.\
account:          Update preferences and related account information. Will not have access to your email or password.\
modtraffic:       Access traffic stats in subreddits I moderate.\
wikiread:         Read wiki pages through my account\
edit:             Edit and delete my comments and submissions.\
modwiki:          Change editors and visibility of wiki pages in subreddits I moderate.\
modself:          Accept invitations to moderate a subreddit. Remove myself as a moderator or contributor of subreddits I moderate or contribute to.\
history:          Access my voting history and comments or submissions I've saved or hidden.\
flair:            Select my subreddit flair. Change link flair on my submissions.";
}
