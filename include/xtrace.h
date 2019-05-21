
#ifndef _XTRACE_H_
#define _XTRACE_H_

#include <cstring>
#include <string>
#include <vector>
#include <map>


#define XTRACE_REPORT_PROTOBUF_TOPIC "xtpb"


#define __SHORT_FILENAME__(x) (strrchr(x, '/') ? strrchr(x, '/') + 1 : x) 

// Macro overloading fuckery
#define XTRACE2(f, l, ...) XTrace::Logger(__SHORT_FILENAME__(f)).log(f, l, __VA_ARGS__)
#define XTRACE(...) XTRACE2(__FILE__, __LINE__, __VA_ARGS__)

namespace XTrace {

class Logger {
public:
	const std::string agent;

	Logger(std::string agent) : agent(agent) {}

	void log(std::string message);

	void log(std::string file, int line, std::string message);

	void log(std::string file, int line, std::string message, std::map<std::string, std::string> annotations);
};


// Start a new trace, with a randomly generated taskID
void StartTrace();
void StartTrace(std::string tag);
void StartTrace(std::vector<std::string> tags);

bool IsTracing();

Logger GetLogger(std::string agent);

void log(std::string message);

void setProcessName(std::string processName);


}


#endif