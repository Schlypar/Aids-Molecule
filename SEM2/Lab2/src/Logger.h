#pragma once

#include <stdio.h>

typedef int Exception;

#define EXCEPTION_INDEX_OUT_OF_RANGE 1
#define EXCEPTION_BAD_POINTER -1
#define EXCEPTION_BAD_CONTAINER -2

enum LogPriority
{
	TracePriority, DebugPriority, InfoPriority,
	WarnPriority, ErrorPriority, FatalPriority
};

class Logger
{
private:
	static LogPriority priority;

public:

	static void setPriority(LogPriority newPriority)
	{
		priority = newPriority;
	}

	template <typename... Args>
	static void Trace(const char* message, Args... args)
	{
		if (priority <= TracePriority)
		{
			printf("[Trace]\t");
			printf(message, args...);
			printf("\n");
		}
	}

	template <typename... Args>
	static void Debug(const char* message, Args... args)
	{
		if (priority <= DebugPriority)
		{
			printf("[Debug]\t");
			printf(message, args...);
			printf("\n");
		}
	}

	template <typename... Args>
	static void Info(const char* message, Args... args)
	{
		if (priority <= InfoPriority)
		{
			printf("[Info]\t");
			printf(message, args...);
			printf("\n");
		}
	}

	template <typename... Args>
	static void Warn(const char* message, Args... args)
	{
		if (priority <= WarnPriority)
		{
			printf("[Warn]\t");
			printf(message, args...);
			printf("\n");
		}
	}

	template <typename... Args>
	static void Error(const char* message, Args... args)
	{
		if (priority <= ErrorPriority)
		{
			printf("[Error]\t");
			printf(message, args...);
			printf("\n");
		}
	}

	template <typename... Args>
	static void Fatal(const char* message, Args... args)
	{
		if (priority <= FatalPriority)
		{
			printf("[Fatal]\t");
			printf(message, args...);
			printf("\n");
		}
	}
};

LogPriority Logger::priority = TracePriority;

void logException(Exception e)
{
	switch (e)
	{
	case EXCEPTION_INDEX_OUT_OF_RANGE:
		Logger::Error("index out of range");
		break;
	case EXCEPTION_BAD_POINTER:
		Logger::Error("Invalid pointer");
		break;
	case EXCEPTION_BAD_CONTAINER:
		Logger::Error("Bad container");
		break;
	default:
		break;
	}
}