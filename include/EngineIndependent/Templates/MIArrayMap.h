#pragma once
#include "MIArray.h"
namespace MI {

	/*array map for KEY types with a defined <, !=, == operators.
	* Every operation with a different key object from the last updates a single-key cache, so sequential workings with the same key only take O(1) time.
	*/
	template<size_t MAX_SIZE, typename KEY, typename VALUE>
	class ArrayMap {
		struct Node {
			KEY key = KEY{};
			VALUE value = VALUE{};
			//Node(const KEY& k, const VALUE val) : key{ k }, value{ val } {}
		};
		Array<Node, MAX_SIZE> arr{};
		Node * cachenode;
		
		int BinarySearch(const KEY& key) {
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
		//this will return pointer to either 1. Node with this key, 2. position a Node with such a key would occupy + cache the searched value
		int Search(const KEY& key) {
			int searchv = BinarySearch(key);
			cachenode = &arr[searchv];
			return searchv;
		}
		
		ArrayMap() {};
		~ArrayMap() {
		};

		const VALUE& operator[](const KEY& key) const {
			if (cachenode->key == key) {
				return cachenode->value;
			} else {
				Search(key);
				if (cachenode->key == key) {
					return cachenode->value;
				} else {
					return nullptr;
				}
			}
		}
		
		Node& GetNodeByIndex(int index) {
			return &arr[index];
		}
		Node& GetNode(const KEY& key) const {
			if (cachenode->key != key) {
				Search(key);
			}
			//might not be instantiated (if no node with key has been added)
			return *cachenode;
		}
/*		Node& GetNodeLessPref(const KEY& key) const {
			if (cachenode->key != key) {
				cachenode = &arr[BinarySearch(key)];
			}
			//might not be instantiated (if no node with key has been added)
			return *(cachenode - 1);
		}
		Node& GetNodeGreaterPref(const KEY& key) const {
			if (cachenode->key != key) {
				//check if cachenode is an overshoot or an undershoot respectively, just do the binary search if so.
				if ((cachenode != &m_arr[0] && (cachenode - 1)->key > key)
					 || (cachenode != m_arr[m_size-1] && (cachenode + 1)->key < key) {
					cachenode = &arr[BinarySearch(key)];
				}
				//Check if this or the right cachenode 
				else if (cachenode == &m_arr[0])
				
				}
				if (arr[index].key > key) {
					return &arr[BinarySearch(key)]
				}
				cachenode = &arr[BinarySearch(key)];
			}
			//might not be instantiated (if no node with key has been added)
			return *(cachenode + 1);
		}*/
		KEY& GetKey(int index) { return arr[index].key; };

		VALUE operator[](const KEY& key) {
			if (cachenode->key == key) {
				return cachenode->value;
			} else {
				Search(key);
				if (cachenode->key == key) {
					return cachenode->value;
				} else {
					return nullptr;
				}
			}
		}


		void Add(const KEY& key, const VALUE& val = VALUE{}) {
			Assert(key != KEY{});

			const int newIndex = Search(key);
			if (arr[newIndex].key != key) { //if they were the same, we assume it's be a mistake to overwrite it
				arr.Insert( Node{key, val} , newIndex);
			}
		}
		void Delete(const KEY& key) {
			if (cachenode == key) {
				arr.Remove(cachenode);
				cachenode = &arr[(arr.Size() - 1)/2];
				return;
			}
			arr.Remove(Search(key));
		}

	};
}
