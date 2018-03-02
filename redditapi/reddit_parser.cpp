#include "reddit_parser.h"

Reddit_Parser::Reddit_Parser(RedditSession *session) : QObject(0){
	this->session = session;
}
