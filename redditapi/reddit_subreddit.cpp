#include "reddit_subreddit.h"

Reddit_Subreddit::Reddit_Subreddit(RedditSession *session, QString subreddit) : QObject(0){
	this->session = session;
	//cleaned up version of the passed subreddit name
	if(subreddit.indexOf("/r/") == 0)
		this->subreddit = subreddit.mid(3);
	else if(subreddit.indexOf("r/") == 0)
		this->subreddit = subreddit.mid(2);
	else if(subreddit.indexOf("/") == 0)
		this->subreddit = subreddit.mid(1);
	else
		this->subreddit = subreddit;
}
Reddit_Subreddit::Reddit_Subreddit(const Reddit_Subreddit &other){
	*this = other;
}
Reddit_Subreddit& Reddit_Subreddit::operator=(const Reddit_Subreddit& other){
	session = other.session;
	subreddit = other.subreddit;
	return *this;
}

QList<Reddit_Post> Reddit_Subreddit::getPosts(int limit, QString before, QString after){
	QString url = "https://oauth.reddit.com/r/" + subreddit;
	QString post_data = "";
	if(limit>0 && limit<=100)
		post_data+= "limit="+QString::number(limit);
	if(after != "")
		post_data+="&after="+after;
	if(before != "")
		post_data+="&before="+before;
	if(post_data.indexOf("&") == 0)//clean empty amperstands
		post_data = post_data.mid(1);//even though it should never really happen

	QNetworkReply *reply = session->makeGETrequest(url+"?"+post_data);
	QJsonDocument top = QJsonDocument::fromJson(reply->readAll());
	if(! top.isObject())
		return QList<Reddit_Post>(); //error, so return empty

	QJsonArray arr = top.object()["data"].toObject()["children"].toArray();
	QList<Reddit_Post> posts;
	for(auto x : arr){
		if(! x.isObject())
			continue; //unknown thing, can't handle
		posts.append(Reddit_Post(x.toObject()["data"].toObject()));
	}
	return posts;
}
