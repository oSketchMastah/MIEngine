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

	}
	void DoSeverityAssert(SeverityLevel lev, const char* msg, const char* file, int line) {

	}
	
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
		snprintf(pImpl->StaticAssertMsg__, sizeof(pImpl->StaticAssertMsg__), "Bad Assert %s - at %s %d", msg, file, line);
		if constexpr (ReportAssertions) {
			assert(0, pImpl->StaticAssertMsg__);
		}
		if constexpr (WriteLogs) {
			pImpl->WriteLog(SeverityLevel::Error, pImpl->StaticAssertMsg__);
		}
	}
}

void Logger::DoFatalAssert(const bool predtv, const char* msg, const char* file, int line) {
	snprintf(pImpl->StaticAssertMsg__, sizeof(pImpl->StaticAssertMsg__), "Bad Assert %s - at %s %d", msg, file, line);
	assert(0, pImpl->StaticAssertMsg__);
	if constexpr (WriteLogs) {
		pImpl->WriteLog(SeverityLevel::Error, pImpl->StaticAssertMsg__);
	}
}

void Logger::DoSeverityAssert(SeverityLevel lev, const bool predtv, const char* msg, const char* file, int line) {

}

void Logger::Initialize(SeverityLevel level, const char* logfname) {

}
