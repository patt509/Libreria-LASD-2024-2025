
/*
 * Priority Queue Implementation using Binary Heap Data Structure
 * 
 * This file implements a heap-based priority queue that combines the abstract
 * priority queue interface (PQ) with the efficient binary heap implementation
 * (HeapVec). A priority queue is a specialized data structure where elements
 * are served based on their priority rather than insertion order.
 * 
 * Key Features:
 * - Logarithmic time complexity for insertion and removal operations
 * - Constant time access to highest priority element
 * - Efficient memory management with dynamic capacity adjustment
 * - Support for priority modification operations
 * 
 * The implementation uses a binary max-heap where the highest priority element
 * is always at the root (index 0). This provides optimal performance for
 * priority-based operations commonly used in algorithms like:
 * - Dijkstra's shortest path algorithm
 * - A* pathfinding algorithm
 * - Huffman coding tree construction
 * - Task scheduling systems
 * - Event-driven simulations
 * 
 * Design Pattern:
 * The class uses virtual inheritance from PQ to avoid diamond inheritance
 * problems when multiple inheritance is involved, while publicly inheriting
 * from HeapVec to leverage its efficient heap operations.
 */

#ifndef PQHEAP_HPP
#define PQHEAP_HPP

/* ************************************************************************** */

#include "../pq.hpp"
#include "../../heap/vec/heapvec.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

/*
 * PQHeap Class - Heap-based Priority Queue Implementation
 * 
 * This class implements a priority queue using a binary max-heap as the underlying
 * data structure. It combines the abstract priority queue interface (PQ) with the
 * efficient heap operations provided by HeapVec.
 * 
 * Inheritance Structure:
 * - Virtual inheritance from PQ<Data>: Provides abstract priority queue interface
 * - Public inheritance from HeapVec<Data>: Leverages binary heap implementation
 * 
 * The virtual inheritance prevents diamond inheritance issues and ensures that
 * only one instance of any common base classes exists in the inheritance hierarchy.
 * 
 * Performance Characteristics:
 * - Tip(): O(1) - Access highest priority element
 * - Insert(): O(log n) - Insert new element and maintain heap property
 * - RemoveTip()/TipNRemove(): O(log n) - Remove highest priority element
 * - Change(): O(n) for value-based, O(log n) for index-based modification
 * - Memory: O(n) space complexity with dynamic capacity management
 * 
 * Template Parameter:
 * - Data: The type of elements stored in the priority queue, must be comparable
 */
template <typename Data>
class PQHeap : virtual public PQ<Data>,
               public HeapVec<Data> {

private:

  // Current capacity for efficient memory management
  // This allows the priority queue to grow and shrink dynamically
  // while minimizing memory allocations and maintaining performance
  ulong capacity = 0;

protected:

  // Import size from Container base class for consistent size management
  using Container::size;

  // Auxiliary Functions for Internal Operations
  
  /*
   * Insert element and maintain heap property through heapification
   * These functions handle the insertion process by adding the element
   * and then restoring the heap property using HeapifyUp operation
   * 
   * Parameters:
   * - element: The data to insert (copy or move version)
   * 
   * Time Complexity: O(log n) - due to HeapifyUp operation
   * Space Complexity: O(1) - constant additional space
   */
  void InsertWithHeapify(const Data&);
  void InsertWithHeapify(Data&&);
  
  /*
   * Clear all data and reset capacity to initial state
   * This function provides a clean reset of the priority queue,
   * releasing all memory and resetting internal state variables
   * 
   * Time Complexity: O(n) - to destroy all elements
   * Space Complexity: O(1) - no additional space required
   */
  void ClearAll();
  
  // Capacity Management Functions for Dynamic Memory Optimization
  
  /*
   * Ensure the underlying storage has at least the specified capacity
   * This function handles memory allocation and reallocation to maintain
   * efficient operations while preventing unnecessary memory waste
   * 
   * Parameters:
   * - minCapacity: Minimum required capacity
   * 
   * Time Complexity: O(n) - when reallocation is needed
   * Space Complexity: O(n) - for the new storage allocation
   */
  void EnsureCapacity(ulong minCapacity);
  
  /*
   * Reduce capacity when the priority queue becomes significantly smaller
   * This function helps minimize memory usage by shrinking the underlying
   * storage when it's much larger than needed
   * 
   * Time Complexity: O(n) - when reallocation occurs
   * Space Complexity: O(n) - for the new smaller storage
   */
  void ShrinkCapacity();

public:

  // Constructors
  
  /*
   * Default Constructor
   * Creates an empty priority queue with minimal initial capacity
   * 
   * Time Complexity: O(1)
   * Space Complexity: O(1)
   * Exception Safety: No-throw guarantee
   */
  PQHeap();

  /* ************************************************************************ */

  // Specific Constructors for Various Initialization Scenarios
  
  /*
   * Constructor with Initial Capacity
   * Creates an empty priority queue with pre-allocated space for efficient insertions
   * 
   * Parameters:
   * - initialCapacity: The initial capacity to allocate
   * 
   * Time Complexity: O(1)
   * Space Complexity: O(n) where n is the initial capacity
   * Exception Safety: Strong guarantee (may throw std::bad_alloc)
   */
  explicit PQHeap(const ulong);
  
  /*
   * Constructor from TraversableContainer
   * Creates a priority queue from any traversable container by copying all elements
   * and applying heapification to establish the heap property
   * 
   * Parameters:
   * - container: Source container to copy elements from
   * 
   * Time Complexity: O(n) - linear heapification using Floyd's algorithm
   * Space Complexity: O(n) - for storing all elements
   * Exception Safety: Strong guarantee
   */
  PQHeap(const TraversableContainer<Data>&);
  
  /*
   * Constructor from MappableContainer (Move Semantics)
   * Creates a priority queue by moving elements from a mappable container
   * This is more efficient than copying when the source container is temporary
   * 
   * Parameters:
   * - container: Source container to move elements from
   * 
   * Time Complexity: O(n) - for heapification after moving elements
   * Space Complexity: O(n) - for the moved elements
   * Exception Safety: Strong guarantee
   */
  PQHeap(MappableContainer<Data>&&);

  /* ************************************************************************ */

  // Copy and Move Constructors
  
  /*
   * Copy Constructor
   * Creates a new priority queue as a deep copy of another priority queue
   * All elements are copied and the heap property is preserved
   * 
   * Parameters:
   * - other: The priority queue to copy from
   * 
   * Time Complexity: O(n) - to copy all elements
   * Space Complexity: O(n) - for the copied elements
   * Exception Safety: Strong guarantee
   */
  PQHeap(const PQHeap&);

  /*
   * Move Constructor
   * Creates a new priority queue by moving resources from another priority queue
   * The source priority queue is left in a valid but unspecified state
   * 
   * Parameters:
   * - other: The priority queue to move from
   * 
   * Time Complexity: O(1) - constant time resource transfer
   * Space Complexity: O(1) - no additional space needed
   * Exception Safety: No-throw guarantee
   */
  PQHeap(PQHeap&&) noexcept;

  /* ************************************************************************ */

  /*
   * Destructor
   * The destructor is defaulted as the base classes handle proper cleanup
   * HeapVec destructor will handle memory deallocation
   * 
   * Time Complexity: O(n) - to destroy all elements
   * Space Complexity: O(1)
   * Exception Safety: No-throw guarantee
   */
  virtual ~PQHeap() = default;

  /* ************************************************************************ */

  // Assignment Operators
  
  /*
   * Copy Assignment Operator
   * Assigns the contents of another priority queue to this one
   * All existing elements are replaced with copies from the source
   * 
   * Parameters:
   * - other: The priority queue to copy from
   * 
   * Returns: Reference to this priority queue for chaining
   * 
   * Time Complexity: O(n + m) where n is current size, m is source size
   * Space Complexity: O(m) - for the copied elements
   * Exception Safety: Strong guarantee
   */
  PQHeap& operator=(const PQHeap&);

  /*
   * Move Assignment Operator
   * Moves the contents from another priority queue to this one
   * The source priority queue is left in a valid but unspecified state
   * 
   * Parameters:
   * - other: The priority queue to move from
   * 
   * Returns: Reference to this priority queue for chaining
   * 
   * Time Complexity: O(n) - to destroy current elements
   * Space Complexity: O(1) - no additional space needed
   * Exception Safety: No-throw guarantee
   */
  PQHeap& operator=(PQHeap&&) noexcept;

  /* ************************************************************************ */

  // Comparison Operators
  
  /*
   * Equality Comparison Operator
   * Compares two priority queues for structural equality
   * Two priority queues are equal if they contain the same elements
   * in the same heap structure (not just the same priority order)
   * 
   * Parameters:
   * - other: The priority queue to compare with
   * 
   * Returns: true if both priority queues are structurally identical
   * 
   * Time Complexity: O(n) - must compare all elements
   * Space Complexity: O(1) - constant additional space
   * Exception Safety: No-throw guarantee
   */
  bool operator==(const PQHeap&) const noexcept;
  
  /*
   * Inequality Comparison Operator
   * Logical negation of the equality operator
   * 
   * Parameters:
   * - other: The priority queue to compare with
   * 
   * Returns: true if priority queues are not structurally identical
   * 
   * Time Complexity: O(n) - delegates to equality operator
   * Space Complexity: O(1) - constant additional space
   * Exception Safety: No-throw guarantee
   */
  bool operator!=(const PQHeap&) const noexcept;

  /* ************************************************************************ */

  // Priority Queue Operations (Inherited from PQ)
  
  /*
   * Access Highest Priority Element
   * Returns a const reference to the element with the highest priority
   * without removing it from the priority queue
   * 
   * Returns: Const reference to the highest priority element
   * 
   * Throws: std::length_error if the priority queue is empty
   * 
   * Time Complexity: O(1) - highest priority element is always at root
   * Space Complexity: O(1) - no additional space required
   * Exception Safety: Strong guarantee
   */
  const Data& Tip() const override;
  
  /*
   * Remove and Return Highest Priority Element
   * Atomically removes and returns the highest priority element
   * This is more efficient than separate Tip() and RemoveTip() calls
   * 
   * Returns: Copy of the highest priority element
   * 
   * Throws: std::length_error if the priority queue is empty
   * 
   * Time Complexity: O(log n) - requires HeapifyDown after removal
   * Space Complexity: O(1) - constant additional space
   * Exception Safety: Strong guarantee
   */
  Data TipNRemove() override;
  
  /*
   * Remove Highest Priority Element
   * Removes the highest priority element without returning its value
   * This is more efficient than TipNRemove() when the value is not needed
   * 
   * Throws: std::length_error if the priority queue is empty
   * 
   * Time Complexity: O(log n) - requires HeapifyDown after removal
   * Space Complexity: O(1) - constant additional space
   * Exception Safety: Strong guarantee
   */
  void RemoveTip() override;

  /*
   * Insert Element (Copy Version)
   * Inserts a new element into the priority queue by copying
   * The heap property is maintained through HeapifyUp operation
   * 
   * Parameters:
   * - element: The element to insert (copied)
   * 
   * Time Complexity: O(log n) - due to HeapifyUp operation
   * Space Complexity: O(1) - constant additional space (amortized)
   * Exception Safety: Strong guarantee
   */
  void Insert(const Data&) override;
  
  /*
   * Insert Element (Move Version)
   * Inserts a new element into the priority queue by moving
   * More efficient than copy version for expensive-to-copy types
   * 
   * Parameters:
   * - element: The element to insert (moved)
   * 
   * Time Complexity: O(log n) - due to HeapifyUp operation
   * Space Complexity: O(1) - constant additional space (amortized)
   * Exception Safety: Strong guarantee
   */
  void Insert(Data&&) override;

  /*
   * Change Element Priority (Value-based, Copy Version)
   * Changes the priority of an element by replacing old value with new value
   * Searches for the old value and replaces it with the new value
   * 
   * Parameters:
   * - oldValue: The current value to find and replace
   * - newValue: The new value to replace with (copied)
   * 
   * Throws: std::length_error if old value is not found
   * 
   * Time Complexity: O(n) - linear search + O(log n) heap maintenance
   * Space Complexity: O(1) - constant additional space
   * Exception Safety: Strong guarantee
   */
  void Change(const Data&, const Data&) override;
  
  /*
   * Change Element Priority (Value-based, Move Version)
   * More efficient version using move semantics for the new value
   * 
   * Parameters:
   * - oldValue: The current value to find and replace
   * - newValue: The new value to replace with (moved)
   * 
   * Throws: std::length_error if old value is not found
   * 
   * Time Complexity: O(n) - linear search + O(log n) heap maintenance
   * Space Complexity: O(1) - constant additional space
   * Exception Safety: Strong guarantee
   */
  void Change(const Data&, Data&&) override;
  
  /*
   * Change Element Priority (Index-based, Copy Version)
   * Changes the priority of an element at a specific index
   * More efficient than value-based change as it avoids linear search
   * 
   * Parameters:
   * - index: The index of the element to change
   * - newValue: The new value to assign (copied)
   * 
   * Throws: std::out_of_range if index is invalid
   * 
   * Time Complexity: O(log n) - direct access + heap maintenance
   * Space Complexity: O(1) - constant additional space
   * Exception Safety: Strong guarantee
   */
  void Change(const ulong&, const Data&) override;
  
  /*
   * Change Element Priority (Index-based, Move Version)
   * More efficient version using move semantics for the new value
   * 
   * Parameters:
   * - index: The index of the element to change
   * - newValue: The new value to assign (moved)
   * 
   * Throws: std::out_of_range if index is invalid
   * 
   * Time Complexity: O(log n) - direct access + heap maintenance
   * Space Complexity: O(1) - constant additional space
   * Exception Safety: Strong guarantee
   */
  void Change(const ulong&, Data&&) override;

protected:

  /*
   * Protected Linear Container Operations
   * 
   * These functions are inherited from HeapVec but made protected to prevent
   * direct access to the internal heap structure. In a priority queue, elements
   * should only be accessed through the priority queue interface (Tip, Insert, etc.)
   * to maintain the heap property and priority-based semantics.
   * 
   * Direct access to arbitrary positions would violate the priority queue
   * abstraction and could lead to inconsistent state if used incorrectly.
   * 
   * Available but protected operations:
   * - operator[]: Direct access to elements by index
   * - Front(): Access to the first element (same as Tip but different semantics)
   * - Back(): Access to the last element in the underlying array
   * 
   * These are available to derived classes but not to external users.
   */
  using HeapVec<Data>::operator[];
  using HeapVec<Data>::Front;
  using HeapVec<Data>::Back;

};

/* ************************************************************************** */

}

#include "pqheap.cpp"

#endif
