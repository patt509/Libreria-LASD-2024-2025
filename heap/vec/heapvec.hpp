
/*
 * heapvec.hpp - Vector-Based Binary Heap Implementation
 * 
 * This file provides a concrete implementation of a binary heap using a dynamic vector
 * as the underlying storage mechanism. The vector-based approach leverages array indexing
 * to efficiently represent the complete binary tree structure required by heaps.
 * 
 * Array-based heap representation uses the following index relationships:
 * - Parent of node at index i: (i-1)/2
 * - Left child of node at index i: 2*i + 1  
 * - Right child of node at index i: 2*i + 2
 * 
 * This implementation supports both max-heap and min-heap configurations and provides
 * efficient heap operations with guaranteed logarithmic time complexity for insertions
 * and deletions. The vector storage enables dynamic resizing and efficient memory usage.
 * 
 * Key Features:
 * - Vector-based complete binary tree representation
 * - Dynamic capacity management with amortized O(1) insertion
 * - Efficient heap maintenance algorithms (heapify up/down)
 * - Integration with sorting algorithms (heapsort)
 * - O(log n) insertion, deletion, and modification operations
 * - O(1) access to heap root (minimum/maximum element)
 */

#ifndef HEAPVEC_HPP
#define HEAPVEC_HPP

/* ************************************************************************** */

#include "../heap.hpp"
#include "../../vector/vector.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

/*
 * HeapVec - Vector-Based Binary Heap Implementation
 * 
 * Concrete implementation of a binary heap using SortableVector as the underlying
 * storage mechanism. This class provides a complete heap data structure with all
 * standard heap operations while leveraging the dynamic array capabilities of vectors.
 * 
 * The heap maintains the complete binary tree property by storing elements in level-order
 * within the vector. This array-based representation enables efficient parent-child
 * navigation using simple arithmetic operations on indices.
 * 
 * Inheritance Structure:
 * - Heap<Data>: Provides abstract heap interface and operations
 * - SortableVector<Data>: Supplies dynamic array storage and sorting capabilities
 * 
 * The dual inheritance allows HeapVec to function both as a heap for priority-based
 * operations and as a sortable container for efficient sorting algorithms.
 * 
 * Performance Characteristics:
 * - Space Complexity: O(n) with dynamic resizing
 * - Access Root: O(1) - always at index 0
 * - Insert: O(log n) - requires heapify up
 * - Delete Root: O(log n) - requires heapify down  
 * - Heapify: O(n) - bottom-up heap construction
 * - Sort: O(n log n) - heapsort algorithm
 */

template <typename Data>
class HeapVec : virtual public Heap<Data>,
                public SortableVector<Data> {

private:
  // No additional private members - uses inherited vector storage

protected:

  // INHERITED MEMBERS ACCESS
  // Provides access to base class members for heap operations
  
  using Container::size;                    // Current number of elements in heap
  using SortableVector<Data>::Elements;     // Dynamic array storing heap elements

public:

  // CONSTRUCTORS
  
  // Default constructor: Creates an empty heap with initial capacity
  // Initializes underlying vector storage for immediate use
  HeapVec();

  /* ************************************************************************ */

  // SPECIALIZED CONSTRUCTORS
  // Various construction methods for different initialization scenarios

  // Capacity constructor: Creates heap with specified initial capacity
  // Useful for performance optimization when expected size is known
  // Parameters: initial capacity for underlying vector storage
  explicit HeapVec(const ulong);
  
  // Container copy constructor: Builds heap from any traversable container
  // Copies all elements and applies heapify to establish heap property
  // Time complexity: O(n) for copy + O(n) for heapify = O(n)
  HeapVec(const TraversableContainer<Data>&);
  
  // Container move constructor: Builds heap from mappable container using move semantics
  // More efficient for expensive-to-copy types, empties source container
  // Time complexity: O(n) for move + O(n) for heapify = O(n)
  HeapVec(MappableContainer<Data>&&);

  /* ************************************************************************ */

  // COPY AND MOVE SEMANTICS
  // Standard copy/move constructors with proper heap property maintenance

  // Copy constructor: Deep copy of another heap maintaining heap structure
  // Preserves heap property without requiring additional heapify operation
  HeapVec(const HeapVec&);

  // Move constructor: Efficiently transfers ownership of heap resources
  // Maintains heap property while avoiding expensive copy operations
  HeapVec(HeapVec&&) noexcept;

  /* ************************************************************************ */

  // DESTRUCTOR
  // Virtual destructor ensures proper cleanup through base class mechanisms
  virtual ~HeapVec() = default;

  /* ************************************************************************ */

  // ASSIGNMENT OPERATORS
  // Copy and move assignment with heap property preservation

  // Copy assignment: Replace current heap with copy of another heap
  // Handles self-assignment and maintains heap invariants
  HeapVec& operator=(const HeapVec&);

  // Move assignment: Efficiently replace current heap using move semantics
  // Provides strong exception safety and optimal performance
  HeapVec& operator=(HeapVec&&) noexcept;

  /* ************************************************************************ */

  // COMPARISON OPERATORS
  // Structural equality comparison for heap containers

  // Equality operator: Compares heap contents for exact match
  // Two heaps are equal if they contain the same elements in same order
  // Note: Same elements in different heap arrangements are considered different
  bool operator==(const HeapVec&) const noexcept;
  
  // Inequality operator: Logical negation of equality comparison
  bool operator!=(const HeapVec&) const noexcept;

  /* ************************************************************************ */

  // HEAP INTERFACE IMPLEMENTATION
  // Core heap operations inherited from abstract Heap class

  // IsHeap: Validates current heap property throughout entire structure  
  // Checks that every parent node satisfies ordering constraint with children
  // Returns true if heap property holds, false otherwise
  // Time complexity: O(n) - examines all parent-child relationships
  bool IsHeap() const noexcept override;

  // Heapify: Restores heap property starting from arbitrary arrangement
  // Reorganizes elements to satisfy heap ordering using bottom-up approach
  // Time complexity: O(n) - more efficient than repeated insertions
  void Heapify() override;

  /* ************************************************************************ */

  // SORTABLE CONTAINER INTERFACE
  // Sorting functionality using heap-based algorithms

    // Sort: Performs in-place heapsort algorithm on heap elements
  // Converts heap to sorted array while destroying heap property
  // Time complexity: O(n log n) - optimal comparison-based sorting
  // After sorting, heap property no longer holds until next Heapify
  void Sort() override;

protected:

  // HEAP MAINTENANCE OPERATIONS
  // Internal algorithms for maintaining heap property during modifications

  // SwapAt: Efficiently swaps elements at two positions with value caching
  // Override of SortableLinearContainer method optimized for heap operations
  // Used internally by heapify algorithms to minimize data movement
  // Parameters: two indices and their current values for optimized swapping
  void SwapAt(ulong, ulong, const Data&, const Data&) override;

  // HEAP NAVIGATION AND MAINTENANCE ALGORITHMS
  // Core algorithms for heap property maintenance and tree navigation

  // HeapifyUp: Restores heap property by moving element up toward root
  // Used after insertion to maintain heap ordering from leaf to root
  // Time complexity: O(log n) - maximum tree height traversal
  // Continues until heap property is satisfied or root is reached
  void HeapifyUp(ulong);
  
  // HeapifyDown: Restores heap property by moving element down toward leaves  
  // Used after root removal to maintain heap ordering from root to leaves
  // Time complexity: O(log n) - maximum tree height traversal
  // Continues until heap property is satisfied or leaf is reached
  void HeapifyDown(ulong);
  
  // BINARY TREE INDEX NAVIGATION
  // Efficient parent-child navigation using arithmetic operations on array indices
  
  // GetParent: Calculates parent index for given node position
  // Uses integer division: parent = (child - 1) / 2
  // Time complexity: O(1) - simple arithmetic operation
  // Returns parent index (may be invalid for root node)
  ulong GetParent(ulong) const noexcept;
  
  // GetLeftChild: Calculates left child index for given parent position
  // Uses arithmetic formula: left_child = 2 * parent + 1
  // Time complexity: O(1) - simple arithmetic operation
  // Returns left child index (may exceed array bounds)
  ulong GetLeftChild(ulong) const noexcept;
  
  // GetRightChild: Calculates right child index for given parent position  
  // Uses arithmetic formula: right_child = 2 * parent + 2
  // Time complexity: O(1) - simple arithmetic operation
  // Returns right child index (may exceed array bounds)
  ulong GetRightChild(ulong) const noexcept;
  
  // CHILD EXISTENCE VALIDATION
  // Boundary checking for safe child access in complete binary tree
  
  // HasLeftChild: Checks if node has valid left child within heap bounds
  // Prevents out-of-bounds access during tree traversal operations
  // Time complexity: O(1) - simple index comparison
  bool HasLeftChild(ulong) const noexcept;
  
  // HasRightChild: Checks if node has valid right child within heap bounds
  // Essential for safe binary tree navigation and heap maintenance
  // Time complexity: O(1) - simple index comparison  
  bool HasRightChild(ulong) const noexcept;

};

/* ************************************************************************** */

}

#include "heapvec.cpp"

#endif
