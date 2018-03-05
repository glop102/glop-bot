#include "reddit_post.h"

Reddit_Post::Reddit_Post(const QJsonObject &raw) : QObject(0){
	parseJson(raw);
}
Reddit_Post::Reddit_Post(const Reddit_Post &other){
	*this = other;
}
Reddit_Post& Reddit_Post::operator=(const Reddit_Post& other){
	approved_at_utc = other.approved_at_utc;
	approved_by = other.approved_by;
	archived = other.archived;
	author = other.author;
	author_flair_text = other.author_flair_text;
	banned_at_utc = other.banned_at_utc;
	banned_by = other.banned_by;
	can_gild = other.can_gild;
	can_mod_post = other.can_mod_post;
	clicked = other.clicked;
	created_time = other.created_time; // seconds
	created_time_utc = other.created_time_utc;
	domain = other.domain;
	edited = other.edited;
	gilded = other.gilded;
	hidden = other.hidden;
	hide_score = other.hide_score;
	id = other.id;
	is_crosspostable = other.is_crosspostable;
	is_reddit_media_domain = other.is_reddit_media_domain;
	is_self = other.is_self;
	is_video = other.is_video;
	link_flair_text = other.link_flair_text;
	locked = other.locked;
	mod_note = other.mod_note;
	mod_reason_by = other.mod_reason_by;
	mod_reason_title = other.mod_reason_title;
	name = other.name;
	over_18 = other.over_18;
	permalink = other.permalink;
	pinned = other.pinned;
	quarantine = other.quarantine;
	removal_reason = other.removal_reason;
	saved = other.saved;
	selftext = other.selftext;
	selftext_html = other.selftext_html;
	spoiler = other.spoiler;
	stickied = other.stickied;
	subreddit = other.subreddit;
	subreddit_id = other.subreddit_id;
	subreddit_name_prefixed = other.subreddit_name_prefixed;
	subreddit_type = other.subreddit_type;
	thumbnail = other.thumbnail;
	thumbnail_height = other.thumbnail_height;
	thumbnail_width = other.thumbnail_width;
	title = other.title;
	url = other.url;
	visited = other.visited;

	down_votes = other.down_votes; //downs
	up_votes = other.up_votes; //ups
	score = other.score;
	likes = other.likes;
	num_comments = other.num_comments;
	num_crossposts = other.num_crossposts;
	num_reports = other.num_reports;
	view_count = other.view_count;
	return *this;
}

void Reddit_Post::parseJson(const QJsonObject &raw){
	//yes, I hand typed all of this
	//yes, it is very repetitive
	approved_at_utc = raw["approved_at_utc"].toBool();
	approved_by = raw["approved_by"].toString();
	archived = raw["archived"].toBool();
	author = raw["author"].toString();
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
	link_flair_text = raw["link_flair_text"].toString();
	locked = raw["locked"].toBool();
	mod_note = raw["mod_note"].toString();
	mod_reason_by = raw["mod_reason_by"].toString();
	mod_reason_title = raw["mod_reason_title"].toString();
	name = raw["name"].toString();
	over_18 = raw["over_18"].toBool();
	permalink = raw["permalink"].toString();
	pinned = raw["pinned"].toBool();
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

	down_votes = raw["downs"].toInt(); //downs
	up_votes = raw["ups"].toInt(); //ups
	score = raw["score"].toInt();
	likes = raw["likes"].toInt();
	num_comments = raw["num_comments"].toInt();
	num_crossposts = raw["num_crossposts"].toInt();
	num_reports = raw["num_reports"].toInt();
	view_count = raw["view_count"].toInt();
}

void Reddit_Post::debug_print(){
	printf("title = %s\n",title.toUtf8().data());
	printf("self text = \n%s\n",selftext.toUtf8().data());
	//printf("\tselftext html = %s\n",selftext_html.toUtf8().data());
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
	printf("\tvisited = %s\n",visited?"true":"false");
	printf("\n");
	printf("\t%d down\n",down_votes);
	printf("\t%d up\n",up_votes);
	printf("\t%d likes\n",likes);
	printf("\t%d score\n",score);
	printf("\t%d comments\n",num_comments);
	printf("\t%d crossposts\n",num_crossposts);
	printf("\t%d reports\n",num_reports);
	printf("\t%d views\n",view_count);
}
