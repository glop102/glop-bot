#include "reddit_post.h"

Reddit_Post::Reddit_Post(RedditSession *sess){
	session	= sess;
}
Reddit_Post::Reddit_Post(const QString &url, RedditSession *sess){
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
	parseJson(top.array()[0].toObject()["data"].toObject()["children"].toArray()[0].toObject()["data"].toObject());
}
Reddit_Post::Reddit_Post(const QJsonObject &raw,RedditSession* sess){
	parseJson(raw);
	session = sess;
}
QString Reddit_Post::fixUrl(QString url){
	url.replace("&amp;","&");
	url.replace("&lt;","<");
	url.replace("&gt;",">");
	return url;
}
QList<Reddit_Comment> Reddit_Post::getComments(){
	if(session == NULL){
		printf("Tried to get comments when object has no session to use\n");
		return QList<Reddit_Comment>();
	}
	auto reply = session->makeGETrequest("https://oauth.reddit.com/"+permalink);
	auto raw = reply->readAll();
	//printf("%s\n",raw.data());
	auto top = QJsonDocument::fromJson(raw);
	QJsonArray comments = top.array()[1].toObject()["data"].toObject()["children"].toArray();
	QList<Reddit_Comment> comms;
	printf("number of comments : %d\n",comments.size());
	for(auto c : comments){
		QJsonObject child = c.toObject();
		comms.push_back(Reddit_Comment(child["data"].toObject(),session));
	}

	return comms;
}

//=================================================================================================
// Line of Sanity
// Below here is very long and repetitive stuff that is just straight ugly
// Also, you can probably assume that below is correct unless reddit changes their API
//=================================================================================================

void Reddit_Post::parseJson(const QJsonObject &raw){
	//yes, I hand typed all of this
	//yes, it is very repetitive
	approved_at_utc = raw["approved_at_utc"].toBool();
	approved_by = raw["approved_by"].toString();
	archived = raw["archived"].toBool();
	author = raw["author"].toString();
	author_flair_css_class = raw["author_flair_css_class"].toString();
	author_flair_text = raw["author_flair_text"].toString();
	banned_at_utc = raw["banned_at_utc"].toDouble();
	banned_by = raw["banned_by"].toString();
	can_gild = raw["can_gild"].toBool();
	can_mod_post = raw["can_mod_post"].toBool();
	clicked = raw["clicked"].toBool();
	created_time = raw["created_time"].toDouble(); // seconds
	created_time_utc = raw["created_time_utc"].toDouble();
	domain = raw["domain"].toString();
	edited = raw["edited"].toBool();
	gilded = raw["gilded"].toInt();
	hidden = raw["hidden"].toBool();
	hide_score = raw["hide_score"].toBool();
	id = raw["id"].toString();
	is_crosspostable = raw["is_crosspostable"].toBool();
	is_reddit_media_domain = raw["is_reddit_media_domain"].toBool();
	is_self = raw["is_self"].toBool();
	is_video = raw["is_video"].toBool();
	link_flair_css_class = raw["link_flair_css_class"].toString();
	link_flair_text = raw["link_flair_text"].toString();
	locked = raw["locked"].toBool();
	mod_note = raw["mod_note"].toString();
	mod_reason_by = raw["mod_reason_by"].toString();
	mod_reason_title = raw["mod_reason_title"].toString();
	name = raw["name"].toString();
	over_18 = raw["over_18"].toBool();
	permalink = raw["permalink"].toString();
	pinned = raw["pinned"].toBool();
	post_hint = raw["post_hint"].toString();
	quarantine = raw["pinned"].toBool();
	removal_reason = raw["removal_reason"].toString();
	saved = raw["saved"].toBool();
	selftext = raw["selftext"].toString();
	selftext_html = raw["selftext_html"].toString();
	spoiler = raw["spoiler"].toBool();
	stickied = raw["stickied"].toBool();
	subreddit = raw["subreddit"].toString();
	subreddit_id = raw["subreddit_id"].toString();
	subreddit_name_prefixed = raw["subreddit_name_prefixed"].toString();
	subreddit_type = raw["subreddit_type"].toString();
	thumbnail = raw["thumbnail"].toString();
	thumbnail_height = raw["thumbnail_height"].toDouble();
	thumbnail_width = raw["thumbnail_width"].toDouble();
	title = raw["title"].toString();
	url = raw["url"].toString();
	visited = raw["visited"].toBool();
	whitelist_status = raw["whitelist_status"].toString();

	down_votes = raw["downs"].toInt(); //downs
	up_votes = raw["ups"].toInt(); //ups
	score = raw["score"].toInt();
	num_comments = raw["num_comments"].toInt();
	num_crossposts = raw["num_crossposts"].toInt();
	num_reports = raw["num_reports"].toInt();
	view_count = raw["view_count"].toInt();

	//preview_images
	//preview_images_blurred
	parseJson_previewImages(raw);
	parseJson_media(raw);

	if(raw["likes"].isNull())
		user_vote="";
	else
		raw["likes"].toBool() ? user_vote="up" : user_vote="down";
}
void Reddit_Post::parseJson_previewImages(const QJsonObject &raw){
	if(raw["preview"].isObject()){
		for(auto i : raw["preview"].toObject()["images"].toArray()){
			QJsonObject im = i.toObject();
			preview_images.push_back({});
			for(auto res : im["resolutions"].toArray()){
				QJsonObject ress = res.toObject();
				preview_images.last().urls.push_back(fixUrl(ress["url"].toString()));
				preview_images.last().height.push_back(ress["height"].toDouble());
				preview_images.last().width.push_back(ress["width"].toDouble());
			}
			QJsonObject ress = im["source"].toObject();
			preview_images.last().urls.push_back(fixUrl(ress["url"].toString()));
			preview_images.last().height.push_back(ress["height"].toDouble());
			preview_images.last().width.push_back(ress["width"].toDouble());

			if(im.contains("variants") && im["variants"].toObject().contains("obfuscated")){
				QJsonObject obf = im["variants"].toObject()["obfuscated"].toObject();
				preview_images_blurred.push_back({});
				for(auto res : obf["resolutions"].toArray()){
					QJsonObject ress = res.toObject();
					preview_images_blurred.last().urls.push_back(fixUrl(ress["url"].toString()));
					preview_images_blurred.last().height.push_back(ress["height"].toDouble());
					preview_images_blurred.last().width.push_back(ress["width"].toDouble());
				}
				QJsonObject ress = obf["source"].toObject();
				preview_images_blurred.last().urls.push_back(fixUrl(ress["url"].toString()));
				preview_images_blurred.last().height.push_back(ress["height"].toDouble());
				preview_images_blurred.last().width.push_back(ress["width"].toDouble());
			}
		}
	}
}
void Reddit_Post::parseJson_media(const QJsonObject &raw){
	if(raw["secure_media"].isObject() || raw["media"].isObject()){
		QJsonObject S;
		raw["secure_media"].isObject()         ?
			S = raw["secure_media"].toObject() :
			S = raw["media"].toObject()		   ;
		if(S.contains("oembed")){
			media.type = S["type"].toString();
			QJsonObject oembed = S["oembed"].toObject();
			media.common.author_name = oembed["author_name"].toString();
			media.common.author_url = oembed["author_url"].toString();
			media.common.description = oembed["description"].toString();
			media.common.height = oembed["height"].toDouble();
			media.common.width = oembed["width"].toDouble();
			media.common.html = oembed["html"].toString();
			media.common.provider_name = oembed["provider_name"].toString();
			media.common.provider_url = oembed["provider_url"].toString();
			media.common.thumbnail_url = oembed["thumbnail_url"].toString();
			media.common.thumbnail_height = oembed["thumbnail_height"].toDouble();
			media.common.thumbnail_width = oembed["thumbnail_width"].toDouble();
			media.common.title = oembed["title"].toString();
			media.common.type = oembed["type"].toString();
			media.common.url = oembed["url"].toString();
			media.common.version = oembed["version"].toString();

			QJsonObject med_embed;
			raw["secure_media"].isObject()                       ?
				med_embed = raw["secure_media_embed"].toObject() :
				med_embed = raw["media_embed"].toObject()		 ;
			media.common.content = med_embed["content"].toString();
			media.common.media_domain_url = med_embed["media_domain_url"].toString();
			media.common.scrolling = med_embed["scrolling"].toBool();
		}
		else if(S.contains("reddit_video")){
			media.type="reddit_video";
			QJsonObject red_vid = S["reddit_video"].toObject();
			media.reddit_video.dash_url = red_vid["dash_url"].toString();
			media.reddit_video.fallback_url = red_vid["fallback_url"].toString();
			media.reddit_video.duration = red_vid["duration"].toDouble();
			media.reddit_video.height = red_vid["height"].toDouble();
			media.reddit_video.width = red_vid["width"].toDouble();
			media.reddit_video.hls_url = red_vid["hls_url"].toString();
			media.reddit_video.scrubber_media_url = red_vid["scrubber_media_url"].toString();
			media.reddit_video.is_gif = red_vid["is_gif"].toBool();
			media.reddit_video.transcoding_status = red_vid["transcoding_status"].toString();
		}
		else{
			if(raw["secure_media"].isObject())
				printf("unknown secure_media object\n");
			else
				printf("unknown media object\n");
			debug_parse(S,"\t");
		}
	}
}

void Reddit_Post::debug_print(){
	printf("title = %s\n",title.toUtf8().data());
	printf("self text = \n%s\n",selftext.toUtf8().data());
	//printf("\tselftext html = %s\n",selftext_html.toUtf8().data()); // just a verbose version of the above
	printf("\tapproved time = %f\n",approved_at_utc);
	printf("\tapproved by = %s\n",approved_by.toUtf8().data());
	printf("\tarchived = %s\n",archived?"true":"false");
	printf("\tauthor = %s\n",author.toUtf8().data());
	printf("\tauthor flair = %s\n",author_flair_text.toUtf8().data());
	printf("\tbanned time = %f\n",banned_at_utc);
	printf("\tbanned by = %s\n",banned_by.toUtf8().data());
	printf("\tcan gild = %s\n",can_gild?"true":"false");
	printf("\tcan mod post = %s\n",can_mod_post?"true":"false");
	printf("\tclicked = %s\n",clicked?"true":"false");
	printf("\tcreated time = %f\n",created_time);
	printf("\tcreated time utc = %f\n",created_time_utc);
	printf("\tdomain = %s\n",domain.toUtf8().data());
	printf("\tedited = %s\n",edited?"true":"false");
	printf("\tgilded = %d\n",gilded);
	printf("\thidden = %s\n",hidden?"true":"false");
	printf("\thide score = %s\n",hide_score?"true":"false");
	printf("\tid = %s\n",id.toUtf8().data());
	printf("\tcrosspostable = %s\n",is_crosspostable?"true":"false");
	printf("\treddit media = %s\n",is_reddit_media_domain?"true":"false");
	printf("\tis self = %s\n",is_self?"true":"false");
	printf("\tis video = %s\n",is_video?"true":"false");
	printf("\tlink flair = %s\n",link_flair_text.toUtf8().data());
	printf("\tlocked = %s\n",locked?"true":"false");
	printf("\tmod note = %s\n",mod_note.toUtf8().data());
	printf("\tmod by = %s\n",mod_reason_by.toUtf8().data());
	printf("\tmod title = %s\n",mod_reason_title.toUtf8().data());
	printf("\tname = %s\n",name.toUtf8().data());
	printf("\tover 18 = %s\n",over_18?"true":"false");
	printf("\tpermalink = %s\n",permalink.toUtf8().data());
	printf("\tpinned = %s\n",pinned?"true":"false");
	printf("\tpost hint = %s\n",post_hint.toUtf8().data());
	printf("\tquarantine = %s\n",quarantine?"true":"false");
	printf("\tremoval reason = %s\n",removal_reason.toUtf8().data());
	printf("\tsaved = %s\n",saved?"true":"false");
	printf("\tspoiler = %s\n",spoiler?"true":"false");
	printf("\tstickied = %s\n",stickied?"true":"false");
	printf("\tsubreddit = %s\n",subreddit.toUtf8().data());
	printf("\tsubreddit id = %s\n",subreddit_id.toUtf8().data());
	//printf("\tsubreddit name prefixed = %s\n",subreddit_name_prefixed.toUtf8().data());
	printf("\tsubreddit type = %s\n",subreddit_type.toUtf8().data());
	printf("\tthumbnail = %s\n",thumbnail.toUtf8().data());
	printf("\tthumbnail size = %f x %f\n",thumbnail_width,thumbnail_height);
	printf("\turl = %s\n",url.toUtf8().data());
	printf("\tuser vote = %s\n",user_vote.toUtf8().data());
	printf("\tvisited = %s\n",visited?"true":"false");
	printf("\twhitelist status = %s\n",whitelist_status.toUtf8().data());
	printf("\n");
	printf("\t%d down\n",down_votes);
	printf("\t%d up\n",up_votes);
	printf("\t%d score\n",score);
	printf("\t%d comments\n",num_comments);
	printf("\t%d crossposts\n",num_crossposts);
	printf("\t%d reports\n",num_reports);
	printf("\t%d views\n",view_count);
}
void Reddit_Post::debug_parse(const QJsonObject &raw, QString pad){
	for(QString key : raw.keys()){
		if(raw[key].isBool()){
			printf("%s%s : bool : %s\n",pad.toUtf8().data(),key.toUtf8().data(),raw[key].toBool()?"true":"false");
		}else if(raw[key].isDouble()){
			printf("%s%s : double : %f\n",pad.toUtf8().data(),key.toUtf8().data(),raw[key].toDouble());
		}else if(raw[key].isNull()){
			printf("%s%s : null : null\n",pad.toUtf8().data(),key.toUtf8().data());
		}else if(raw[key].isString()){
			QString temp = raw[key].toString().left(200);
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
void Reddit_Post::debug_parse(const QJsonArray &arr, QString pad){
	for(int x=0; x<arr.size(); x++){
		if(arr[x].isBool()){
			printf("%s%d : bool : %s\n",pad.toUtf8().data(),x,arr[x].toBool()?"true":"false");
		}else if(arr[x].isDouble()){
			printf("%s%d : double : %f\n",pad.toUtf8().data(),x,arr[x].toDouble());
		}else if(arr[x].isNull()){
			printf("%s%d : null : null\n",pad.toUtf8().data(),x);
		}else if(arr[x].isString()){
			QString temp = arr[x].toString().left(200);
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
