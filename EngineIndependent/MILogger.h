#pragma once
#include "MIUniquePtr.h"
#include "MIComparable.h"
namespace MI {
	//SeverityLevel helps in conditional warnings, for example:  some severity level might serve as
	//		a boundary for what gets reported in early development vs pre-release (ex. missing art)
	struct SeverityLevel : Comparable<SeverityLevel, char> {
		static constexpr char Marker = 0; //Just logged to make sure things are happening
		static constexpr char Info = 51; //Information for when you need to see more about what's happening
		static constexpr char WarningWeak = 102; //a non-critical warning, may be shown in release builds
		static constexpr char WarningStrong = 153; //something bad, probably worth being reported in a warning log
		static constexpr char Error = 204; //worse than warnings, critical errors that need attention
		static constexpr char Deadly = 255; //Something terrible has happened

#ifdef _DEBUG
		static constexpr char MinAssertLevel = WarningWeak;
#elif NDEBUG
		static constexpr char MinAssertLevel = WarningWeak;
#else
#error make sure a MinAssertLevel is setup in MILogger.h
#endif

		SeverityLevel(char);
	};

	/*Logger provides an interface to control report and flow of error messages in Engine,
	can be used to send log errors, get data for fixme emails, etc. */
	struct Logger {
		~Logger();
		//configure these constexpr's if you want something different / add more & wrap checks around function bodies
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