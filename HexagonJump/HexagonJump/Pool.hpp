#pragma once

#include <functional>
#include <vector>

namespace hexagon {

// Container with non-shrinking removal mechanism
template<typename T, typename = std::enable_if<std::is_default_constructible<T>::value>>
class Pool {
public:

	using PoolContainer = std::vector<T>;
	using RemovalFunc = std::function<bool(const T&)>;

	Pool(size_t poolSize) : _pool(poolSize) {}

	virtual ~Pool() = default;

	size_t Size() const { return _index; }

	T& At(size_t index) { return _pool[index]; }
	const T& At(size_t index) const { return _pool[index]; }

	T& operator[](size_t index) { return _pool[index]; }
	const T& operator[](size_t index) const { return _pool[index]; }

	T& Add() { return AddImpl(_pool, T{}, _index); }

	T& Add(const T& elem) { return const_cast<T&>(AddImpl(_pool, elem, _index)); }

	T& Add(T&& elem) { return AddImpl(_pool, std::move(elem), _index); }

	void RemoveAll(const RemovalFunc& predicate) {
		for (size_t i = 0u; i < _index;) {
			if (predicate(_pool[i])) {
				_index--;
				if (i < _index) {
					std::swap(_pool[i], _pool[_index]);
				}
			}
			else {
				i++;
			}
		}
	}

	typename PoolContainer::iterator begin() { return _pool.begin(); }
	typename PoolContainer::const_iterator begin() const { return _pool.begin(); }
	typename PoolContainer::iterator end() { return _pool.begin() + _index; }
	typename PoolContainer::const_iterator end() const { return _pool.begin() + _index; }

	typename PoolContainer::reverse_iterator rbegin() { 
		return _pool.rbegin() + (_pool.size() - _index); 
	}
	typename PoolContainer::const_reverse_iterator rbegin() const {
		return _pool.rbegin() + (_pool.size() - _index);
	}
	typename PoolContainer::reverse_iterator rend() { return _pool.rend(); }
	typename PoolContainer::const_reverse_iterator rend() const { return _pool.rend(); }

protected:

	template<typename T>
	static T& AddImpl(PoolContainer& pool, T&& elem, size_t& index) {
		if (index >= pool.size()) {
			pool.resize(pool.size() * 2u);
		}
		pool[index++] = std::move(elem);
		return pool[index - 1];
	}

	PoolContainer _pool;
	size_t _index = 0u;
};

}