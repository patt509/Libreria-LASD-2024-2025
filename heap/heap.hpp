
/*
 * heap.hpp - Abstract Heap Data Structure Interface
 * 
 * This file defines the abstract interface for heap data structures, which are specialized
 * complete binary trees that satisfy the heap property. The heap property ensures that
 * parent nodes are always greater than or equal to (max-heap) or less than or equal to
 * (min-heap) their children, enabling efficient priority-based operations.
 * 
 * The Heap class inherits from both SortableLinearContainer and ClearableContainer,
 * providing a foundation for heap-based algorithms like heapsort and priority queues.
 * Concrete implementations must define the heap maintenance operations and heap property
 * validation.
 * 
 * Key Features:
 * - Abstract interface for heap data structures
 * - Integration with sortable linear container hierarchy
 * - Heap property validation and maintenance operations
 * - Foundation for priority queue implementations
 * - Efficient O(log n) insertion and extraction operations
 */

#ifndef HEAP_HPP
#define HEAP_HPP

/* ************************************************************************** */

#include "../container/linear.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

/*
 * Heap - Abstract Binary Heap Interface
 * 
 * Abstract base class for heap data structures implementing the heap property.
 * A heap is a complete binary tree where each parent node satisfies a specific
 * ordering relationship with its children (either max-heap or min-heap property).
 * 
 * This class combines the capabilities of:
 * - SortableLinearContainer: Provides sorting functionality using heap algorithms
 * - ClearableContainer: Enables efficient clearing of heap contents
 * 
 * The heap property enables:
 * - O(1) access to the maximum/minimum element
 * - O(log n) insertion and deletion operations
 * - O(n log n) sorting algorithm (heapsort)
 * - Efficient priority queue implementations
 * 
 * Concrete implementations must:
 * - Define heap property validation (IsHeap)
 * - Implement heap maintenance operations (Heapify)
 * - Ensure complete binary tree structure
 */

template <typename Data>
class Heap : virtual public SortableLinearContainer<Data>,
             virtual public ClearableContainer {

private:
  // No private members in abstract interface

protected:
  // Protected section reserved for concrete implementations

public:

  // DESTRUCTOR
  // Virtual destructor ensures proper cleanup of derived heap implementations
  virtual ~Heap() = default;

  /* ************************************************************************ */

  // ASSIGNMENT OPERATORS
  // Assignment operations are disabled for abstract heap interface
  
  // Copy assignment is not permitted for abstract types
  // Concrete heap implementations must provide their own assignment semantics
  Heap& operator=(const Heap&) = delete;

  // Move assignment is not permitted for abstract types  
  // Concrete implementations handle move semantics based on their storage strategy
  Heap& operator=(Heap&&) noexcept = delete;

  /* ************************************************************************ */

  // COMPARISON OPERATORS
  // Comparison operations are disabled for abstract heap interface
  
  // Equality comparison is not defined for abstract heap types
  // Concrete implementations may define comparison based on heap structure/content
  bool operator==(const Heap&) const noexcept = delete;
  
  // Inequality comparison follows from disabled equality
  bool operator!=(const Heap&) const noexcept = delete;

  /* ************************************************************************ */

  // HEAP-SPECIFIC OPERATIONS
  // Core heap functionality that concrete implementations must provide

  // IsHeap: Validates whether the current structure satisfies heap property
  // Returns true if all parent-child relationships maintain heap ordering
  // Time complexity: O(n) - must check all parent-child pairs
  // Used for debugging and verification of heap invariants
  virtual bool IsHeap() const noexcept = 0;

  // Heapify: Restores heap property throughout the entire structure
  // Reorganizes elements to satisfy heap ordering constraints
  // Time complexity: O(n) - processes all nodes to establish heap property
  // Essential after bulk modifications or construction from unordered data
  virtual void Heapify() = 0;

};

/* ************************************************************************** */

}

#endif
