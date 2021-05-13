#pragma once

//was defining this in the .h for a lot of different things, thought I might as well just put one definition here.
template <typename T>
struct Range {
	T first;
	T last;
	constexpr Range(T e1, T e2) : first{e1}, last{e2} {}
	inline bool Contains(const T& v) { return first <= v && v <= last; }
};

