#ifndef REDDIT_SUBREDDIT
#define REDDIT_SUBREDDIT

#include "reddit_parser.h"
#include <QString>
#include <QList>

class Reddit_Subreddit : public Reddit_Parser{
	Q_OBJECT
	QString subreddit;
public:
	explicit Reddit_Subreddit(RedditSession* session, QString subreddit);
};

#endif
