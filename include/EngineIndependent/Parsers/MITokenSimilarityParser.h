#pragma once
/*A very minimal Parser, gets first token, then parses while further tokens are in
*  the same similarity group, once an unsimilar token is encountered, breaks and
*  shows the next group of tokens (in the same similarity group).
The idea is to hook this up to more advanced parsers and maintain good performance + abstractions.
*/
namespace MI {

class TokenParser {
	char __swapspot;
	char * parsestr;
public:
	TokenParser();
	~TokenParser();
	/*register the string for parsing, modifications will be made during
	* the process (swapping characters for '\0' temporarily) */
	void BeginParsing(char * str);
	char * NextToken();
	void StopParsing();
};

};//MI
