/*
 * SetVec Implementation File
 * 
 * This file contains the complete implementation of the SetVec class, which
 * provides a mathematical set using a sorted dynamic array as the underlying
 * data structure. The implementation leverages binary search for efficient
 * operations while maintaining set semantics and sorted order.
 * 
 * Implementation highlights:
 * - Binary search for O(log n) element location
 * - Sorted insertion to maintain order invariant
 * - Efficient min/max operations due to sorted structure
 * - Circular access patterns for advanced navigation
 * - Capacity management for memory efficiency
 * - Set semantics with duplicate prevention
 */

#include "setvec.hpp"

namespace lasd {

/* ************************************************************************** */

// PROTECTED AUXILIARY METHODS
// Internal helper functions for set operations and maintenance

// CheckUnique: Verifies if an element would be unique in the set
// Returns true if element doesn't exist, false if it already exists
// Time complexity: O(log n) using binary search on sorted array
template <typename Data>
bool SetVec<Data>::CheckUnique(const Data& data) const noexcept {
  // Element is unique if it doesn't already exist in the set
  return !Exists(data);
}

// CircularGet (const): Access elements with circular wrap-around behavior
// Allows accessing elements beyond array bounds by wrapping to beginning
// Used for advanced iteration patterns and circular navigation
template <typename Data>
const Data& SetVec<Data>::CircularGet(ulong index) const {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  // Calculate circular index starting from current position
  // The modulo operation ensures wrap-around behavior
  return Elements[(current + index) % size];
}

// CircularGet (mutable): Mutable version of circular access
// Provides the same circular wrap-around behavior for modification operations
template <typename Data>
Data& SetVec<Data>::CircularGet(ulong index) {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  // Calculate circular index starting from current position
  return Elements[(current + index) % size];
}

// BinarySearch: Core search algorithm leveraging sorted array structure
// Implements standard binary search with optional insertion point calculation
// Time complexity: O(log n) - logarithmic search in sorted array
// Returns index of element if found, or -1 if not found
// If insertPoint is provided, sets it to the index where element should be inserted
template <typename Data>
long SetVec<Data>::BinarySearch(const Data& data, long* insertPoint) const noexcept {
    long low = 0;                              // Lower bound of search range
    long high = static_cast<long>(size) - 1;   // Upper bound of search range
    long mid = 0;                              // Middle point for binary division
    bool found = false;                        // Flag to track if element is found

    // Binary search loop with early termination
    while (low <= high && !found) {
        mid = (low + high) / 2;  // Calculate middle point avoiding overflow
        
        if (Elements[mid] == data) {
            found = true;        // Element found at mid position
        } else if (Elements[mid] < data) {
            low = mid + 1;       // Search in upper half
        } else {
            high = mid - 1;      // Search in lower half
        }
    }

    if (found) {
        // Element was found - return its index
        if (insertPoint != nullptr) {
            *insertPoint = mid; // For duplicates, could be mid + 1
        }
        return mid;
    } else {
        // Element not found - calculate insertion point for maintaining sorted order
        if (insertPoint != nullptr) {
            *insertPoint = low; // Position where element should be inserted
        }
        return -1; // Indicate element was not found
    }
}

// FindIndex: Simplified wrapper for element location
// Provides a clean interface for basic element location without insertion point
template <typename Data>
long SetVec<Data>::FindIndex(const Data& data) const noexcept {
  // Delegate to BinarySearch without requesting insertion point
  return BinarySearch(data);
}

/* ************************************************************************** */

// SPECIALIZED CONSTRUCTORS
// Various construction methods for different initialization scenarios

// Capacity constructor: Creates set with specified initial capacity
// Useful for performance optimization when expected size is known
// The vector is initialized with default values and then sorted
template <typename Data>
SetVec<Data>::SetVec(const ulong initialSize) : SortableVector<Data>(initialSize) {
  // The vector is already initialized with default values by parent constructor
  // Since it's a set, we need to ensure uniqueness, but default values should be unique
  capacity = initialSize;  // Set capacity to match the initial allocation
  Sort();                  // Ensure the vector is sorted for set operations
}

// TraversableContainer constructor: Creates set from existing container
// Copies all elements while maintaining sorted order and uniqueness
// Time complexity: O(n log n) due to sorted insertion for each element
template <typename Data>
SetVec<Data>::SetVec(const TraversableContainer<Data>& container) : SortableVector<Data>() {
  // Insert all elements from the traversable container, ensuring uniqueness
  container.Traverse([this](const Data& data) {
    if (CheckUnique(data)) {
      // Use binary search to find the correct insertion point
      long insertPoint;
      BinarySearch(data, &insertPoint);
      
      // Ensure we have capacity for one more element
      EnsureCapacity(size + 1);
      
      // Shift elements to the right to make room for insertion
      for (ulong i = size; i > static_cast<ulong>(insertPoint); i--) {
        Elements[i] = Elements[i - 1];
      }
      
      // Insert the new element at the correct sorted position
      Elements[insertPoint] = data;
      size++; // Increment size after successful insertion
    }
  });
}

// MappableContainer constructor: Creates set by moving from container
// Efficiently transfers elements using move semantics for better performance
// Time complexity: O(n log n) due to sorted insertion with move operations
template <typename Data>
SetVec<Data>::SetVec(MappableContainer<Data>&& container) : SortableVector<Data>() {
  // Insert all elements from the mappable container using move semantics
  container.Map([this](Data& data) {
    if (CheckUnique(data)) {
      // Use binary search to find the correct insertion point
      long insertPoint;
      BinarySearch(data, &insertPoint);
      
      // Ensure we have capacity for one more element
      EnsureCapacity(size + 1);
      
      // Shift elements to the right to make room for insertion
      for (ulong i = size; i > static_cast<ulong>(insertPoint); i--) {
        Elements[i] = Elements[i - 1];
      }
      
      // Insert the new element using move semantics for efficiency
      Elements[insertPoint] = std::move(data);
      size++; // Increment size after successful insertion
    }
  });
  
  // Properly clear the source container if it supports clearing
  // This ensures the moved-from container is in a clean state
  if (dynamic_cast<ClearableContainer*>(&container) != nullptr) {
    dynamic_cast<ClearableContainer*>(&container)->Clear();
  }
}

/* ************************************************************************** */

// COPY AND MOVE SEMANTICS
// Efficient object copying and transfer operations

// Copy constructor: Creates deep copy while preserving all state
// Copies both the sorted elements and the circular access position
template <typename Data>
SetVec<Data>::SetVec(const SetVec<Data>& other) : SortableVector<Data>(other), current(other.current) {
  // The parent constructor copies the actual elements (other.size elements)
  // We need to set our capacity to match the actual allocated space
  // Note: We set capacity to match size, not other.capacity, for memory efficiency
  capacity = this->size;
}

// Move constructor: Efficiently transfers ownership from another SetVec
// Transfers all resources without copying, leaving the source in a valid empty state
template <typename Data>
SetVec<Data>::SetVec(SetVec<Data>&& other) noexcept : SortableVector<Data>(std::move(other)), current(other.current), capacity(other.capacity) {
  // The parent move constructor handles the transfer of the entire vector
  // We safely copy the capacity since we're taking ownership of the entire object
  
  // Leave the moved-from object in a valid empty state
  other.current = 0;
  other.capacity = 0;
}

/* ************************************************************************** */

// ASSIGNMENT OPERATORS
// Handle assignment between SetVec instances with proper resource management

// Copy assignment: Replaces current content with deep copy of another set
// Handles self-assignment and maintains all SetVec-specific state
template <typename Data>
SetVec<Data>& SetVec<Data>::operator=(const SetVec<Data>& other) {
  if (this != &other) { // Guard against self-assignment
    // Delegate array copying to parent class assignment operator
    SortableVector<Data>::operator=(other);
    
    // Copy SetVec-specific state
    current = other.current;
    
    // Set capacity to match the actual size copied for memory efficiency
    // We don't copy other.capacity to avoid over-allocation
    capacity = this->size;
  }
  return *this; // Enable assignment chaining
}

// Move assignment: Efficiently transfers ownership from another SetVec
// Swaps resources to avoid unnecessary copying and maintain exception safety
template <typename Data>
SetVec<Data>& SetVec<Data>::operator=(SetVec<Data>&& other) noexcept {
  if (this != &other) { // Guard against self-move
    // Delegate array moving to parent class move assignment operator
    SortableVector<Data>::operator=(std::move(other));
    
    // Swap SetVec-specific state for exception safety
    std::swap(current, other.current);
    std::swap(capacity, other.capacity);
  }
  return *this; // Enable assignment chaining
}

/* ************************************************************************** */

// COMPARISON OPERATORS
// Structural equality testing for set comparison

// Equality operator: Tests if sets contain identical elements in same order
// Compares the actual sorted content regardless of circular access position
// Two sets are equal if they have the same size and same elements in same order
template <typename Data>
bool SetVec<Data>::operator==(const SetVec<Data>& other) const noexcept {
  if (size != other.size) {
    return false; // Different sizes cannot be equal
  }
  
  // Compare elements directly using normal array indexing (not circular)
  // This ensures comparison is based on actual content, not current position
  // Both sets maintain sorted order, so direct comparison is sufficient
  for (ulong i = 0; i < size; i++) {
    if (Elements[i] != other.Elements[i]) {
      return false; // Found differing elements
    }
  }
  
  return true; // All elements matched
}

// Inequality operator: Tests if sets differ in content or order
// Simply negates the equality result for efficient implementation
template <typename Data>
bool SetVec<Data>::operator!=(const SetVec<Data>& other) const noexcept {
  return !(*this == other); // Logical negation of equality
}

/* ************************************************************************** */

// CONTAINER INTERFACE IMPLEMENTATION
// Basic container operations for set management

// Clear: Removes all elements and resets the set to empty state
// Resets all SetVec-specific state including circular access position and capacity
template <typename Data>
void SetVec<Data>::Clear() {
  // Reset circular access position to beginning
  current = 0;
  
  // Clear the underlying vector data and reset size
  this->size = 0;
  this->Resize(0);  // Release allocated memory
  capacity = 0;     // Reset capacity tracking
}

/* ************************************************************************** */

// TESTABLE CONTAINER INTERFACE IMPLEMENTATION
// Efficient existence testing using binary search

// Exists: Tests if element exists in set using O(log n) binary search
// Leverages the sorted nature of the array for efficient searching
template <typename Data>
bool SetVec<Data>::Exists(const Data& data) const noexcept {
  // Use FindIndex which internally uses binary search
  // Returns true if element is found (index >= 0), false otherwise
  return FindIndex(data) >= 0;
}

/* ************************************************************************** */

// ORDERED DICTIONARY INTERFACE IMPLEMENTATION - MIN/MAX OPERATIONS
// Operations that leverage the sorted nature of the vector for O(1) extrema access

// Min: Returns the smallest element in the set
// Time complexity: O(1) - minimum is always at index 0 in sorted array
template <typename Data>
const Data& SetVec<Data>::Min() const {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  // In a sorted vector, the minimum element is always at position 0
  return Elements[0];
}

// MinNRemove: Returns and removes the smallest element atomically
// Time complexity: O(n) due to shifting elements after removal
template <typename Data>
Data SetVec<Data>::MinNRemove() {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  // Store the minimum element before removal
  Data min = Elements[0];
  
  // Shift all elements one position to the left to fill the gap
  for (ulong i = 0; i < size - 1; i++) {
    Elements[i] = Elements[i + 1];
  }
  
  // Adjust current position if necessary after removal
  if (current > 0) {
    current = (current - 1) % size; // Adjust for removal at beginning
  }
  
  size--;             // Decrement size
  ShrinkCapacity();   // Optimize memory usage if needed
  
  return min; // Return the removed minimum value
}

// RemoveMin: Removes the smallest element without returning it
// More efficient when the value is not needed
template <typename Data>
void SetVec<Data>::RemoveMin() {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  // Shift all elements one position to the left
  for (ulong i = 0; i < size - 1; i++) {
    Elements[i] = Elements[i + 1];
  }
  
  // Adjust current position if necessary after removal
  if (current > 0) {
    current = (current - 1) % size; // Adjust for removal at beginning
  }
  
  size--;             // Decrement size
  ShrinkCapacity();   // Optimize memory usage if needed
}

// Max: Returns the largest element in the set
// Time complexity: O(1) - maximum is always at last index in sorted array
template <typename Data>
const Data& SetVec<Data>::Max() const {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  // In a sorted vector, the maximum element is always at position size - 1
  return Elements[size - 1];
}

// MaxNRemove: Returns and removes the largest element atomically
// Time complexity: O(1) since removal is at the end (no shifting needed)
template <typename Data>
Data SetVec<Data>::MaxNRemove() {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  // Store the maximum element before removal
  Data max = Elements[size - 1];
  
  // Adjust current position if necessary before size change
  if (size == 1) {
    // If there's only one element, set will be empty after removal
    current = 0;
  } else if (current >= size - 1) {
    // If current points to the max element, move to the new last element
    current = size - 2;
  }
  
  size--;             // Decrement size (no shifting needed for last element)
  ShrinkCapacity();   // Optimize memory usage if needed
  
  return max; // Return the removed maximum value
}

// RemoveMax: Removes the largest element without returning it
// Most efficient min/max removal since no shifting is required
template <typename Data>
void SetVec<Data>::RemoveMax() {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  // Adjust current position if necessary before size change
  if (size == 1) {
    // If there's only one element, set will be empty after removal
    current = 0;
  } else if (current >= size - 1) {
    // If current points to the max element, move to the new last element
    current = size - 2;
  }
  
  size--;             // Decrement size (no shifting needed for last element)
  ShrinkCapacity();   // Optimize memory usage if needed
}

// ORDERED DICTIONARY INTERFACE IMPLEMENTATION - PREDECESSOR OPERATIONS
// Operations to find and manipulate elements immediately smaller than a given value

// Predecessor: Finds the largest element smaller than the given data
// Uses binary search to efficiently locate the predecessor in sorted array
// Time complexity: O(log n) for the search operation
template <typename Data>
const Data& SetVec<Data>::Predecessor(const Data& data) const {
  if (this->size == 0) {
    throw std::length_error("Access to an empty set.");
  }

  long insertPointVal;
  long exactMatchIndex = this->BinarySearch(data, &insertPointVal);
  long predecessorIndex = -1;

  if (exactMatchIndex != -1) { 
    // Data was found in the set - predecessor is the element immediately before it
    predecessorIndex = exactMatchIndex - 1; 
  } else { 
    // Data was not found in the set - find element that would come before insertion point
    predecessorIndex = insertPointVal - 1;
  }

  if (predecessorIndex < 0) { 
    // No valid predecessor exists (data would be the smallest element)
    throw std::length_error("Predecessor not found.");
  }

  return this->Elements[predecessorIndex];
}

// PredecessorNRemove: Finds, returns, and removes the predecessor atomically
// Combines predecessor finding with removal for atomic operation
// Time complexity: O(n) due to element shifting after removal
template <typename Data>
Data SetVec<Data>::PredecessorNRemove(const Data& data) {
  if (this->size == 0) {
    throw std::length_error("Access to an empty set.");
  }

  long insertPointVal;
  long exactMatchIndex = this->BinarySearch(data, &insertPointVal);
  long predecessorIndex = -1;

  // Determine predecessor index using same logic as Predecessor()
  if (exactMatchIndex != -1) { 
    predecessorIndex = exactMatchIndex - 1;
  } else { 
    predecessorIndex = insertPointVal - 1;
  }

  if (predecessorIndex < 0) {
    throw std::length_error("Predecessor not found.");
  }

  // Store the predecessor data before removing it
  Data predecessorData = this->Elements[predecessorIndex];
  
  // Remove the predecessor by shifting all subsequent elements left
  for (ulong i = static_cast<ulong>(predecessorIndex); i < this->size - 1; ++i) {
    this->Elements[i] = this->Elements[i + 1];
  }

  // Adjust current position based on removal location
  if (static_cast<ulong>(predecessorIndex) < current) {
    // Removed element was before current, so decrement current
    if (current > 0) current--;
  } else if (static_cast<ulong>(predecessorIndex) == current) {
    // Removed element was at current position
    // Current now points to the next element (which shifted into this position)
    if (current >= size - 1) {
      current = size - 2; // Adjust to new last element after size decrement
    }
  }
  
  // Decrement size and handle edge cases
  this->size--;
  
  if (size == 0) {
    current = 0; // Reset current for empty set
  }
  
  ShrinkCapacity(); // Optimize memory usage
  
  return predecessorData;
}

// RemovePredecessor: Removes the predecessor without returning its value
// More efficient when the predecessor value is not needed
template <typename Data>
void SetVec<Data>::RemovePredecessor(const Data& data) {
  if (this->size == 0) {
    throw std::length_error("Access to an empty set.");
  }

  long insertPointVal;
  long exactMatchIndex = this->BinarySearch(data, &insertPointVal);
  long predecessorIndex = -1;

  // Determine predecessor index using same logic as Predecessor()
  if (exactMatchIndex != -1) { 
    predecessorIndex = exactMatchIndex - 1;
  } else { 
    predecessorIndex = insertPointVal - 1;
  }

  if (predecessorIndex < 0) {
    throw std::length_error("Predecessor not found.");
  }
  
  // Determine position adjustments before removal
  bool removingCurrent = (static_cast<ulong>(predecessorIndex) == current);
  bool removingBeforeCurrent = (static_cast<ulong>(predecessorIndex) < current);
  
  // Remove the predecessor by shifting all subsequent elements left
  for (ulong i = static_cast<ulong>(predecessorIndex); i < this->size - 1; ++i) {
    this->Elements[i] = this->Elements[i + 1];
  }

  // Decrement size first
  this->size--;
  
  // Adjust current position based on what was removed
  if (size == 0) {
    // Set is now empty
    current = 0;
  } else if (removingCurrent) {
    // We removed the current element - keep the position
    // (now points to the next element that shifted into this position)
    if (current >= size) {
      // If it was the last position, move to the new last element
      current = size - 1;
    }
  } else if (removingBeforeCurrent && current > 0) {
    // We removed an element before current, so decrement current
    current--;
  }
  
  ShrinkCapacity(); // Optimize memory usage
}

// ORDERED DICTIONARY INTERFACE IMPLEMENTATION - SUCCESSOR OPERATIONS
// Operations to find and manipulate elements immediately larger than a given value

// Successor: Finds the smallest element larger than the given data
// Uses binary search to efficiently locate the successor in sorted array
// Time complexity: O(log n) for the search operation
template <typename Data>
const Data& SetVec<Data>::Successor(const Data& data) const {
  if (this->size == 0) {
    throw std::length_error("Access to an empty set.");
  }

  long insertPointVal; 
  long exactMatchIndex = this->BinarySearch(data, &insertPointVal);
  long successorIndex = -1;

  if (exactMatchIndex != -1) { 
    // Data was found in the set - successor is the element immediately after it
    successorIndex = exactMatchIndex + 1; 
  } else { 
    // Data was not found in the set - find element that would come after insertion point
    successorIndex = insertPointVal;
  }

  // Check if the determined successorIndex is valid (within bounds)
  if (successorIndex >= static_cast<long>(this->size) || successorIndex < 0) {
    // No valid successor exists (data would be the largest element or larger)
    throw std::length_error("Successor not found.");
  }

  return this->Elements[successorIndex];
}

// SuccessorNRemove: Finds, returns, and removes the successor atomically
// Combines successor finding with removal for atomic operation
// Time complexity: O(n) due to element shifting after removal
template <typename Data>
Data SetVec<Data>::SuccessorNRemove(const Data& data) {
  if (this->size == 0) {
    throw std::length_error("Access to an empty set.");
  }

  long insertPointVal;
  long exactMatchIndex = this->BinarySearch(data, &insertPointVal);
  long successorIndex = -1;

  // Determine successor index using same logic as Successor()
  if (exactMatchIndex != -1) { 
    // Data found - successor is next element
    successorIndex = exactMatchIndex + 1;
  } else { 
    // Data not found - successor is at insertion point
    successorIndex = insertPointVal;
  }

  if (successorIndex >= static_cast<long>(this->size) || successorIndex < 0) {
    throw std::length_error("Successor not found.");
  }

  // Store the successor data before removing it
  Data successorData = this->Elements[successorIndex];

  // Remove the successor by shifting all subsequent elements left
  for (ulong i = static_cast<ulong>(successorIndex); i < this->size - 1; ++i) {
    this->Elements[i] = this->Elements[i + 1];
  }
  
  // Adjust current position based on removal location
  if (static_cast<ulong>(successorIndex) < this->current && this->current > 0) {
      // Removed element was before current, so decrement current
      this->current--;
  } else if (static_cast<ulong>(successorIndex) == this->current && this->current == this->size -1 && this->size > 1) {
      // Removed element was at current position and was the last element
      this->current = 0; // Loop around to first element
  }

  // Decrement size and handle edge cases
  this->size--;
  ShrinkCapacity(); // Optimize memory usage
  
  // Ensure current position is valid after size change
  if (this->size > 0 && this->current >= this->size) {
    this->current = this->size -1;
  } else if (this->size == 0) {
    this->current = 0;
  }

  return successorData;
}

// RemoveSuccessor: Removes the successor without returning its value
// More efficient when the successor value is not needed
template <typename Data>
void SetVec<Data>::RemoveSuccessor(const Data& data) {
  if (this->size == 0) {
    throw std::length_error("Access to an empty set.");
  }

  long insertPointVal;
  long exactMatchIndex = this->BinarySearch(data, &insertPointVal);
  long successorIndex = -1;

  // Determine successor index using same logic as Successor()
  if (exactMatchIndex != -1) { 
    // Data found - successor is next element
    successorIndex = exactMatchIndex + 1;
  } else { 
    // Data not found - successor is at insertion point
    successorIndex = insertPointVal;
  }

  if (successorIndex >= static_cast<long>(this->size) || successorIndex < 0) {
    throw std::length_error("Successor not found.");
  }

  // Remove the successor by shifting all subsequent elements left
  for (ulong i = static_cast<ulong>(successorIndex); i < this->size - 1; ++i) {
    this->Elements[i] = this->Elements[i + 1];
  }

  // Adjust current position based on removal location
  if (static_cast<ulong>(successorIndex) < this->current && this->current > 0) {
      // Removed element was before current, so decrement current
      this->current--;
  } else if (static_cast<ulong>(successorIndex) == this->current && this->current == this->size -1 && this->size > 1) {
      // Removed element was at current position and was the last element
      this->current = 0; // Loop around to first element
  }
  
  // Decrement size and handle edge cases
  this->size--;
  ShrinkCapacity(); // Optimize memory usage

  // Ensure current position is valid after size change
  if (this->size > 0 && this->current >= this->size) {
    this->current = this->size -1;
  } else if (this->size == 0) {
    this->current = 0;
  }
}

/* ************************************************************************** */

// DICTIONARY CONTAINER INTERFACE IMPLEMENTATION
// Core set operations maintaining sorted order and uniqueness constraints

// Insert (copy version): Adds a new element to the set maintaining sorted order
// Returns true if element was inserted, false if already exists
// Time complexity: O(n) due to shifting elements for insertion
template <typename Data>
bool SetVec<Data>::Insert(const Data& data) {
  // Check if the element already exists using binary search
  if (!CheckUnique(data)) {
    return false; // Element already exists, no insertion needed
  }
  
  // Use binary search to find the correct insertion point to maintain order
  long insertPoint;
  BinarySearch(data, &insertPoint);
  
  // Ensure we have capacity for one more element
  EnsureCapacity(size + 1);
  
  // Shift all elements from insertion point to the right to make room
  for (ulong i = size; i > static_cast<ulong>(insertPoint); i--) {
    Elements[i] = Elements[i - 1];
  }
  
  // Insert the new element at the correct position and increment size
  Elements[insertPoint] = data;
  size++;

  // Adjust current position if insertion happened at or before current position
  if (current >= static_cast<ulong>(insertPoint)) {
    current++; // Current element shifted right, so increment current index
  }
  
  return true; // Successful insertion
}

// Insert (move version): Adds a new element to the set using move semantics
// More efficient for expensive-to-copy types as it moves rather than copies
// Time complexity: O(n) due to shifting elements for insertion
template <typename Data>
bool SetVec<Data>::Insert(Data&& data) {
  // Check if the element already exists using binary search
  if (!CheckUnique(data)) {
    return false; // Element already exists, no insertion needed
  }
  
  // Use binary search to find the correct insertion point to maintain order
  long insertPoint;
  BinarySearch(data, &insertPoint);
  
  // Ensure we have capacity for one more element
  EnsureCapacity(size + 1);
  
  // Shift all elements from insertion point to the right to make room
  for (ulong i = size; i > static_cast<ulong>(insertPoint); i--) {
    Elements[i] = Elements[i - 1];
  }
  
  // Insert the new element using move semantics and increment size
  Elements[insertPoint] = std::move(data);
  size++;

  // Adjust current position if insertion happened at or before current position
  if (current >= static_cast<ulong>(insertPoint)) {
    current++; // Current element shifted right, so increment current index
  }
  
  return true; // Successful insertion
}

// Remove: Removes an element from the set if it exists
// Uses binary search to locate element, then shifts remaining elements left
// Time complexity: O(n) due to element shifting after removal
template <typename Data>
bool SetVec<Data>::Remove(const Data& data) {
  // Use binary search to find the element
  long index = BinarySearch(data);
  if (index < 0) {
    return false; // Element not found, nothing to remove
  }
  
  // Remove the element by shifting all subsequent elements left
  for (ulong i = index; i < size - 1; i++) {
    Elements[i] = Elements[i + 1];
  }
  
  // Adjust current position based on removal location
  if (index >= 0 && current > static_cast<ulong>(index)) {
    current--; // Element removed before current, so decrement current index
  }
  // Ensure current remains within bounds using modulo for circular behavior
  current = size > 1 ? current % (size - 1) : 0;
  
  this->size--; // Decrement size
  ShrinkCapacity(); // Optimize memory usage if possible
  
  return true; // Successful removal
}

// CONTAINER EXTENSION OPERATIONS
// Bulk operations for inserting/removing multiple elements

// InsertAll (const version): Attempts to insert all elements from a container
// Returns true only if ALL elements were successfully inserted (none existed)
// Uses traversal to iterate through source container elements
template <typename Data>
bool SetVec<Data>::InsertAll(const TraversableContainer<Data>& container) {
  bool allInserted = true;
  
  // Try to insert all elements using lambda function
  container.Traverse([this, &allInserted](const Data& data) {
    if (!Insert(data)) {
      allInserted = false; // At least one element already existed
    }
  });
  
  return allInserted; // True only if all elements were new and inserted
}

// InsertAll (move version): Attempts to insert all elements using move semantics
// More efficient for expensive-to-copy types as it moves elements from source
// Empties the source container by moving its elements into this set
template <typename Data>
bool SetVec<Data>::InsertAll(MappableContainer<Data>&& container) {
  bool allInserted = true;
  
  // Try to insert all elements using move semantics
  container.Map([this, &allInserted](Data& data) {
    if (!Insert(std::move(data))) {
      allInserted = false; // At least one element already existed
    }
  });
  
  return allInserted; // True only if all elements were new and inserted
}

// RemoveAll: Attempts to remove all elements present in the given container
// Returns true only if ALL specified elements were found and removed
// Elements not found in this set will cause the method to return false
template <typename Data>
bool SetVec<Data>::RemoveAll(const TraversableContainer<Data>& container) {
  bool allRemoved = true;
  
  // Try to remove all specified elements
  container.Traverse([this, &allRemoved](const Data& data) {
    if (!Remove(data)) {
      allRemoved = false; // At least one element was not found
    }
  });
  
  return allRemoved; // True only if all elements were found and removed
}

// InsertSome (const version): Attempts to insert elements, succeeds if any insertion occurs
// Returns true if at least one element was successfully inserted
// Tolerates duplicate elements - doesn't require all insertions to succeed
template <typename Data>
bool SetVec<Data>::InsertSome(const TraversableContainer<Data>& container) {
  bool someInserted = false;
  
  // Try to insert elements, track if any succeed
  container.Traverse([this, &someInserted](const Data& data) {
    if (Insert(data)) {
      someInserted = true; // At least one element was successfully inserted
    }
  });
  
  return someInserted; // True if any element was inserted
}

// InsertSome (move version): Attempts to insert elements using move semantics
// More efficient version that moves elements from source container
// Returns true if at least one element was successfully inserted
template <typename Data>
bool SetVec<Data>::InsertSome(MappableContainer<Data>&& container) {
  bool someInserted = false;
  
  // Try to insert elements using move semantics
  container.Map([this, &someInserted](Data& data) {
    if (Insert(std::move(data))) {
      someInserted = true; // At least one element was successfully inserted
    }
  });
  
  return someInserted; // True if any element was inserted
}

// RemoveSome: Attempts to remove elements, succeeds if any removal occurs
// Returns true if at least one element was successfully removed
// Tolerates missing elements - doesn't require all removals to succeed
template <typename Data>
bool SetVec<Data>::RemoveSome(const TraversableContainer<Data>& container) {
  bool someRemoved = false;
  
  // Try to remove elements, track if any succeed
  container.Traverse([this, &someRemoved](const Data& data) {
    if (Remove(data)) {
      someRemoved = true; // At least one element was successfully removed
    }
  });
  
  return someRemoved; // True if any element was removed
}

/* ************************************************************************** */

// LINEAR CONTAINER INTERFACE IMPLEMENTATION
// Provides array-like access to elements with both direct and circular indexing

// operator[] (const version): Direct access to elements by index
// Provides non-circular access for compatibility with standard LinearContainer expectations
// Time complexity: O(1) - direct array access
template <typename Data>
const Data& SetVec<Data>::operator[](const ulong index) const {
  if (index >= size) {
    throw std::out_of_range("Access at index " + std::to_string(index) + "; SetVec size " + std::to_string(size) + ".");
  }
  
  // Uses direct access (non-circular) to maintain compatibility with LinearContainer interface
  // This is necessary for correct comparisons with other LinearContainers that use normal indexing
  return Elements[index];
}

// operator[] (mutable version): Direct access to elements by index with modification capability
// Allows modification of elements but does not enforce set ordering constraints
// WARNING: Modifying elements can break the sorted invariant - use with caution
template <typename Data>
Data& SetVec<Data>::operator[](const ulong index) {
  if (index >= size) {
    throw std::out_of_range("Access at index " + std::to_string(index) + "; SetVec size " + std::to_string(size) + ".");
  }
  
  // Uses direct access (non-circular) for consistency with const version
  return Elements[index];
}

// Front (const version): Returns the element at the current circular position
// Provides access to the "front" element in the current circular view
// Time complexity: O(1) - direct access using current index
template <typename Data>
const Data& SetVec<Data>::Front() const {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  // Return the element at the current position in circular ordering
  return Elements[current];
}

// Front (mutable version): Returns mutable reference to element at current position
// Allows modification of the front element but may break set ordering
// WARNING: Modifying elements can violate sorted invariant
template <typename Data>
Data& SetVec<Data>::Front() {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  // Return mutable reference to element at current position
  return Elements[current];
}

// Back (const version): Returns the last element in circular ordering
// Provides access to the element that would be "last" relative to current position
// Time complexity: O(1) - calculated using modular arithmetic
template <typename Data>
const Data& SetVec<Data>::Back() const {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  // Return the element that comes before current in circular order
  // This is the "last" element when current is considered the "first"
  return Elements[(current + size - 1) % size];
}

// Back (mutable version): Returns mutable reference to last element in circular ordering
// Allows modification of the back element but may break set ordering
// WARNING: Modifying elements can violate sorted invariant
template <typename Data>
Data& SetVec<Data>::Back() {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  // Return mutable reference to element that comes before current in circular order
  return Elements[(current + size - 1) % size];
}

/* ************************************************************************** */

// CIRCULAR ACCESS INTERFACE IMPLEMENTATION
// Provides circular navigation through the set elements with a movable current position

// SetCurrent: Sets the current position for circular access
// Safely handles indices larger than size using modulo arithmetic
// Time complexity: O(1) - simple modulo calculation
template <typename Data>
void SetVec<Data>::SetCurrent(ulong index) noexcept {
  if (size > 0) {
    // Use modulo to ensure index is within valid range [0, size-1]
    current = index % size;
  } else {
    // For empty sets, current is always 0
    current = 0;
  }
}

// GetCurrent: Returns the current position index in the circular access
// Useful for saving and restoring circular access state
// Time complexity: O(1) - simple member variable access
template <typename Data>
ulong SetVec<Data>::GetCurrent() const noexcept {
  return current; // Return current circular position index
}

// Next: Advances current position to the next element in circular order
// Wraps around to first element when reaching the end
// Time complexity: O(1) - simple modulo arithmetic
template <typename Data>
void SetVec<Data>::Next() noexcept {
  if (size > 0) {
    // Move to next position, wrapping around using modulo
    current = (current + 1) % size;
  }
  // For empty sets, current remains 0
}

// Prev: Moves current position to the previous element in circular order
// Wraps around to last element when at the beginning
// Time complexity: O(1) - conditional arithmetic with wrap-around
template <typename Data>
void SetVec<Data>::Prev() noexcept {
  if (size > 0) {
    // Move to previous position with proper wrap-around handling
    current = (current == 0) ? size - 1 : current - 1;
  }
  // For empty sets, current remains 0
}

/* ************************************************************************** */

// DEBUG AND UTILITY FUNCTIONS
// Helper functions for debugging and specialized access patterns

// PrintDebug: Outputs all elements in the set for debugging purposes
// Displays elements in their stored (sorted) order regardless of current position
template <typename Data>
void SetVec<Data>::PrintDebug() const {
  std::cout << "DEBUG: SetVec content: ";
  for (ulong i = 0; i < size; ++i) {
    std::cout << Elements[i] << " ";
  }
  std::cout << std::endl;
}

// GetAtCurrent (const version): Access elements relative to current position
// Provides circular indexing where index 0 is current position, 1 is next, etc.
// Useful for algorithms that need to process elements in circular order
// Time complexity: O(1) - direct access with modulo calculation
template <typename Data>
const Data& SetVec<Data>::GetAtCurrent(ulong index) const {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  if (index >= size) {
    throw std::out_of_range("Access at index " + std::to_string(index) + "; SetVec size " + std::to_string(size) + ".");
  }
  
  // Use circular access with current index as the starting point
  // index 0 = current position, index 1 = next position, etc.
  return Elements[(current + index) % size];
}

// GetAtCurrent (mutable version): Mutable access to elements relative to current position
// Allows modification of elements accessed in circular order from current position
// WARNING: Modifying elements can break sorted order invariant
template <typename Data>
Data& SetVec<Data>::GetAtCurrent(ulong index) {
  if (size == 0) {
    throw std::length_error("Access to an empty set.");
  }
  
  if (index >= size) {
    throw std::out_of_range("Access at index " + std::to_string(index) + "; SetVec size " + std::to_string(size) + ".");
  }
  
  // Use circular access with current index as the starting point
  // Provides mutable reference for element modification
  return Elements[(current + index) % size];
}

// CAPACITY MANAGEMENT IMPLEMENTATION
// Dynamic memory management for optimal space utilization

// EnsureCapacity: Guarantees minimum capacity for efficient insertions
// Doubles capacity when more space is needed to minimize reallocations
// Uses amortized O(1) insertion by growing exponentially
template <typename Data>
void SetVec<Data>::EnsureCapacity(ulong minCapacity) {
  if (minCapacity > capacity) {
    // Calculate new capacity: double until we meet minimum requirement
    ulong newCapacity = (capacity == 0) ? 1 : capacity;
    while (newCapacity < minCapacity) {
      newCapacity *= 2; // Exponential growth for amortized efficiency
    }
    
    // Save current logical size before resizing
    ulong currentSize = this->size;
    
    // Resize to new capacity (this temporarily changes both physical and logical size)
    this->Resize(newCapacity);
    
    // Restore the logical size to maintain set state
    this->size = currentSize;
    
    // Update our capacity tracking
    capacity = newCapacity;
  }
}

// ShrinkCapacity: Reduces capacity when space utilization is low
// Shrinks to half capacity when using less than 1/4 of allocated space
// Prevents excessive memory usage while maintaining reasonable buffer
// Minimum capacity of 4 prevents frequent reallocations for small sets
template <typename Data>
void SetVec<Data>::ShrinkCapacity() {
  // Shrink capacity if using less than 1/4 of allocated space and capacity > 4
  if (capacity > 4 && this->size <= capacity / 4) {
    ulong newCapacity = capacity / 2; // Reduce by half
    
    // Save the current logical size before resizing
    ulong currentLogicalSize = this->size;
    
    // Resize to new capacity (this temporarily changes both physical and logical size)
    this->Resize(newCapacity);
    
    // Restore the logical size to maintain set state
    this->size = currentLogicalSize;
    
    // Update our capacity tracking
    capacity = newCapacity;
  }
}

/* ************************************************************************** */

}