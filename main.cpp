//#include "MIBuffer.h"
#include "MIString.h"
#include "MIVector.h"
#include "MIArray.h"
#include "MIMap.h"

#include "MIAssert.h"
#include "MIFile.h"

#include "MILogicSequence.h"

#include <iostream>

using namespace MI;
namespace MI {
	struct Config {
		static constexpr bool RunningTestSuite = false; //not done
	};
} //M



//TEST BITCALC
template <size_t bitid>
constexpr size_t BitV() {
	return 1 << bitid;
}
 
template <int bitnum>
struct Bitcalc {

#define CheckBit(id) if constexpr ((bitnum & BitV<id>()) != 0) { return id; }
//#define CheckBits(_0, ...) CheckBit(_0) else CheckBit( __VA_ARGS__)

	/*template<int id>
	constexpr bool CheckB() {
		if constexpr ((bitnum & BitV<id>()) != 0) { return true; }
		return false;
	}

	template <int num1, int ... nums>
	constexpr int CheckBits(int) {
		if constexpr CheckB(num1) { return };
		return CheckBits(nums...);
	}*/


	constexpr int first_on_bit_id() {
		//return CheckBits<0, 1, 2, 3, 4, 5, 6, 7, 8>();
		CheckBit(0)
		else CheckBit(1)
		else CheckBit(2)
		else CheckBit(3)
		else CheckBit(4)
		else CheckBit(5)
		else CheckBit(6)
		else CheckBit(7)
		return -1;
	}
};
// BITCALC

/*
struct Convolution;

struct VirtualExpression {

public:
	VirtualExpression();
	~VirtualExpression();

	virtual VirtualExpression& Morph(const Convolution& conv, VirtualExpression subexpr);
	virtual VirtualExpression& Evaluate();
	virtual void Display();
};

//helpers for testing operators
struct Null {};
//template<typename T, typename Arg> Null operator== (const T&, const Arg&);
template<typename T, typename Arg> Null operator!= (const T&, const Arg&);
*//*

#include "MIDetectionTests.h"
constexpr size_t hashBase = 27;//only count letters
constexpr size_t hashTableSize = 3000;
//returns a numeric representation for a unique character,
//  0 for no character, [1, 26] for letters in english alphabet
size_t HashChar(char c) {
	if ('a' <= c && c <= 'z') {
		return c - 'a' + 1;
	}
	else if ('A' <= c && c <= 'Z') {
		return c - 'A' + 1;
	}
	//The hashing of letter might need changing if you rely on lower/uppercase differences
	return 0;
}
size_t ClassHash(const char * name) {
	long long int hashv = 0;
	for (;	*name != '\0' ; name++) {
		if ('a' <= *name && *name <= 'z' || 'A' <= *name && *name <= 'Z') {
			hashv = hashBase * hashv + HashChar(*name);
		}
	}
	return hashv % hashTableSize;
}
namespace MI {
	struct MetaData {
		String name;
		MetaData() : name() {}
		MetaData(const String& str) : name(str) {}
		~MetaData() {}
		bool Initialized() { return name.Size() == 0; }
	};
}
struct Base {

	static inline Array<MetaData*, hashTableSize> classnames;
	int hashv;

	void Test() {
		printf("Base for %s called%s", classnames[, "\n");
	}
	template <typename Class>
	static void Initialize() {
		//hash the name into the static classname list, 
		int hash = ClassHash(Class::MetaData::name);
		if (!classnames[hash].Initialized()) {
			DoAssertMsg("Hash collision detected");
		}
		classnames.Insert( Class::MetaData , hash);
	}
};

struct A : public Base {
	static struct MetaData {
		const char* name = "B";
	};

	static void Initialize() {
		Base::Initialize<A>();
	}
	void Test() {
		Base::Test();
		printf("%s%s", "A called", "\n");
	}
};

struct B : public A, public Base {
	static struct MetaData {
		const char* name = "B";
	};

	static void Initialize() {
		Base::Initialize<B>();
	}
	void Test() {
		Base::Test();
		A::Test();
		printf("%s%s", "B called", "\n");
	}
};*/


bool greaterThan9(int k) { return k >= 10; };
bool lessThan10(int k) { return k < 10; };

int main(int argc, char* argv[]){
	if constexpr (Config::RunningTestSuite) {
		if (argc != 3) {
			DoAssertMsg("Parameters: InputDir OutputDir (every file with name 'filename', should appear as filename.txt in each directory)");
			return 1;
		}
		//1. Check directory name validity

		//2. Create directory if it does not exist

		/*3. If they do exist, open the files with corresponding names in each directory, use it as sequentially parsed input 
				(PullValue might be handy), should have some kind of special character (or sequence, or even predefined structure)
				indicating points where the user would press enter, or switch input modes (texting vs controller input signal), etc.
		*/
	}
	Vector<char> v;
	v.AddEnd('c');
	std::cout << v[0] << std::endl;

	String str = "hello world!";

	std::cout << str + " hello fools!" + " I am a library god!" << std::endl;
	std::cout << "hello fools! " + str + " I am a library god!" << std::endl;

	str += " hello fools!";
	std::cout << str << std::endl;

	str += " I am a library god!";
	std::cout << str << std::endl;

	String nstr(std::move(str));
	std::cout << nstr << std::endl;
	String nstr2 = std::move(nstr);
	std::cout << nstr2 << std::endl;

	Bitcalc<8> calc;
	std::cout << calc.first_on_bit_id() << std::endl;

	Array<int, 10> arr;
	arr.AddEnd(1);
	arr.AddEnd(2);
	arr.AddEnd(4);
	arr.Insert(3, 2);
	for (int index = 0; index < arr.Size(); index++) {
		std::cout << arr[index] << std::endl;
	}

	ArrayMap<10, int, String> narr;
	narr.Add(1, "hello");	fprintf(stdout, "%s", "done 1");
	
	narr.Add(2, "my");	fprintf(stdout, "%s", "done 2");
	narr.Add(3, "friend");
		fprintf(stdout, "%s", "done 3");
	fprintf(stdout, "%s", "done--");
	for (int index = 1; index <= 3; index++) {
		std::cout << narr[index] << std::endl;
	}

	fprintf(stdout, "%s", "done print--");
	fflush(stdout);
	File mf;
	mf.Open("NEWfile.txt", FMode::out);
	fprintf(stdout, "%s", "file opened--");
	fflush(stdout);
	mf.WriteLine("bruh");
	mf.WriteLine("bruh");
	mf.WriteLine("bruh");
	mf.WriteLine("bruh");
	mf.WriteLine("yeh sweet yokay");

	mf.Close();
	
	mf.Open("NEWfile.txt", FMode::in);

	bool receiving = true;
	for (; receiving; ) {
		printf("%s%s", mf.ReadLine(receiving).c_str() , "\n");
	}
	mf.Close();
	
	
	printf("whole print:\n");
	mf.Open("NEWfile.txt", FMode::in);
	printf("%s%s", mf.Read().c_str(), "\n");
	mf.Close();
	
	/*String a = "a";
	String b = "b";
	String abc = "abc" ;
	ComparisonTester<String> stringcomptest;
	Assert(stringcomptest.Test(a, b, Assertion::Less));
*/

	/*LogicSequence<int, 10> ls;
	ls.StartTrue(1);
	ls.AndNew(7);
	ls.AndNewNot(9); //FALSE
	ls.OrNewNot(13);
	std::cout << (std::boolalpha) << ls.Evaluate(lessThan10, greaterThan9) << std::endl;

*/
	//A aex;
	/*B bex;
	B::Initialize();
	//aex.Test();
	bex.Test();*/
	return 0;
}
