#include "Utils.h"

std::string Utils::getCurrentTimestamp()
{
    using std::chrono::system_clock;
    auto currentTime = std::chrono::system_clock::now();

    auto transformed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();
    auto millis = transformed % 1000;

    std::time_t tt = system_clock::to_time_t(currentTime);
    auto timeinfo = localtime(&tt);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%F %H:%M:%S", timeinfo);

    char finalBuffer[100];
    snprintf(finalBuffer, sizeof(finalBuffer), "%s:%03d", buffer, static_cast<int>(millis));

    return std::string(finalBuffer);
}
