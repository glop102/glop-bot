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

class Reddit_Subreddit : public QObject{
	Q_OBJECT
protected:
	RedditSession *session = NULL;
	QString subreddit;
public:
	explicit Reddit_Subreddit(RedditSession* session, QString subreddit);
	Reddit_Subreddit(const Reddit_Subreddit& other);
	Reddit_Subreddit& operator=(const Reddit_Subreddit& other);
	//before is the post before what you want to retrieve
	QList<Reddit_Post> getPosts(int limit=25, QString before="", QString after="");
public slots:
};

#endif
