#pragma once
/*give a basic type, deriving from this class makes it comparable against a type T to which it delegates
*	for comparisons. This is only really intended for structures that need quick comparison functionality*/
namespace MI {
	template<typename T>
	struct Comparable {
		Comparable(T c) : obj{ c } {}
		Comparable(const Comparable& other) { obj = other.obj; }
		~Comparable() {}
		operator T() { return obj; }
		Comparable operator< (const T& other) { return obj < other; }
		Comparable operator> (const T& other) { return obj > other; }
		Comparable operator== (const T& other) { return obj == other; }
		Comparable operator!= (const T& other) { return obj != other; }
	private:
		T obj;
	};
}
