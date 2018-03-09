#include "reddit_account.h"

Reddit_Account::Reddit_Account(RedditSession *sess){
	session = sess;
}

void Reddit_Account::cacheCurrentAccountInfo(){
	auto reply = session->makeGETrequest("https://oauth.reddit.com/api/v1/me");
	reply->deleteLater();
	QJsonDocument top = QJsonDocument::fromJson(reply->readAll());
	if(! top.isObject()){
		printf("Something is wrong, did not get a JSON object back\n");
		return;
	}
	parseJson(top.object());
}
void Reddit_Account::cacheUserAccountInfo(QString username){
	auto reply = session->makeGETrequest("https://oauth.reddit.com/user/"+username+"/about/");
	reply->deleteLater();
	QJsonDocument top = QJsonDocument::fromJson(reply->readAll());
	if(! top.isObject()){
		printf("Something is wrong, did not get a JSON object back\n");
		return;
	}
	//debug_parse(top.object()["data"].toObject());
	parseJson(top.object()["data"].toObject());
}

QList<Reddit_Comment> Reddit_Account::getUserComments(QString username,int limit,QString sort,QString after, QString before){
	QString options="";
	if(limit>0 && limit<=100) options+= "?limit="+QString::number(limit);
	else options += "?limit=100";
	if(sort != "") options+= "&sort="+sort;
	if(after != "") options+= "&after="+after;
	if(before != "") options+= "&before="+before;

	auto reply = session->makeGETrequest("https://oauth.reddit.com/user/"+username+"/comments/"+options);
	reply->deleteLater();

	QJsonDocument top = QJsonDocument::fromJson(reply->readAll());
	if(! top.isObject()){
		printf("Something is wrong, did not get a JSON object back\n");
		return QList<Reddit_Comment>();
	}

	QList<Reddit_Comment> coms;
	QJsonArray commms = top.object()["data"].toObject()["children"].toArray();
	for(auto comm : commms){
		if(comm.toObject()["kind"].toString() != "t1"){
			printf("Got a non-comment item :: %s\nPlease let the developer know since you are missing out on items in the list\n",comm.toObject()["kind"].toString().toUtf8().data());
			continue;
		}
		coms.push_back(Reddit_Comment(comm.toObject()["data"].toObject(),session));
	}
	return coms;
}
QList<Reddit_Post> Reddit_Account::getUserPosts(QString username,int limit,QString sort,QString after, QString before){
	QString options="";
	if(limit>0 && limit<=100) options+= "?limit="+QString::number(limit);
	else options += "?limit=100";
	if(sort != "") options+= "&sort="+sort;
	if(after != "") options+= "&after="+after;
	if(before != "") options+= "&before="+before;

	auto reply = session->makeGETrequest("https://oauth.reddit.com/user/"+username+"/submitted/"+options);
	reply->deleteLater();

	QJsonDocument top = QJsonDocument::fromJson(reply->readAll());
	if(! top.isObject()){
		printf("Something is wrong, did not get a JSON object back\n");
		return QList<Reddit_Post>();
	}

	QList<Reddit_Post> posts;
	QJsonArray pottts = top.object()["data"].toObject()["children"].toArray();
	for(auto post : pottts){
		if(post.toObject()["kind"].toString() != "t3"){
			printf("Got a non-post item :: %s\nPlease let the developer know since you are missing out on items in the list\n",post.toObject()["kind"].toString().toUtf8().data());
			continue;
		}
		posts.push_back(Reddit_Post(post.toObject()["data"].toObject(),session));
	}
	return posts;
}

QList<Reddit_Post> Reddit_Account::getUserUpvotedPosts(QString username,int limit,QString sort,QString after, QString before){
	QString options="";
	if(limit>0 && limit<=100) options+= "?limit="+QString::number(limit);
	else options += "?limit=100";
	if(sort != "") options+= "&sort="+sort;
	if(after != "") options+= "&after="+after;
	if(before != "") options+= "&before="+before;

	auto reply = session->makeGETrequest("https://oauth.reddit.com/user/"+username+"/upvoted/"+options);
	reply->deleteLater();

	QJsonDocument top = QJsonDocument::fromJson(reply->readAll());
	if(! top.isObject()){
		printf("Something is wrong, did not get a JSON object back\n");
		return QList<Reddit_Post>();
	}

	QList<Reddit_Post> posts;
	QJsonArray pottts = top.object()["data"].toObject()["children"].toArray();
	for(auto post : pottts){
		if(post.toObject()["kind"].toString() != "t3"){
			printf("Got a non-post item :: %s\nPlease let the developer know since you are missing out on items in the list\n",post.toObject()["kind"].toString().toUtf8().data());
			continue;
		}
		posts.push_back(Reddit_Post(post.toObject()["data"].toObject(),session));
	}
	return posts;
}
QList<Reddit_Post> Reddit_Account::getUserDownvotedPosts(QString username,int limit,QString sort,QString after, QString before){
	QString options="";
	if(limit>0 && limit<=100) options+= "?limit="+QString::number(limit);
	else options += "?limit=100";
	if(sort != "") options+= "&sort="+sort;
	if(after != "") options+= "&after="+after;
	if(before != "") options+= "&before="+before;

	auto reply = session->makeGETrequest("https://oauth.reddit.com/user/"+username+"/downvoted/"+options);
	reply->deleteLater();

	QJsonDocument top = QJsonDocument::fromJson(reply->readAll());
	if(! top.isObject()){
		printf("Something is wrong, did not get a JSON object back\n");
		return QList<Reddit_Post>();
	}

	QList<Reddit_Post> posts;
	QJsonArray pottts = top.object()["data"].toObject()["children"].toArray();
	for(auto post : pottts){
		if(post.toObject()["kind"].toString() != "t3"){
			printf("Got a non-post item :: %s\nPlease let the developer know since you are missing out on items in the list\n",post.toObject()["kind"].toString().toUtf8().data());
			continue;
		}
		posts.push_back(Reddit_Post(post.toObject()["data"].toObject(),session));
	}
	return posts;
}

void Reddit_Account::parseJson(const QJsonObject &raw){
	//we do not parse the "features" object because it seems to be internal stuff
	comment_karma = raw["comment_karma"].toInt();
	created = raw["created"].toDouble();
	created_utc = raw["created_utc"].toDouble();
	gold_creddits = raw["gold_creddits"].toInt();
	gold_expiration = raw["gold_expiration"].toDouble();
	has_mail = raw["has_mail"].toBool();
	has_mod_mail = raw["has_mod_mail"].toBool();
	has_subscribed = raw["has_subscribed"].toBool();
	has_verified_email = raw["has_verified_email"].toBool();
	hide_from_robots = raw["hide_from_robots"].toBool();
	icon_img = raw["icon_img"].toString();
	id = raw["id"].toString();
	in_beta = raw["in_beta"].toBool();
	inbox_count = raw["inbox_count"].toInt();
	is_employee = raw["is_employee"].toBool();
	is_gold = raw["is_gold"].toBool();
	is_mod = raw["is_mod"].toBool();
	is_sponsor = raw["is_sponsor"].toBool();
	is_suspended = raw["is_suspended"].toBool();
	link_karma = raw["link_karma"].toInt();
	name = raw["name"].toString();
	oauth_client_id = raw["oauth_client_id"].toString();
	over_18 = raw["over_18"].toBool();
	pref_geopopular = raw["pref_geopopular"].toString();
	pref_no_profanity = raw["pref_no_profanity"].toBool();
	pref_show_snoovatar = raw["pref_show_snoovatar"].toBool();
	pref_top_karma_subreddits = raw["pref_top_karma_subreddits"].toBool();
	suspension_expiration_utc = raw["suspension_expiration_utc"].toDouble();
	verified = raw["verified"].toBool();

	parseJson_subreddit(raw["subreddit"].toObject());
}
void Reddit_Account::parseJson_subreddit(const QJsonObject &raw){
	// personal_subreddit struct
	personal_subreddit.default_set = raw["default_set"].toBool();
	personal_subreddit.description = raw["description"].toString();
	personal_subreddit.public_description = raw["public_description"].toString();
	personal_subreddit.display_name = raw["display_name"].toString();
	personal_subreddit.display_name_prefixed = raw["display_name_prefixed"].toString();
	personal_subreddit.icon_color = raw["icon_color"].toString();
	personal_subreddit.is_default_banner = raw["is_default_banner"].toBool();
	personal_subreddit.is_default_icon = raw["is_default_icon"].toBool();
	personal_subreddit.key_color = raw["key_color"].toString();
	personal_subreddit.primary_color = raw["primary_color"].toString();
	personal_subreddit.link_flair_enabled = raw["link_flair_enabled"].toBool();
	personal_subreddit.name = raw["name"].toString();
	personal_subreddit.over_18 = raw["over_18"].toBool();
	personal_subreddit.show_media = raw["show_media"].toBool();
	personal_subreddit.subreddit_type = raw["subreddit_type"].toString();
	personal_subreddit.subscribers = raw["subscribers"].toInt();
	personal_subreddit.title = raw["title"].toString();
	personal_subreddit.url = raw["url"].toString();
	personal_subreddit.user_is_banned = raw["user_is_banned"].toBool();
	personal_subreddit.user_is_contributor = raw["user_is_contributor"].toBool();
	personal_subreddit.user_is_moderator = raw["user_is_moderator"].toBool();
	personal_subreddit.user_is_muted = raw["user_is_muted"].toBool();
	personal_subreddit.user_is_subscriber = raw["user_is_subscriber"].toBool();

	QJsonArray arr;
	personal_subreddit.banner_img = raw["banner_img"].toString();
	if(raw["banner_size"].isArray()){
		arr = raw["banner_size"].toArray();
		personal_subreddit.banner_width = arr[1].toDouble();
		personal_subreddit.banner_height = arr[0].toDouble();
	}
	personal_subreddit.header_img = raw["header_img"].toString();
	if(raw["header_size"].isArray()){
		arr = raw["header_size"].toArray();
		personal_subreddit.header_width = arr[1].toDouble();
		personal_subreddit.header_height = arr[0].toDouble();
	}
	personal_subreddit.icon_img = raw["icon_img"].toString();
	if(raw["icon_size"].isArray()){
		arr = raw["icon_size"].toArray();
		personal_subreddit.icon_width = arr[1].toDouble();
		personal_subreddit.icon_height = arr[0].toDouble(); // icon_size comes as 2 index array height x width
	}
}
void Reddit_Account::debug_parse(const QJsonObject &raw, QString pad){
	for(QString key : raw.keys()){
		if(raw[key].isBool()){
			printf("%s%s : bool : %s\n",pad.toUtf8().data(),key.toUtf8().data(),raw[key].toBool()?"true":"false");
		}else if(raw[key].isDouble()){
			printf("%s%s : double : %f\n",pad.toUtf8().data(),key.toUtf8().data(),raw[key].toDouble());
		}else if(raw[key].isNull()){
			printf("%s%s : null : null\n",pad.toUtf8().data(),key.toUtf8().data());
		}else if(raw[key].isString()){
			QString temp = raw[key].toString().left(200); // limit to 200 characters
			printf("%s%s : string : %s\n",pad.toUtf8().data(),key.toUtf8().data(),temp.toUtf8().data());
		}else if(raw[key].isUndefined()){
			printf("%s%s : undefined : undefined\n",pad.toUtf8().data(),key.toUtf8().data());
		}else if(raw[key].isArray()){
			printf("%s%s : array : []\n",pad.toUtf8().data(),key.toUtf8().data());
			debug_parse(raw[key].toArray(),pad+"\t");
		}else if(raw[key].isObject()){
			printf("%s%s : object : {}\n",pad.toUtf8().data(),key.toUtf8().data());
			debug_parse(raw[key].toObject(),pad+"\t");
		}
	}
}
void Reddit_Account::debug_parse(const QJsonArray &arr, QString pad){
	for(int x=0; x<arr.size(); x++){
		if(arr[x].isBool()){
			printf("%s%d : bool : %s\n",pad.toUtf8().data(),x,arr[x].toBool()?"true":"false");
		}else if(arr[x].isDouble()){
			printf("%s%d : double : %f\n",pad.toUtf8().data(),x,arr[x].toDouble());
		}else if(arr[x].isNull()){
			printf("%s%d : null : null\n",pad.toUtf8().data(),x);
		}else if(arr[x].isString()){
			QString temp = arr[x].toString().left(200); // limit to 200 characters
			printf("%s%d : string : %s\n",pad.toUtf8().data(),x,temp.toUtf8().data());
		}else if(arr[x].isUndefined()){
			printf("%s%d : undefined : undefined\n",pad.toUtf8().data(),x);
		}else if(arr[x].isArray()){
			printf("%s%d : array : []\n",pad.toUtf8().data(),x);
			debug_parse(arr[x].toArray(),pad+"\t");
		}else if(arr[x].isObject()){
			printf("%s%d : object : {}\n",pad.toUtf8().data(),x);
			debug_parse(arr[x].toObject(),pad+"\t");
		}
	}
}
