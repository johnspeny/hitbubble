#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <cstring>
#include <cstdarg>

#define CLOG(...) Logger::log(__FILE__, __LINE__, __VA_ARGS__)

class Logger
{
public:
	static void log(const char* filename, int lineNumber, const char* format, ...)
	{
		// Extract the filename from the full path
		const char* lastSlash = std::strrchr(filename, '/');
		const char* lastBackslash = std::strrchr(filename, '\\');
		const char* shortFilename = (lastSlash || lastBackslash) ? ((lastSlash > lastBackslash) ? lastSlash + 1 : lastBackslash + 1) : filename;

		va_list args;
		va_start(args, format);

		char buffer[512]; // Adjust the buffer size as needed
		std::vsprintf(buffer, format, args);

		va_end(args);

		std::cout << "[" << shortFilename << ":" << lineNumber << "] " << buffer << "\n";
	}
};

#endif // LOGGER_H
