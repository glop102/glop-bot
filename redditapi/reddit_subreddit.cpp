#include "reddit_subreddit.h"

Reddit_Subreddit::Reddit_Subreddit(RedditSession *session, QString subreddit){
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
//Reddit_Subreddit::Reddit_Subreddit(const Reddit_Subreddit &other){
//	*this = other;
//}
//Reddit_Subreddit& Reddit_Subreddit::operator=(const Reddit_Subreddit& other){
//	session = other.session;
//	subreddit = other.subreddit;
//	return *this;
//}

QList<Reddit_Post> Reddit_Subreddit::getPosts(int limit, QString after, QString before){
	QString url = "https://oauth.reddit.com/r/" + subreddit;
	QString post_data = "";
	if(limit<=0 && limit>100)
		limit = 100; // fix limits that make no sense
	post_data+= "limit="+QString::number(limit);
	if(after != "")
		after.indexOf("t3_")==0       ?
		post_data+="&after="+after    :
		post_data+="&after=t3_"+after ; // attach t3_ if we are just given the id
	if(before != "")
		before.indexOf("t3_")==0        ?
		post_data+="&before="+before    :
		post_data+="&before=t3_"+before ;

	//printf("%s\n",(url+"?"+post_data).toUtf8().data());
	QNetworkReply *reply = session->makeGETrequest(url+"?"+post_data);
	reply->deleteLater();
	QJsonDocument top = QJsonDocument::fromJson(reply->readAll());
	if(! top.isObject())
		return QList<Reddit_Post>(); //error, so return empty

	QJsonArray arr = top.object()["data"].toObject()["children"].toArray();
	QList<Reddit_Post> posts;
	for(auto x : arr){
		if(! x.isObject())
			continue; //unknown thing, can't handle
		posts.append(Reddit_Post(x.toObject()["data"].toObject(),session));
		//Reddit_Post::debug_parse(x.toObject()["data"].toObject());
		//printf("\n\n");
	}
	return posts;
}

void Reddit_Subreddit::cacheSubredditStats(){
	auto reply = session->makeGETrequest("https://oauth.reddit.com/r/"+subreddit+"/about");
	reply->deleteLater();
	QJsonDocument top = QJsonDocument::fromJson(reply->readAll());
	if(! top.isObject()){
		printf("can't read subreddit stats - not an object\n");
		return;
	}
	QJsonObject raw = top.object()["data"].toObject();
	accounts_active = raw["accounts_active"].toInt();
	accounts_active_is_fuzzed = raw["accounts_active_is_fuzzed"].toBool();
	active_user_count = raw["active_user_count"].toInt();
	advertiser_category = raw["advertiser_category"].toString();
	allow_discovery = raw["allow_discovery"].toBool();
	allow_images = raw["allow_images"].toBool();
	allow_videogifs = raw["allow_videogifs"].toBool();
	allow_videos = raw["allow_videos"].toBool();
	audience_target = raw["audience_target"].toString();
	collapse_deleted_comments = raw["collapse_deleted_comments"].toBool();
	comment_score_hide_mins = raw["comment_score_hide_mins"].toInt();
	created = raw["created"].toDouble();
	created_utc = raw["created_utc"].toDouble();
	description = raw["description"].toString();
	description_html = raw["description_html"].toString();
	display_name = raw["display_name"].toString();
	display_name_prefixed = raw["display_name_prefixed"].toString();
	hide_ads = raw["hide_ads"].toBool();
	id = raw["id"].toString();
	name = raw["name"].toString();
	key_color = raw["key_color"].toString();
	lang = raw["lang"].toString();
	link_flair_enabled = raw["link_flair_enabled"].toBool();
	notification_level = raw["notification_level"].toString();
	over18 = raw["over18"].toBool();
	public_description = raw["public_description"].toString();
	public_description_html = raw["public_description_html"].toString();
	public_traffic = raw["public_traffic"].toBool();
	quarantine = raw["quarantine"].toBool();
	show_media = raw["show_media"].toBool();
	show_media_preview = raw["show_media_preview"].toBool();
	spoilers_enabled = raw["spoilers_enabled"].toBool();
	submission_type = raw["submission_type"].toString();
	submit_text = raw["submit_text"].toString();
	submit_text_html = raw["submit_text_html"].toString();
	subreddit_type = raw["subreddit_type"].toString();
	subscribers = raw["subscribers"].toInt();
	title = raw["title"].toString();
	url = raw["url"].toString();
	user_can_flair_in_sr = raw["user_can_flair_in_sr"].toBool();
	user_flair_enabled_in_sr = raw["user_flair_enabled_in_sr"].toBool();
	user_has_favorited = raw["user_has_favorited"].toBool();
	user_is_banned = raw["user_is_banned"].toBool();
	user_is_contributor = raw["user_is_contributor"].toBool();
	user_is_moderator = raw["user_is_moderator"].toBool();
	user_is_muted = raw["user_is_muted"].toBool();
	user_is_subscriber = raw["user_is_subscriber"].toBool();
	user_sr_flair_enabled = raw["user_sr_flair_enabled"].toBool();
	user_sr_theme_enabled = raw["user_sr_theme_enabled"].toBool();
	videostream_links_count = raw["videostream_links_count"].toInt();
	whitelist_status = raw["whitelist_status"].toString();
	wiki_enabled = raw["wiki_enabled"].toBool();

	QJsonArray arr;
	banner_img = raw["banner_img"].toString();
	if(raw["banner_size"].isArray()){
		arr = raw["banner_size"].toArray();
		banner_width = arr[1].toDouble();
		banner_height = arr[0].toDouble();
	}
	header_img = raw["header_img"].toString();
	header_title = raw["header_title"].toString();
	if(raw["header_size"].isArray()){
		arr = raw["header_size"].toArray();
		header_width = arr[1].toDouble();
		header_height = arr[0].toDouble();
	}
	icon_img = raw["icon_img"].toString();
	if(raw["icon_size"].isArray()){
		arr = raw["icon_size"].toArray();
		icon_width = arr[1].toDouble();
		icon_height = arr[0].toDouble(); // icon_size comes as 2 index array height x width
	}
}

void Reddit_Subreddit::debug_parse(const QJsonObject &raw, QString pad){
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
void Reddit_Subreddit::debug_parse(const QJsonArray &arr, QString pad){
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
