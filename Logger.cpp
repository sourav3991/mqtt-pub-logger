#include "Logger.h"
#include <mosquitto.h>
#include <string>
#include <string.h>
#include <unistd.h>

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
		_buffer.str("");
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
	std::string message = _getCurrentTimestamp();
	message = message + " : " +_buffer.str();
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

std::string Logger::_getCurrentTimestamp()
{
        using std::chrono::system_clock;
        auto currentTime = std::chrono::system_clock::now();
        char buffer[80];
        auto transformed = currentTime.time_since_epoch().count() / 1000000;
        auto millis = transformed % 1000;
        std::time_t tt;
        tt = system_clock::to_time_t ( currentTime );
        auto timeinfo = localtime (&tt);
        strftime (buffer,80,"%F %H:%M:%S",timeinfo);
        sprintf(buffer, "%s:%03d",buffer,(int)millis);

        return std::string(buffer);
}

