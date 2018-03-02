#ifndef REDDIT_PARSER
#define REDDIT_PARSER

#include <QObject>
#include "reddit_session.h"

class Reddit_Parser : public QObject{
	Q_OBJECT
	RedditSession *session = NULL;
public:
	explicit Reddit_Parser(RedditSession* session);
};

#endif
