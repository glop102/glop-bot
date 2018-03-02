cmake_minimum_required(VERSION 2.8.12)

project(redditapi)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui Network REQUIRED)

set(API_SRCS
	redditapi.h
	reddit_session.h
	reddit_session.cpp
	reddit_parser.h
	reddit_parser.cpp
	reddit_subreddit.h
	reddit_subreddit.cpp
)
set(CONFIG_SRCS
	configFileParser/glopConfig.cpp
	configFileParser/glopConfig.h
)

add_library(redditapi STATIC
	${API_SRCS}
	${CONFIG_SRCS}
)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Network)
