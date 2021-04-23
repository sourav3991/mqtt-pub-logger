#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "LoggerEnums.h"
#include <iostream>
#include <fstream>
#include <mutex>

class LoggerImpl
{
	static LoggerImpl* _instance;
	static std::mutex _loggerMutex;
	std::mutex _loggingMutex;
	LoggerImpl();
	void _logger(loglevel, const std::string&, const std::string&);
	std::string _getCurrentTimestamp();

	public:
	static LoggerImpl* getInstance();
	void trace(const std::string&, const std::string&);
	void debug(const std::string&, const std::string&);
	void error(const std::string&, const std::string&);
};

#endif
