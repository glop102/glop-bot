#include "reddit_common.h"

Reddit_Common::Reddit_Common(RedditSession *session) : QObject(0){
	this->session = session;
}
