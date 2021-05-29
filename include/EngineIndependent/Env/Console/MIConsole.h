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
	/*A method for getting access to different components of the console (Ex. include "MIConsoleOutput.h", then you can use Get<ConsoleOutput>())
	*types accessable using Get (specialized for in implementation), and their include file:
	*	- Get<ConsoleOutput>()	 -	MIConsoleOutput.h
	*	- Get<ConsoleInput>()	 -	MIConsoleInput.h
	*	- Get<ConsoleSettings>() -	MIConsoleSettings.h
	*/
	template <typename T>
	T& Get();
	
	
	/*Sets up the console, might include installing our own buffer in the current terminal, setting up virtual sequence interpretation (in windows)
	*   Note: Windows sets the arbitrary limit for outbufsize to 16385 (2^14), which makes writing to console significantly faster on any OS.
	*   In non-Windows OS, increasing beyond this might be worth it if you are doing advanced console animations, or are just doing 
	*	a lot of console writing and don't mind using the extra memory. You can flush stdout to see anything written by an application.
	*   IMPORTANT: this will modify the underlying console (running the app) with its effects until another call to Initialize or setvbuf.
	*/
	static int Initialize(const int inbufsize = -1, const int outbufsize = 16385);
private:
	struct Impl;
	UniquePtr<Impl> pImpl;
};

};
