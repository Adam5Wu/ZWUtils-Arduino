/*
 * Simple LinkedList
 * Zhenyu Wu (Adam_5Wu@hotmail.com)
 *
 * Inspired by ESPAsyncWebServer project by Hristo Gochkov.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
 */
#ifndef ZWUtils_LinkedList_H_
#define ZWUtils_LinkedList_H_

#include "stddef.h"
#include <functional>

template <typename T>
class LinkedListNode {
		T _value;
	public:
		LinkedListNode<T>* next;

		LinkedListNode(const T &val): _value(val), next(nullptr) {}
		LinkedListNode(T &&val): _value(std::move(val)), next(nullptr) {}

		T const &value(void) const { return _value; }
		T& value(void){ return _value; }
};

template <typename T, template<typename> class ItemT = LinkedListNode>
class LinkedList {
	public:
		typedef ItemT<T> ItemType;

		class Iterator {
				ItemType* _node;
			public:
				Iterator(ItemType* current = nullptr) : _node(current) {}
				Iterator(const Iterator& i) : _node(i._node) {}
				Iterator& operator ++(void)
				{ return (_node = _node? _node->next : _node), *this; }
				operator bool() const { return _node; }
				bool operator != (const Iterator& i) const
				{ return _node != i._node; }
				const T* operator -> (void) const
				{ return _node? &_node->value() : nullptr; }
				T const &operator * (void) const { return _node->value(); }
		};

		typedef const Iterator ConstIterator;

		typedef std::function<bool(const T&)> Predicate;
		typedef std::function<bool(ConstIterator&, ConstIterator&)> DuoPredicate;
		typedef std::function<void(T&)> Modifier;
		typedef std::function<bool(T&)> Modicate;

	protected:
		ItemType *_head, *_tail;
		size_t _count;
		Modifier _onRemove;

		size_t _addhead(ItemType *it) {
			if (it) {
				it->next = _head;
				_head = it;
				if (!_tail) _tail = it;
				return _count++;
			}
			return _count;
		}

		size_t _addtail(ItemType *it) {
			if (it) {
				if (_tail) _tail->next = it;
				_tail = it;
				if (!_head) _head = it;
				return _count++;
			}
			return _count;
		}

		size_t _addBetween(ItemType *it, DuoPredicate const &predicate) {
			if (it) {
				ItemType *A = nullptr;
				ItemType *B = _head;
				while (true) {
					if (predicate(A, B)) {
						if (!A) return _addhead(it);
						if (!B) return _addtail(it);
						A->next = it;
						it->next = B;
						it = nullptr;
						break;
					}
					A = B;
					if (B) B = B->next;
					else break;
				}
				if (it) delete(it);
				else return _count++;
			}
			return _count;
		}

		typedef std::function<bool(ItemType *)> EnumStep;
		void enumerate(EnumStep const &enumstep) const {
			auto next = _head;
			while (auto it = next) {
				next = it->next;
				if (!enumstep(it)) break;
			}
		}

		void remove(ItemType *prev, Predicate const &takeown) {
			auto it = prev ? prev->next : _head;
			if (prev) prev->next = it->next;
			else _head = it->next;
			if (_tail == it) _tail = prev;
			_count--;

			bool handoff = takeown && takeown(it->value());
			if (!handoff && _onRemove) _onRemove(it->value());
			delete it;
		}

	public:
		LinkedList(Modifier const &onRemove)
		: _head(nullptr), _tail(nullptr), _count(0), _onRemove(onRemove) {}
		LinkedList(Modifier const &onRemove, std::initializer_list<T> items)
		: LinkedList(onRemove) { for (auto& item : items) append(item); }
		~LinkedList(void) { clear(); }

		LinkedList(LinkedList &&src)
		: _head(src._head), _tail(src._tail), _count(src._count), _onRemove(std::move(src._onRemove))
		{ src._head = src._tail = nullptr; src._count = 0; }

		LinkedList& operator=(LinkedList &&src) {
			clear();
			_head = src._head; _tail = src._tail; _count = src._count;
			_onRemove = std::move(src._onRemove);
			src._head = src._tail = nullptr; src._count = 0;
		}

		bool isEmpty(void) const { return _head == nullptr; }
		T& front(void) const { return _head->value(); }
		T& back(void) const { return _tail->value(); }
		ConstIterator begin(void) const { return ConstIterator(_head); }
		ConstIterator end(void) const { return ConstIterator(); }

		size_t prepend(T const &t) { return _addhead(new ItemType(t)); }
		size_t prepend(T && t) { return _addhead(new ItemType(std::move(t))); }
		size_t append(T const &t) { return _addtail(new ItemType(t)); }
		size_t append(T && t) { return _addtail(new ItemType(std::move(t))); }

		size_t insert(T const &t, DuoPredicate const &predicate)
		{ return _addBetween(new ItemType(t), predicate); }
		size_t insert(T && t, DuoPredicate const &predicate)
		{ return _addBetween(new ItemType(std::move(t)), predicate); }

		size_t length(void) const { return _count; }

		size_t count_if(Predicate const &predicate) const {
			size_t i = 0;
			enumerate([&](ItemType *it){
				i+= (!predicate || predicate(it->value()))? 1: 0;
				return true;
			});
			return i;
		}

		T* first(void) const
		{ return get_if(nullptr); }

		T* get_if(Predicate const &predicate) const
		{ return get_nth_if(0, predicate); }

		T* get_nth(size_t N) const
		{ return get_nth_if(N, nullptr); }

		T* get_nth_if(size_t N, Predicate const &predicate) const {
			T* Ret = nullptr;
			size_t i = 0;
			enumerate([&](ItemType *it){
				if (!predicate || predicate(it->value()))
					if (i++ == N) {
						Ret = &it->value();
						return false;
					}
				return true;
			});
			return Ret;
		}

		bool remove(T const& item)
		{ return remove_if([&](T const& t){ return item == t; }); }

		bool remove_if(Predicate const &predicate)
		{ return remove_nth_if(0, predicate); }

		bool remove_nth(size_t N, Predicate const &takeown=nullptr)
		{ return remove_nth_if(N, nullptr, takeown); }

		bool remove_nth_if(size_t N, Predicate const &predicate, Predicate const &takeown=nullptr) {
			bool Ret = false;
			ItemType* prev = nullptr;
			size_t i = 0;
			enumerate([&](ItemType *it){
				if (!predicate || predicate(it->value()))
					if (i++ == N) {
						remove(prev, takeown);
						Ret = true;
						return false;
					}
				prev = it;
				return true;
			});
			return Ret;
		}

		bool pop_front(Predicate const &takeown)
		{ return remove_nth_if(0, nullptr, takeown); }

		void clear(void){
			enumerate([&](ItemType *it){
				// Since everything will be removed, don't bother update chain/head
				if (_onRemove) _onRemove(it->value());
				delete it;
				return true;
			});
			_head = _tail = nullptr;
			_count = 0;
		}

		size_t apply(Modicate const &modicate) {
			size_t i = 0;
			enumerate([&](ItemType *it){
				return ++i, modicate(it->value());
			});
			return i;
		}

		size_t prune(Predicate const &predicate, Predicate const &takeown=nullptr) {
			size_t i = 0;
			enumerate([&](ItemType *it){
				if (predicate(it->value())) {
					remove(it, takeown), ++i;
				}
				return true;
			});
			return i;
		}
};

#endif /* LinkedList_H_ */
