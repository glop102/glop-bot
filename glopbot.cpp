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
	Reddit_Subreddit *test = new Reddit_Subreddit(session,"anime");
	QList<Reddit_Post> p = test->getPosts(2);
	//p[0].debug_print();
	for(auto k : p){
		k.debug_print();
	}
}

void GlopBot::setProcessInterval(int seconds){
	recuringTimeout->setInterval(seconds*1000);
}
