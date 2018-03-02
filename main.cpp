#include <QCoreApplication>
#include "glopbot.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	GlopBot *bot = new GlopBot;
	//RedditSession *ra = new RedditSession;
	//QMetaObject::invokeMethod(ra,"parseSettings",Qt::QueuedConnection);
	//QMetaObject::invokeMethod(ra,"testFunction",Qt::QueuedConnection);

	return a.exec();
}
