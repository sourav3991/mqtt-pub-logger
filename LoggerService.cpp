#include "Logger.h"
#include <unistd.h>
#include <thread>

Logger DEBUG_LOG("GlobalMessages.log", loglevel::debug, true);

std::mutex cout_mutex;

void print_from_thread(const std::string& thread_name, int count) {
    for (int i = 0; i < count; ++i) {
        // Lock the mutex before printing
        //std::lock_guard<std::mutex> lock(cout_mutex);
	DEBUG_LOG << " this is a test message :" << i << std::endl;
        std::cout << "Message " << i << " from " << thread_name << std::endl;
    }
}

int main() {
    // Number of messages to print from each thread
    int message_count = 10;

    // Create two threads
    std::thread thread1(print_from_thread, "Thread 1", message_count);
    std::thread thread2(print_from_thread, "Thread 2", message_count);

    // Wait for both threads to finish
    thread1.join();
    thread2.join();

    return 0;
}

/*
int main()
{
	DEBUG_LOG << " this is a test message 1" << std::endl;
	DEBUG_LOG << " this is a test message 2" << std::endl;
	DEBUG_LOG << " this is a test message 3" << std::endl;
	DEBUG_LOG << " this is a test message 4" << std::endl;
	sleep(3);
}*/
