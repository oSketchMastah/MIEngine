#include "MISymbolBank.h"

template <typename VALUE>
class RadixMap {
	struct Node {
			BankString key = KEY{};
			VALUE value = VALUE{};
			//Node(const KEY& k, const VALUE val) : key{ k }, value{ val } {}
			Array<Node, 62> arr{};
	};
	SymbolBank bank;
	Array<Node*, 62> arr{};
		//this will return pointer to either 1. Node with this key, 2. position a Node with such a key would occupy
	int BinarySearch(const char key) {
		int leftBound = 0;
		int rightBound = arr.Size() - 1;
		if (arr.Size() == 0 || key <= arr[leftBound].key) { return 0; }
		else if (arr[rightBound].key == key) { return rightBound; }
		else if (arr[rightBound].key < key) { return rightBound + 1; }
		for (int checkIndex = (leftBound + rightBound) / 2; leftBound != checkIndex && checkIndex != rightBound; checkIndex = (leftBound + rightBound) / 2) {
			if (arr[checkIndex].key == key) { return checkIndex; }
			else if (arr[checkIndex].key < key) { leftBound = checkIndex; }
			else if (key < arr[checkIndex].key) { rightBound = checkIndex; }
		}
		if (key < arr[leftBound + 1].key) { return leftBound + 1; }
		else if (arr[rightBound - 1].key < key) { return rightBound; }
		else { return 0; }
	}
	
	public:
	
		const VALUE& operator[](const char* key) const {
			const Node& kvpair = arr[BinarySearch(key)];
			return (kvpair.key == key) ? &kvpair.value : nullptr;
		}
		
		
		BankString GetKey(int index) { return arr[index].key; };

		VALUE& operator[](const char* key) {
			int i = BinarySearch(key);
			Node& kvpair = arr[i];
			return kvpair.value;
		}
		

		void Add(const char* key, const VALUE& val = VALUE{}) {
			const int newIndex = BinarySearch(*key);
			if (arr[newIndex].key != key) { //if they were the same, we assume it's be a mistake to overwrite it
				arr.Insert( Node{key, val} , newIndex);
			}
		}
};
