#include "reddit_account.h"

Reddit_Account::Reddit_Account(RedditSession *sess){
	session = sess;
}

void Reddit_Account::cacheCurrentAccountInfo(){
	//auto reply = session->makeGETrequest("https://oauth.reddit.com/api/v1/me");
	auto reply = session->makeGETrequest("https://oauth.reddit.com/api/v1/me");
	reply->deleteLater();
	debug_parse(QJsonDocument::fromJson(reply->readAll()).object());
}


void Reddit_Account::debug_parse(const QJsonObject &raw, QString pad){
	for(QString key : raw.keys()){
		if(raw[key].isBool()){
			printf("%s%s : bool : %s\n",pad.toUtf8().data(),key.toUtf8().data(),raw[key].toBool()?"true":"false");
		}else if(raw[key].isDouble()){
			printf("%s%s : double : %f\n",pad.toUtf8().data(),key.toUtf8().data(),raw[key].toDouble());
		}else if(raw[key].isNull()){
			printf("%s%s : null : null\n",pad.toUtf8().data(),key.toUtf8().data());
		}else if(raw[key].isString()){
			QString temp = raw[key].toString().left(200); // limit to 200 characters
			printf("%s%s : string : %s\n",pad.toUtf8().data(),key.toUtf8().data(),temp.toUtf8().data());
		}else if(raw[key].isUndefined()){
			printf("%s%s : undefined : undefined\n",pad.toUtf8().data(),key.toUtf8().data());
		}else if(raw[key].isArray()){
			printf("%s%s : array : []\n",pad.toUtf8().data(),key.toUtf8().data());
			debug_parse(raw[key].toArray(),pad+"\t");
		}else if(raw[key].isObject()){
			printf("%s%s : object : {}\n",pad.toUtf8().data(),key.toUtf8().data());
			debug_parse(raw[key].toObject(),pad+"\t");
		}
	}
}
void Reddit_Account::debug_parse(const QJsonArray &arr, QString pad){
	for(int x=0; x<arr.size(); x++){
		if(arr[x].isBool()){
			printf("%s%d : bool : %s\n",pad.toUtf8().data(),x,arr[x].toBool()?"true":"false");
		}else if(arr[x].isDouble()){
			printf("%s%d : double : %f\n",pad.toUtf8().data(),x,arr[x].toDouble());
		}else if(arr[x].isNull()){
			printf("%s%d : null : null\n",pad.toUtf8().data(),x);
		}else if(arr[x].isString()){
			QString temp = arr[x].toString().left(200); // limit to 200 characters
			printf("%s%d : string : %s\n",pad.toUtf8().data(),x,temp.toUtf8().data());
		}else if(arr[x].isUndefined()){
			printf("%s%d : undefined : undefined\n",pad.toUtf8().data(),x);
		}else if(arr[x].isArray()){
			printf("%s%d : array : []\n",pad.toUtf8().data(),x);
			debug_parse(arr[x].toArray(),pad+"\t");
		}else if(arr[x].isObject()){
			printf("%s%d : object : {}\n",pad.toUtf8().data(),x);
			debug_parse(arr[x].toObject(),pad+"\t");
		}
	}
}
