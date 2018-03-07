#ifndef REDDIT_POST_H
#define REDDIT_POST_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QUrl>
#include <QString>
#include <QList>
#include "reddit_session.h"
#include "reddit_comment.h"

class Reddit_Post{
protected:
	RedditSession* session = NULL;
public: // variables
	double approved_at_utc;
	QString approved_by;
	bool archived;
	QString author;
	QString author_flair_text,author_flair_css_class;
	double banned_at_utc;
	QString banned_by;
	bool can_gild;
	bool can_mod_post;
	bool clicked;
	double created_time; // seconds
	double created_time_utc;
	QString domain;
	bool edited;
	int gilded;
	bool hidden;
	bool hide_score;
	QString id;
	bool is_crosspostable;
	bool is_reddit_media_domain;
	bool is_self;
	bool is_video;
	QString link_flair_text,link_flair_css_class;
	bool locked;
	QString mod_note;
	QString mod_reason_by;
	QString mod_reason_title;
	QString name;
	bool over_18;
	QString permalink;
	bool pinned;
	QString post_hint;
	bool quarantine;
	QString removal_reason;
	bool saved;
	QString selftext;
	QString selftext_html;
	bool spoiler;
	bool stickied;
	QString subreddit;
	QString subreddit_id;
	QString subreddit_name_prefixed;
	QString subreddit_type;
	QString thumbnail;
	double thumbnail_height;
	double thumbnail_width;
	QString title;
	QString url;
	QString user_vote; // if the user voted "up" "down" or nothing ("")
	bool visited;
	QString whitelist_status;

	int down_votes; //downs
	int up_votes; //ups
	int score;
	int num_comments;
	int num_crossposts;
	int num_reports;
	int view_count;

	struct PreviewImage{
		QList<QString> urls={}; // each preview image has multiple resolutions on the server
		QList<double> width,height;
	};
	QList<struct PreviewImage> preview_images;
	QList<struct PreviewImage> preview_images_blurred; // only has things if spoiler==true

	// type ussually one of
	//youtube.com,m.youtube.com,streamable.com,gfycat.com,reddit_video,imgur.com,m.imgur.com
	//reddit_video decided to be special and screw every thing up
	struct{
		QString type;

		struct {
			QString content,media_domain_url;
			bool scrolling;

			QString author_name,author_url;
			QString description;
			double height,width;
			QString html;
			QString provider_name,provider_url;
			QString thumbnail_url;
			double thumbnail_height, thumbnail_width;
			QString title;
			QString type;
			QString url;
			QString version;
		}common; // everything but reddit-video
		struct {
			QString dash_url,fallback_url;
			double duration;
			double height,width;
			QString hls_url,scrubber_media_url;
			bool is_gif;
			QString transcoding_status;
		}reddit_video;
	}media;
public: //normally used methods
	explicit Reddit_Post(RedditSession *sess);
	Reddit_Post(const QString &url, RedditSession *sess);
	Reddit_Post(const QJsonObject &raw,RedditSession* sess);
	void parseJson(const QJsonObject &raw);
	QList<Reddit_Comment> getComments();

protected: // things that are usually internal helper functions
	QString fixUrl(QString);
	void parseJson_previewImages(const QJsonObject &raw);
	void parseJson_media(const QJsonObject &raw);
public: // debug stuff
	void debug_print(); // prints every single variable
	static void debug_parse(const QJsonObject &raw, QString pad="");
	static void debug_parse(const QJsonArray &arr, QString pad="");
};

#endif // REDDIT_POST_H
