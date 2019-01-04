#pragma once

#include <functional>
#include <vector>

namespace hexagon {

template<typename T, typename = std::enable_if<std::is_default_constructible<T>::value>>
class Pool {
public:

	using PoolContainer = std::vector<T>;
	using IterateFunc = std::function<void(T&)>;
	using ConstIterateFunc = std::function<void(const T&)>;
	using RemovalFunc = std::function<bool(const T&)>;

	Pool(size_t poolSize)
		: _pool(poolSize) {}

	virtual ~Pool() = default;

	size_t GetNumberOfElements() const { return _index; }

	T& At(size_t index) { return _pool[index]; }
	const T& At(size_t index) const { return _pool[index]; }

	T& Add() {
		return AddImpl(_pool, T{}, _index);
	}

	T& Add(T& elem) {
		return AddImpl(_pool, elem, _index);
	}

	T& Add(T&& elem) {
		return AddImpl(_pool, std::move(elem), _index);
	}

	void Iterate(const IterateFunc& func) {
		IterateImpl(_pool, func, _index);
	}

	void Iterate(const ConstIterateFunc& func) const {
		IterateImpl(_pool, func, _index);
	}

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

protected:

	// Both const and non-const implementation of iterate function
	template<typename Container, typename Func>
	static void IterateImpl(Container& pool, Func func, size_t size) {
		for (size_t i = 0u; i < size; i++) {
			func(pool[i]);
		}
	}

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