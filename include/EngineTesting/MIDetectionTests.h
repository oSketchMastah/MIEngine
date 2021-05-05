#pragma once
//concepts to check that certain members are defined
template <typename T1, typename T2>
concept EqDefined = requires(const T1 & obj1, const T2 & obj2) { obj1.operator==(obj2); };

template <typename T1, typename T2>
concept NotEqDefined = requires(const T1 & obj1, const T2 & obj2) { obj1.operator!=(obj2); };

template <typename T1, typename T2>
concept LessDefined = requires(const T1 & obj1, const T2 & obj2) { obj1.operator<(obj2); };

template <typename T1, typename T2>
concept GreaterDefined = requires(const T1 & obj1, const T2 & obj2) { obj1.operator>(obj2); };

template <typename T1, typename T2>
concept LeqDefined = requires(const T1 & obj1, const T2 & obj2) { obj1.operator<=(obj2); };

template <typename T1, typename T2>
concept GeqDefined = requires(const T1 & obj1, const T2 & obj2) { obj1.operator>=(obj2); };

//Strong assertions, which imply the result of any defined comparison
enum class Assertion { Equal, Less, Greater };

//this here will test that all defined comparison operators agree with each other, 
//  and an expected (asserted) relative state (Equal, less than, or greater than)
template <typename T1, typename T2 = T1>
struct ComparisonTester {
	
	bool Test(T1 obj1, T2 obj2, Assertion assert) {
		bool result = true;
		if (assert == Assertion::Equal) {
			if constexpr (EqDefined<T1, T2>) {
				result = (obj1 == obj2); if (!result) { return false; }
			}
			if constexpr (NotEqDefined<T1, T2>) {
				result = !(obj1 != obj2); if (!result) { return false; }
			}
			if constexpr (LessDefined<T1, T2>) {
				result = !(obj1 < obj2); if (!result) { return false; }
			}
			if constexpr (GreaterDefined<T1, T2>) {
				result = !(obj1 > obj2); if (!result) { return false; }
			}
			if constexpr (LeqDefined<T1, T2>) {
				result = (obj1 <= obj2); if (!result) { return false; }
			}
			if constexpr (GeqDefined<T1, T2>) {
				result = (obj1 >= obj2); if (!result) { return false; }
			}
		}
		if (assert == Assertion::Less) {
			if constexpr (EqDefined<T1, T2>) {
				result = !(obj1 == obj2); if (!result) { return false; }
			}
			if constexpr (NotEqDefined<T1, T2>) {
				result = (obj1 != obj2); if (!result) { return false; }
			}
			if constexpr (LessDefined<T1, T2>) {
				result = (obj1 < obj2); if (!result) { return false; }
			}
			if constexpr (GreaterDefined<T1, T2>) {
				result = !(obj1 > obj2); if (!result) { return false; }
			}
			if constexpr (LeqDefined<T1, T2>) {
				result = (obj1 <= obj2); if (!result) { return false; }
			}
			if constexpr (GeqDefined<T1, T2>) {
				result = !(obj1 >= obj2); if (!result) { return false; }
			}
		}
		if (assert == Assertion::Greater) {
			if constexpr (EqDefined<T1, T2>) {
				result = !(obj1 == obj2); if (!result) { return false; }
			}
			if constexpr (NotEqDefined<T1, T2>) {
				result = (obj1 != obj2); if (!result) { return false; }
			}
			if constexpr (LessDefined<T1, T2>) {
				result = !(obj1 < obj2); if (!result) { return false; }
			}
			if constexpr (GreaterDefined<T1, T2>) {
				result = (obj1 > obj2); if (!result) { return false; }
			}
			if constexpr (LeqDefined<T1, T2>) {
				result = !(obj1 <= obj2); if (!result) { return false; }
			}
			if constexpr (GeqDefined<T1, T2>) {
				result = (obj1 >= obj2); if (!result) { return false; }
			}
		}
		return true;
	}
};