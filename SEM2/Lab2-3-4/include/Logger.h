#pragma once

#include <stdio.h>

#include <iostream>

template <typename... Args>
void print(Args... args)
{
}

template <typename T, typename... Args>
void print(T& data, Args... args)
{
	if (sizeof...(args) == 0)
		std::cout << data;
	else
		std::cout << data;

	print(args...);
}

typedef int Exception;

#define EXCEPTION_BAD_LOGIC 2
#define EXCEPTION_INDEX_OUT_OF_RANGE 1
#define EXCEPTION_BAD_POINTER -1
#define EXCEPTION_BAD_CONTAINER -2

enum LogPriority
{
	TracePriority,
	DebugPriority,
	InfoPriority,
	WarnPriority,
	ErrorPriority,
	FatalPriority
};

class Logger
{
protected:
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
			fprintf(stdout, "\033[1m\033[34m[Trace]\t\033[0m");
			fprintf(stdout, message, args...);
			fprintf(stdout, "\n");
		}
	}

	template <typename... Args>
	static void Debug(const char* message, Args... args)
	{
		if (priority <= DebugPriority)
		{
			fprintf(stdout, "\033[1;32m[Debug]\t\033[0m");
			fprintf(stdout, message, args...);
			fprintf(stdout, "\n");
		}
	}

	template <typename... Args>
	static void Info(const char* message, Args... args)
	{
		if (priority <= InfoPriority)
		{
			fprintf(stdout, "\033[33m[Info]\t\033[0m");
			fprintf(stdout, message, args...);
			fprintf(stdout, "\n");
		}
	}

	template <typename... Args>
	static void Warn(const char* message, Args... args)
	{
		if (priority <= WarnPriority)
		{
			fprintf(stderr, "\033[35m[Warn]\t\033[0m");
			fprintf(stderr, message, args...);
			fprintf(stderr, "\n");
		}
	}

	template <typename... Args>
	static void Error(const char* message, Args... args)
	{
		if (priority <= ErrorPriority)
		{
			fprintf(stderr, "\033[31m[Error]\t\033[0m");
			fprintf(stderr, message, args...);
			fprintf(stderr, "\n");
		}
	}

	template <typename... Args>
	static void Fatal(const char* message, Args... args)
	{
		if (priority <= FatalPriority)
		{
			fprintf(stderr, "\033[1;31m[Fatal]\t\033[0m");
			fprintf(stderr, message, args...);
			fprintf(stderr, "\n");
		}
	}
};

inline void logException(Exception e)
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
		case EXCEPTION_BAD_LOGIC:
			Logger::Fatal("Logic was ruined");
			break;
		default:
			break;
	}
}