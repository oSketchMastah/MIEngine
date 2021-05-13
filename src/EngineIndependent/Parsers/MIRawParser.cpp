#include "MIRawParser.h"
using namespace MI;
	//char __swapspot;
	//char * pstr;
void RawParser::BeginParsing(char * str) {
	pstr = str;
}

void RawParser::MarkIsolationPoint() {
	isorv = pstr;
}

char * RawParser::IsolateHere() {
	__swap = *pstr;
	swapspot = pstr;
	*pstr = '\0';
	return isorv;
}

void RawParser::UnIsolate() {
	*swapspot = __swap;
}
	
void RawParser::Advance(int totalbytes) { pstr += totalbytes; }
bool RawParser::IsInRange(const char low, const char high) {
	return low <= *pstr && *pstr <= high;
}

void RawParser::SkipRange(const char low, const char high) {
	for (; IsInRange(low, high) ; pstr++) { continue; }
}
	
bool RawParser::IsAtValue(const char v) {
	return *pstr == v;
}

void RawParser::SkipValue(const char v) {
	for (; IsAtValue(v) ; pstr++) { continue; }
}

/*if the initial substring (from current position of parser) compares equal to the given string
	 then returns the size, else returns 0. */
int RawParser::IsAtString(const char* v){
	int rv = 0;
	for(const char *temp = pstr; *v != '\0' && *temp == *v ; v++, rv++, temp++) { continue; }
	return (*v == '\0') ? rv : 0;//v will have advanced to end iff every character compared equal.
}

//if the initial substring is the given string, skips it. (just once, unlike other "Skip" methods)
bool RawParser::SkipString(const char* v) {
	int rv = IsAtString(v);
	pstr += rv;
	return rv;
}
char RawParser::operator*() {
	return *pstr;
}
void RawParser::operator++(int) {
	pstr++;
}



