#ifndef GLOPBOT_H
#define GLOPBOT_H
#include <QObject>
#include <QTimer>
#include "redditapi.h"
#include <stdio.h>

class GlopBot : public QObject{
	Q_OBJECT
	RedditSession *session = NULL;
	QTimer *recuringTimeout;
public:
	GlopBot();
public slots:
	void mainProcess();
public slots:
	void setProcessInterval(int seconds);
};

#endif // GLOPBOT_H
