#pragma once

#define DISPROVE_CHECK(x) if (!(x)) {\
fprintf(stderr, "\033[31mFAILED CHECK - %s\n       FILE: %s\n       LINE: %d\033[0m\n", #x, __FILE__, __LINE__);\
return false;\
}


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
	bool testedForward = false; //flag to know when to test backwards
	bool Test(T1 obj1, T2 obj2, Assertion assert) {
		if (assert == Assertion::Equal) {
			if constexpr (EqDefined<T1, T2>) {
				DISPROVE_CHECK(obj1 == obj2)
			}
			if constexpr (NotEqDefined<T1, T2>) {
				DISPROVE_CHECK(!(obj1 != obj2))
			}
			if constexpr (LessDefined<T1, T2>) {
				DISPROVE_CHECK(!(obj1 < obj2))
			}
			if constexpr (GreaterDefined<T1, T2>) {
				DISPROVE_CHECK(!(obj1 > obj2))
			}
			if constexpr (LeqDefined<T1, T2>) {
				DISPROVE_CHECK((obj1 <= obj2))
			}
			if constexpr (GeqDefined<T1, T2>) {
				DISPROVE_CHECK((obj1 >= obj2))
			}
		}
		if (assert == Assertion::Less) {
			if constexpr (EqDefined<T1, T2>) {
				DISPROVE_CHECK(!(obj1 == obj2))
			}
			if constexpr (NotEqDefined<T1, T2>) {
				DISPROVE_CHECK((obj1 != obj2))
			}
			if constexpr (LessDefined<T1, T2>) {
				DISPROVE_CHECK((obj1 < obj2))
			}
			if constexpr (GreaterDefined<T1, T2>) {
				DISPROVE_CHECK(!(obj1 > obj2))
			}
			if constexpr (LeqDefined<T1, T2>) {
				DISPROVE_CHECK((obj1 <= obj2))
			}
			if constexpr (GeqDefined<T1, T2>) {
				DISPROVE_CHECK(!(obj1 >= obj2))
			}
		}
		if (assert == Assertion::Greater) {
			if constexpr (EqDefined<T1, T2>) {
				DISPROVE_CHECK(!(obj1 == obj2))
			}
			if constexpr (NotEqDefined<T1, T2>) {
				DISPROVE_CHECK((obj1 != obj2))
			}
			if constexpr (LessDefined<T1, T2>) {
				DISPROVE_CHECK(!(obj1 < obj2))
			}
			if constexpr (GreaterDefined<T1, T2>) {
				DISPROVE_CHECK((obj1 > obj2))
			}
			if constexpr (LeqDefined<T1, T2>) {
				DISPROVE_CHECK(!(obj1 <= obj2))
			}
			if constexpr (GeqDefined<T1, T2>) {
				DISPROVE_CHECK((obj1 >= obj2))
			}
		}
		if (!testedForward) {
			testedForward = true;
			if (assert == Assertion::Less) {
				return Test(obj2, obj1, Assertion::Greater);
			}
			else if (assert == Assertion::Greater) {
				return Test(obj2, obj1, Assertion::Less);
			}
			else {
				return Test(obj2, obj1, Assertion::Equal);
			}
		}
		return true;
	}
};

template <typename T1, typename T2 = T1>
bool IsLogicallyValid(T1 o1, T2 o2) {
	return !((o1 == o2 && o1 != o2) || (!(o1 == o2) && !(o1 != o2)) || 
		(o2 == o1 && o2 != o1) || (!(o1 == o2) && !(o1 != o2)));
}


//order defines relative states. o1 can be less than OR equal to o2, same between o2 and o3.
template <typename T1, typename T2 = T1, typename T3 = T2>
bool TriCompare(T1 o1, T2 o2, T3 o3) {
	ComparisonTester<T1, T2> test1;
	DISPROVE_CHECK(IsLogicallyValid(o1, o2))
	else if (o1 == o2) {
		DISPROVE_CHECK(test1.Test(o1, o2, Assertion::Equal))
	} else {
		DISPROVE_CHECK(test1.Test(o1, o2, Assertion::Less))
	}
	
	ComparisonTester<T2, T3> test2;
	DISPROVE_CHECK(IsLogicallyValid(o2, o3))
	else if (o2 == o3) {
		DISPROVE_CHECK(test2.Test(o2, o3, Assertion::Equal))
	} else {
		DISPROVE_CHECK(test2.Test(o2, o3, Assertion::Less))
	}
	
	ComparisonTester<T1, T3> test3;
	DISPROVE_CHECK(IsLogicallyValid(o1, o3))
	else if (o1 == o3) {
		DISPROVE_CHECK(test3.Test(o1, o3, Assertion::Equal))
	} else {
		DISPROVE_CHECK(test3.Test(o1, o3, Assertion::Less))
	}
	return true;
};
