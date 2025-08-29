/*
 * SetVec Header File
 * 
 * This file defines the SetVec class, which implements a mathematical set using
 * a dynamic array (vector) as the underlying data structure. The implementation
 * leverages the efficiency of arrays for random access while maintaining set
 * semantics through sorted order and duplicate prevention.
 * 
 * Key features:
 * - Sorted array implementation for efficient ordered operations
 * - O(log n) search using binary search algorithm
 * - O(1) access to min/max elements due to sorted structure
 * - Circular access patterns for advanced traversal modes
 * - Efficient memory management with capacity control
 * - Inherits from both Set and SortableVector for full functionality
 * 
 * Design decisions:
 * - Uses SortableVector as base to leverage QuickSort implementation
 * - Maintains sorted invariant for all operations
 * - Implements binary search for efficient element location
 * - Provides circular access for specialized iteration patterns
 */

#ifndef SETVEC_HPP
#define SETVEC_HPP

/* ************************************************************************** */

#include "../set.hpp"
#include "../../vector/vector.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

// SetVec: Vector-based Set Implementation
// Implements a mathematical set using a sorted dynamic array for efficient operations
// Virtual inheritance ensures proper diamond inheritance resolution with Set interface
template <typename Data>
class SetVec : virtual public Set<Data>,
               virtual public SortableVector<Data> {

private:

  // CIRCULAR ACCESS STATE
  // Supports advanced iteration patterns and circular navigation
  
  ulong current = 0; // Current position for circular access operations
  ulong capacity = 0; // Reserved capacity for efficient memory management

protected:

  // INHERITED MEMBER ACCESS
  // Bring base class members into scope for convenient access
  using Container::size;
  using SortableVector<Data>::Elements;
  using SortableVector<Data>::Sort;
  using SortableVector<Data>::Resize;

  // UTILITY METHODS
  // Internal helper functions for set operations and maintenance
  
  // CheckUnique: Verifies if an element would be unique in the set
  // Used before insertion to maintain set semantics (no duplicates)
  // Time complexity: O(log n) using binary search
  bool CheckUnique(const Data&) const noexcept;

  // CIRCULAR ACCESS METHODS
  // Support for advanced iteration patterns and circular navigation
  
  // CircularGet: Access elements with circular wrap-around behavior
  // Allows accessing elements beyond array bounds by wrapping to beginning
  const Data& CircularGet(ulong) const;
  Data& CircularGet(ulong);
  
  // GetAtCurrent: Access elements relative to current circular position
  // Combines current position with offset for relative access
  const Data& GetAtCurrent(ulong index) const;
  Data& GetAtCurrent(ulong index);

  // SEARCH AND LOCATION METHODS
  // Efficient element location using binary search on sorted array
  
  // BinarySearch: Core search algorithm leveraging sorted array structure
  // Returns index of element if found, or -1 if not found
  // Optionally provides insertion point for maintaining sorted order
  // Time complexity: O(log n)
  long BinarySearch(const Data& data, long* insertPoint = nullptr) const noexcept;

  // FindIndex: Wrapper for element location without insertion point
  // Simplified interface for basic element location operations
  long FindIndex(const Data&) const noexcept;

  // CAPACITY MANAGEMENT METHODS
  // Efficient memory management for dynamic resizing operations
  
  // EnsureCapacity: Guarantees minimum capacity for efficient insertions
  // Prevents frequent reallocation during bulk operations
  void EnsureCapacity(ulong minCapacity);
  
  // ShrinkCapacity: Reduces capacity to optimize memory usage
  // Called after bulk removals to prevent memory waste
  void ShrinkCapacity();

public:

  // DEFAULT CONSTRUCTOR
  // Creates an empty set with minimal initial capacity
  SetVec() = default;

  /* ************************************************************************ */

  // SPECIALIZED CONSTRUCTORS
  // Various construction methods for different initialization scenarios
  
  // Capacity constructor: Creates set with specified initial capacity
  // Useful for performance optimization when expected size is known
  SetVec(const ulong);
  
  // TraversableContainer constructor: Creates set from existing container
  // Copies all elements while maintaining sorted order and uniqueness
  SetVec(const TraversableContainer<Data>&);
  
  // MappableContainer constructor: Creates set by moving from container
  // Efficiently transfers elements using move semantics
  SetVec(MappableContainer<Data>&&);

  /* ************************************************************************ */

  // COPY AND MOVE SEMANTICS
  // Efficient object copying and transfer operations
  
  // Copy constructor: Creates deep copy while preserving all state
  SetVec(const SetVec&);

  // Move constructor: Efficiently transfers ownership of resources
  SetVec(SetVec&&) noexcept;

  /* ************************************************************************ */

  // DESTRUCTOR
  // Default destructor is sufficient due to RAII in base classes
  virtual ~SetVec() = default;

  /* ************************************************************************ */

  // ASSIGNMENT OPERATORS
  // Handle assignment between SetVec instances
  
  // Copy assignment: Replaces content with deep copy of another set
  SetVec& operator=(const SetVec&);

  // Move assignment: Efficiently transfers ownership from another set
  SetVec& operator=(SetVec&&) noexcept;

  /* ************************************************************************ */

  // COMPARISON OPERATORS
  // Structural equality testing for set comparison
  
  // Equality operator: Tests if sets contain identical elements in same order
  bool operator==(const SetVec&) const noexcept;
  
  // Inequality operator: Tests if sets differ in content or order
  bool operator!=(const SetVec&) const noexcept;

  /* ************************************************************************ */

  // CONTAINER INTERFACE IMPLEMENTATION
  // Basic container operations for set management
  
  // Clear: Removes all elements and resets the set to empty state
  void Clear() override;

  /* ************************************************************************ */

  // TESTABLE CONTAINER INTERFACE IMPLEMENTATION
  // Efficient existence testing using binary search
  
  // Exists: Tests if element exists in set using O(log n) binary search
  bool Exists(const Data&) const noexcept override;

  /* ************************************************************************ */

  // ORDERED DICTIONARY INTERFACE IMPLEMENTATION
  // Operations that leverage the sorted nature of the vector
  
  // MIN/MAX OPERATIONS - O(1) due to sorted array structure
  const Data& Min() const override;        // Returns smallest element
  Data MinNRemove() override;              // Returns and removes smallest element
  void RemoveMin() override;               // Removes smallest element
  
  const Data& Max() const override;        // Returns largest element
  Data MaxNRemove() override;              // Returns and removes largest element
  void RemoveMax() override;               // Removes largest element

  // PREDECESSOR/SUCCESSOR OPERATIONS - O(log n) using binary search
  const Data& Predecessor(const Data&) const override;     // Finds largest element < data
  Data PredecessorNRemove(const Data&) override;           // Finds and removes predecessor
  void RemovePredecessor(const Data&) override;            // Removes predecessor
  
  const Data& Successor(const Data&) const override;       // Finds smallest element > data
  Data SuccessorNRemove(const Data&) override;             // Finds and removes successor
  void RemoveSuccessor(const Data&) override;              // Removes successor

  /* ************************************************************************ */

  // DICTIONARY CONTAINER INTERFACE IMPLEMENTATION
  // Core set operations maintaining sorted order and uniqueness
  
  // SINGLE ELEMENT OPERATIONS
  bool Insert(const Data&) override;    // Insert element (copy version)
  bool Insert(Data&&) override;         // Insert element (move version)
  bool Remove(const Data&) override;    // Remove specific element
  
  // BULK OPERATIONS
  bool InsertAll(const TraversableContainer<Data>&) override;  // Insert all elements
  bool InsertAll(MappableContainer<Data>&&) override;          // Insert all (move version)
  bool RemoveAll(const TraversableContainer<Data>&) override;  // Remove all specified elements
  
  // PROBABILISTIC OPERATIONS
  bool InsertSome(const TraversableContainer<Data>&) override; // Insert random subset
  bool InsertSome(MappableContainer<Data>&&) override;         // Insert random subset (move)
  bool RemoveSome(const TraversableContainer<Data>&) override; // Remove random subset

  /* ************************************************************************ */

  // LINEAR CONTAINER INTERFACE IMPLEMENTATION
  // Array-like access to elements in sorted order
  
  // INDEXED ACCESS - O(1) due to array structure
  const Data& operator[](const ulong) const override;  // Read-only indexed access
  Data& operator[](const ulong) override;              // Mutable indexed access

  // FRONT/BACK ACCESS - O(1) due to sorted array
  const Data& Front() const override;  // Access smallest element (read-only)
  Data& Front() override;              // Access smallest element (mutable)
  
  const Data& Back() const override;   // Access largest element (read-only)
  Data& Back() override;               // Access largest element (mutable)

  /* ************************************************************************ */

  // CIRCULAR ACCESS INTERFACE
  // Advanced navigation patterns for specialized iteration
  
  // SetCurrent: Positions circular access pointer at specified index
  void SetCurrent(ulong) noexcept;
  
  // GetCurrent: Returns current position of circular access pointer
  ulong GetCurrent() const noexcept;
  
  // Next: Advances circular access pointer (wraps to beginning at end)
  void Next() noexcept;
  
  // Prev: Moves circular access pointer backward (wraps to end at beginning)
  void Prev() noexcept;

  // INHERITED INTERFACE EXPOSURE
  // Make selected base class methods accessible through SetVec interface
  using Container::Size;
  using Container::Empty;
  using TraversableContainer<Data>::Traverse;
  using PreOrderTraversableContainer<Data>::PreOrderTraverse;
  using PostOrderTraversableContainer<Data>::PostOrderTraverse;
  using MappableContainer<Data>::Map;

  // DEBUG FUNCTIONALITY
  // Development and testing support
  void PrintDebug() const; // Prints set contents for debugging purposes
};

/* ************************************************************************** */

}

#include "setvec.cpp"

#endif
