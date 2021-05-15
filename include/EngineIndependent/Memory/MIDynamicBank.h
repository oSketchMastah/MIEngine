#include "MIBank.h"
#include "MIArrayMap.h"
namespace MI {
	#define FREE_LIST_SIZE 128
	//A cache to be searched to fill in 'gaps' made from deallocations in the stack
	struct FreeList {
		//Should make these sorted, and maybe arrays since it should be safe to write here in destructors SortedArray type maybe
		ArrayMap<char*, int> locsizemap;
		//Vector<void*> locs;
		//Vector<int> sizes;
		//find the gap with this size, or if none exists, then finds the one with the smallest size larger than the given size
		//	returns -1 if no freed size is small enough to fit 
		Node& FindBestMatch(long unsigned int size);
	};
	
	template <typename T>
	T Get(void *mem) { return reinterpret_cast<T>(mem); }
	
	//Bank that can handle deallocations with some grace (be conservative with what you deallocate tho).
	//	Currently, will check all freed sizes (gaps) for a perfect match on every allocation
	class DynamicBank : public Bank { //overloading operators from Bank to be able to deallocate and allocate into dealloc blocks
		//can cast to a Bank to get simple allocation behavior prior to any deallocations in the program.
		FreeList freed;
		
		void *GetAlloc(long unsigned int sizealloc) {
			void* m_alloc;
			auto& [loc, size] = FreedList.FindBestMatch(sizealloc);
			if (iMatch != -1) {
				m_alloc = loc;
				if (size == sizealloc) {
					freed.locsizemap.Delete(loc);
				} else {
					loc += sizealloc;
					size -= sizealloc;
				}
			} else {
				m_alloc = Bank::end;
				Bank::end = reinterpret_cast<char*>(Bank::end) + sizealloc;
			}
			return m_alloc;
		}
	public:
		//allocates an aligned bank of memory
		void Initialize(int bytesize, int bytealign = BYTEALIGN);
		~DynamicBank();
		
		template <typename T>
		T* Allocate() {
			long unsigned int sizealloc = sizeof(T) + (BYTEALIGN - sizeof(T) % BYTEALIGN);
			return Get<T*>(GetAlloc(sizealloc));
		}
		template <typename T>
		T* Allocate(long unsigned int neededsize) {
			long unsigned int sizealloc = neededsize + (BYTEALIGN - neededsize % BYTEALIGN);
			return Get<T*>(GetAlloc(sizealloc)); //GetAlloc handles filling deallocated blocks
		}
		
		//deallocate: might need adjustment to handle combining deallocated chunks upon removal
		void Deallocate(void *mem, long unsigned int size) {
			long unsigned int sizealloc = neededsize + (BYTEALIGN - neededsize % BYTEALIGN);
			char * cp = reinterpret_cast<char*>(mem);
			//Proc: look up memory surrounding this chunk
			int i = freed.locsizemap.Search(cp);
			
			Node& pnode = freed.locsizemap.GetNodeByIndex(i-1); //the node that may be at the start.
			Node& nnode = freed.locsizemap.GetNodeByIndex(i); //the node that may be at the end.
			
			//pmem + psize == cp. Invariant: ... < pnode.key < cp < nnode.key < ...  (cp should not already be deallocated)
			//	-> Any way of collapsing two consecutive keys into one will maintain this.
			if (pnode.key + pnode.value == cp) {//combine previous chunk to this
				if (cp + sizealloc == nnode.key) { //combine both chunks into first (delete the second)
					pnode.value += sizealloc + nnode.value;
					freed.locsizemap.Delete(nnode); //should move cache stuff to BinarySearch so that this takes no time
				} else {//combine just the first node
					pnode.value += sizealloc;
				}
			}
			else if (cp + sizealloc == nnode.key) {//just combine the next node
				nnode.key = cp;
				nnode.value += sizealloc;
			} else {
				freed.locsizemap.Add(cp, sizealloc);
			}
		};
	};
}
