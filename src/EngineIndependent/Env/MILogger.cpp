#include "MILogger.h"
//#include "common.h"
#include "MIFile.h" //Error logs
#include "MIAssert.h"
#include "MIVector.h"

#include <cassert>
#include <exception>

using namespace MI;
SeverityLevel::SeverityLevel(char level) : Comparable(level) {}

//holds the static instance of logger
struct Logger::Impl {

	char StaticAssertMsg__[512]{};

	Vector<File> logfiles;

	//called for macros that explicity only write logs, when a condition (predtv) is not satisfied
	void DoSeverityAssert(SeverityLevel lev, const bool predtv, const char* msg, const char* file, int line) {
		//deadly and everything larger than error gets put into error right now
		if (lev >= SeverityLevel::Error) {

		}
		else if (lev >= SeverityLevel::WarningStrong) {
			if constexpr (SeverityLevel::MinAssertLevel > SeverityLevel::WarningStrong) { return; }
		}
		else if (lev >= SeverityLevel::WarningWeak) {
			if constexpr (SeverityLevel::MinAssertLevel > SeverityLevel::WarningWeak) { return; }

		}
		//anything less than this probably isn't involved in an Assertion

	}
	/*void DoSeverityAssert(SeverityLevel lev, const char* msg, const char* file, int line) {

	}*/
	
	void WriteLog(SeverityLevel lev, const char* msg, const char* file, int line) {
		
	}

	void WriteLog(SeverityLevel lev, const char* msg) {//msg should include file and line if this is used

	}
	Impl() {}
	~Impl() {}
};


//**************************************************************************************************************

Logger * Logger::loggerInstance = nullptr;

Logger& Logger::Instance() {
	if (!loggerInstance) {
		loggerInstance = new Logger;
	}
	return *loggerInstance;
}

Logger::Logger() : pImpl{} {}
Logger::~Logger() {}

void Logger::DoAssert(const bool predtv, const char* msg, const char* file, int line) {
	if constexpr (ReportAssertions || WriteLogs) {
		if (!predtv) {
			snprintf(pImpl->StaticAssertMsg__, sizeof(pImpl->StaticAssertMsg__), "Bad Assert %s - at %s %d", msg, file, line);
			if constexpr (ReportAssertions) {
				//Probably more should be done here (like __debugbreak(), but portable pls)
				std::cerr << pImpl->StaticAssertMsg__ << std::endl;
				assert(0);
			}
			if constexpr (WriteLogs) {
				pImpl->WriteLog(SeverityLevel::Error, pImpl->StaticAssertMsg__);
			}
		}
	}
}

void Logger::DoFatalAssert(const bool predtv, const char* msg, const char* file, int line) {
	snprintf(pImpl->StaticAssertMsg__, sizeof(pImpl->StaticAssertMsg__), "Bad Assert %s - at %s %d", msg, file, line);
	std::cerr << pImpl->StaticAssertMsg__ << std::endl;
	assert(0);
	if constexpr (WriteLogs) {
		pImpl->WriteLog(SeverityLevel::Error, pImpl->StaticAssertMsg__);
	}
}

void Logger::DoSeverityAssert(SeverityLevel lev, const bool predtv, const char* msg, const char* file, int line) {
	pImpl->DoSeverityAssert(lev, predtv, msg, file, line);
}

void Logger::Initialize(SeverityLevel level, const char* logfname) {

}
