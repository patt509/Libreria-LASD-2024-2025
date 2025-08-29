
/*
 * pq.hpp - Abstract Priority Queue Interface
 * 
 * This file defines the abstract interface for priority queue data structures, which are
 * specialized containers that provide efficient access to the highest (or lowest) priority
 * element. Priority queues are fundamental data structures used in algorithms like Dijkstra's
 * shortest path, A* search, task scheduling, and event simulation.
 * 
 * The priority queue interface supports:
 * - O(1) access to the highest priority element (Tip)
 * - O(log n) insertion of new elements with arbitrary priorities
 * - O(log n) removal of the highest priority element
 * - O(log n) priority modification of existing elements
 * - Integration with linear container hierarchy for iteration and access
 * 
 * Priority queues differ from regular queues by serving elements based on priority
 * rather than insertion order (FIFO). This makes them ideal for scenarios where
 * processing order depends on element importance or urgency.
 * 
 * Key Design Features:
 * - Abstract interface enabling multiple implementation strategies
 * - Support for both copy and move semantics for efficient element handling
 * - Priority modification operations for dynamic priority adjustment
 * - Integration with container hierarchy for polymorphic usage
 */

#ifndef PQ_HPP
#define PQ_HPP

/* ************************************************************************** */

#include "../container/linear.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

/*
 * PQ - Abstract Priority Queue Interface
 * 
 * Abstract base class for priority queue implementations providing priority-based
 * element access and manipulation. A priority queue maintains elements in a way
 * that allows efficient access to the element with the highest priority.
 * 
 * This class extends the linear container hierarchy to provide:
 * - LinearContainer: Enables indexed access and iteration capabilities
 * - ClearableContainer: Provides efficient clearing of all elements
 * 
 * The priority queue operations enable:
 * - Constant time access to highest priority element
 * - Logarithmic time insertion and deletion operations
 * - Dynamic priority modification for existing elements
 * - Efficient bulk operations for priority-based algorithms
 * 
 * Concrete implementations must define:
 * - Storage strategy (heap-based, sorted list, etc.)
 * - Priority ordering (max-priority or min-priority)
 * - Element insertion and removal algorithms
 * - Priority modification mechanisms
 * 
 * Common implementations include:
 * - Binary heap: Efficient O(log n) operations with O(1) space overhead
 * - Binomial heap: Supports efficient merge operations
 * - Fibonacci heap: Optimized for decrease-key operations
 */

template <typename Data>
class PQ : virtual public LinearContainer<Data>,
           virtual public ClearableContainer {

private:

protected:

public:

  // Destructor
  virtual ~PQ() = default;

  /* ************************************************************************ */

  /*
   * Assignment Operators
   * 
   * Assignment operations are disabled for abstract priority queue types to prevent
   * object slicing and maintain type safety in polymorphic hierarchies. Concrete
   * implementations should provide their own assignment operators with appropriate
   * semantics for the specific priority queue implementation.
   */

  // Copy assignment - Disabled for abstract base class
  // Prevents object slicing when assigning through base class pointers/references
  // Concrete derived classes must implement their own copy assignment operators
  PQ& operator=(const PQ&) = delete;

  // Move assignment - Disabled for abstract base class  
  // Prevents resource management issues in polymorphic contexts
  // Concrete derived classes must implement their own move assignment operators
  PQ& operator=(PQ&&) noexcept = delete;

  /* ************************************************************************ */

  /*
   * Comparison Operators
   * 
   * Comparison operations are disabled for abstract priority queue types because:
   * 1. Priority queue equality depends on implementation-specific details
   * 2. Element ordering in priority queues is based on priority, not insertion order
   * 3. Different implementations may have incompatible internal representations
   * 
   * Concrete implementations should define comparison semantics appropriate for
   * their specific priority queue behavior and use cases.
   */

  // Equality comparison - Disabled for abstract interface
  // Priority queue equality semantics depend on concrete implementation details
  bool operator==(const PQ&) const noexcept = delete;

  // Inequality comparison - Disabled for abstract interface
  // Derived from equality operator, also implementation-dependent
  bool operator!=(const PQ&) const noexcept = delete;

  /* ************************************************************************ */

  /*
   * Priority Queue Specific Operations
   * 
   * Core operations that define priority queue behavior, providing efficient
   * access to highest priority elements and dynamic priority management.
   * All operations maintain the priority queue invariant that the highest
   * priority element is always accessible in constant time.
   */

  /*
   * Tip() - Access Highest Priority Element
   * 
   * Returns a constant reference to the element with the highest priority
   * without removing it from the priority queue. This operation provides
   * O(1) access to the most important element for inspection or processing.
   * 
   * Returns: const Data& - Reference to highest priority element
   * Throws: std::length_error - If priority queue is empty
   * Complexity: O(1) - Constant time access regardless of queue size
   * 
   * Usage: Essential for algorithms that need to examine the next element
   * to process without committing to its removal (e.g., conditional processing)
   */
  virtual const Data& Tip() const = 0;

  /*
   * RemoveTip() - Remove Highest Priority Element
   * 
   * Removes the element with the highest priority from the priority queue
   * without returning its value. This operation is more efficient than
   * TipNRemove when the element value is not needed, as it avoids copying.
   * 
   * Throws: std::length_error - If priority queue is empty
   * Complexity: O(log n) - Logarithmic time due to heap property restoration
   * 
   * Side Effects: Reduces queue size by 1, maintains priority queue invariant
   * Usage: Optimal for bulk processing where element values are already known
   */
  virtual void RemoveTip() = 0;

  /*
   * TipNRemove() - Access and Remove Highest Priority Element
   * 
   * Atomically retrieves the highest priority element and removes it from
   * the priority queue. This combines the functionality of Tip() and RemoveTip()
   * in a single operation, ensuring consistency in concurrent access scenarios.
   * 
   * Returns: Data - Copy of the highest priority element (moved if possible)
   * Throws: std::length_error - If priority queue is empty
   * Complexity: O(log n) - Dominated by heap property restoration after removal
   * 
   * Usage: Standard operation for priority-based processing algorithms
   * where each element is processed exactly once in priority order
   */
  virtual Data TipNRemove() = 0;

  /*
   * Insert() - Add New Element with Priority
   * 
   * Inserts a new element into the priority queue, automatically positioning
   * it according to its priority value. The priority queue maintains its
   * ordering invariant after insertion, ensuring efficient future access.
   * 
   * Two overloads support both copy and move semantics for optimal performance:
   * - Copy version: Creates internal copy of the element
   * - Move version: Transfers ownership of the element (more efficient)
   * 
   * Parameters: 
   *   const Data& - Element to insert (copy semantics)
   *   Data&& - Element to insert (move semantics)
   * Complexity: O(log n) - Logarithmic insertion with heap property maintenance
   * 
   * Side Effects: Increases queue size by 1, may trigger internal reorganization
   * Usage: Primary method for adding elements to priority-based processing queues
   */
  virtual void Insert(const Data&) = 0; // Copy insertion
  virtual void Insert(Data&&) = 0;      // Move insertion

  /*
   * Change() - Modify Element Priority
   * 
   * Updates the priority of an existing element in the priority queue,
   * maintaining the priority queue invariant through internal reorganization.
   * This operation is essential for algorithms with dynamic priorities.
   * 
   * Four overloads provide flexible element identification and value semantics:
   * - By value identification with copy/move replacement
   * - By index identification with copy/move replacement
   * 
   * Value-based Change:
   *   Locates element matching the first parameter and replaces it with second
   *   Useful when elements are uniquely identifiable by value
   * 
   * Index-based Change:
   *   Directly modifies element at specified index position
   *   More efficient when element position is known
   * 
   * Parameters:
   *   const Data& old_val, const Data& new_val - Find old_val, replace with new_val (copy)
   *   const Data& old_val, Data&& new_val - Find old_val, replace with new_val (move)
   *   const ulong& index, const Data& new_val - Replace element at index (copy)
   *   const ulong& index, Data&& new_val - Replace element at index (move)
   * 
   * Complexity: 
   *   Value-based: O(n + log n) - Linear search + logarithmic reorganization
   *   Index-based: O(log n) - Direct access + logarithmic reorganization
   * 
   * Usage: Critical for algorithms like Dijkstra's algorithm where priorities
   * change dynamically based on new information or path discoveries
   */
  virtual void Change(const Data&, const Data&) = 0;        // Value-based change (copy)
  virtual void Change(const Data&, Data&&) = 0;             // Value-based change (move)
  virtual void Change(const ulong&, const Data&) = 0;       // Index-based change (copy)
  virtual void Change(const ulong&, Data&&) = 0;            // Index-based change (move)

};

/* ************************************************************************** */

}

#endif
