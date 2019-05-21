#include <string>
#include <iostream>
#include <thread>
#include <random>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <time.h>


#include "baggage.h"
#include "xtrace.h"
#include "xtrace.pb.h"
#include "xtrace_baggage.h"
#include "baggageprotocol.h"
#include "pubsub.h"
#include <boost/asio/ip/host_name.hpp>

std::random_device rd;
std::mt19937_64 eng(rd());
std::uniform_int_distribution<uint64_t> distr;

uint64_t make_event_id() {
	return distr(eng);
}

uint64_t get_thread_cpu_time() {
   struct timespec tp;
   clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp);
   return 1000000000LL * ((long long) tp.tv_sec) + ((long long) tp.tv_nsec);
}

extern char *__progname;


XTraceReportv4 makeReport() {
	XTraceReportv4 report;
	{
		using namespace std::chrono;
		auto now = high_resolution_clock::now();
		report.set_hrt(duration_cast<nanoseconds>(now.time_since_epoch()).count());
		report.set_timestamp(duration_cast<milliseconds>(now.time_since_epoch()).count());
	}
	report.set_cycles(get_thread_cpu_time());

	uint64_t taskId = XTraceBaggage::GetTaskID();
	std::vector<uint64_t> parentIds = XTraceBaggage::GetParentEventIDs();

	report.set_task_id(taskId);
	for (int i = 0; i < parentIds.size(); i++) {
		report.add_parent_event_id(parentIds[i]);
	}

	report.set_process_id(getpid());
	report.set_process_name(__progname);
	report.set_thread_id(std::hash<std::thread::id>{}(std::this_thread::get_id()));
	report.set_host(boost::asio::ip::host_name());

	uint64_t eventId = make_event_id();
	report.set_event_id(eventId);
	XTraceBaggage::SetParentEventID(eventId);

	return report;
}

void sendReport(XTraceReportv4 &report) {
	std::string s;
	report.SerializeToString(&s);
	PubSub::publish(XTRACE_REPORT_PROTOBUF_TOPIC, s);
}

void XTrace::Logger::log(std::string message) {
	if (!XTraceBaggage::HasTaskID()) {
		return;
	}

	XTraceReportv4 report = makeReport();
	report.set_agent(this->agent);
	report.set_label(message);
	sendReport(report);
}

void XTrace::Logger::log(std::string file, int line, std::string message) {
	if (!XTraceBaggage::HasTaskID()) {
		return;
	}

	XTraceReportv4 report = makeReport();
	report.set_agent(this->agent);
	report.set_label(message);

	std::ostringstream ss;
	ss << file << ":" << line;
	std::string source = ss.str();
	report.set_source(source);

	sendReport(report);
}

void XTrace::Logger::log(std::string file, int line, std::string message, std::map<std::string, std::string> annotations) {
	if (!XTraceBaggage::HasTaskID()) {
		return;
	}

	XTraceReportv4 report = makeReport();
	report.set_agent(this->agent);
	report.set_label(message);

	std::ostringstream ss;
	ss << file << ":" << line;
	std::string source = ss.str();
	report.set_source(source);

	for (std::map<std::string, std::string>::iterator it = annotations.begin(); it != annotations.end(); it++) {
		report.add_key(it->first);
		report.add_value(it->second);
	}

	sendReport(report);
}

void XTrace::StartTrace() {
	XTraceBaggage::Clear();	// Clear old X-Trace metadata if it exists
	XTraceBaggage::SetTaskID(make_event_id());

	XTraceReportv4 report = makeReport();
	report.set_label("Start Trace");
	sendReport(report);
}

void XTrace::StartTrace(std::string tag) {
	XTrace::StartTrace(std::vector<std::string>{tag});
}

void XTrace::StartTrace(std::vector<std::string> tags) {
	XTraceBaggage::Clear();	// Clear old X-Trace metadata if it exists
	XTraceBaggage::SetTaskID(make_event_id());

	XTraceReportv4 report = makeReport();
	report.set_agent("StartTrace");
	report.set_label("Start Trace");
	for (int i = 0; i < tags.size(); i++) {
		report.add_tags(tags[i]);
	}
	sendReport(report);
}

bool XTrace::IsTracing() {
	return XTraceBaggage::HasTaskID();
}


XTrace::Logger XTrace::GetLogger(std::string agent) {
	return XTrace::Logger(agent);
}

XTrace::Logger defaultLogger = XTrace::Logger("default");

void XTrace::log(std::string message) {
	defaultLogger.log(message);
}