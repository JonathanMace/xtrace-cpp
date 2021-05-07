#include <string>
#include <iostream>
#include <thread>
#include <random>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <time.h>
extern "C" {
	#include "hindsight.h"
}


#include "baggage.h"
#include "xtrace.h"
#include "xtrace_baggage.h"
#include "baggageprotocol.h"
#include "pubsub.h"
#include <boost/asio/ip/host_name.hpp>
#include "reporting_generated.h"

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

void makeReport(
		flatbuffers::FlatBufferBuilder &builder,
		std::string agent,
		std::string label
	) {

	// uint64_t taskId = XTraceBaggage::GetTaskID();
	// std::vector<uint64_t> parentIds = XTraceBaggage::GetParentEventIDs();
	// auto parents = builder.CreateVector((int64_t*) parentIds.data(), parentIds.size());

	auto process_name = builder.CreateString(__progname);
	auto host_name = builder.CreateString(boost::asio::ip::host_name());
	auto b_agent = builder.CreateString(agent);
	auto b_label = builder.CreateString(label);

	xtraceflatbuffers::XTraceReportv4Builder b(builder);
	{
		using namespace std::chrono;
		auto now = high_resolution_clock::now();
		b.add_hrt(duration_cast<nanoseconds>(now.time_since_epoch()).count());
		b.add_timestamp(duration_cast<milliseconds>(now.time_since_epoch()).count());
	}
	b.add_cycles(get_thread_cpu_time());


	// b.add_task_id(taskId);
	// b.add_parent_event_id(parents);
	b.add_process_id(getpid());
	b.add_process_name(process_name);
	b.add_thread_id(std::hash<std::thread::id>{}(std::this_thread::get_id()));
	b.add_host(host_name);

	uint64_t eventId = make_event_id();
	b.add_event_id(eventId);
	// XTraceBaggage::SetParentEventID(eventId);

	b.add_agent(b_agent);
	b.add_label(b_label);

	auto report = b.Finish();
	builder.Finish(report);
}

void sendReport(flatbuffers::FlatBufferBuilder &builder) {
	// uint8_t *buf = builder.GetBufferPointer();
	// int size = builder.GetSize();
	// hindsight_tracepoint((char*) buf, size);

	uint8_t *buf = builder.GetBufferPointer();
	int size = builder.GetSize();
	std::string s((char*) buf, size);
	PubSub::publish(XTRACE_REPORT_FLATBUF_TOPIC, s);


	// std::cout << "Sending " << size << " bytes" << std::endl;
}

void XTrace::Logger::log(std::string message) {
	if (!XTraceBaggage::HasTaskID()) {
		return;
	}

	flatbuffers::FlatBufferBuilder builder(1024);
	makeReport(builder, this->agent, message);
	sendReport(builder);
}

void XTrace::Logger::log(std::string file, int line, std::string message) {
	if (!XTraceBaggage::HasTaskID()) {
		return;
	}

	flatbuffers::FlatBufferBuilder builder(1024);
	makeReport(builder, this->agent, message);

	// std::ostringstream ss;
	// ss << file << ":" << line;
	// std::string source = ss.str();
	// report.set_source(source);

	sendReport(builder);
}

void XTrace::Logger::log(std::string file, int line, std::string message, std::map<std::string, std::string> annotations) {
	if (!XTraceBaggage::HasTaskID()) {
		return;
	}

	flatbuffers::FlatBufferBuilder builder(1024);
	makeReport(builder, this->agent, message);

	// std::ostringstream ss;
	// ss << file << ":" << line;
	// std::string source = ss.str();
	// report.set_source(source);

	// for (std::map<std::string, std::string>::iterator it = annotations.begin(); it != annotations.end(); it++) {
	// 	report.add_key(it->first);
	// 	report.add_value(it->second);
	// }

	sendReport(builder);
}

void XTrace::StartTrace() {
	XTraceBaggage::Clear();	// Clear old X-Trace metadata if it exists
	XTraceBaggage::SetTaskID(make_event_id());

	flatbuffers::FlatBufferBuilder builder(1024);
	makeReport(builder, "StartTrace", "Start Trace");
	sendReport(builder);
}

void XTrace::StartTrace(std::string tag) {
	XTrace::StartTrace(std::vector<std::string>{tag});
}

void XTrace::StartTrace(std::vector<std::string> tags) {
	XTraceBaggage::Clear();	// Clear old X-Trace metadata if it exists
	XTraceBaggage::SetTaskID(make_event_id());

	flatbuffers::FlatBufferBuilder builder(1024);
	makeReport(builder, "StartTrace", "Start Trace");
	// for (int i = 0; i < tags.size(); i++) {
	// 	report.add_tags(tags[i]);
	// }
	sendReport(builder);
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