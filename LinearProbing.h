#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H
#include "HashTableInterface.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
using namespace std;

int nextPrime(int n);

// LinearProbing Hash table class
//
// CONSTRUCTION: an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// bool insert( x )       --> Insert x
// bool remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// void makeEmpty( )      --> Remove all items
// int hashCode( string str ) --> Global method to hash strings

template <typename HashedObj, typename HashFunc = hash<HashedObj>>
class LinearHashTable : HashTableInterface<HashedObj>
{
public:
	explicit LinearHashTable(int size = 101) : array(nextPrime(size))
	{
		//Pretty sure we dont have to qualify it, but its better to do so than not do so
		//As for why we do it, because its a virtual function used in the constructor
		LinearHashTable<HashedObj, HashFunc>::makeEmpty();
	}

	bool contains(const HashedObj & x) const override
	{
		return isActive(findPos(x));
	}

	void makeEmpty() override
	{
		currentSize = 0;
		for (auto & entry : array)
			entry.info = EMPTY;
	}

	bool insert(const HashedObj & x) override
	{
		// Insert x as active
		int currentPos = findPos(x);
		if (isActive(currentPos))
			return false;

		if (array[currentPos].info != DELETED)
			++currentSize;

		array[currentPos].element = x;
		array[currentPos].info = ACTIVE;

		// Rehash; see Section 5.5
		if (currentSize > array.size() / 2)
			rehash();

		return true;
	}

	bool insert(HashedObj && x) override
	{
		// Insert x as active
		int currentPos = findPos(x);
		if (isActive(currentPos))
			return false;

		if (array[currentPos].info != DELETED)
			++currentSize;

		array[currentPos] = std::move(x);
		array[currentPos].info = ACTIVE;

		// Rehash; see Section 5.5
		if (currentSize > array.size() / 2)
			rehash();

		return true;
	}

	bool remove(const HashedObj & x) override
	{
		int currentPos = findPos(x);
		if (!isActive(currentPos))
			return false;

		array[currentPos].info = DELETED;
		return true;
	}

	enum EntryType { ACTIVE, EMPTY, DELETED };

private:
	struct HashEntry
	{
		HashedObj element;
		EntryType info;

		HashEntry(const HashedObj & e = HashedObj{}, EntryType i = EMPTY)
			: element{ e }, info{ i } { }

		HashEntry(HashedObj && e, EntryType i = EMPTY)
			: element{ std::move(e) }, info{ i } { }
	};

	vector<HashEntry> array;
	int currentSize;

	bool isActive(int currentPos) const
	{
		return array[currentPos].info == ACTIVE;
	}

	int findPos(const HashedObj & x) const
	{
		int currentPos = myhash(x);

		while (array[currentPos].info != EMPTY &&
			array[currentPos].element != x)
		{
			currentPos ++;  // Compute ith probe
			if (currentPos >= array.size())
				currentPos -= array.size();
		}

		return currentPos;
	}

	void rehash()
	{
		vector<HashEntry> oldArray = array;

		// Create new double-sized, empty table
		array.resize(nextPrime(2 * oldArray.size()));
		for (auto & entry : array)
			entry.info = EMPTY;

		// Copy table over
		currentSize = 0;
		for (auto & entry : oldArray)
			if (entry.info == ACTIVE)
				insert(std::move(entry.element));
	}

	size_t myhash(const HashedObj & x) const
	{
		static HashFunc hf;
		return hf(x) % array.size();
	}
};

#endif