#pragma once
#include "MIArray.h"

namespace MI {

	//a mathematical vector type.
	template <unsigned int dims, typename T>
	struct MVector {
		Array<T, dims> data;

		template <typename T, T ... Entries>
		MVector(T arg, T ... rest);
		~MVector{}
	}

	template <typename T, T ... Entries>
	MVector::MVector(T arg, T ... rest) : data{ arg, rest... } {}


}