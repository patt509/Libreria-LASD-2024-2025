#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

/* ************************************************************************** */

#include "testable.hpp"
#include "mappable.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

// DictionaryContainer Class
// ------------------------
// An abstract container that represents a collection of unique elements (set-like behavior)
// It supports insertion, removal, and existence testing operations
// This forms the basis for set implementations and other unique-element collections
template <typename Data>
class DictionaryContainer : virtual public TestableContainer<Data> {

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~DictionaryContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  DictionaryContainer & operator=(const DictionaryContainer &) = delete;

  // Move assignment - deleted for abstract class
  DictionaryContainer & operator=(DictionaryContainer &&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const DictionaryContainer &) const noexcept = delete;
  bool operator!=(const DictionaryContainer &) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member functions

  // Insert() - Add a single element to the dictionary
  // Returns true if the element was inserted, false if it already existed
  virtual bool Insert(const Data &) = 0; // Copy of the value
  virtual bool Insert(Data &&) = 0; // Move of the value

  // Remove() - Remove a single element from the dictionary
  // Returns true if the element was removed, false if it didn't exist
  virtual bool Remove(const Data &) = 0;

  // Bulk insertion operations
  // InsertAll() - Attempt to insert all elements from another container
  // Returns true if ALL elements were successfully inserted (none already existed)
  virtual bool InsertAll(const TraversableContainer<Data> &) = 0; // Copy of the values
  virtual bool InsertAll(MappableContainer<Data> &&) = 0; // Move of the values

  // RemoveAll() - Remove all elements that exist in another container
  // Returns true if ALL specified elements were found and removed
  virtual bool RemoveAll(const TraversableContainer<Data> &) = 0;

  // Partial operations
  // InsertSome() - Insert elements from another container, allowing some duplicates
  // Returns true if at least ONE element was successfully inserted
  virtual bool InsertSome(const TraversableContainer<Data> &) = 0; // Copy of the values
  virtual bool InsertSome(MappableContainer<Data> &&) = 0; // Move of the values

  // RemoveSome() - Remove any elements that exist in another container
  // Returns true if at least ONE element was successfully removed
  virtual bool RemoveSome(const TraversableContainer<Data> &) = 0;

};

/* ************************************************************************** */

// OrderedDictionaryContainer Class
// -------------------------------
// An extension of DictionaryContainer that maintains elements in sorted order
// Provides additional operations for accessing minimum, maximum, predecessor, and successor elements
// This is the foundation for ordered set implementations like binary search trees
template <typename Data>
class OrderedDictionaryContainer : virtual public DictionaryContainer<Data> {

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~OrderedDictionaryContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  OrderedDictionaryContainer & operator=(const OrderedDictionaryContainer &) = delete;

  // Move assignment - deleted for abstract class
  OrderedDictionaryContainer & operator=(OrderedDictionaryContainer &&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const OrderedDictionaryContainer &) const noexcept = delete;
  bool operator!=(const OrderedDictionaryContainer &) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member functions for ordered access

  // Minimum element operations
  // Min() - Access the smallest element without removing it
  // Throws std::length_error when the container is empty
  virtual const Data & Min() const = 0;

  // MinNRemove() - Get and remove the smallest element in one operation
  // Returns the removed element; throws std::length_error when empty
  virtual Data MinNRemove() = 0;

  // RemoveMin() - Remove the smallest element without returning it
  // Throws std::length_error when the container is empty
  virtual void RemoveMin() = 0;

  // Maximum element operations
  // Max() - Access the largest element without removing it
  // Throws std::length_error when the container is empty
  virtual const Data & Max() const = 0;

  // MaxNRemove() - Get and remove the largest element in one operation
  // Returns the removed element; throws std::length_error when empty
  virtual Data MaxNRemove() = 0;

  // RemoveMax() - Remove the largest element without returning it
  // Throws std::length_error when the container is empty
  virtual void RemoveMax() = 0;

  // Predecessor operations (largest element smaller than the given value)
  // Predecessor() - Access the predecessor without removing it
  // Throws std::length_error when no predecessor exists
  virtual const Data & Predecessor(const Data &) const = 0;

  // PredecessorNRemove() - Get and remove the predecessor in one operation
  // Returns the removed predecessor; throws std::length_error when not found
  virtual Data PredecessorNRemove(const Data &) = 0;

  // RemovePredecessor() - Remove the predecessor without returning it
  // Throws std::length_error when no predecessor exists
  virtual void RemovePredecessor(const Data &) = 0;

  // Successor operations (smallest element larger than the given value)
  // Successor() - Access the successor without removing it
  // Throws std::length_error when no successor exists
  virtual const Data & Successor(const Data &) const = 0;

  // SuccessorNRemove() - Get and remove the successor in one operation
  // Returns the removed successor; throws std::length_error when not found
  virtual Data SuccessorNRemove(const Data &) = 0;

  // RemoveSuccessor() - Remove the successor without returning it
  // Throws std::length_error when no successor exists
  virtual void RemoveSuccessor(const Data &) = 0;

};

/* ************************************************************************** */

}

#include "dictionary.cpp"

#endif
