/**
 * @file UnorderedSetWithIndexes.hpp
 * @author Tomáš Ludrovan
 * @brief UnorderedSetWithIndexes class
 * @version 0.1
 * @date 2024-03-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef UNORDEREDSETWITHINDEXES_HPP
#define UNORDEREDSETWITHINDEXES_HPP

#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

/**
 * @brief Unordered set whose items can be additionally accessed using
 *        integral indexes.
 * 
 * @tparam T Value type.
 * @tparam Hash Hash function for `T`.
 * @tparam Equal Comparison function for `T`.
 * 
 * @note Inspired from https://stackoverflow.com/a/51973872/22178095.
 */
template <
	typename T,
	class Hash = std::hash<T>,
	class Equal = std::equal_to<T>
>
class UnorderedSetWithIndexes {
private:
	struct Entry {
		T key;    // The stored value
		int idx;  // Index within the vector

		Entry(const T& key_, int idx_)
			: key{key_}
			, idx{idx_}
		{}
		Entry(T&& key_, int idx_)
			: key{std::move(key_)}
			, idx{idx_}
		{}
	};
	typedef std::unique_ptr<Entry> EntryPtr;
	// Function object for Entry hashing
	struct EntryPtrHash {
		size_t operator()(const EntryPtr& k) const noexcept {
			return Hash{}(k->key);
		}
	};
	// Function object for Entry comparison
	struct EntryPtrEqual {
		bool operator()(const EntryPtr& lhs, const EntryPtr& rhs) const {
			return Equal{}(lhs->key, rhs->key);
		}
	};
	typedef typename std::unordered_set<EntryPtr, EntryPtrHash, EntryPtrEqual> SetType;
	typedef typename std::vector<Entry*> VectorType;
public:
	// Declare the generic iterator a friend
	template <class SetIteratorType>
	friend class GenericIterator;
	/**
	 * @brief Iterator for iterating the set elements.
	 * 
	 * @tparam SetIteratorType Should meet the "LegacyForwardIterator" named
	 *         requirement.
	 * 
	 * @remark This class should not be used directly. Instead, use one of the
	 *         typedefs below.
	 */
	template <class SetIteratorType>
	class GenericIterator {
	private:
		SetIteratorType m_it;
	public:
		/**
		 * @brief Constructs a new GenericIterator object.
		 * 
		 * @param it Embedded iterator.
		 */
		GenericIterator(SetIteratorType it)
			: m_it{it}
		{}

		/**
		 * @brief Returns the embedded iterator.
		 * 
		 * @remark This method should not be used outside this module. This
		 *         class and `UnorderedSetWithIndexes` class need to access the
		 *         embedded iterator for different purposes, so it cannot be
		 *         private nor protected, but other than that this method should
		 *         not be used.
		 */
		const SetIteratorType& getNestedIterator() const {
			return m_it;
		}

		/**
		 * @brief Returns the embedded iterator.
		 * 
		 * @remark This method should not be used outside this module. This
		 *         class and `UnorderedSetWithIndexes` class need to access the
		 *         embedded iterator for different purposes, so it cannot be
		 *         private nor protected, but other than that this method should
		 *         not be used.
		 */
		GenericIterator<SetIteratorType>& getNestedIterator() {
			return m_it;
		}

		/**
		 * @brief Increment the iterator.
		 * 
		 * @return Reference to this object.
		 */
		GenericIterator<SetIteratorType>& operator++ () {
			++m_it;
			return *this;
		}

		/**
		 * @brief Increment the iterator.
		 * 
		 * @return This object before increment.
		 * 
		 * @remark The "prefix" variant of this operation should be preferred.
		 */
		GenericIterator<SetIteratorType> operator++ (int) {
			Iterator tmp = *this;
			m_it++;
			return tmp;
		}

		/**
		 * @brief Compare two iterators for equality.
		 * 
		 * @tparam SetIteratorTypeOther A `GenericIterator` with the
		 *         `SetIteratorType` template parameter being either the same,
		 *         or with different constness (`iterator` or `const_iterator`).
		 */
		template <typename SetIteratorTypeOther>
		bool operator== (const GenericIterator<SetIteratorTypeOther>& rhs) const {
			return (this->getNestedIterator() == rhs.getNestedIterator());
		}

		/**
		 * @brief Compare two iterators for inequality.
		 * 
		 * @tparam SetIteratorTypeOther A `GenericIterator` with the
		 *         `SetIteratorType` template parameter being either the same,
		 *         or with different constness (`iterator` or `const_iterator`).
		 */
		template <typename SetIteratorTypeOther>
		bool operator!= (const GenericIterator<SetIteratorTypeOther>& rhs) const {
			return !(*this == rhs);
		}

		/**
		 * @brief Access the element at the iterator position.
		 */
		T operator* () const {
			return (*m_it)->key;
		}

		/**
		 * @brief Access a memeber of the element at the iterator position.
		 */
		T* operator-> () const {
			return &((*m_it)->key);
		}
	};
	typedef GenericIterator<typename SetType::iterator> Iterator;
	typedef GenericIterator<typename SetType::const_iterator> ConstIterator;
private:
	typename UnorderedSetWithIndexes::SetType m_set;
	typename UnorderedSetWithIndexes::VectorType m_vector;
public:
	// Note: Some of the method descriptions were copied from
	// en.cppreference.com website

	/**
	 * @brief Returns an iterator to the first element of the set.
	 */
	Iterator begin() {
		return Iterator(m_set.begin());
	}
	/**
	 * @brief Returns an iterator to the first element of the set.
	 */
	ConstIterator begin() const {
		return ConstIterator(m_set.begin());
	}
	/**
	 * @brief Returns a constant iterator to the first element of the set.
	 */
	ConstIterator cbegin() const {
		return ConstIterator(m_set.cbegin());
	}

	/**
	 * @brief Returns an iterator to the element following the last element of
	 *        the set.
	 */
	Iterator end() {
		return Iterator(m_set.end());
	}
	/**
	 * @brief Returns an iterator to the element following the last element of
	 *        the set.
	 */
	ConstIterator end() const {
		return ConstIterator(m_set.end());
	}
	/**
	 * @brief Returns a constant iterator to the element following the last
	 *        element of the set.
	 */
	ConstIterator cend() const {
		return ConstIterator(m_set.cend());
	}

	/**
	 * @brief Find an element with the key equivalent to `key`.
	 * 
	 * @return Iterator to the requested element or `end()` if the element was
	 *         not found.
	 */
	Iterator find(const T& key) {
		auto k = std::make_unique<Entry>(key, 0);
		return Iterator(m_set.find(k));
	}

	/**
	 * @brief Find an element with the key equivalent to `key`.
	 * 
	 * @return Iterator to the requested element or `end()` if the element was
	 *         not found.
	 */
	ConstIterator find(const T& key) const {
		auto k = std::make_unique<Entry>(key, 0);
		return ConstIterator(m_set.find(k));
	}

	/**
	 * @brief Insert `value` to the set if the value is not present in the set.
	 * 
	 * @return Pair consisting of an iterator to the inserted element (or to
	 *         the element that prevented the insertion) and a bool denoting
	 *         whether the insertion took place.
	 */
	std::pair<Iterator, bool> insert(const T& value) {
		T tmp(value);
		return insert(std::move(tmp));
	}

	/**
	 * @brief Insert `value` to the set if the value is not present in the set.
	 * 
	 * @return Pair consisting of an iterator to the inserted element (or to
	 *         the element that prevented the insertion) and a bool denoting
	 *         whether the insertion took place.
	 */
	std::pair<Iterator, bool> insert(T&& value) {
		auto entryPtr = std::make_unique<Entry>(std::move(value),
			m_vector.size());

		// Insert new item to the vector
		m_vector.push_back(entryPtr.get());

		std::pair<typename SetType::iterator, bool> ret;

		// Create entry in the set
		try {
			ret = m_set.insert(std::move(entryPtr));
		} catch (...) {
			// Ensure consistency in case of error
			m_vector.pop_back();
			throw;
		}

		// Set iterator
		auto& iter = ret.first;
		// Whether the insertion took place
		auto& didInsert = ret.second;

		if (!didInsert) {
			// The `value` already exists

			m_vector.pop_back();
		}

		return std::make_pair(Iterator(iter), didInsert);
	}

	/**
	 * @brief Removes the element at `pos`.
	 * 
	 * @return Iterator following the removed element.
	 */
	Iterator erase(const Iterator pos) {
		// Remove from vector

		// Index of the removed item (in vector).
		// After item removal this is the index of the former last item
		// in the vector.
		int idx = (*pos.getNestedIterator())->idx;

		// Better clean everything because `swap()` would mess up the
		// meaning of some variables
		{
			auto& rmItem = m_vector[idx];
			auto& lastItem = m_vector.back();
			std::swap(rmItem, lastItem);
			m_vector.pop_back();
		}

		// We chnged the index of the last item, so we have to fix the index
		// value in the entry
		auto& swappedItem = m_vector[idx];
		swappedItem->idx = idx;


		// Remove from set
		return Iterator(m_set.erase(pos.getNestedIterator()));
	}

	/**
	 * @brief Removes the element (if one exists) with the key equivalent
	 *        to `key`.
	 * 
	 * @return Number of elements removed (0 or 1).
	 */
	size_t erase(const T& key) {
		Iterator it = find(key);
		if (it == cend()) {
			return 0;
		} else {
			erase(it);
			return 1;
		}
	}

	/**
	 * @brief Returns the number of elements in the set.
	 */
	size_t size() const {
		return m_set.size();
	}

	/**
	 * @brief Erases all elements from the set.
	 */
	void clear() {
		m_set.clear();
		m_vector.clear();
	}

	/**
	 * @brief Reserves space for at least `count` elements.
	 */
	void reserve(size_t count) {
		m_set.reserve(count);
		m_vector.reserve(count);
	}

	/**
	 * @brief Returns the reference to the element at location `i`.
	 */
	const T& atIndex(size_t i) const {
		return m_vector.at(i)->key;
	}

	/**
	 * @brief Returns the reference to the element at location `i`.
	 */
	T& atIndex(size_t i) {
		return m_vector.at(i)->key;
	}
};

#endif // UNORDEREDSETWITHINDEXES_HPP
