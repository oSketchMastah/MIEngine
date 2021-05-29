#pragma once
#include "MIRange.h"
#include "MIMVector.h"
namespace MI {

	template<typename T, unsigned int index, unsigned int dims>
	struct BoundsImpl;

	template <unsigned int dims, typename T>
	struct Bounds : protected BoundsImpl<T, 0, dims> {
		bool Contains(MVector<dims, T> vec) {
			if (BoundsImpl<T, 0, dims>::Contains(vec)) {

			}
			BoundsImpl<Range<T>, dims>::Contains(vec)
		}
	};

	template<typename T, unsigned int dims>
	struct BoundsImpl<T, dims, dims> : public Range<T> {
		bool Contains(const T* entries) {
			if (first < *entries < last) {
				return true;
			}
			return false;
		}
	};

	template<typename T, int index, unsigned int dims>
	struct BoundsImpl : public Range<T>, public BoundsImpl<T, index + 1, dims> {
		BoundsImpl<T, index+1, dims> Next() { return BoundsImpl<T, index+1, dims> }
		bool Contains(const T* entries) {
			if (first <= *entries <= last) {
				return Next().Contains(entries + 1);
			}
			return false;
		}
	};

	template<typename T, typename ... Args>
	struct BoundsImpl {
		T arg;
	};
}