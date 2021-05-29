#pragma once
#include "MIBank.h"
#include <stdio.h>
#include <typeinfo>
#include <type_traits>

namespace MI {
	
	//To detect when T has a helper for collision testing (important that T is comparable to a string iff that is the hash key).
	template <typename T>
	concept StringComparable = requires (const T & ex) { ex == "example"; };

	template <typename T>
	concept IndirectStringComparable = requires (const T & ex) { *ex == "example"; };

	//To detect when T is defaulted (nullptr if a pointer type, or default initialized for solid types)
	template <typename T>
	concept InnerComparable = requires (const T & obj1, const T & obj2) {
		obj1 == obj2;
	};


	//StringHashMap: holds all the mapped data.
	template<typename T, unsigned int SIZE>
	class StringHashMap {
		T* hashmap;
	public:
		//Allocates space for the specified number of T (SIZE) in the hash map.
		void Initialize( Bank & bank) {
			if constexpr (!InnerComparable<T>) {
				fprintf(stdout, "\033[31mError: objects of type %s cannot be compared against other objects of this type.\033[0m\n", typeid(T).name());
			}
			if constexpr (!std::is_pointer_v<T>) {
				if constexpr (!(StringComparable<T> || IndirectStringComparable<T>)) {
					fprintf(stdout, "\033[33mWarning: the type %s, nor its indirection could be compared to a string for collision checking in StringHashMap\033[0m\n", typeid(T).name());
				}
			} else if constexpr (!IndirectStringComparable<T>) {
				//T must either not be a pointer and have some sort of comparability with strings, or it is a pointer with indirection comparable to strings.
				fprintf(stdout, "\033[33mWarning: the type pointed to be the pointer type %s, cannot be compared to a string for collision checking in StringHashMap\033[0m\n", typeid(T).name());
			}
			hashmap = bank.Allocate<T>(SIZE);
			return;
		}
		int Hash(const char* str) {
			int hashv = 0;
			for (const char * temp = str; *temp != '\0'; temp++) {
				hashv = (hashv * 128 + *str) % SIZE;
			}
			T* pdata = hashmap + hashv;
			if constexpr (!std::is_pointer_v<T>) {
				if constexpr (IndirectStringComparable<T>) {
					for (; **pdata != "" && **pdata != str; pdata++, hashv++) {//should only be true iff object is not defaulted
						continue;
					}
				}
				else if constexpr (StringComparable<T>) {
					for (; *pdata != "" && *pdata != str; pdata++, hashv++) {//should only be true iff object is not defaulted
						continue;
					}
				}
			}
			else {
				for (; *pdata != nullptr && **pdata != str; pdata++, hashv++) {//should only be true iff object is not defaulted
					continue;
				}
			}
			return hashv;
		}
		T& HashStructure(int hashid) {
			return *(hashmap + hashid);
		}
		T& HashStructure(const char* str) {
			return HashStructure(Hash(str));
		}
		int Register(const char* str, T init) {
			int hash = Hash(str);
			*(hashmap + hash) = init;
			return hash;
		}
	};
}