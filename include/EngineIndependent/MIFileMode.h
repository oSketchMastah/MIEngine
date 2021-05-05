#pragma once
namespace MI {
	//this gets a file open mode string from a bunch of bitflags, Ex. F
	struct FMode {
		//constant bitflags that can be or'd together to combine and get a Mode
		static constexpr char in = 1;
		static constexpr char out = 2;
		static constexpr char app = 4; //append to file in out mode, repositioning is ignored
		static constexpr char txt = 8; //full buffering
		static constexpr char ovw = 16; //overwrite the file if it exists

		FMode(const char c) noexcept;
		FMode(const FMode& c);

		operator const char* () const noexcept { return modeStr; }
		char modeStr [4] = "";
	};

	//pretty much used all the time

	//open for reading and writing, binary mode (doesn't append, or overwrite any files)
	static FMode DefaultFileMode = FMode::in | FMode::out;
	static FMode inmode = FMode::in;
	static FMode outmode = FMode::out;
}