
/*
 * Set Interface Definition
 * 
 * This file defines the abstract Set interface that provides mathematical set
 * functionality with ordered dictionary capabilities. The Set class combines
 * dictionary operations (insert, remove, exists) with ordered access patterns
 * (min, max, predecessor, successor) and linear container indexing.
 * 
 * Key Features:
 * - Mathematical set semantics (no duplicates)
 * - Ordered dictionary operations for finding min/max and neighbors
 * - Linear container interface for indexed access
 * - Abstract interface implemented by concrete set classes
 * 
 * The Set class serves as the base interface for various set implementations
 * such as list-based sets (SetLst) and vector-based sets (SetVec).
 */

#ifndef SET_HPP
#define SET_HPP

/* ************************************************************************** */

#include "../container/dictionary.hpp"
#include "../container/linear.hpp"
#include "../container/container.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

/*
 * Set Class - Abstract Interface
 * 
 * The Set class provides an abstract interface for mathematical set operations
 * combined with ordered dictionary functionality. It inherits from both
 * OrderedDictionaryContainer and LinearContainer to provide:
 * 
 * 1. Set Operations: Insert, Remove, Exists (from DictionaryContainer)
 * 2. Ordered Operations: Min, Max, Predecessor, Successor (from OrderedDictionaryContainer)
 * 3. Indexed Access: operator[], Front, Back (from LinearContainer)
 * 
 * The virtual inheritance ensures proper multiple inheritance behavior
 * when concrete implementations inherit from this interface.
 * 
 * Design Notes:
 * - Elements are maintained in sorted order for efficient ordered operations
 * - No duplicate elements are allowed (set semantics)
 * - Comparison and assignment operators are deleted for abstract interface
 */
template <typename Data>
class Set : virtual public OrderedDictionaryContainer<Data>,
            virtual public LinearContainer<Data> {

private:

protected:

  // Import base class members to avoid scope resolution in derived classes
  using Container::size; // Size of the set (number of elements)
  using LinearContainer<Data>::operator[]; // Indexed access to elements
  using LinearContainer<Data>::Front; // Access to smallest element
  using LinearContainer<Data>::Back; // Access to largest element

public:

  // Destructor: Virtual destructor for proper polymorphic destruction
  virtual ~Set() = default;

  /* ************************************************************************ */

  // Assignment operators are deleted for abstract interface
  // Concrete implementations should provide their own assignment semantics

  // Copy assignment: Deleted for abstract interface
  Set& operator=(const Set&) = delete; // Copy assignment of abstract types is not possible

  // Move assignment: Deleted for abstract interface  
  Set& operator=(Set&&) noexcept = delete; // Move assignment of abstract types is not possible
  
  /* ************************************************************************ */

  // Comparison operators are deleted for abstract interface
  // Concrete implementations should provide their own comparison logic

  bool operator==(const Set&) const noexcept = delete; // Equality comparison not available for abstract interface
  bool operator!=(const Set&) const noexcept = delete; // Inequality comparison not available for abstract interface

  /* ************************************************************************ */

  // Specific member functions - Core set operations

  // Clear operation: Removes all elements from the set
  virtual void Clear() = 0; // Clear the entire set and reset to empty state

  /* ************************************************************************ */

  // Specific member functions (inherited from OrderedDictionaryContainer)
  // These functions provide ordered access to set elements based on their sorted position

  // Minimum element operations: Access and removal of the smallest element
  virtual const Data& Min() const = 0; // Returns const reference to smallest element (throws std::length_error when empty)
  virtual Data MinNRemove() = 0; // Returns copy of smallest element and removes it (throws std::length_error when empty)
  virtual void RemoveMin() = 0; // Removes the smallest element (throws std::length_error when empty)

  // Maximum element operations: Access and removal of the largest element
  virtual const Data& Max() const = 0; // Returns const reference to largest element (throws std::length_error when empty)
  virtual Data MaxNRemove() = 0; // Returns copy of largest element and removes it (throws std::length_error when empty)
  virtual void RemoveMax() = 0; // Removes the largest element (throws std::length_error when empty)

  // Predecessor operations: Access and removal of the largest element smaller than the given value
  virtual const Data& Predecessor(const Data&) const = 0; // Returns largest element < given value (throws std::length_error when not found)
  virtual Data PredecessorNRemove(const Data&) = 0; // Returns and removes largest element < given value (throws std::length_error when not found)
  virtual void RemovePredecessor(const Data&) = 0; // Removes largest element < given value (throws std::length_error when not found)

  // Successor operations: Access and removal of the smallest element larger than the given value
  virtual const Data& Successor(const Data&) const = 0; // Returns smallest element > given value (throws std::length_error when not found)
  virtual Data SuccessorNRemove(const Data&) = 0; // Returns and removes smallest element > given value (throws std::length_error when not found)
  virtual void RemoveSuccessor(const Data&) = 0; // Removes smallest element > given value (throws std::length_error when not found)
};

/* ************************************************************************** */

}

#endif
