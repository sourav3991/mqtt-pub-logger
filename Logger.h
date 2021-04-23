#include "LoggerImpl.h"
#include <string>
#include <sstream>
#include <type_traits>
#include <typeinfo>
class Logger
{
	std::string _logFile;
	LoggerImpl* _log;
	loglevel _level;
	bool _publish;
	std::stringstream _buffer;
	std::mutex _bufferMtx;
	void _writeToBuffer(const std::stringstream& string);
	void _flushBuffer();

	struct mosquitto *mosq;
	void _publishToTopic();
	void _mosquittoInit();
	std::string _getCurrentTimestamp();

	public:
	Logger(const std::string& logFile, loglevel level, bool publish);

	template<typename T>
	Logger& operator<<(const T& message)
	{
		std::stringstream ss;
		ss << message;
		_writeToBuffer(ss);
		return *this;
	}

	Logger& operator<<(std::ostream& (*var)(std::ostream&));
};
