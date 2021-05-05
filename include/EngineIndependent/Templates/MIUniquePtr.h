#pragma once
namespace MI {
	//A UniquePtr class that works with incomplete types, 
	//   ie. this should not need to know anything specific about T, it's just a heap object
	template <typename T>
	struct UniquePtr {
		UniquePtr() { m_pObj = new T; }
		UniquePtr(T* pObj) { m_pObj = pObj; }
		~UniquePtr() { delete m_pObj; }

		bool IsInitialized() { return m_pObj != nullptr; }

		T* operator->() const noexcept { return m_pObj; }
		T& Get() const noexcept { return *m_pObj; }
	private:
		T* m_pObj = nullptr;
	};


	template <typename T, typename ...ArgT>
	static UniquePtr<T> MakeUnique(ArgT...args) {
		T* obj = new T{ args... };
		return UniquePtr<T>(obj);
	};
}