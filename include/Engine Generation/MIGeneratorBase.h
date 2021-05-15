#include "MIFile.h"
class Generator {
	File input;
	File output;
public:
	void Initialize(const char* inf, const char* outf, int inbuf = 16385, int outbuf = 16385);
	void SetStopOnMatch(const char* v);
};
