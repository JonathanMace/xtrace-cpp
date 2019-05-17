
#ifndef _XTRACE_H_
#define _XTRACE_H_


#include <string>
#include <vector>


#define XTRACE_REPORT_PROTOBUF_TOPIC "xtpb"


#define __XTRACE_FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

// Macro overloading fuckery
#define XTRACE1(msg) XTrace::Logger(__XTRACE_FILENAME__).log(msg, __FILE__, __LINE__)
#define XTRACE2(logger, msg) logger.log(msg, __FILE__, __LINE__)
#define GET_XTRACE_MACRO(_1,_2,NAME,...) NAME
#define XTRACE(...) GET_XTRACE_MACRO(__VA_ARGS__,XTRACE2,XTRACE1)(__VA_ARGS__)

namespace XTrace {

class Logger {
public:
	const std::string agent;

	Logger(std::string agent) : agent(agent) {}

	void log(std::string message);

	void log(std::string message, std::string file, int line);
};


// Start a new trace, with a randomly generated taskID
void StartTrace();
void StartTrace(std::string tag);
void StartTrace(std::vector<std::string> tags);

Logger GetLogger(std::string agent);

void log(std::string message);

void setProcessName(std::string processName);


}


#endif