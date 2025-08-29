
/*
 * SetLst - List-Based Set Implementation
 * 
 * This file defines a Set implementation based on a sorted linked list structure.
 * The SetLst class provides mathematical set functionality with ordered operations
 * by maintaining elements in a sorted singly-linked list.
 * 
 * Key Features:
 * - Sorted linked list maintains elements in ascending order
 * - O(n) insertion due to sorted positioning requirement
 * - O(1) access to min element (head), O(n) access to max element (tail)
 * - O(n) search operations through linear traversal
 * - Memory-efficient storage with only necessary allocations
 * - Suitable for sets with frequent min operations and moderate sizes
 * 
 * The implementation combines List<Data> for storage with Set<Data> interface
 * to provide both linear container operations and set-specific functionality.
 * Elements are kept sorted to enable efficient ordered dictionary operations.
 */

#ifndef SETLST_HPP
#define SETLST_HPP

/* ************************************************************************** */

#include "../set.hpp"
#include "../../list/list.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

/*
 * SetLst Class - List-Based Set Implementation
 * 
 * Implements a mathematical set using a sorted linked list as the underlying
 * data structure. This class inherits from both Set<Data> and List<Data>
 * using virtual inheritance to provide:
 * 
 * 1. Set semantics (no duplicates, ordered operations)
 * 2. Linked list storage (dynamic memory, sequential access)
 * 3. Sorted maintenance (elements kept in ascending order)
 * 
 * Performance Characteristics:
 * - Insert: O(n) - must find correct sorted position
 * - Remove: O(n) - must search through list
 * - Min: O(1) - first element in sorted list
 * - Max: O(1) - last element via tail pointer
 * - Search: O(n) - linear traversal
 * - Memory: O(n) - only allocated nodes, no wasted space
 * 
 * Best suited for scenarios where:
 * - Set size is moderate (up to thousands of elements)
 * - Memory efficiency is important
 * - Frequent min/max operations
 * - Infrequent random access by index
 */
template <typename Data>
class SetLst : virtual public Set<Data>, 
               virtual public List<Data> {
  // Must extend Set<Data>,
  //             List<Data>

private:

  // No additional private members needed - inherits from List<Data>
  // The sorted order is maintained by insertion/removal logic

protected:

  // Import base class members for easier access
  using Container::size; // Number of elements in the set
  using List<Data>::head; // Pointer to first node (smallest element)
  using List<Data>::tail; // Pointer to last node (largest element)

public:

  // Default constructor: Creates an empty set
  SetLst() = default;

  /* ************************************************************************ */

  // Specific constructors for creating sets from existing containers
  
  SetLst(const TraversableContainer<Data>& container); // Creates set by inserting all elements from traversable container
  SetLst(MappableContainer<Data>&& container); // Creates set by moving/inserting all elements from mappable container

  /* ************************************************************************ */

  // Copy constructor: Creates a deep copy of another SetLst
  SetLst(const SetLst& other);

  // Move constructor: Efficiently transfers ownership from another SetLst
  SetLst(SetLst&& other) noexcept;

  /* ************************************************************************ */

  // Destructor: Uses default behavior (List destructor handles cleanup)
  virtual ~SetLst() = default;

  /* ************************************************************************ */

  // Assignment operators for copying and moving set contents
  
  SetLst& operator=(const SetLst& other); // Copy assignment with deep copying
  SetLst& operator=(SetLst&& other) noexcept; // Move assignment with resource transfer

  /* ************************************************************************ */

  // Comparison operators for structural equality testing
  
  bool operator==(const SetLst& other) const noexcept; // Returns true if sets contain same elements
  bool operator!=(const SetLst& other) const noexcept; // Returns true if sets differ in content

  /* ************************************************************************ */

  // Specific member function (inherited from ClearableContainer)
  
  void Clear() override; // Removes all elements from the set and frees memory

  /* ************************************************************************ */

  // Specific member function (inherited from TestableContainer)
  
  bool Exists(const Data& data) const noexcept override; // Tests if element exists in set (O(n) search)

  /* ************************************************************************ */

  // Specific member functions (inherited from OrderedDictionaryContainer)
  // These provide ordered access to elements based on their sorted position
  
  // Minimum element operations (head of sorted list)
  const Data& Min() const override; // Returns reference to smallest element (first in list)
  Data MinNRemove() override; // Returns copy of smallest element and removes it
  void RemoveMin() override; // Removes the smallest element from the set
  
  // Maximum element operations (tail of sorted list)
  const Data& Max() const override; // Returns reference to largest element (last in list)
  Data MaxNRemove() override; // Returns copy of largest element and removes it
  void RemoveMax() override; // Removes the largest element from the set
  
  // Predecessor operations (largest element smaller than given value)
  const Data& Predecessor(const Data& data) const override; // Finds largest element < data
  Data PredecessorNRemove(const Data& data) override; // Returns and removes largest element < data
  void RemovePredecessor(const Data& data) override; // Removes largest element < data
  
  // Successor operations (smallest element larger than given value)
  const Data& Successor(const Data& data) const override; // Finds smallest element > data
  Data SuccessorNRemove(const Data& data) override; // Returns and removes smallest element > data
  void RemoveSuccessor(const Data& data) override; // Removes smallest element > data

  /* ************************************************************************ */

  // Specific member functions (inherited from DictionaryContainer)
  // These maintain set semantics by preventing duplicates and keeping sorted order
  
  bool Insert(const Data& data) override; // Inserts element in correct sorted position (copy semantics)
  bool Insert(Data&& data) override; // Inserts element in correct sorted position (move semantics)
  bool Remove(const Data& data) override; // Removes specified element if it exists

  // Bulk operations for multiple elements
  bool InsertAll(const TraversableContainer<Data>& container) override; // Attempts to insert all elements from container
  bool InsertAll(MappableContainer<Data>&& container) override; // Attempts to insert all elements (move version)
  bool RemoveAll(const TraversableContainer<Data>& container) override; // Removes all matching elements from container
  
  bool InsertSome(const TraversableContainer<Data>& container) override; // Inserts any non-duplicate elements
  bool InsertSome(MappableContainer<Data>&& container) override; // Inserts any non-duplicate elements (move version)
  bool RemoveSome(const TraversableContainer<Data>& container) override; // Removes any matching elements

};

/* ************************************************************************** */

}

#include "setlst.cpp"

#endif
