#include "MIString.h"
#include "MIVector.h"
#include "MIArray.h"
#include "MIMap.h"

#include "MIAssert.h"
#include "MIFile.h"

#include "MILogicSequence.h"
#include "MIDetectionTests.h"

#include <iostream>

using namespace MI;
namespace MI {
	struct Config {
		static constexpr bool RunningTestSuite = false; //not done
	};
} //M
template <typename T>
bool Test();

//Add the implementations for the tests of each type here

template <>
bool Test<String>() {

	//Test Comparisons
	String l1 = "a";
	String b1 = "b";
	String m1 = "abc" ;
	
	DISPROVE_CHECK(TriCompare<String>(l1, m1, b1))

	ComparisonTester<String> tester;
	String n = l1 + b1 + "c";
	DISPROVE_CHECK(tester.Test(n, m1, Assertion::Equal))
	//If those last tests passed, assume comparison works..

	//Test move semantics
	String nstr(std::move(m1));
	DISPROVE_CHECK(nstr == "abc")
	DISPROVE_CHECK(m1.GetBuffer() == nullptr)
	
	
	fprintf(stdout, "\033[32mString test passed\033[0m\n");
	return true;
	
}

template <>
bool Test<Vector<char>>() {
	Vector<char> v;
	v.AddEnd('c');
	DISPROVE_CHECK(v[0] == 'c')

	fprintf(stdout, "\033[32mVector test passed\033[0m\n");
	return true;
}

template <>
bool Test<Array<int, 10>>() {
	Array<int, 10> arr;
	arr.AddEnd(1);
	arr.AddEnd(2);
	arr.AddEnd(4);
	arr.Insert(3, 2);
	DISPROVE_CHECK(arr[0] == 1 && arr[1] == 2 && arr[2] == 3 && arr[3] == 4)
	fprintf(stdout, "\033[32mArray test passed\033[0m\n");
	return true;
	
}

template <>
bool Test<ArrayMap<10, int, String>>() {
	ArrayMap<10, int, String> narr;
	narr.Add(1, "hello");
	narr.Add(2, "my");
	narr.Add(3, "friend");
	
	DISPROVE_CHECK(narr[1] == "hello")
	DISPROVE_CHECK(narr[2] == "my")
	DISPROVE_CHECK(narr[3] == "friend")
	fprintf(stdout, "\033[32mArrayMap test passed\033[0m\n");
	return true;
	
}

template <>
bool Test<File>() {
File mf;
	mf.Open("NEWfile.txt", FMode::out);
	mf.WriteLine("bruh");
	mf.WriteLine("bruh");
	mf.WriteLine("bruh");
	mf.WriteLine("bruh");
	mf.WriteLine("yeh sweet yokay");

	mf.Close();
	
	mf.Open("NEWfile.txt", FMode::in);

	bool receiving = true;
	DISPROVE_CHECK(mf.ReadLine(receiving) == "bruh")
	DISPROVE_CHECK(mf.ReadLine(receiving) == "bruh")
	DISPROVE_CHECK(mf.ReadLine(receiving) == "bruh")
	DISPROVE_CHECK(mf.ReadLine(receiving) == "bruh")
	DISPROVE_CHECK(mf.ReadLine(receiving) == "yeh sweet yokay")
	mf.Close();
	
	
	mf.Open("NEWfile.txt", FMode::in);
	DISPROVE_CHECK( mf.Read() == "bruh\nbruh\nbruh\nbruh\nyeh sweet yokay\n" )
	mf.Close();
	fprintf(stdout, "\033[32mFile test passed\033[0m\n");
	return true;
	
}
bool greaterThan9(int k) { return k >= 10; };
bool lessThan10(int k) { return k < 10; };

template <>
bool Test<LogicSequence<int, 10>>() {
	LogicSequence<int, 10> ls;
	ls.StartTrue(1);
	ls.AndNew(7);
	ls.AndNewNot(9); //FALSE
	ls.OrNewNot(13);
	DISPROVE_CHECK(ls.Evaluate(lessThan10, greaterThan9))
	
	fprintf(stdout, "\033[32mLogicSequence test passed\033[0m\n");
	return true;
}

template<typename T, typename ...Rest>
bool TestAll_Impl() {
	if (!Test<T>()) {
		//failed test for T
		fprintf(stderr, "\033[31mFailed Tests\033[0m");
		return false;
	}
	if constexpr (sizeof...(Rest) > 0) {
		return TestAll_Impl<Rest...>();
	} else {
		return true;
	}
}

template <typename ...Types>
bool TestAll() {
	return TestAll_Impl<Types...>();
}

//this and the part before the tests setting up ansi-sequence interpretation should be abstracted out eventually
#if defined( WIN32 ) || defined(WIN64)
#include <Windows.h>
#endif

int main(int argc, char* argv[]){
#if defined( WIN32 ) || defined(WIN64)
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode;
	GetConsoleMode(hOutput, &dwMode);
	dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOutput, dwMode)) {
		perror("SetConsoleMode failed.");
		return 1;
	}
#endif
	if (TestAll<	String, 
			Vector<char>, 
			Array<int, 10>, 
			ArrayMap<10, int, String>, 
			File, 
			LogicSequence<int, 10>
		   >()) {
		return 0;
	}
	return 1;
}
