#pragma once
#define BYTEALIGN 8
namespace MI {
//stores Bank types, ie. Allocate things here if you want them to be stored for the same duration as the Bank
//	make sure you're not using types that delete their data (ie. no dynamic types).
	class Bank {
	protected:
		void * bank;
		void * end;
	public:
		void * GetBank() { return bank; }
		//allocates an aligned bank of memory
		void Initialize(int bytesize, int bytealign = BYTEALIGN);
		~Bank();
		
		template <typename T>
		T* Allocate() {
			T* m_alloc = reinterpret_cast<T*>(end);
			end = reinterpret_cast<char*>(end) + sizeof(T) + (BYTEALIGN - sizeof(T) % BYTEALIGN);
			return m_alloc;
		}
		template <typename T>
		T* Allocate(long unsigned int neededsize) {
			T* m_alloc = reinterpret_cast<T*>(end);
			end = reinterpret_cast<char*>(end) + neededsize + (BYTEALIGN - neededsize % BYTEALIGN);
			return m_alloc;
		}
	};

}
