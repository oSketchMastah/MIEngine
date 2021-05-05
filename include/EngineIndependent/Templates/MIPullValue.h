#pragma once
namespace MI {
	// get a single digit int from it's ascii char representation
	inline int ctoi(char c) {
		return c - '0';
	}

	//******************************************************************************************************************************
	/** PullValue template -
	*	    pass in an ascii parsing pointer, extract a numerical primitive 'R' from this string
	*		and advance the pointer to the position after any digits/parts composing the extracted value.
	*/
	template <typename R>
	inline R PullValue(const char*& strV) {
		R temp = 0;
		const bool isNegative = (*strV == '-' ? true : false);
		switch (isNegative) {
		case true:
			strV++;
			for (; '0' <= *strV && *strV <= '9'; strV++) {
				temp = temp * 10 - ctoi(*strV);
			}
		default:
			for (; '0' <= *strV && *strV <= '9'; strV++) {
				temp = temp * 10 + ctoi(*strV);
			}
		}

		//Handle floating points
		if (*strV == '.') {
			strV++;
			if constexpr (std::is_floating_point_v<R>) { //constexpr test for floating point property (can make this true for own types)
				double temp2 = 0;
				double divider = 10;
				switch (isNegative) {
				case true:
					for (; '0' <= *strV && *strV <= '9'; strV++, divider *= 10) {
						temp2 = 10 * temp2 - ctoi(*strV);
					}
				default:
					for (; '0' <= *strV && *strV <= '9'; strV++, divider *= 10) {
						temp2 = 10 * temp2 + ctoi(*strV);
					}
				}
				temp += temp2 / divider;
			}
			else {
				if (*strV >= '5') {
					temp = (isNegative ? temp - 1 : temp + 1);
				}
				while ('0' <= *strV && *strV <= '9') { continue; } //do this for parsing requirement
			}
		}
		return temp;
	}
	/*decides return from first character, parses past all a-z or A-Z characters if first is a letter,
		default assumption is false, '1', 't', or 'T' will trigger true.*/
	template <>
	inline bool PullValue(const char*& strV) {
		if ('a' <= *strV && *strV <= 'z' || 'A' <= *strV && *strV <= 'Z') {
			bool ret = false;
			switch (*strV) {
			case 'T':
				ret = true; break;
			case 't':
				ret = true; break;
			}
			while ('a' <= *strV && *strV <= 'z' || 'A' <= *strV && *strV <= 'Z') { continue; }
			return ret;
		}
		bool ret = false;
		switch (*strV) {
		case '1':
			ret = true; break;
		}
		strV++;
		return ret;
	}
}