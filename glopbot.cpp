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

void small_test_recursive_comment_funct(QList<Reddit_Comment> coms){
	for(auto com : coms){
		com.makeNewReply("REPLY TO ALL THE THINGS!!!!!!!!!!!!!!!!!!!!!!!!!\n"
						 "\n"
						 ">Pretend Quote\n"
						 "\n"
						 "`code stuff`\n"
						 "\n"
						 "^^^tiny ^^text\n"
						);
		small_test_recursive_comment_funct(com.replies);
	}
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
	//test->makeNewPost_text("Simple Post Test","I like hard coded strings, how about you?",false,false,true,"Random Flair Text","Random ID");
	//test->makeNewPost_text("Title","Text");
	//test->makeNewPost_link("Google","https://google.com");

	//test->cacheSubredditStats();
	//printf("%d current users\n",test->accounts_active);
	//printf("%d subscribers\n",test->subscribers);

	//p[0].getComments();
	//Reddit_Comment com("https://oauth.reddit.com/r/anime/comments/82aefz/the_winter_2018_midseason_survey_results/dv8lmsq/",session);

	//Reddit_Account me(session);
	//me.cacheCurrentAccountInfo();

	//Reddit_Post post("/r/bottest/comments/82zexp/google/",session);
	//post.makeNewComment("Do I Need to encode special chars? this&that");

	//Reddit_Comment com1("https://oauth.reddit.com/r/bottest/comments/82zexp/google/dve0c8r/",session);
	//Reddit_Comment com1("/r/bottest/comments/82zexp/google/dve0c8r/",session);
	//com1.makeNewReply("More Testing !!! るようにお願い申し上げます。当サブ ...");

	Reddit_Subreddit *test = new Reddit_Subreddit(session,"bottest");
	QList<Reddit_Post> posts = test->getPosts(10);
	for(auto post : posts){
		if(post.author == "glop102"){
			QList<Reddit_Comment> coms =  post.getComments();
			small_test_recursive_comment_funct(coms);
		}
	}
	exit(0);
}

void GlopBot::setProcessInterval(int seconds){
	recuringTimeout->setInterval(seconds*1000);
}
