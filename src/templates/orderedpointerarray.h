/*
	*** Simple Ordered Pointer Array Class
	*** src/templates/orderedpointerarray.h
	Copyright T. Youngs 2012-2020

	This file is part of Dissolve.

	Dissolve is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Dissolve is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Dissolve.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DISSOLVE_ORDEREDPOINTERARRAY_H
#define DISSOLVE_ORDEREDPOINTERARRAY_H

#include <stdlib.h>
#include <stdio.h>
#include "base/messenger.h"

// Forward Declarations
/* none */

/*
 * Simple Ordered Pointer Array Class
 * A contiguous pointer array class designed to be as lightweight as possible.
 * Each pointer may exist in the list only once.
 * Order of pointers, based on their increasing value, is maintained.
 */
template <class T> class OrderedPointerArray
{
	public:
	// Constructor
	OrderedPointerArray<T>()
	{
		nItems_ = 0;
		arraySize_ = 0;
		items_ = NULL;
	}
	// Destructor
	~OrderedPointerArray()
	{
		clear();
	}
	// Copy Constructor
	OrderedPointerArray<T>(const OrderedPointerArray<T>& source)
	{
		nItems_ = 0;
		arraySize_ = 0;
		items_ = NULL;
		(*this) = source;
	}
	// Assignment operator
	OrderedPointerArray<T>& operator=(const OrderedPointerArray<T>& source)
	{
		// Clear any current data in the list...
		clear();

		initialise(source.nItems());

		for (int n=0; n<source.nItems(); ++n) add(source.value(n));

		return *this;
	}
	// Element access operator
	T* operator[](int index)
	{
#ifdef CHECKS
		if ((index < 0) || (index >= nItems_))
		{
			Messenger::error("OrderedPointerArray<T>::operator[](%i) - Array index out of bounds (%i items in array).\n", index, nItems_);
			return NULL;
		}
#endif
		return items_[index];
	}


	/*
	 * Basic Data
	 */
	protected:
	// Array of items
	T** items_;
	// Size of items array
	int arraySize_;
	// Number of items in array
	int nItems_;

	public:
	// Clear the list
	void clear()
	{
		if (items_) delete[] items_;
		items_ = NULL;
		nItems_ = 0;
		arraySize_ = 0;
	}
	// Create empty list of specified size
	void initialise(int size)
	{
		clear();

		arraySize_ = size;

		if (size == 0) items_ = NULL;
		else items_ = new T*[arraySize_];

		for (int n=0; n<arraySize_; ++n) items_[n] = NULL;
	}
	// Returns the number of items in the list
	int nItems() const
	{
		return nItems_;
	}
	// Const pointer access
	T* pointer(int index) const
	{
#ifdef CHECKS
		if ((index < 0) || (index >= nItems_))
		{
			Messenger::error("OrderedPointerArray<T>::index(%i) - Array index out of bounds (%i items in array).\n", index, nItems_);
			return NULL;
		}
#endif

		return items_[index];
	}
	// Return pointer array
	T** items()
	{
		return items_;
	}
	// Return pointer array (const)
	T** constItems() const
	{
		return items_;
	}


	/*
	 * Item Management
	 */
	protected:
	// Compare the two objects, returning if their 'indices' are equal
	virtual inline bool itemIndicesEqual(T* a, T* b)
	{
		// Default implementation compares the pointers
		return (a == b);
	}
	// Compare the two objects, returning if the 'index' of a is greater than that of b
	virtual inline bool itemIndexGreaterThan(T* a, T* b)
	{
		// Default implementation compares the pointers
		return (a > b);
	}
	// Compare the two objects, returning if the 'index' of a is less than that of b
	virtual inline bool itemIndexLessThan(T* a, T* b)
	{
		// Default implementation compares the pointers
		return (a < b);
	}

	public:
	// Add an item to the list
	void add(T* ptr)
	{
		// Loop over pointers, searching for the first existing item that has a higher pointer address.
		// If we find the same pointer address in the list, return immediately.
		int insertAt, n;
		for (insertAt = 0; insertAt<nItems_; ++insertAt)
		{
			if (itemIndicesEqual(items_[insertAt], ptr)) return;
			if (itemIndexGreaterThan(items_[insertAt], ptr)) break;
		}

		// The pointer is not currently in the list, and we have the position at which it should be inserted in 'insertAt'.
		// If there is no more space, resize the array and copy the old items, inserting our new pointer at the correct position.
		// If there is no space for the new item we must resize the array
		if (nItems_ == arraySize_)
		{
			// Create a new array with some more item space
			T** newItems = new T*[arraySize_+2];

			// Copy existing pointers up to 'insertAt'
			for (n=0; n<insertAt; ++n) newItems[n] = items_[n];

			// Place our new pointer here
			newItems[insertAt] = ptr;

			// Copy the remaining pointers, shifting them one place up in the new list
			for (n=insertAt; n<nItems_; ++n) newItems[n+1] = items_[n];

			// Delete the old arrays if they exist
			if (items_) delete[] items_;

			// Set the new arrays and array size
			items_ = newItems;
			arraySize_ += 2;
		}
		else
		{
			// Shuffle items from the insertion point up one place in the array (to higher indices), working backwards to avoid overwriting data
			for (n=nItems_-1; n>=insertAt; --n) items_[n+1] = items_[n];

			// Now put the new item into the 'insertAt' position
			items_[insertAt] = ptr;
		}

		++nItems_;
	}
	// Remove an item from the array, leaving the remaining items contiguous in memory
	bool remove(T* ptr)
	{
		// Step through the items until we find the specified pointer
		for (int n=0; n<nItems_; ++n)
		{
			if (itemIndexGreaterThan(items_[n], ptr)) return false;
			if (itemIndicesEqual(items_[n], ptr))
			{
				// Found it. Move all items from this point forward back one place
				for (int m=n+1; m<nItems_; ++m) items_[m-1] = items_[m];
				items_[nItems_-1] = NULL;
				--nItems_;

				return true;
			}
		}
		Messenger::print("OrderedPointerArray<T>::remove(%p) - Couldn't find pointer in array.\n", ptr);
		return false;
	}
	// Return array index of pointer within the list
	int indexOf(const T* ptr) const
	{
		for (int n=0; n<nItems_; ++n) if (items_[n] == ptr) return n;

		return -1;
	}
};

#endif
