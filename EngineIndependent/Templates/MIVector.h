#pragma once
#include "MIUtilities.h"
#include <iostream>
namespace MI {
	//A bit bulky,, but it does the trick
	template <typename T>
	class Vector {
	protected:
		size_t capElements;
		size_t numElements;
		T* data;

		void Resize(size_t newCap) {
			T* newData = new T[newCap];
			if (data) {
				pcpy(newData, numElements, data);
				delete[] data;
			}
			capElements = newCap; //newSize should never be less than the number of elements
			data = newData;
		}
	public:
		Vector() : capElements{ 0 }, numElements{ 0 }, data{ nullptr } {}
		Vector(size_t startCap) : capElements{ startCap }, numElements{ 0 }, data{ new T[startCap] } {}
		Vector(const T* dat, size_t len, size_t cap = len) : capElements{ cap }, numElements{ len }, data{ new T[cap] } {
			pcpy(data, len, dat);
		}
		~Vector() { delete[] data; }
		size_t Size() const noexcept { return numElements; }
		size_t Capacity() const noexcept { return capElements; }

		//Shrinks the capacity (allocated space) to just enough to hold the current data.
		void FinalizeSize() noexcept { Resize(numElements); }

		//Adds an element to the end and resizes if necessary.
		void AddEnd(const T& obj) {
			if (numElements == capElements) { Resize(2 * ((capElements >= 1) ? capElements : 1)); }
			*(data + numElements) = obj;
			numElements++;
		}

		const T& operator[](const int i) const {
			return data[i];
		}
		T& operator[](const int i) {
			return data[i];
		}
		Vector& operator=(const Vector& vec) {
			if (data != nullptr) {
				delete[] data;
			}
			capElements = vec.capElements;
			numElements = vec.numElements;

			data = new T[capElements]{};
			pcpy(data, numElements, vec.data);
			return this;
		}

		Vector& operator=(Vector&& vec) {
			Swap(capElements, vec.capElements);
			Swap(numElements, vec.numElements);
			Swap(data, vec.data);
			return this;
		}

		class Iterator {
			T* curr;
		public:
			Iterator(const Vector<T>& vec) { curr = vec.data; }
			const Iterator(const T* elm) { curr = elm; };
			Iterator(T* elm) { curr = elm; };

			~Iterator() {}
			bool operator !=(Iterator other) { return curr != other.curr; }

			const T& operator++() const { return *curr++; };
			T& operator++() { return *curr++; };

			const T& operator*() const { return *curr; };
			T& operator*() { return *curr; };
		};

		const Iterator& begin() const { return Iterator(*this); };
		const Iterator& last() const { return Iterator(data + numElements - 1); };
		const Iterator& end() const { return Iterator(data + numElements); };

		//this demonstrates declaring a method for another type on this generic type
		friend std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
			Iterator it = vec.begin();
			for (; it != vec.last(); ++it) {
				os << *it << " ";
			}
			os << *it << std::endl;
			//for ( const T& dat : vec) {
			//	os << dat << std::endl;
			//}
			return os;
		}
	};
}