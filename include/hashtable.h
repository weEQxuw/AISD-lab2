#pragma once
#include <iostream>
#include <random>
#include <cmath>
#include <vector>

using namespace std;

namespace hashtable 
{

	template <typename T>
	struct Node {
		T value;
		Node* next;

		Node(const T& value, Node<T>* next = nullptr) : value(value), next(next) {}
	};


	template <typename T>
	class LinkedList {
	private:
		Node<T>* _head;
	public:

		Node<T>* get_head() const {
			return _head;
		}

		LinkedList() : _head(nullptr) {}

		LinkedList(const LinkedList& lst) : _head(nullptr) {
			auto ptr = lst._head;
			while (ptr) {
				push_tail(ptr->value);
				ptr = ptr->next;
			}
		}

		LinkedList(T key) {
			_head = new Node(key);
		}

		LinkedList<T>& operator=(const LinkedList<T>& lst) {
			LinkedList<T> a(lst);
			std::swap(a._head, _head);
			return *this;
		}

		~LinkedList() {
			while (_head) {
				auto ptr = _head;
				_head = _head->next;
				delete ptr;
			}

			_head = nullptr;
		}


		void push_head(const T value) {
			_head = new Node<T>(value, _head);
		}

		void pop_head() {
			if (!_head) {
				throw out_of_range("List is empty");
			}
			auto ptr = _head;
			_head = _head->next;
			delete ptr;
			ptr = nullptr;

		}

		void push_tail(const T value) {
			if (_head == nullptr) {
				_head = new Node<T>(value, nullptr);
				return;
			}
			Node<T>* ptr = _head;
			while (ptr->next != nullptr) {
				ptr = ptr->next;
			}
			ptr->next = new Node<T>(value, nullptr);
		}

		void push_tail(const LinkedList<T>& lst) {
			auto ptr = lst._head;
			while (ptr) {
				this->push_tail(ptr->value);
				ptr = ptr->next;
			}
		}

		void push_head(const LinkedList<T>& lst) {
			LinkedList<T> copy(lst);
			copy.push_tail(*this);
			std::swap(_head, copy._head);
		}

		void pop_tail() {
			if (!_head) {
				throw out_of_range("List is empty");
			}
			auto ptr = _head;
			while (ptr->next->next) {
				ptr = ptr->next;
			}
			delete ptr->next;
			ptr->next = nullptr;
		}

		void print() const {

			auto ptr = _head;

			while (ptr) {
				cout << ptr->value << endl;
				ptr = ptr->next;
			}
		}

		size_t size() const {
			auto ptr = _head;
			size_t size = 0;
			while (ptr) {
				ptr = ptr->next;
				size++;
			}
			return size;
		}

		T operator[](size_t index) const {
			if (index < 0 || index >= this->size()) {
				throw out_of_range("Index it out of range");
			}
			auto ptr = _head;
			for (int i = 0; i < index; i++) {
				ptr = ptr->next;
			}
			return ptr->value;
		}

		T& operator[](size_t index) {
			if (index < 0 || index >= this->size()) {
				throw out_of_range("Index it out of range");
			}
			auto ptr = _head;
			for (int i = 0; i < index; i++) {
				ptr = ptr->next;
			}
			return ptr->value;
		}

		void delete_node(const T& value) {
			auto ptr = _head;
			while (_head && _head->value == value) {
				ptr = _head->next;
				delete _head;
				_head = ptr;
			}
			Node<T>* dop_ptr;
			while (ptr && ptr->next) {
				if (ptr->next->value == value) {
					dop_ptr = ptr->next;
					ptr->next = ptr->next->next;
					delete dop_ptr;
					dop_ptr = nullptr;
				}
				else {
					ptr = ptr->next;
				}
			}
		}

		bool contains(T val) const {
			auto ptr = _head;
			while (ptr) {
				if (val == ptr->value) {
					return true;
				}
				ptr = ptr->next;
			}
			return false;
		}

		Node<T>* search(T val) const {
			auto ptr = _head;
			while (ptr) {
				if (val == ptr->value) {
					return ptr;
				}
				ptr = ptr->next;
			}
			return nullptr;
		}
	};

	template <typename T>
	ostream& operator<<(ostream& a, const LinkedList<T>& lst) {
		Node<T>* ptr = lst.get_head();
		while (ptr) {
			a << ptr->value << " ";
			ptr = ptr->next;
		}
		return a;
	}


	struct Pair {
		unsigned int key;
		int info;

		Pair() : key(rand()), info(rand()) {};
		Pair(unsigned int k) : key(k), info(rand()) {};
		Pair(unsigned int k, int inf) : key(k), info(inf) {};

		bool operator==(Pair rhs) {
			return this->key == rhs.key;
		}
	};

	ostream& operator<<(ostream& os, Pair p) {
		if (p.info == -1)
		{
			os << p.key << " ";
			return os;
		}
		os << "(" << p.key << ", " << p.info << ") ";
		return os;
	}


	class HashTable {
	private:

		unsigned int a;
		vector<LinkedList<Pair>> _table;
		size_t _capacity;

	public:

		vector<LinkedList<Pair>> get_table() const {
			return _table;
		}

		void swap(HashTable& other) {
			std::swap(a, other.a);
			std::swap(_table, other._table);
			std::swap(_capacity, other._capacity);
		}

		int get_size() const {
			int size = 0;
			for (LinkedList<Pair> elem : _table) {
				size = size + elem.size() - 1;
			}
			return size;
		}

		int get_capacity() const {
			return pow(2, _capacity);
		}

		LinkedList<Pair> operator[](int i) const {
			return _table[i];
		}

		HashTable() {
			a = rand();
			_capacity = 0;
			_table.push_back(Pair(0, -1));
		}

		

		HashTable(const HashTable& other) {
			this->a = other.a;
			this->_capacity = other._capacity;
			for (LinkedList<Pair> elem : other._table) {
				_table.push_back(elem);
			}
		}

		HashTable(size_t capacity) {
			a = rand();
			this->_capacity = capacity;
			for (int i = 0; i < pow(2, _capacity); ++i) {
				_table.push_back(LinkedList(Pair(i, -1)));
			}
		}

		HashTable& operator=(const HashTable& other) {
			HashTable a(other);
			swap(a);
			return *this;
		}

		~HashTable() {
			_table.clear();
		}

		void print() {
			for (LinkedList<Pair> elem : _table) {
				cout << "Key: " << elem << endl;
			}
		}

		unsigned int hash_function(unsigned int value) const {
			return (value * a % 65536 >> (16 - _capacity)); // 65536 - 2^16 взял длину битовго слова 16
		}

		double load_factor() {
			return 1.0 * this->get_size() / this->get_capacity();
		}

		void set_capacity(int capacity) {
			if (capacity > 16 || capacity < 0) {
				throw "The table size has been exceeded or an incorrect value has been passed";
			}
			HashTable new_table(capacity);
			for (LinkedList<Pair> elem : _table) {
				Node<Pair>* ptr = elem.get_head()->next;
				while (ptr) {
					new_table.insert_value(ptr->value);
					ptr = ptr->next;
				}
			}
			swap(new_table);
		}

		bool insert_value(Pair p) {
			if (contains(p.key)) {
				return false;
			}
			_table[hash_function(p.key)].push_tail(p);
			if (load_factor() >= 0.75) {
				set_capacity(_capacity + 1);
			}
			return true;
		}

		bool contains(unsigned int value) const {
			return _table[hash_function(value)].contains(value);
		}

		Node<Pair>* search(unsigned int value) const {
			return _table[hash_function(value)].search(value);
		}

		int count(unsigned int key) const {
			if (key >= pow(2, _capacity)) {
				return -1;
			}
			return _table[key].size() - 1;
		}

		bool erase(unsigned int value) {
			if (!contains(value)) {
				return false;
			}
			_table[hash_function(value)].delete_node(value);
			return true;
		}
	};


	void hash_task()
	{
		HashTable T;
		int summa = 0;
		size_t size = 1000;
		for (int i = 0; i < size; ++i) 
		{
			unsigned int b = rand();
			if (T.contains(b)) 
			{
				summa++;	
			}
			T.insert_value(b);
			cout << b << " ";
		}
		cout << "\n" << summa << " --- Количество повторяющихся элементов" << endl;
	}
}