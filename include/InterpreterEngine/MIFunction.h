#pragma once
namespace MI {

//A non-virtual, non-templated class wrapping arbitrary functions on user types

class Object {
	
	struct Method {
		Object<T> * owner;
		void * mp;
		template<typename Rv, typename Args...>
		Method(Rv (T::* mp) (Args...)) : mp{mp} {
		}
		
		template<typename Rv, typename Args...>
		Rv operator()(Args... args) {
			return (owner->(*(reinterpret_cast<Rv (T::*) (Args...)>(mp))))(args...);
		}
	};
	
	template <typename Rv, typename ...Args>
	Rv Do (Method* mthd, Args ... args) {
		(*this->mthd)(
	}
};

}template<typename T>
