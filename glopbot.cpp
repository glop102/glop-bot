#include "glopbot.h"

GlopBot::GlopBot() : QObject(0){
	//timer to run our bot
	recuringTimeout = new QTimer;
	setProcessInterval(60); // every minute
	recuringTimeout->start();
	connect(recuringTimeout, SIGNAL(timeout()), this,SLOT(mainProcess()) );

	// don't wait on the timer and just do it now
	QMetaObject::invokeMethod(this,"mainProcess",Qt::QueuedConnection);
}

void GlopBot::mainProcess(){
	if( session == NULL){
		session = new RedditSession;
		session->parseSettings();
	}

	//ra->testFunction();
	//QByteArray temp = session->makeGETrequest("https://oauth.reddit.com/r/bottest/about")->readAll();
	//printf("%s\n",temp.data());
	//Reddit_Subreddit *test = new Reddit_Subreddit(session,"bottest");
	//Reddit_Subreddit *test = new Reddit_Subreddit(session,"newsokur");
	//Reddit_Subreddit *test = new Reddit_Subreddit(session,"anime");
	//Reddit_Subreddit *test = new Reddit_Subreddit(session,"whatcouldgowrong");
	//QList<Reddit_Post> p = test->getPosts(100);
	//Reddit_Subreddit *test = new Reddit_Subreddit(session,"asuka");
	//Reddit_Subreddit *test = new Reddit_Subreddit(session,"kemonomimi");
	//QList<Reddit_Post> p = test->getPosts(10);
	//p = test->getPosts(100,p.last().id);
	//p[0].debug_print();
	//for(auto k : p){
	//	printf("%s\n\t%s\n",k.title.toUtf8().data(),k.url.toUtf8().data());
	//}

	//test->cacheSubredditStats();
	//printf("%d current users\n",test->accounts_active);
	//printf("%d subscribers\n",test->subscribers);

	//p[0].getComments();
	Reddit_Comment com("https://oauth.reddit.com/r/anime/comments/82aefz/the_winter_2018_midseason_survey_results/dv8lmsq/",session);
	exit(0);
}

void GlopBot::setProcessInterval(int seconds){
	recuringTimeout->setInterval(seconds*1000);
}
