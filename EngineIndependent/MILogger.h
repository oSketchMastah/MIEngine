#pragma once
#include "MIUniquePtr.h"
#include "MIComparable.h"
//SeverityLevel helps to
namespace MI {
	struct SeverityLevel : Comparable<SeverityLevel, char> {
		static constexpr char Marker = 0; //Just logged to make sure things are happening
		static constexpr char Info = 1; //Information for when you need to see more about what's happening
		static constexpr char Warning = 2; //something bad
		static constexpr char Error = 3; //worse than warnings
		static constexpr char Deadly = 4; //Something terrible has happened
		SeverityLevel(char);
	};

	/*Logger provides an interface to control report and flow of error messages in Engine,
	can be used to send log errors, get data for fixme emails, etc. */
	struct Logger {
		~Logger();
		//configure these constexpr's if you want something different / add more & wrap checks around function body
		static constexpr bool ReportAssertions = true;
		static constexpr bool WriteLogs = false;
		//A macro of equivalent name calls these, giving the actual file and line to the logger
		void DoAssert(const bool predtv, const char* msg, const char* file, int line); //basic assert
		void DoFatalAssert(const bool predtv, const char* msg, const char* file, int line); //fatal assert
		//report a severity, only do the assert if given level is above reporting level
		void SetLogLevel(const char* str) {}
		void SetAssertLevel(const char* str) {}
		//initializes a file for logging of errors at a level at or above the given level (up to the next reporting level)
		void Initialize(SeverityLevel level, const char* logfname);
		void DoSeverityAssert(SeverityLevel lev, const bool predtv, const char* msg, const char* file, int line);
		
		static Logger& Instance();
	private:
		Logger();
		struct Impl;
		
		static Logger* loggerInstance;
		UniquePtr<Impl> pImpl;
	};
}