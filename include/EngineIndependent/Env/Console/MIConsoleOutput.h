#pragma once
namespace MI {
	struct ConsoleOutput {
		//Write a TerminalString to the console
		void Write(const char* tstr);

		template<typename TString, typename ...Rest>
		void out(const TString& tstr, const Rest & ... rest) {
			Write(tstr);
			if constexpr (sizeof...(Rest) > 0) { out(rest...); }
		};
	};
}