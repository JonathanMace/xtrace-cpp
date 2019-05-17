#include "pubsub.h"
#include <iostream>
#include <boost/asio.hpp>
#include <unistd.h>
#include <thread>
#include <mutex>



PubSubClient::PubSubClient(std::string server_hostname, std::string server_port) {
	this->server_hostname = server_hostname;
	this->server_port = server_port;
}

void PubSubClient::publish(std::string topic, std::string message) {
	std::unique_lock<std::mutex> l(this->lock);
	if (this->publisher_thread == nullptr) {
		this->publisher_thread = new std::thread(&PubSubClient::publisher_main, this);
	}
	if (this->queue.size() < QUEUE_MAX_MESSAGES) {
		this->queue.push(PubSubMessage(topic, message));
	}
	this->condition.notify_all();
}

void PubSubClient::start() {
	std::unique_lock<std::mutex> l(this->lock);
	if (this->publisher_thread == nullptr) {
		this->publisher_thread = new std::thread(&PubSubClient::publisher_main, this);
	}
}



std::vector<uint8_t> intToBytes(int x) {
	std::vector<uint8_t> buf(4);
	for (unsigned i = 0; i < 4; i++) {
		buf[3-i] = (x >> (i * 8));
	}
	return buf;
}

void PubSubClient::publish_loop(boost::asio::ip::tcp::socket &socket) {
	std::cout << "PubSub connected to " << this->server_hostname << ":" << this->server_port << std::endl;
	while (true) {
		std::unique_lock<std::mutex> l(this->lock);
		if (this->queue.size() == 0) {
			if (this->is_shutdown) {
				return; // only shutdown when queue is drained
			} else {
				this->condition.wait(l);
			}
		} else {
			PubSubMessage msg = this->queue.front();
			std::string topic = msg.first;
			std::string data = msg.second;

			try {
				boost::asio::write(socket, boost::asio::buffer(intToBytes(topic.size())));
				boost::asio::write(socket, boost::asio::buffer(topic));
				boost::asio::write(socket, boost::asio::buffer(intToBytes(data.size())));
				boost::asio::write(socket, boost::asio::buffer(data));
				this->queue.pop();
			} catch (const boost::system::system_error& ex) {
				std::cout << "Disconnected from " << this->server_hostname << ":" << this->server_port << std::endl;
				return;
			}
		}
	}
}

void PubSubClient::publisher_main() {
	std::cout << "Publisher thread started" << std::endl;
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(this->server_hostname, this->server_port);

	do {
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator;
		try {
			endpoint_iterator = resolver.resolve(query);
		} catch (const boost::system::system_error& ex) {
			std::cout << "Unable to resolve " << this->server_hostname << ":" << this->server_port << std::endl;
			if (!this->is_shutdown) usleep(PUBLISHER_CONNECT_RETRY_TIMEOUT_MICROS);
			continue;
		}

		boost::asio::ip::tcp::socket socket(io_service);
		try {
			boost::asio::connect(socket, endpoint_iterator);
		} catch (const boost::system::system_error& ex) {
			std::cout << "Unable to connect to " << this->server_hostname << ":" << this->server_port << std::endl;
			if (!this->is_shutdown) usleep(PUBLISHER_CONNECT_RETRY_TIMEOUT_MICROS);
			continue;
		}

		this->publish_loop(socket);

	} while (!this->is_shutdown);
	std::cout << "Shutting down" << std::endl;
}

void PubSubClient::join() {
	if (this->publisher_thread != nullptr) {
		this->publisher_thread->join();
	}
}

void PubSubClient::shutdown() {
	std::unique_lock<std::mutex> l(this->lock);
	this->is_shutdown = true;
	this->condition.notify_all();
}


PubSubClient* PubSub::client = new PubSubClient(DEFAULT_SERVER_ADDRESS, DEFAULT_SERVER_PORT);

void PubSub::publish(std::string topic, std::string message) {
	PubSub::client->publish(topic, message);
}

void PubSub::shutdown() {
	PubSub::client->shutdown();
}

void PubSub::join() {
	PubSub::client->join();
}