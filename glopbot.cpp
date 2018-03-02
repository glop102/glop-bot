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
	if( ra == NULL){
		ra = new RedditSession;
		ra->parseSettings();
	}

	ra->testFunction();
}

void GlopBot::setProcessInterval(int seconds){
	recuringTimeout->setInterval(seconds*1000);
}
