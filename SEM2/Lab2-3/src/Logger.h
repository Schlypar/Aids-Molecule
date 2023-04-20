#pragma once

#include <stdio.h>

#include <iostream>

template <typename... Args> 
void print(Args...args) {}

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
			printf("\033[1m\033[34m[Trace]\t\033[0m");
			printf(message, args...);
			printf("\n");
		}
	}

	template <typename... Args>
	static void Debug(const char* message, Args... args)
	{
		if (priority <= DebugPriority)
		{
			printf("\033[1;32m[Debug]\t\033[0m");
			printf(message, args...);
			printf("\n");
		}
	}

	template <typename... Args>
	static void Info(const char* message, Args... args)
	{
		if (priority <= InfoPriority)
		{
			printf("\033[33m[Info]\t\033[0m");
			printf(message, args...);
			printf("\n");
		}
	}

	template <typename... Args>
	static void Warn(const char* message, Args... args)
	{
		if (priority <= WarnPriority)
		{
			printf("\033[35m[Warn]\t\033[0m");
			printf(message, args...);
			printf("\n");
		}
	}

	template <typename... Args>
	static void Error(const char* message, Args... args)
	{
		if (priority <= ErrorPriority)
		{
			printf("\033[31m[Error]\t\033[0m");
			printf(message, args...);
			printf("\n");
		}
	}

	template <typename... Args>
	static void Fatal(const char* message, Args... args)
	{
		if (priority <= FatalPriority)
		{
			printf("\033[1;31m[Fatal]\t\033[0m");
			printf(message, args...);
			printf("\n");
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
		Logger::Fatal("Invalid pointer");
		break;
	case EXCEPTION_BAD_CONTAINER:
		Logger::Fatal("Bad container");
		break;
	default:
		break;
	}
}