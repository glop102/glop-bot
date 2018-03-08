#ifndef REDDIT_SUBREDDIT
#define REDDIT_SUBREDDIT

#include "reddit_session.h"
#include "reddit_post.h"
#include <QString>
#include <QList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QMap>
#include <QList>
#include <QByteArray>
#include <stdio.h>

class Reddit_Subreddit{
protected:
	RedditSession *session = NULL;
	QString subreddit;
public:
	explicit Reddit_Subreddit(RedditSession* session, QString subreddit);
	//Reddit_Subreddit(const Reddit_Subreddit& other);
	//Reddit_Subreddit& operator=(const Reddit_Subreddit& other);

	//after is what post to you want the listing to start after
	QList<Reddit_Post> getPosts(int limit=100, QString after_id="", QString before_id="");

	Reddit_Post makeNewPost_text(QString title,QString body,bool nsfw=false,bool spoiler=false,bool resubmit=true,QString flair_text="",QString flair_id="",bool ad=false);
	Reddit_Post makeNewPost_link(QString title,QString url,bool nsfw=false,bool spoiler=false,bool resubmit=true,QString flair_text="",QString flair_id="",bool ad=false);

	// by default, we do not use up a second request to get stats about teh subreddit, and so the variables are empty
	void cacheSubredditStats();
public:
	int accounts_active;
	bool accounts_active_is_fuzzed;
	int active_user_count;
	QString advertiser_category;
	bool allow_discovery,allow_images,allow_videogifs,allow_videos;
	QString audience_target;
	QString banner_img;
	double banner_width,banner_height; // banner_size comes as 2 index array height x width
	bool collapse_deleted_comments;
	int comment_score_hide_mins;
	double created,created_utc;
	QString description,description_html; // text on the right side of the subreddit
	QString display_name,display_name_prefixed;
	QString header_img,header_title;
	double header_width,header_height; // header_size comes as 2 index array height x width
	bool hide_ads;
	QString icon_img;
	double icon_width,icon_height; // icon_size comes as 2 index array height x width
	QString id,name;
	//is_enrolled_in_new_modmail null
	QString key_color;
	QString lang;
	bool link_flair_enabled;
	QString notification_level;
	bool over18;
	QString public_description,public_description_html;
	bool public_traffic;
	bool quarantine;
	bool show_media,show_media_preview;
	bool spoilers_enabled;
	QString submission_type;
	//submit_link_label null
	QString submit_text,submit_text_html; // should be displayed when user trying to post something
	//submit_text_label null
	QString subreddit_type;
	int subscribers;
	//suggested_comment_sort null
	QString title;
	QString url;
	bool user_can_flair_in_sr,user_flair_enabled_in_sr;
	QString user_flair_css_class,user_flair_text;
	bool user_has_favorited;
	bool user_is_banned;
	bool user_is_contributor;
	bool user_is_moderator;
	bool user_is_muted;
	bool user_is_subscriber;
	bool user_sr_flair_enabled,user_sr_theme_enabled;
	int videostream_links_count;
	QString whitelist_status;
	bool wiki_enabled;
public:
	static void debug_parse(const QJsonObject &raw, QString pad="");
	static void debug_parse(const QJsonArray &arr, QString pad="");
};

#endif
