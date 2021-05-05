#pragma once
#include "MILogger.h"

namespace MI {
static Logger& logs = Logger::Instance();
}
//assert the truth of a condition, if false, show the failed condition (pls make assert conditions readable)
#define Assert(x) MI::logs.DoAssert(x, #x, __FILE__, __LINE__);
#define AssertMsg(x, msg) MI::logs.DoAssert(x, msg, __FILE__, __LINE__)
#define DoAssertMsg(msg) MI::logs.DoAssert(0, msg, __FILE__, __LINE__ )
#define SevAssert(severityLevel, x) MI::logs.DoSeverityAssert(severityLevel, x, #x, __FILE__, __LINE__)
#define SevAssertMsg(severityLevel, x, msg) MI::logs.DoSeverityAssert(severityLevel, x, msg, __FILE__, __LINE__)
#define DoSevAssertMsg(severityLevel, msg) MI::logs.DoSeverityAssert(severityLevel, 0, msg, __FILE__, __LINE__)
