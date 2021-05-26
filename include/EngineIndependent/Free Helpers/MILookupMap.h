#pragma once
namespace MI {

/*A type that can be used to replace if/else logic with the procedure of using
* a custom parametric equation to map from parameter values to the appropriate function.
* 
* Example. (consider MethodN functions to be defined with signature "void MethodN (char)") you could do:
* 	LookupMap<4, void, char> lookup(Method1, Method2, Method3, Method4);
*	char c = 'a'
*	lookup[c/32](c);
* Which would call Method4, passing it the character c. (This is because lowercase
*	 letters fall in the range where c/32 is exactly 3, the index of the 4th element)
*/
template <int TOTAL, typename RvT, typename ArgT>
struct LookupMap {
	using FuncT = RvT (*)(ArgT);
	FuncT arr[TOTAL];
	
	//Construct an array of this many func
	template< typename ... Funcs >
	constexpr LookupMap(Funcs ... funcs);
	
	constexpr void Set(int index, FuncT func);
	
	//Get the function from a transform on the argument.
	FuncT operator[](int);
private:
	template<int index, typename TFunc, typename ...Funcs>
	constexpr void Initialize(TFunc func, Funcs... funcs);
};


template <int TOTAL, typename Rv, typename Arg>
template<int index, typename TFunc, typename ...Funcs>
constexpr void LookupMap<TOTAL, Rv, Arg>::Initialize(TFunc func, Funcs... funcs ) {
	Set(index, func);
	if constexpr (index != TOTAL - 1) {
		Initialize<index + 1>(funcs...);
	}
}

template <int TOTAL, typename Rv, typename Arg>
constexpr void LookupMap<TOTAL, Rv, Arg>::Set(int index, FuncT func) {
	arr[index] = func;
}

template <int TOTAL, typename Rv, typename Arg>
template<typename ...Funcs>
constexpr LookupMap<TOTAL, Rv, Arg>::LookupMap(Funcs... funcs) {
	LookupMap<TOTAL, Rv, Arg>::Initialize<0>(funcs...);
}

template <int TOTAL, typename Rv, typename Arg>
LookupMap<TOTAL, Rv, Arg>::FuncT LookupMap<TOTAL, Rv, Arg>::operator[](int arg) {
	return arr[arg];
}



}
