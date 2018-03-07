#ifndef REDDIT_COMMENT
#define REDDIT_COMMENT

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QString>
#include <QList>
#include "reddit_session.h"

class Reddit_Comment {
protected:
	RedditSession *session=NULL;
public:
	QList<Reddit_Comment> replies;

	double approved_at_utc;
	QString approved_by;
	bool archived;
	QString author,author_flair_text,author_flair_css_class;
	double banned_at_utc;
	QString banned_by;
	QString body,body_html;
	bool can_gild;
	bool can_mod_post;
	bool collapsed;
	QString collapsed_reason;
	double controversiality;
	double created,created_utc;
	double depth; // current depth into teh comments
	bool edited;
	double gilded;
	QString id;
	bool is_submitter;
	QString link_id;
	QString mod_note,mod_reason_by,mod_reason_title;
	QString name;
	int num_reports;
	QString parent_id;
	QString permalink;
	QString removal_reason;
	bool saved;
	bool score_hidden;
	bool stickied;
	QString subreddit,subreddit_id,subreddit_name_prefixed;
	QString subreddit_type;
	QString user_vote; // if the user voted "up" "down" or nothing ("")

	int down_votes,up_votes;
	int score;
public:
	explicit Reddit_Comment(RedditSession *sess);
	Reddit_Comment(const QString &url, RedditSession* sess);
	Reddit_Comment(const QJsonObject &raw,RedditSession* sess);
	void parseJson(const QJsonObject &raw);
public:
	static void debug_parse(const QJsonObject &raw, QString pad="");
	static void debug_parse(const QJsonArray &arr, QString pad="");
};

#endif
