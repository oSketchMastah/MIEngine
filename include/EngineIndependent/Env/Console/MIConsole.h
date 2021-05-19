#pragma once
#include "MIUniquePtr.h"

namespace MI {

struct ConsoleOutput;
struct ConsoleInput;

/*model: the console is a subject, animations are listeners
*	(ie. animations sign themselves up (to be animated), and expose some method(s)
*	for the console to update the animation & get the display form)
*/

struct Console {
	Console(){}
	~Console();
	//A method for getting access to different components of the console
	template <typename T>
	T& Get();
	//ConsoleOutput&	Get<ConsoleOutput>();
	//ConsoleInput&	Get<ConsoleInput>();

	//Sets up the console, might include installing our own buffer in the current terminal, setting up virtual sequence interpretation (in windows)
	static int Initialize(const int inbufsize = 512, const int outbufsize = 16385);
private:
	struct Impl;
	UniquePtr<Impl> pImpl;
};

};
