#include "LoggerImpl.h"
#include <string>
#include "Utils.h"

LoggerImpl::LoggerImpl()
{
}
LoggerImpl* LoggerImpl::getInstance()
{
	std::lock_guard<std::mutex> lock(_loggerMutex);
	if(_instance == nullptr)
	{
		_instance = new LoggerImpl();
	}
	return _instance;
}

void LoggerImpl::_logger(loglevel level, const std::string& message, const std::string& logFile)
{
        std::string levelStr;
        if(level == loglevel::trace)
                levelStr = "TRACE";
        if(level == loglevel::debug)
                levelStr = "DEBUG";
        if(level == loglevel::error)
                levelStr = "ERROR";
        {
                std::lock_guard<std::mutex> lock(_loggingMutex);
                std::ofstream outFile;
                outFile.open(logFile, std::ios::out | std::ios::app);
                outFile << "[" <<Utils::getCurrentTimestamp() << ":" << levelStr << "] "<< message << std::endl;
                outFile.flush();
                outFile.close();
        }
}

void LoggerImpl::trace(const std::string&  message, const std::string& logFile)
{
        _logger(loglevel::trace, message, logFile);
}

void LoggerImpl::debug(const std::string&  message, const std::string& logFile)
{
        _logger(loglevel::debug, message, logFile);
}

void LoggerImpl::error(const std::string&  message, const std::string& logFile)
{
        _logger(loglevel::error, message, logFile);
}

LoggerImpl* LoggerImpl::_instance{nullptr};
std::mutex LoggerImpl::_loggerMutex;
