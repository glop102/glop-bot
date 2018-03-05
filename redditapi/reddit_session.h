#ifndef REDDITAPI_H
#define REDDITAPI_H

#include <QObject>
#include <QDir>
#include <QString>
#include <QCoreApplication>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <QElapsedTimer>
#include <QTimer>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QAuthenticator>
#include <QNetworkCookieJar>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

#include "configFileParser/glopConfig.h"

class RedditSession : public QObject{
	Q_OBJECT
protected:
	static const QString clientID;
	static const QString UserAgent;
	static const QString redirectUri;
	QString API_Token; //identifies that the user authorised our program
	QString API_Session; //temperary session key
	static QNetworkAccessManager *networkManager;

	QElapsedTimer Session_Time_Used; // to compare to for knowing when to refresh - maybe a QTimer for 1 hour and auto-refresh? prob will already do timer every 5 sec to do the things
	QElapsedTimer RequestLimit_TimeElapsed; //elapsed time since we were told anything
	int RequestLimit_RequestsLeft; // how many times the API can be used before we are abusing
	int RequestLimit_TimeToReset; // time we needed to wait for the limit to be reset

	void commonRequestSetup(QNetworkRequest &req,QString URL);
	void commonReplyWait(QNetworkReply *reply);
	void waitForRequestLimitReset();
public:
	QNetworkReply* makePOSTrequest(QString URL, QString postData);
	QNetworkReply* makeGETrequest(QString URL);
public slots:
	bool parseSettings(bool allow_example_firstRun = true); // returns false if unable to get API token, probably need to run the firstTime code
	void saveSettings();
	void firstTimeRun_example(); // prints things out and has the user do things on the command line
	QString firstTimeRun_authorizeURL(QString wanted_scopes); //have the user visit this link
	void firstTimeRun_redirectedURL(QString URL); // after the user says yes, they are sent to a new URL with info we need here
	QString scopesDescriptions();

	void testFunction(); // for me to test things with and random junk in it

	void refreshSession();
	void sendAuthentication(QNetworkReply * reply, QAuthenticator * authenticator);

public:
};

#endif // REDDITAPI_H
