#pragma once
#include "MIAssert.h"

namespace MI {
	template <typename T, size_t MAX_SIZE>
	struct Array {
		size_t m_size = 0;
		T m_arr[MAX_SIZE] {};

	public:
		Array() {}
		~Array() {}

		void Clear() { m_size = 0; }

		size_t Size() { return m_size; }

		void Shift(int index, const int offset = 1) {
			Assert(m_size + offset < MAX_SIZE + 1)
			T* temp = m_arr + m_size - 1 + offset;
			for (; temp >= m_arr + index + offset; temp--) {
				*temp = *(temp - offset);
			}
			for (; temp >= m_arr + index; temp--) {
				*temp = T{};
			}
			m_size += offset;
		}

		void inline AddEnd(const T& elm) {
			m_arr[m_size] = elm;
			m_size++;
		}

		void inline Insert(const T& elm, const int index) {
			Shift(index, 1);
			m_arr[index] = elm;
		}

		T& Back() {
			return m_arr[m_size - 1];
		}

		const T& operator[](int index) const { return m_arr[index]; }
		T& operator[](int index) { return m_arr[index]; }
	};

}