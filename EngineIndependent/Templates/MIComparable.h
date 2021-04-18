#pragma once
/*give a class type, and a basic type, using CRTP this signs up this class
to be comparable amongst itself using a type T to which it delegates for comparisons*/
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