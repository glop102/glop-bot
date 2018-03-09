#ifndef REDDIT_ACCOUNT
#define REDDIT_ACCOUNT

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QString>
#include <QList>
#include "reddit_session.h"
#include "reddit_comment.h"
#include "reddit_post.h"

class Reddit_Account{
protected:
	RedditSession *session;
public:
	explicit Reddit_Account(RedditSession *sess);
	void cacheCurrentAccountInfo();
	void cacheUserAccountInfo(QString username);

	//sort = {hot, new, top, controversial}
	QList<Reddit_Comment> getUserComments(QString username,int limit=100,QString sort="",QString after="", QString before="");
	QList<Reddit_Post> getUserPosts(QString username,int limit=100,QString sort="",QString after="", QString before="");
	QList<Reddit_Post> getUserUpvotedPosts(QString username,int limit=100,QString sort="",QString after="", QString before="");
	QList<Reddit_Post> getUserDownvotedPosts(QString username,int limit=100,QString sort="",QString after="", QString before="");
public:
	void parseJson(const QJsonObject &raw);
	void parseJson_subreddit(const QJsonObject &raw);
	static void debug_parse(const QJsonObject &raw, QString pad="");
	static void debug_parse(const QJsonArray &arr, QString pad="");

public:
	//there is a sctruct that independently holds the personal subreddit data
	//It is not an instance of Reddit_Subreddit to try to keep coupling to a minimum,
	//and not all variables seem to be around for the personal version

	int comment_karma;
	double created,created_utc;
	int gold_creddits;
	double gold_expiration;
	bool has_mail,has_mod_mail;
	bool has_subscribed;
	bool has_verified_email;
	// bool has_visited_new_profile; - not sure this matters much nor will stay around for the long time
	bool hide_from_robots;
	QString icon_img;
	QString id;
	bool in_beta;
	// bool in_redesign_beta; - fairly certain this will go away in the future
	int inbox_count; // number of new messages?
	bool is_employee;
	bool is_gold;
	bool is_mod;
	bool is_sponsor;
	bool is_suspended;
	int link_karma;
	QString name;
	QString oauth_client_id; //???
	bool over_18;
	QString pref_geopopular;
	bool pref_no_profanity,pref_show_snoovatar;
	bool pref_top_karma_subreddits;
	double suspension_expiration_utc;
	bool verified;

	struct {
		QString banner_img;
		double banner_height,banner_width;
		bool default_set;
		QString description,public_description;
		QString display_name,display_name_prefixed;
		QString header_img;
		double header_height,header_width;
		QString icon_color;
		QString icon_img;
		double icon_height,icon_width;
		bool is_default_banner,is_default_icon;
		QString key_color,primary_color;
		bool link_flair_enabled;
		QString name;
		bool over_18;
		bool show_media;
		QString subreddit_type;
		int subscribers;
		QString title;
		QString url;
		bool user_is_banned,user_is_contributor,user_is_moderator,user_is_muted,user_is_subscriber;
	} personal_subreddit;
};

#endif
