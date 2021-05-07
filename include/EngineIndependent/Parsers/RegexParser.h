namespace MI {

enum class RegexType {
	Determinate, //a subpattern that occurs exactly once in the position
	FixedComposition, //repeating a subpattern a fixed number of times
	Composition, //repeating the subpattern >= 0 times
	Possible //subpattern may occur (or not)
}

struct RegexPart {
	String pattern;
	RegexType ptype;
};

struct Regex {
	Vector<RegexPart> parts;
};

class RegexParser {
	char __swapspot;
	char * parsestr;
public: 
	RegexParser();
	~RegexParser();
	void BeginParsing(char * str);
	RegexPart NextPart();
	void StopParsing();
};

};//MI

