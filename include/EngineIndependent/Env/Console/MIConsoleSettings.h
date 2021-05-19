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
	inline static DWORD init_inmode;
	inline static DWORD curr_inmode;
	
	inline static DWORD init_outmode;
	inline static DWORD curr_outmode;
#else
	inline static struct termios init_inmode;
	inline static struct termios curr_inmode;
	
	inline static struct termios init_outmode;
	inline static struct termios curr_outmode;
#endif

public:
	//On Linux, Restore is always failing (todo: fix this, put in destructor of Console)
	//static int Restore();
	static void Initialize();
	
#ifdef WINDOWS
	static int DisjunctInputFlags(DWORD flags);
	static int ConjunctInputFlags(DWORD flags);
	
	static int DisjunctOutputFlags( DWORD flags);
	static int ConjunctOutputFlags( DWORD flags);
#else
	static int DisjunctInputFlags(tcflag_t flags);
	static int ConjunctInputFlags(tcflag_t flags);
	
	static int DisjunctOutputFlags(tcflag_t flags);
	static int ConjunctOutputFlags(tcflag_t flags);
#endif
};

}
