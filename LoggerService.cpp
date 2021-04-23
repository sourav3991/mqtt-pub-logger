#include "Logger.h"

Logger DEBUG_LOG("GlobalMessages.log", loglevel::debug, true);

int main()
{
	DEBUG_LOG << "sourav " << std::endl;
}
