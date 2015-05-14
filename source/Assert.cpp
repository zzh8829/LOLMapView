#include "Assert.h"

static const size_t MAX_MESSAGE_BYTES = 1024;

void Assert (bool condition, const char* file, int line, const char* format, ...)
{
	if (condition)
	{
		// The assertion is satisfied.
		return;
	}

	// The message prefix.
	char message[MAX_MESSAGE_BYTES];
	snprintf(message, MAX_MESSAGE_BYTES, "\nAssert failed at %s(%d):\n", file, line);

	// Append user-specified arguments.
	va_list arguments;
	va_start(arguments, format);
	size_t length = strlen(message);
	vsnprintf(message + length, MAX_MESSAGE_BYTES-length, format, arguments);
	va_end(arguments);

	std::cerr << message << std::endl;
}