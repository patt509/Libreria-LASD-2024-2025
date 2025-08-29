/*
 * Priority Queue Implementation using Binary Heap Data Structure
 * 
 * This file contains the implementation of PQHeap class methods, providing
 * a complete heap-based priority queue with efficient operations and
 * dynamic memory management.
 * 
 * Key Implementation Features:
 * - Dynamic capacity management with exponential growth and shrinkage
 * - Efficient heap maintenance using HeapifyUp and HeapifyDown operations
 * - Strong exception safety guarantees for all operations
 * - Move semantics support for performance optimization
 * - Comprehensive error handling with appropriate exception types
 * 
 * Performance Optimizations:
 * - Capacity management minimizes memory allocations
 * - Move semantics reduce unnecessary copying
 * - Direct heap operations avoid redundant validations
 * - Efficient element swapping using std::move
 */

#include "pqheap.hpp"
#include <stdexcept>
#include <utility>

namespace lasd {

/* ************************************************************************** */

// PQHEAP CLASS IMPLEMENTATION

/*
 * Default Constructor
 * Creates an empty priority queue with minimal initial state
 * Initializes capacity to 0 for lazy allocation strategy
 */
template <typename Data>
PQHeap<Data>::PQHeap() : HeapVec<Data>() {
  capacity = 0;
}

// Specific Constructors

/*
 * Constructor with Initial Capacity
 * Pre-allocates space for efficient insertions, avoiding early reallocations
 * Delegates heap initialization to HeapVec constructor
 */
template <typename Data>
PQHeap<Data>::PQHeap(const ulong newSize) : HeapVec<Data>(newSize) {
  capacity = newSize;
}

/*
 * Constructor from TraversableContainer
 * Creates priority queue by copying elements and applying heapification
 * HeapVec constructor handles the heapification process automatically
 */
template <typename Data>
PQHeap<Data>::PQHeap(const TraversableContainer<Data>& container) : HeapVec<Data>(container) {
  capacity = this->size;
}

/*
 * Constructor from MappableContainer (Move Semantics)
 * More efficient construction by moving elements instead of copying
 * Particularly beneficial for containers with expensive-to-copy elements
 */
template <typename Data>
PQHeap<Data>::PQHeap(MappableContainer<Data>&& container) : HeapVec<Data>(std::move(container)) {
  capacity = this->size;
}

/*
 * Copy Constructor
 * Creates deep copy of another priority queue, preserving heap structure
 * Copies both data and capacity information for consistent behavior
 */
template <typename Data>
PQHeap<Data>::PQHeap(const PQHeap<Data>& other) : HeapVec<Data>(other) {
  capacity = other.capacity;
}

/*
 * Move Constructor
 * Efficiently transfers ownership of resources from another priority queue
 * Leaves source in valid but empty state with zero capacity
 */
template <typename Data>
PQHeap<Data>::PQHeap(PQHeap<Data>&& other) noexcept : HeapVec<Data>(std::move(other)) {
  capacity = other.capacity;
  other.capacity = 0;
}

/*
 * Copy Assignment Operator
 * Replaces current contents with deep copy of another priority queue
 * HeapVec assignment handles element copying and heap property maintenance
 */
template <typename Data>
PQHeap<Data>& PQHeap<Data>::operator=(const PQHeap<Data>& other) {
  HeapVec<Data>::operator=(other);
  capacity = other.capacity;
  return *this;
}

/*
 * Move Assignment Operator
 * Efficiently transfers ownership while cleaning up current resources
 * Source priority queue is left in valid but empty state
 */
template <typename Data>
PQHeap<Data>& PQHeap<Data>::operator=(PQHeap<Data>&& other) noexcept {
  HeapVec<Data>::operator=(std::move(other));
  capacity = other.capacity;
  other.capacity = 0;
  return *this;
}

// Comparison Operators

/*
 * Equality Comparison Operator
 * Compares priority queues for structural equality
 * Delegates to HeapVec comparison which checks heap structure
 */
template <typename Data>
bool PQHeap<Data>::operator==(const PQHeap<Data>& other) const noexcept {
  return HeapVec<Data>::operator==(other);
}

/*
 * Inequality Comparison Operator
 * Logical negation of equality comparison
 */
template <typename Data>
bool PQHeap<Data>::operator!=(const PQHeap<Data>& other) const noexcept {
  return !(*this == other);
}

// Priority Queue Operations Implementation

/*
 * Access Highest Priority Element (Tip)
 * Returns const reference to root element without modification
 * Root element (index 0) always contains highest priority in max-heap
 */
template <typename Data>
const Data& PQHeap<Data>::Tip() const {
  if (this->size == 0)
    throw std::length_error("Priority queue is empty");
  return this->Elements[0];
}

/*
 * Remove Highest Priority Element
 * Optimized implementation with integrated capacity management
 * 
 * Algorithm:
 * 1. Handle empty queue error case
 * 2. Special case for single element: direct removal
 * 3. General case: move last element to root and heapify down
 * 4. Shrink capacity if appropriate to minimize memory usage
 */
template <typename Data>
void PQHeap<Data>::RemoveTip() {
  if (this->size == 0)
    throw std::length_error("Priority queue is empty");
  
  if (this->size == 1) {
    this->size = 0;
    ShrinkCapacity();
    return;
  }
  
  // Move the last element to root and restore heap property
  this->Elements[0] = std::move(this->Elements[this->size - 1]);
  this->size--;
  this->HeapifyDown(0);
  ShrinkCapacity();
}

/*
 * Access and Remove Highest Priority Element (TipNRemove)
 * Atomically retrieves and removes the highest priority element
 * More efficient than separate Tip() and RemoveTip() calls
 * 
 * Algorithm:
 * 1. Store root element value for return
 * 2. Handle single element case with capacity management
 * 3. General case: replace root with last element and heapify
 * 4. Return the original root value
 */
template <typename Data>
Data PQHeap<Data>::TipNRemove() {
  if (this->size == 0)
    throw std::length_error("Priority queue is empty");
  
  Data result = this->Elements[0];
  
  if (this->size == 1) {
    this->size = 0;
    ShrinkCapacity();
  } else {
    // Replace root with last element and restore heap property
    this->Elements[0] = std::move(this->Elements[this->size - 1]);
    this->size--;
    this->HeapifyDown(0);
    ShrinkCapacity();
  }
  
  return result;
}

/*
 * Insert Element (Copy Version)
 * Adds new element to priority queue maintaining heap property
 * Uses capacity management for efficient memory allocation
 * 
 * Algorithm:
 * 1. Ensure sufficient capacity for new element
 * 2. Place element at end of heap (last position)
 * 3. Restore heap property using HeapifyUp from insertion point
 */
template <typename Data>
void PQHeap<Data>::Insert(const Data& value) {
  EnsureCapacity(this->size + 1);
  this->Elements[this->size] = value;
  this->size++;
  this->HeapifyUp(this->size - 1);
}

/*
 * Insert Element (Move Version)
 * More efficient insertion using move semantics
 * Particularly beneficial for expensive-to-copy data types
 */
template <typename Data>
void PQHeap<Data>::Insert(Data&& value) {
  EnsureCapacity(this->size + 1);
  this->Elements[this->size] = std::move(value);
  this->size++;
  this->HeapifyUp(this->size - 1);
}

/*
 * Change Element Priority by Value (Copy Version)
 * Searches for old value and replaces with new value
 * Efficiently maintains heap property based on priority change direction
 * 
 * Algorithm:
 * 1. Linear search to find element with old value
 * 2. Replace with new value
 * 3. Determine heap maintenance direction based on priority comparison
 * 4. Apply appropriate heapify operation (up or down)
 */
template <typename Data>
void PQHeap<Data>::Change(const Data& oldValue, const Data& newValue) {
  ulong idx = 0;
  while (idx < this->size && !(this->Elements[idx] == oldValue)) {
    ++idx;
  }
  
  if (idx == this->size)
    throw std::length_error("Value not found");
  
  Data oldData = this->Elements[idx];
  this->Elements[idx] = newValue;
  
  // Restore heap property based on priority change direction
  if (newValue > oldData)
    this->HeapifyUp(idx);        // Priority increased: bubble up
  else if (newValue < oldData)
    this->HeapifyDown(idx);      // Priority decreased: bubble down
}

/*
 * Change Element Priority by Value (Move Version)
 * More efficient version using move semantics for new value
 * Uses move construction to avoid unnecessary copying
 */
template <typename Data>
void PQHeap<Data>::Change(const Data& oldValue, Data&& newValue) {
  ulong idx = 0;
  while (idx < this->size && !(this->Elements[idx] == oldValue)) {
    ++idx;
  }
  
  if (idx == this->size)
    throw std::length_error("Value not found");
  
  // Store old value before moving new value into position
  Data oldData = std::move(this->Elements[idx]);
  this->Elements[idx] = std::move(newValue);
  
  // Restore heap property based on priority comparison
  if (this->Elements[idx] > oldData)
    this->HeapifyUp(idx);
  else if (this->Elements[idx] < oldData)
    this->HeapifyDown(idx);
}

/*
 * Change Element Priority by Index (Copy Version)
 * More efficient than value-based change as it avoids linear search
 * Direct access by index provides O(log n) complexity instead of O(n)
 */
template <typename Data>
void PQHeap<Data>::Change(const ulong& idx, const Data& newValue) {
  if (idx >= this->size)
    throw std::out_of_range("Index out of range");
  
  Data oldData = this->Elements[idx];
  this->Elements[idx] = newValue;
  
  // Determine and apply appropriate heap maintenance
  if (newValue > oldData)
    this->HeapifyUp(idx);
  else if (newValue < oldData)
    this->HeapifyDown(idx);
}

/*
 * Change Element Priority by Index (Move Version)
 * Most efficient priority change operation combining direct access with move semantics
 */
template <typename Data>
void PQHeap<Data>::Change(const ulong& idx, Data&& newValue) {
  if (idx >= this->size)
    throw std::out_of_range("Index out of range");
  
  Data oldData = std::move(this->Elements[idx]);
  this->Elements[idx] = std::move(newValue);
  
  if (this->Elements[idx] > oldData)
    this->HeapifyUp(idx);
  else if (this->Elements[idx] < oldData)
    this->HeapifyDown(idx);
}

// Auxiliary Functions Implementation

/*
 * Insert with Heapify (Copy Version)
 * Internal helper function that combines insertion with heap property maintenance
 * Used by public Insert methods and constructor implementations
 */
template <typename Data>
void PQHeap<Data>::InsertWithHeapify(const Data& value) {
  EnsureCapacity(this->size + 1);
  this->Elements[this->size] = value;
  this->size++;
  this->HeapifyUp(this->size - 1);
}

/*
 * Insert with Heapify (Move Version)
 * Move semantics version for performance optimization
 */
template <typename Data>
void PQHeap<Data>::InsertWithHeapify(Data&& value) {
  EnsureCapacity(this->size + 1);
  this->Elements[this->size] = std::move(value);
  this->size++;
  this->HeapifyUp(this->size - 1);
}

/*
 * Clear All Data and Reset Capacity
 * Complete cleanup function that deallocates memory and resets state
 * Used for implementing efficient assignment and destruction operations
 */
template <typename Data>
void PQHeap<Data>::ClearAll() {
  // Deallocate existing elements array
  if (this->Elements != nullptr) {
    delete[] this->Elements;
    this->Elements = nullptr;
  }
  this->size = 0;
  capacity = 0;  // Reset capacity to enable fresh allocation strategy
}

// Dynamic Capacity Management Implementation

/*
 * Ensure Minimum Capacity is Available
 * Implements exponential growth strategy for amortized O(1) insertion performance
 * 
 * Growth Strategy:
 * - Start with capacity 1 for first allocation
 * - Double capacity until minimum requirement is met
 * - This ensures amortized O(1) insertion time complexity
 * 
 * Memory Management:
 * - Creates new array with increased capacity
 * - Moves existing elements to new location
 * - Deallocates old array to prevent memory leaks
 */
template <typename Data>
void PQHeap<Data>::EnsureCapacity(ulong minCapacity) {
  if (capacity < minCapacity) {
    // Exponential growth: double capacity or use minCapacity, whichever is larger
    ulong newCapacity = (capacity == 0) ? 1 : capacity;
    while (newCapacity < minCapacity) {
      newCapacity *= 2;
    }
    
    // Allocate new array with increased capacity
    Data* newElements = new Data[newCapacity]{};
    
    // Move existing elements to new location for efficiency
    for (ulong i = 0; i < this->size; i++) {
      newElements[i] = std::move(this->Elements[i]);
    }
    
    // Clean up old array and update internal state
    delete[] this->Elements;
    this->Elements = newElements;
    capacity = newCapacity;
  }
}

/*
 * Shrink Capacity for Memory Optimization
 * Implements capacity reduction strategy to minimize memory usage
 * 
 * Shrinkage Strategy:
 * - Shrink when size is ≤ 1/4 of capacity and capacity > 4
 * - Reduce capacity by half to maintain reasonable buffer
 * - Prevents pathological shrinking for small priority queues
 * 
 * Memory Management:
 * - Creates new smaller array when shrinkage is beneficial
 * - Moves elements to new location efficiently
 * - Maintains minimum capacity for performance
 * 
 * Performance Considerations:
 * - Avoids frequent reallocations by using 25% threshold
 * - Maintains reasonable buffer space for future insertions
 * - Balances memory usage with performance requirements
 */
template <typename Data>
void PQHeap<Data>::ShrinkCapacity() {
  // Apply shrinking policy: shrink if size ≤ 25% of capacity and capacity > 4
  if (capacity > 4 && this->size <= capacity / 4) {
    ulong newCapacity = capacity / 2;
    if (newCapacity < this->size) {
      newCapacity = this->size;  // Ensure capacity is never less than size
    }
    
    // Allocate new smaller array
    Data* newElements = new Data[newCapacity]{};
    
    // Move existing elements to new location
    for (ulong i = 0; i < this->size; i++) {
      newElements[i] = std::move(this->Elements[i]);
    }
    
    // Clean up old array and update capacity
    delete[] this->Elements;
    this->Elements = newElements;
    capacity = newCapacity;
  }
}

/* ************************************************************************** */

} // namespace lasd
