#include <mosquitto.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include "Logger.h"
#include "Utils.h"

void on_disconnect(struct mosquitto *mosq, void *obj, int rc) {
    if (rc == 0) {
        printf("Disconnected successfully.\n");
    } else {
        printf("Unexpected disconnection (code %d): %s\n", rc, mosquitto_strerror(rc));
    }
}

Logger::Logger(const std::string& logFile, loglevel level, bool publish)
{
	_logFile = logFile;
	_level = level;
	_log = LoggerImpl::getInstance();
	_publish = publish;
	mosq = nullptr;
	if(_publish == true)
	{
		_mosquittoInit();
	}
}

Logger& Logger::operator<<(std::ostream& (*var)(std::ostream&))
{
	_flushBuffer();
	return *this;
}
void Logger::_writeToBuffer(const std::stringstream& ss)
{
	std::lock_guard<std::mutex> lock(_bufferMtx);
	_buffer << ss.str();
}
void Logger::_flushBuffer()
{
	if(_publish)
	{
		std::lock_guard<std::mutex> lock(_bufferMtx);
		_publishToTopic();
	}
	std::lock_guard<std::mutex> lock(_bufferMtx);
	if(_level == loglevel::trace)
		_log->trace(_buffer.str(), _logFile);
	if(_level == loglevel::debug)
		_log->debug(_buffer.str(), _logFile);
	if(_level== loglevel::error)
		_log->error(_buffer.str(), _logFile);
	_buffer.str("");
}

void Logger::_publishToTopic()
{
	std::string topic;
	if(_level == loglevel::trace)
		topic = "/logger/debug";
	if(_level == loglevel::debug)
		topic = "/logger/debug";
	if(_level == loglevel::error)
		topic = "/logger/debug";
	std::string message = Utils::getCurrentTimestamp();
	message += ":";
	message = message + _buffer.str();
	mosquitto_publish(mosq, NULL, topic.c_str(), message.length(), message.c_str(), 0, 0);
	sleep(0.001);
}

void Logger::_mosquittoInit()
{
	const char *host = "localhost";
	int port = 1883;
	int keepalive = 60;
	bool clean_session = true;

	mosquitto_lib_init();
	mosq = mosquitto_new(NULL, clean_session, NULL);
	if(!mosq)
	{
		printf("Error: Out of memory.\n");
		exit(1);
	}
	mosquitto_disconnect_callback_set(mosq, on_disconnect);


	if(mosquitto_connect(mosq, host, port, keepalive))
	{
		printf("Unable to connect.\n");
		exit(1);
	}
	int loop = mosquitto_loop_start(mosq);
	if(loop != MOSQ_ERR_SUCCESS)
	{
		printf("Unable to start loop: %i\n", loop);
		exit(1);
	}
}

Logger::~Logger()
{
        if (mosq) {
        // Disconnect from the broker
        int rc = mosquitto_disconnect(mosq);
        if (rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Error disconnecting: %s\n", mosquitto_strerror(rc));
        }

        // Clean up the Mosquitto library
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();

        printf("Mosquitto connection closed.\n");
    } else {
        printf("No active Mosquitto connection to close.\n");
    }
    sleep(1);
}
