
#ifndef _PUBSUB_H_
#define _PUBSUB_H_

#include <boost/asio.hpp>
#include <string>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unistd.h>
#include <thread>


// Statically define these variables for now
#define QUEUE_MAX_MESSAGES 1000
#define PUBLISHER_CONNECT_RETRY_TIMEOUT_MICROS 1000000
#define DEFAULT_SERVER_ADDRESS "localhost"
#define DEFAULT_SERVER_PORT "5563"

typedef std::pair<std::string, std::string> PubSubMessage;

class PubSubClient {

public:

	PubSubClient(std::string server_address, std::string server_port);

	// Starts the publisher thread.  Not necessary to call explicitly; publisher thread is automatically created when publishing.
	void start();
	void shutdown();
	void join();

	void publish(std::string topic, std::string message);

private:
	std::string server_hostname, server_port;

	std::queue<PubSubMessage> queue;
	std::mutex lock;
	std::condition_variable condition;
	std::thread* publisher_thread = nullptr;

	volatile bool is_shutdown = false;

	void publisher_main();
	void publish_loop(boost::asio::ip::tcp::socket &socket);

};

class PubSub {

public:

	static void publish(std::string topic, std::string message);

	static void shutdown();

	static void join();

private:
	PubSub() {}
	static PubSubClient* client;
};



#endif