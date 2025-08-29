
/*
 * heapvec.cpp - Vector-Based Binary Heap Implementation
 * 
 * This file contains the complete implementation of a binary heap using a dynamic vector
 * as the underlying storage mechanism. The implementation provides efficient heap operations
 * while maintaining the complete binary tree structure and heap property.
 * 
 * The heap implementation supports:
 * - Dynamic sizing with vector-based storage
 * - Efficient heap maintenance algorithms (heapify up/down)
 * - In-place heapsort algorithm with O(n log n) complexity
 * - Integration with container hierarchy for versatile usage
 * 
 * Array-based heap representation:
 * - Root element at index 0 (maximum for max-heap)
 * - Parent-child relationships: parent(i) = (i-1)/2, children(i) = 2i+1, 2i+2
 * - Complete binary tree property maintained automatically
 * - Efficient navigation using arithmetic operations on indices
 */

namespace lasd {

/* ************************************************************************** */

// HEAPVEC CLASS IMPLEMENTATION - CONSTRUCTORS AND INITIALIZATION

// Default constructor: Creates empty heap with default vector capacity
// Initializes underlying vector storage for immediate heap operations
template <typename Data>
HeapVec<Data>::HeapVec() : SortableVector<Data>() {
  // Inherits vector's default constructor - no heap property to establish yet
}

// Capacity constructor: Creates heap with specified initial capacity
// Optimizes performance when expected heap size is known in advance
// Does not establish heap property as no elements are present yet
template <typename Data>
HeapVec<Data>::HeapVec(const ulong newSize) : SortableVector<Data>(newSize) {
  // Underlying vector initialized with specified capacity
  // Heap property will be established when elements are added
}

// Container copy constructor: Creates heap from any traversable container
// Copies all elements from source container and establishes heap property
// Time complexity: O(n) for copying + O(n) for heapify = O(n) total
template <typename Data>
HeapVec<Data>::HeapVec(const TraversableContainer<Data>& container) : SortableVector<Data>(container) {
  // Elements copied via vector constructor, now establish heap property
  Heapify(); // O(n) bottom-up heapification more efficient than n insertions
}

// Container move constructor: Creates heap from mappable container using move semantics
// Moves elements from source container (emptying it) and establishes heap property
// More efficient for expensive-to-copy types, optimal resource utilization
template <typename Data>
HeapVec<Data>::HeapVec(MappableContainer<Data>&& container) : SortableVector<Data>(std::move(container)) {
  // Elements moved via vector constructor, now establish heap property
  Heapify(); // Required since moved elements may not satisfy heap property
}

// Copy constructor: Deep copy of another heap preserving heap structure
// No heapify needed as source heap already satisfies heap property
// Efficient copy that maintains heap invariants without reorganization
template <typename Data>
HeapVec<Data>::HeapVec(const HeapVec<Data>& other) : SortableVector<Data>(other) {
  // Vector copy constructor handles element duplication
  // Heap property preserved since source is already a valid heap
}

// Move constructor: Efficiently transfers ownership of heap resources
// Optimal performance with no data copying, maintains heap property
// Source object left in valid but unspecified state
template <typename Data>
HeapVec<Data>::HeapVec(HeapVec<Data>&& other) noexcept : SortableVector<Data>(std::move(other)) {
  // Vector move constructor handles resource transfer
  // Heap property preserved since source was a valid heap
}

/* ************************************************************************** */

// ASSIGNMENT OPERATORS - COPY AND MOVE SEMANTICS

// Copy assignment: Replace current heap with deep copy of another heap
// Handles self-assignment safely and maintains heap property
// No heapify needed as source heap structure is preserved
template <typename Data>
HeapVec<Data>& HeapVec<Data>::operator=(const HeapVec<Data>& other) {
  SortableVector<Data>::operator=(other); // Delegate to vector copy assignment
  return *this; // Heap property maintained through vector copy
}

// Move assignment: Efficiently replace current heap using move semantics
// Provides strong exception safety and optimal performance
// Maintains heap property while avoiding expensive copy operations
template <typename Data>
HeapVec<Data>& HeapVec<Data>::operator=(HeapVec<Data>&& other) noexcept {
  SortableVector<Data>::operator=(std::move(other)); // Delegate to vector move assignment
  return *this; // Heap property maintained through efficient resource transfer
}

/* ************************************************************************** */

// COMPARISON OPERATORS - STRUCTURAL EQUALITY

// Equality operator: Compares heap contents for exact structural match
// Two heaps are equal if they contain same elements in same positions
// Note: Different heap arrangements of same elements are considered different
template <typename Data>
bool HeapVec<Data>::operator==(const HeapVec<Data>& other) const noexcept {
  return SortableVector<Data>::operator==(other); // Delegate to vector comparison
}

// Inequality operator: Logical negation of equality comparison
// Returns true if heaps differ in size or element arrangement
template <typename Data>
bool HeapVec<Data>::operator!=(const HeapVec<Data>& other) const noexcept {
  return !(*this == other); // Efficient negation of equality test
}

/* ************************************************************************** */

// HEAP PROPERTY VALIDATION AND MAINTENANCE

// IsHeap: Validates heap property throughout entire structure
// Checks that every parent satisfies ordering constraint with its children
// Returns true if all parent-child relationships maintain heap ordering
// Time complexity: O(n) - examines all internal nodes in the heap
template <typename Data>
bool HeapVec<Data>::IsHeap() const noexcept {
  // Iterate through all nodes that have at least one child
  for (ulong i = 0; i < size; ++i) {
    // Check left child relationship if it exists
    if (HasLeftChild(i)) {
      if (Elements[i] < Elements[GetLeftChild(i)]) {
        return false; // Heap property violated: parent < left child
      }
    }
    // Check right child relationship if it exists
    if (HasRightChild(i)) {
      if (Elements[i] < Elements[GetRightChild(i)]) {
        return false; // Heap property violated: parent < right child
      }
    }
  }
  return true; // All parent-child relationships satisfy heap property
}

// Heapify: Establishes heap property using bottom-up approach
// Converts arbitrary array into valid heap structure efficiently
// More efficient than individual insertions: O(n) vs O(n log n)
// Uses Floyd's heap construction algorithm
template <typename Data>
void HeapVec<Data>::Heapify() {
  // Build heap from bottom up, starting from the last non-leaf node
  if (size > 1) {
    // Last non-leaf node is at index (size/2 - 1)
    // Process nodes from bottom to top to ensure heap property
    for (long i = (size / 2) - 1; i >= 0; --i) {
      HeapifyDown(i); // Restore heap property for subtree rooted at i
    }
  }
  // Empty heaps and single-element heaps are trivially valid heaps
}

/* ************************************************************************** */

// HEAPSORT ALGORITHM IMPLEMENTATION

// Sort: Performs in-place heapsort algorithm on heap elements
// Converts heap to sorted array, destroying heap property in the process
// Time complexity: O(n log n) - optimal comparison-based sorting algorithm
// Space complexity: O(1) - sorts in-place using existing storage
template <typename Data>
void HeapVec<Data>::Sort() {
  // HeapSort algorithm: extract maximum elements repeatedly
  if (size > 1) {
    // Phase 1: Ensure we have a valid max-heap
    Heapify();
    
    // Phase 2: Extract elements one by one from heap root
    for (ulong i = size - 1; i > 0; --i) {
      // Move current maximum (root) to its final sorted position
      std::swap(Elements[0], Elements[i]);
      
      // Restore heap property for reduced heap (excluding sorted elements)
      ulong heapSize = i; // Current heap size (excluding sorted tail)
      ulong current = 0;  // Start heapify from root
      
      // Manual heapify down with reduced size for efficiency
      while (true) {
        ulong largest = current;
        ulong leftChild = GetLeftChild(current);
        ulong rightChild = GetRightChild(current);
        
        // Find largest among current node and its children
        if (leftChild < heapSize && Elements[leftChild] > Elements[largest]) {
          largest = leftChild;
        }
        
        if (rightChild < heapSize && Elements[rightChild] > Elements[largest]) {
          largest = rightChild;
        }
        
        // If heap property is satisfied, we're done
        if (largest != current) {
          std::swap(Elements[current], Elements[largest]);
          current = largest; // Continue heapifying down
        } else {
          break; // Heap property restored
        }
      }
    }
  }
  // Result: Array sorted in ascending order, heap property no longer holds
}

/* ************************************************************************** */

// HEAP MAINTENANCE ALGORITHMS - INTERNAL OPERATIONS

// HeapifyUp: Restores heap property by moving element upward toward root
// Used after insertion to maintain heap ordering from leaf to root
// Continues until heap property is satisfied or root is reached
// Time complexity: O(log n) - maximum tree height traversal
template <typename Data>
void HeapVec<Data>::HeapifyUp(ulong index) {
  // Move element up the tree until heap property is satisfied
  while (index > 0) {
    ulong parentIndex = GetParent(index);
    
    // Check if heap property is violated (child > parent in max-heap)
    if (Elements[index] > Elements[parentIndex]) {
      std::swap(Elements[index], Elements[parentIndex]);
      index = parentIndex; // Continue checking upward
    } else {
      break; // Heap property satisfied, stop moving up
    }
  }
}

// HeapifyDown: Restores heap property by moving element downward toward leaves
// Used after root removal to maintain heap ordering from root to leaves
// Continues until heap property is satisfied or leaf level is reached
// Time complexity: O(log n) - maximum tree height traversal
template <typename Data>
void HeapVec<Data>::HeapifyDown(ulong index) {
  // Move element down the tree until heap property is satisfied
  while (HasLeftChild(index)) {
    // Find the largest child to potentially swap with
    ulong largestChild = GetLeftChild(index);
    
    // Check if right child exists and is larger than left child
    if (HasRightChild(index) && Elements[GetRightChild(index)] > Elements[largestChild]) {
      largestChild = GetRightChild(index);
    }
    
    // Check if heap property is already satisfied
    if (Elements[index] >= Elements[largestChild]) {
      break; // Parent >= largest child, heap property satisfied
    }
    
    // Swap with largest child and continue downward
    std::swap(Elements[index], Elements[largestChild]);
    index = largestChild;
  }
}

// BINARY TREE INDEX NAVIGATION - ARITHMETIC OPERATIONS
// Efficient parent-child relationships using array index arithmetic

// GetParent: Calculates parent index for given node position
// Uses the fundamental heap property: parent(i) = (i-1)/2
// Time complexity: O(1) - simple integer division
template <typename Data>
ulong HeapVec<Data>::GetParent(ulong index) const noexcept {
  return (index - 1) / 2; // Standard binary heap parent formula
}

// GetLeftChild: Calculates left child index for given parent position
// Uses the fundamental heap property: left_child(i) = 2*i + 1
// Time complexity: O(1) - simple arithmetic operation
template <typename Data>
ulong HeapVec<Data>::GetLeftChild(ulong index) const noexcept {
  return (2 * index) + 1; // Standard binary heap left child formula
}

// GetRightChild: Calculates right child index for given parent position
// Uses the fundamental heap property: right_child(i) = 2*i + 2
// Time complexity: O(1) - simple arithmetic operation
template <typename Data>
ulong HeapVec<Data>::GetRightChild(ulong index) const noexcept {
  return (2 * index) + 2; // Standard binary heap right child formula
}

// CHILD EXISTENCE VALIDATION - BOUNDARY CHECKING
// Safe navigation functions preventing out-of-bounds access

// HasLeftChild: Checks if node has valid left child within heap bounds
// Prevents array access violations during heap traversal operations
// Time complexity: O(1) - simple index comparison with heap size
template <typename Data>
bool HeapVec<Data>::HasLeftChild(ulong index) const noexcept {
  return GetLeftChild(index) < size; // Left child index must be within bounds
}

// HasRightChild: Checks if node has valid right child within heap bounds
// Essential for safe binary tree navigation and heap maintenance algorithms
// Time complexity: O(1) - simple index comparison with heap size
template <typename Data>
bool HeapVec<Data>::HasRightChild(ulong index) const noexcept {
  return GetRightChild(index) < size; // Right child index must be within bounds
}

// OPTIMIZED SWAPPING OPERATION

// SwapAt: Efficiently swaps elements at two positions using pre-fetched values
// Override of SortableLinearContainer method optimized for heap operations
// Reduces data movement by using already-fetched values from calling context
// Used internally by heapify algorithms to minimize memory accesses
template <typename Data>
void HeapVec<Data>::SwapAt(ulong i, ulong j, const Data& temp_i, const Data& temp_j) {
  // Direct assignment using pre-fetched values (more efficient than std::swap)
  this->Elements[i] = temp_j; // Place j's value at position i
  this->Elements[j] = temp_i; // Place i's value at position j
}

/* ************************************************************************** */

}
