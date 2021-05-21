#pragma once
#include <cstdio>
#define BYTEALIGN 8

#define BANK_WATCH_CAP

namespace MI {
//stores Bank types, ie. Allocate things here if you want them to be stored for the same duration as the Bank
//	make sure you're not using types that delete their data (ie. no dynamic types).
	class Bank {
	protected:
#if defined(WIN32) || defined(WIN64)
		//because windows can't handle some standards
		void* allocbank;
		void * bank;
#else
		void* bank;
#endif
		void * end;
		int cap;
	public:
		void * GetBank() { return bank; }
		//allocates an aligned bank of memory
		void Initialize(int bytesize, int bytealign = BYTEALIGN);
		~Bank();
		
		template <typename T>
		T* Allocate() {
			T* m_alloc = reinterpret_cast<T*>(end);
			end = reinterpret_cast<char*>(end) + sizeof(T) + (BYTEALIGN - sizeof(T) % BYTEALIGN);
#ifdef BANK_WATCH_CAP
			if (static_cast<int>((long unsigned int)(reinterpret_cast<char*>(bank) + cap) - (long unsigned int)(reinterpret_cast<char*>(end))) < 0) {
				fprintf(stdout, "\033[31mBuffer Overflow: File: %s Line: %d\033[0m\n", __FILE__, __LINE__);
			}
#endif
			return m_alloc;
		}
		/*template <typename T>
		T* Allocate(long unsigned int neededsize) {
			T* m_alloc = reinterpret_cast<T*>(end);
			end = reinterpret_cast<char*>(end) + neededsize + (BYTEALIGN - neededsize % BYTEALIGN);
			return m_alloc;
		}*/
		template <typename T>
		T* Allocate(long unsigned int needed) {
			long unsigned int neededbytes = sizeof(T) * needed;
			T* m_alloc = reinterpret_cast<T*>(end);
			end = reinterpret_cast<char*>(end) + neededbytes + (BYTEALIGN - neededbytes % BYTEALIGN);
#ifdef BANK_WATCH_CAP
			if (static_cast<int>((long unsigned int)(reinterpret_cast<char*>(bank) + cap) - (long unsigned int)(reinterpret_cast<char*>(end))) < 0) {
				fprintf(stdout, "\033[31mBuffer Overflow: File: %s Line: %d\033[0m\n", __FILE__, __LINE__);
			}
#endif
			return m_alloc;
		}
	};

}
