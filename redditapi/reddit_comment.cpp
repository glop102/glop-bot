#include "reddit_comment.h"

Reddit_Comment::Reddit_Comment(RedditSession *sess){
	session	= sess;
}
Reddit_Comment::Reddit_Comment(const QString &url, RedditSession *sess){
	session = sess;
	QString scrubbed_url = url;
	if(url.indexOf("http")!=0){
		scrubbed_url = "https://oauth.reddit.com/";
	}
	auto reply = session->makeGETrequest(scrubbed_url);
	reply->deleteLater();
	QJsonDocument top = QJsonDocument::fromJson(reply->readAll());
	if(! top.isArray()){
		printf("url seemes to not be correct\n");
		return;
	}
	parseJson(top.array()[1].toObject()["data"].toObject()["children"].toArray()[0].toObject()["data"].toObject());
}
Reddit_Comment::Reddit_Comment(const QJsonObject &raw,RedditSession* sess){
	session = sess;
	parseJson(raw);
}

void Reddit_Comment::parseJson(const QJsonObject &raw){
	approved_at_utc = raw["approved_at_utc"].toDouble();
	approved_by = raw["approved_by"].toString();
	archived = raw["archived"].toBool();
	author = raw["author"].toString();
	author_flair_css_class = raw["author_flair_css_class"].toString();
	author_flair_text = raw["author_flair_text"].toString();
	banned_at_utc = raw["banned_at_utc"].toDouble();
	banned_by = raw["banned_by"].toString();
	body = raw["body"].toString();
	body_html = raw["body_html"].toString();
	can_gild = raw["can_gild"].toBool();
	can_mod_post = raw["can_mod_post"].toBool();
	collapsed = raw["collapsed"].toBool();
	collapsed_reason = raw["collapsed_reason"].toString();
	controversiality = raw["controversiality"].toDouble();
	created = raw["created"].toDouble();
	created_utc = raw["created_utc"].toDouble();
	depth = raw["depth"].toDouble();
	edited = raw["edited"].toBool();
	gilded = raw["gilded"].toDouble();
	id = raw["id"].toString();
	is_submitter = raw["is_submitter"].toBool();
	link_id = raw["link_id"].toString();
	mod_note = raw["mod_note"].toString();
	mod_reason_by = raw["mod_reason_by"].toString();
	mod_reason_title = raw["mod_reason_title"].toString();
	name = raw["name"].toString();
	num_reports = raw["num_reports"].toInt();
	parent_id = raw["parent_id"].toString();
	permalink = raw["permalink"].toString();
	removal_reason = raw["removal_reason"].toString();
	saved = raw["saved"].toBool();
	score_hidden = raw["score_hidden"].toBool();
	stickied = raw["stickied"].toBool();
	subreddit = raw["subreddit"].toString();
	subreddit_id = raw["subreddit_id"].toString();
	subreddit_name_prefixed = raw["subreddit_name_prefixed"].toString();
	subreddit_type = raw["subreddit_type"].toString();

	down_votes = raw["downs"].toInt();
	up_votes = raw["ups"].toInt();
	score = raw["score"].toInt();

	if(raw["likes"].isNull())
		user_vote="";
	else
		raw["likes"].toBool() ? user_vote="up" : user_vote="down";

	QJsonArray child_replies = raw["replies"].toObject()["data"].toObject()["children"].toArray();
	for(auto reply : child_replies){
		QJsonObject json = reply.toObject()["data"].toObject();
		replies.push_back(Reddit_Comment(json,session));
	}
}

void Reddit_Comment::debug_parse(const QJsonObject &raw, QString pad){
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
void Reddit_Comment::debug_parse(const QJsonArray &arr, QString pad){
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
