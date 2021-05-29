#include "MIString.h"
#include "MIVector.h"
#include "MIArray.h"
#include "MIArrayMap.h"
#include "MISymbolBank.h"

#include "MIAssert.h"
#include "MIFile.h"

#include "MIRawParser.h"
#include "MIBank.h"
#include "MIBankString.h"
#include "MIRange.h"

#include "MILogicSequence.h"
#include "MIDetectionTests.h"
#include "MIStringUtils.h"

#include "MIStringHashMap.h"
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

template <>
bool Test<RawParser>() {
	RawParser parser;
	String teststr = "a +b*( 10 + y)";
	parser.BeginParsing(teststr.RawMemory());
	parser.IsAtValue('a');
	DISPROVE_CHECK(parser.IsAtValue('a'))
	parser.SkipSeq('a', ' ');
	DISPROVE_CHECK(parser.IsAtValue('+'))
	parser.SkipAny('+', 'b', '*', '(');
	DISPROVE_CHECK(parser.IsAtValue(' '))
	parser.Skip(' ');
	DISPROVE_CHECK(parser.IsAtValue('1'))
	parser.MarkIsolationPoint();
	parser.Skip(Range{'0', '9'});
	String isotestv = parser.IsolateHere();
	DISPROVE_CHECK(isotestv == "10");
	parser.UnIsolate();
	parser.SkipSeq(' ', '+', ' ');
	DISPROVE_CHECK(*parser == 'y');
	parser++;
	DISPROVE_CHECK(*parser == ')');
	
	fprintf(stdout, "\033[32mRawParser test passed\033[0m\n");
	return true;
}

void BankAllocString(Bank& bank, const char* str) {
	size_t rawlen = MI::strlen(str) + 1;
	char * alloc_str = bank.Allocate<char>(rawlen);
	MI::strcpy(alloc_str, rawlen, str);
}
template <>
bool Test<Bank>() {
	Bank b;
	b.Initialize(4096);
	BankAllocString(b, "henlo frends");
	String testv = "henlo frends";
	DISPROVE_CHECK(testv == reinterpret_cast<char*>(b.GetBank()))
	fprintf(stdout, "\033[32mBank test passed\033[0m\n");
	return true;
}

template <>
bool Test<BankString>() {
	SymbolBank bank;
	bank.Initialize(4096);
	BankString bankstring = bank.Allocate("henlo frends");

	String testv = "henlo frends";
	DISPROVE_CHECK(bankstring == testv.c_str())
	
	String testv2 = "henlo again";
	BankString bankstr2 = bank.Allocate("henlo again");

	DISPROVE_CHECK(bankstring == testv.c_str() && bankstr2 == testv2.c_str());
	
	BankString l1 = bank.Allocate("a");
	BankString b1 = bank.Allocate("b");
	BankString m1 = bank.Allocate("abc");

	DISPROVE_CHECK(TriCompare<BankString>(l1, m1, b1))

	fprintf(stdout, "\033[32mSymbolBank/BankString test passed\033[0m\n");
	return true;
}

template<typename T, typename ...Rest>
bool TestAll_Impl() {
	if (!Test<T>()) {
		//failed test for T
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

#include "MIConsole.h"


int main(int argc, char* argv[]){
	Console::Initialize();
	
	if (TestAll<	String, 
			Vector<char>, 
			Array<int, 10>, 
			ArrayMap<10, int, String>, 
			File, 
			LogicSequence<int, 10>,
			RawParser,
			Bank,
			BankString
		   >()) {
		return 0;
	}
	return 1;
}
