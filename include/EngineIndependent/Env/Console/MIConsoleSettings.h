#pragma once
#include "MIPlatDefMacro.h"

#ifdef WINDOWS
#include <windows.h>
#else
#include <termios.h>
#endif

namespace MI {

//might be worth separating these into two derived classes of some ConsoleSettingType class (Input and Output).
class ConsoleSettings {
#ifdef WINDOWS
	inline static struct DWORD init_inmode;
	inline static struct DWORD curr_inmode;
	
	inline static struct DWORD init_outmode;
	inline static struct DWORD curr_outmode;
#else
	inline static struct termios init_inmode;
	inline static struct termios curr_inmode;
	
	inline static struct termios init_outmode;
	inline static struct termios curr_outmode;
#endif

public:
	static int Restore();
	static void Initialize();
	
#ifdef WINDOWS
	static int DisjunctInputFlags(__in__ DWORD flags);
	static int ConjunctInputFlags(__in__ DWORD flags);
	
	static int DisjunctOutputFlags(__in__ DWORD flags);
	static int ConjunctOutputFlags(__in__ DWORD flags);
#else
	static int DisjunctInputFlags(tcflag_t flags);
	static int ConjunctInputFlags(tcflag_t flags);
	
	static int DisjunctOutputFlags(tcflag_t flags);
	static int ConjunctOutputFlags(tcflag_t flags);
#endif
};

}
