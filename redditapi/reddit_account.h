#ifndef REDDIT_ACCOUNT
#define REDDIT_ACCOUNT

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QString>
#include <QList>
#include "reddit_session.h"

class Reddit_Account{
protected:
	RedditSession *session;
public:
	explicit Reddit_Account(RedditSession *sess);
	void cacheCurrentAccountInfo();
public:
	static void debug_parse(const QJsonObject &raw, QString pad="");
	static void debug_parse(const QJsonArray &arr, QString pad="");
};

#endif
