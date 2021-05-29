#pragma once

template <typename T>
struct DLinkList {
	struct Node {
		Node* prev = nullptr;
		Node* next = nullptr;
		T val = T{};

		void BreakLeft() {
			prev->next = next;
			prev = nullptr;
		}
		void BreakRight() {
			next->prev = prev;
			next = nullptr;
		}
	};
	Node* first;
	Node* last;

	void AddBegin(T val);
	void AddEnd(T val);

	void AddBefore(T val, Node* node);
	void AddAfter(T val, Node* node);

	void InsertBefore(Node* ins, Node* node);
	void InsertAfter(Node* ins, Node* node);

	Node* begin();
	Node* end();
};



template<typename T>
void DLinkList<T>::AddBegin(T val) {
	first->prev = new Node{ nullptr, first, val };
}

template<typename T>
void DLinkList<T>::AddEnd(T val) {
	last->next = new Node{ last, nullptr, val };
}

template<typename T>
void DLinkList<T>::AddBefore(T val, Node* node) {
	InsertBefore(new Node{ node->prev, node, val }, node);
}
template<typename T>
void DLinkList<T>::AddAfter(T val, Node* node) {
	InsertAfter(new Node{ node, node->next, val }, node);
}

template<typename T>
void DLinkList<T>::InsertBefore(Node* ins, Node* node) {
	if (node->prev != nullptr) {
		node->prev->next = ins;
	}
	node->prev = ins;
}
template<typename T>
void DLinkList<T>::InsertAfter(Node* ins, Node* node) {
	if (node->next != nullptr) {
		node->next->prev = ins;
	}
	node->next = ins;
}
template<typename T>
DLinkList<T>::Node* DLinkList<T>::begin() {
	return first;
}
template<typename T>
DLinkList<T>::Node* DLinkList<T>::end() {
	return nullptr;
}