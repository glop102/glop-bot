#ifndef REDDIT_POST_H
#define REDDIT_POST_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

class Reddit_Post : public QObject{
	Q_OBJECT
public:
	double approved_at_utc;
	QString approved_by;
	bool archived;
	QString author;
	QString author_flair_text;
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
	QString link_flair_text;
	bool locked;
	QString mod_note;
	QString mod_reason_by;
	QString mod_reason_title;
	QString name;
	bool over_18;
	QString permalink;
	bool pinned;
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
	bool visited;

	int down_votes; //downs
	int up_votes; //ups
	int score;
	int likes;
	int num_comments;
	int num_crossposts;
	int num_reports;
	int view_count;
public:
	Reddit_Post(const QJsonObject &raw);
	Reddit_Post(const Reddit_Post &other);
	Reddit_Post& operator=(const Reddit_Post &other);
public slots:
	void parseJson(const QJsonObject &raw);

public:
	void debug_print(); // prints every single variable
};

#endif // REDDIT_POST_H
