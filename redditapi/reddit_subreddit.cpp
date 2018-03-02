#include "reddit_subreddit.h"

Reddit_Subreddit::Reddit_Subreddit(RedditSession *session, QString subreddit) : Reddit_Parser(session){
	//cleaned up version of the passed subreddit name
	if(subreddit.indexOf("/r/") == 0)
		this->subreddit = subreddit.mid(3);
	if(subreddit.indexOf("r/") == 0)
		this->subreddit = subreddit.mid(2);
}
