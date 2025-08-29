
/*
 * SetLst Implementation File
 * 
 * This file contains the complete implementation of the SetLst class, which
 * provides a mathematical set using a sorted linked list as the underlying
 * data structure. The implementation ensures all elements are maintained
 * in ascending order to enable efficient ordered operations.
 * 
 * Implementation highlights:
 * - Sorted insertion to maintain order invariant
 * - Efficient min/max operations due to sorted structure
 * - Linear search with early termination optimization
 * - Set semantics with duplicate prevention
 */

#include <stdexcept>
#include <string>
#include <random>

namespace lasd {

/* ************************************************************************** */

// SETLST CONSTRUCTORS AND INITIALIZATION
// These constructors create sets from various data sources while maintaining sorted order

// Constructor from TraversableContainer: Creates set by copying elements in sorted order
template <typename Data>
SetLst<Data>::SetLst(const TraversableContainer<Data>& container) {
  // Use container's traverse function to visit each element
  // Insert function ensures elements are placed in correct sorted position
  container.Traverse([this](const Data& item) {
    Insert(item); // Insert handles sorting and duplicate prevention
  });
}

// Constructor from MappableContainer: Creates set by moving elements for efficiency
template <typename Data>
SetLst<Data>::SetLst(MappableContainer<Data>&& container) {
  // Use container's map function to access elements for moving
  container.Map([this](Data& item) {
    Insert(std::move(item)); // Move elements to avoid unnecessary copying
  });
  
  // Properly clear the source container if it supports clearing
  // This ensures the moved-from container is in a clean state
  if (dynamic_cast<ClearableContainer*>(&container) != nullptr) {
    dynamic_cast<ClearableContainer*>(&container)->Clear();
  }
}

/* ************************************************************************** */

// COPY AND MOVE CONSTRUCTORS
// Handle creating new SetLst instances from existing ones

// Copy constructor: Creates a deep copy while preserving sorted order
template <typename Data>
SetLst<Data>::SetLst(const SetLst& other) : List<Data>() {
  // Since the source is already sorted, we can traverse and insert sequentially
  // This maintains the sorted order in the new set
  auto current = other.head;
  while (current != nullptr) {
    Insert(current->element); // Insert maintains order and prevents duplicates
    current = current->next;
  }
}

// Move constructor: Efficiently transfers ownership from another SetLst
template <typename Data>
SetLst<Data>::SetLst(SetLst&& other) noexcept : List<Data>(std::move(other)) {
  // List's move constructor handles the transfer of nodes, size, head, and tail
  // The moved-from object becomes empty but valid
}

/* ************************************************************************** */

// ASSIGNMENT OPERATORS
// Handle assignment of SetLst contents from other SetLst instances

// Copy assignment: Replaces current content with deep copy of another set
template <typename Data>
SetLst<Data>& SetLst<Data>::operator=(const SetLst& other) {
  // Protect against self-assignment which would be destructive
  if (this != &other) {
    // Clear current contents to start fresh
    Clear();
    
    // Copy all elements from source set in order
    auto current = other.head;
    while (current != nullptr) {
      Insert(current->element); // Maintains sorted order
      current = current->next;
    }
  }
  return *this; // Return reference for chaining
}

// Move assignment: Efficiently transfers ownership from another SetLst
template <typename Data>
SetLst<Data>& SetLst<Data>::operator=(SetLst&& other) noexcept {
  // Delegate to List's move assignment which handles resource transfer
  List<Data>::operator=(std::move(other));
  return *this; // Return reference for chaining
}

/* ************************************************************************** */

// COMPARISON OPERATORS
// Test structural equality between sets

// Equality operator: Returns true if sets contain same elements in same order
template <typename Data>
bool SetLst<Data>::operator==(const SetLst& other) const noexcept {
  if (size != other.size) {
    return false; // Different sizes cannot be equal
  }

  // Since both lists are maintained in sorted order, we can compare sequentially
  auto thisCurrent = head;
  auto otherCurrent = other.head;
  
  while (thisCurrent != nullptr && otherCurrent != nullptr) {
    if (thisCurrent->element != otherCurrent->element) {
      return false; // Found differing elements
    }
    thisCurrent = thisCurrent->next;
    otherCurrent = otherCurrent->next;
  }
  
  return true; // All elements matched
}

// Inequality operator: Returns true if sets differ in content
template <typename Data>
bool SetLst<Data>::operator!=(const SetLst& other) const noexcept {
  return !(*this == other); // Simply negate equality result
}

/* ************************************************************************** */

// CONTAINER OPERATIONS
// Basic container functionality for clearing and testing element existence

// Clear function: Removes all elements from the set
template <typename Data>
void SetLst<Data>::Clear() {
  // Delegate to List's Clear implementation which handles proper memory deallocation
  List<Data>::Clear();
}

/* ************************************************************************** */

// TESTABLE CONTAINER IMPLEMENTATION
// Provides efficient existence testing with early termination

// Exists function: Tests if an element exists in the set
// Optimized for sorted list with early termination when element cannot exist
template <typename Data>
bool SetLst<Data>::Exists(const Data& data) const noexcept {
  // Traverse the sorted list with early termination optimization
  auto current = head;
  
  while (current != nullptr) {
    if (current->element == data) {
      return true; // Found the element
    } else if (current->element > data) {
      // Since list is sorted in ascending order, if current element > data,
      // then data cannot exist anywhere else in the list
      return false;
    }
    
    current = current->next;
  }
  
  return false; // Element not found after full traversal
}

/* ************************************************************************** */

// DICTIONARY CONTAINER IMPLEMENTATION - INSERT OPERATIONS
// Maintains set semantics by preventing duplicates and preserving sorted order

// Insert function - Copy version: Inserts element in correct sorted position
template <typename Data>
bool SetLst<Data>::Insert(const Data& data) {
  // First check if element already exists (set semantics - no duplicates)
  if (Exists(data)) {
    return false; // Element already exists, insertion failed
  }

  // Handle insertion at the beginning (empty list or smallest element)
  if (head == nullptr || data < head->element) {
    // Create new node and insert at head
    auto newNode = new typename List<Data>::Node(data);
    newNode->next = head;
    head = newNode;
    
    if (tail == nullptr) {
      // This is the first element, so it's also the tail
      tail = newNode;
    }
  } else {
    // Find the correct insertion point to maintain sorted order
    auto current = head;
    
    // Traverse until we find where to insert (current->next > data or at end)
    while (current->next != nullptr && current->next->element <= data) {
      current = current->next;
    }
    
    // Create new node and insert after current
    auto newNode = new typename List<Data>::Node(data);
    newNode->next = current->next;
    current->next = newNode;
    
    if (current == tail) {
      // We're inserting at the end, update tail pointer
      tail = newNode;
    }
  }
  
  size++; // Increment size counter
  return true; // Insertion successful
}

// Insert function - Move version: Inserts element with move semantics for efficiency
template <typename Data>
bool SetLst<Data>::Insert(Data&& data) {
  // First check if element already exists (set semantics - no duplicates)
  if (Exists(data)) {
    return false; // Element already exists, insertion failed
  }
  
  // Handle insertion at the beginning (empty list or smallest element)
  if (head == nullptr || data < head->element) {
    // Create new node with moved data and insert at head
    auto newNode = new typename List<Data>::Node(std::move(data));
    newNode->next = head;
    head = newNode;
    
    if (tail == nullptr) {
      // This is the first element, so it's also the tail
      tail = newNode;
    }
  } else {
    // Find the correct insertion point to maintain sorted order
    auto current = head;
    
    // Traverse until we find where to insert (current->next > data or at end)
    while (current->next != nullptr && current->next->element <= data) {
      current = current->next;
    }
    
    // Create new node with moved data and insert after current
    auto newNode = new typename List<Data>::Node(std::move(data));
    newNode->next = current->next;
    current->next = newNode;
    
    if (current == tail) {
      // We're inserting at the end, update tail pointer
      tail = newNode;
    }
  }
  
  size++; // Increment size counter
  return true; // Insertion successful
}

// Remove function: Removes specified element if it exists in the set
template <typename Data>
bool SetLst<Data>::Remove(const Data& data) {
  if (size == 0) {
    return false; // Empty set, nothing to remove
  }

  // Search for the node to remove, keeping track of previous node for linking
  typename List<Data>::Node* prev = nullptr;
  auto current = head;
  
  // Traverse with early termination optimization for sorted list
  while (current != nullptr && current->element != data) {
    // If current element > data, then data doesn't exist (sorted property)
    if (current->element > data) {
      return false; // Element not in set
    }
    
    prev = current;
    current = current->next;
  }
  
  if (current == nullptr) {
    // Reached end of list without finding element
    return false;
  }
  
  // Remove the found node and update pointers
  if (prev == nullptr) {
    // Removing the head node
    head = current->next;
    
    if (head == nullptr) {
      // Was the only element
      tail = nullptr;
    }
  } else {
    // It's in the middle or at the end
    prev->next = current->next;
    
    if (current == tail) {
      // Update tail if removing last element
      tail = prev;
    }
  }
  
  delete current;
  size--;
  return true;
}

/* ************************************************************************** */

// RANDOM NUMBER GENERATION
// Helper function for probabilistic container operations
// Uses thread-safe static variables to maintain state across calls

// Random helper for InsertSome/RemoveSome operations
// Returns true or false with 50% probability for each call
static bool Random() {
  // Static variables ensure single initialization and persistent state
  static std::default_random_engine gen(std::random_device{}());
  static std::uniform_int_distribution<int> dist(0, 1);
  return dist(gen); // Returns 0 or 1 with equal probability
}

/* ************************************************************************** */

// CONTAINER EXTENSION OPERATIONS
// These operations work with other containers to perform bulk insertions and removals
// They follow set semantics, preventing duplicates and maintaining sorted order

// InsertAll (TraversableContainer): Attempts to insert all elements from a container
// Returns true if at least one element was successfully inserted
template <typename Data>
bool SetLst<Data>::InsertAll(const TraversableContainer<Data>& container) {
  bool inserted = false; // Track if any insertion occurred
  
  // Traverse all elements in the source container
  container.Traverse([this, &inserted](const Data& item) {
    // Try to insert each element (Insert handles duplicates)
    if (Insert(item)) {
      inserted = true; // Mark that at least one insertion succeeded
    }
  });
  
  return inserted; // Return whether any elements were actually inserted
}

// InsertAll (MappableContainer): Move version for efficiency with movable containers
// Moves elements instead of copying them to improve performance
template <typename Data>
bool SetLst<Data>::InsertAll(MappableContainer<Data>&& container) {
  bool inserted = false; // Track if any insertion occurred
  
  // Map over all elements to access them for moving
  container.Map([this, &inserted](Data& item) {
    // Try to move each element (Insert handles duplicates)
    if (Insert(std::move(item))) {
      inserted = true; // Mark that at least one insertion succeeded
    }
  });
  
  return inserted; // Return whether any elements were actually inserted
}

// RemoveAll: Attempts to remove all specified elements from the set
// Returns true if at least one element was successfully removed
template <typename Data>
bool SetLst<Data>::RemoveAll(const TraversableContainer<Data>& container) {
  bool removed = false; // Track if any removal occurred
  
  // Traverse all elements in the container to remove
  container.Traverse([this, &removed](const Data& item) {
    // Try to remove each element
    if (Remove(item)) {
      removed = true; // Mark that at least one removal succeeded
    }
  });
  
  return removed; // Return whether any elements were actually removed
}

// InsertSome (TraversableContainer): Probabilistic insertion using random selection
// Each element has a 50% chance of being inserted, providing randomized subset insertion
template <typename Data>
bool SetLst<Data>::InsertSome(const TraversableContainer<Data>& container) {
  bool inserted = false; // Track if any insertion occurred
  
  // Traverse elements and randomly decide whether to insert each one
  container.Traverse([this, &inserted](const Data& item) {
    // Use random function to decide whether to insert (50% probability)
    if (Random() && Insert(item)) {
      inserted = true; // Mark that at least one insertion succeeded
    }
  });
  
  return inserted; // Return whether any elements were actually inserted
}

// InsertSome (MappableContainer): Move version with probabilistic insertion
// Combines random selection with move semantics for efficiency
template <typename Data>
bool SetLst<Data>::InsertSome(MappableContainer<Data>&& container) {
  bool inserted = false; // Track if any insertion occurred
  
  // Map over elements to access them for moving, with random selection
  container.Map([this, &inserted](Data& item) {
    // Use random function to decide whether to insert (50% probability)
    if (Random() && Insert(std::move(item))) {
      inserted = true; // Mark that at least one insertion succeeded
    }
  });
  
  return inserted; // Return whether any elements were actually inserted
}

// RemoveSome: Probabilistic removal using random selection
// Each element has a 50% chance of being removed, providing randomized subset removal
template <typename Data>
bool SetLst<Data>::RemoveSome(const TraversableContainer<Data>& container) {
  bool removed = false; // Track if any removal occurred
  
  // Traverse elements and randomly decide whether to remove each one
  container.Traverse([this, &removed](const Data& item) {
    // Use random function to decide whether to remove (50% probability)
    if (Random() && Remove(item)) {
      removed = true; // Mark that at least one removal succeeded
    }
  });
  
  return removed; // Return whether any elements were actually removed
}

/* ************************************************************************** */

/* ************************************************************************** */

// ORDERED DICTIONARY IMPLEMENTATION - MIN/MAX OPERATIONS
// These operations leverage the sorted nature of the list for efficient extrema access
// Time complexity: O(1) for min operations, O(n) for max operations due to single-linked list

// Min: Returns the smallest element in the set
// Since elements are sorted in ascending order, minimum is always at the head
template <typename Data>
const Data& SetLst<Data>::Min() const {
  if (size == 0) {
    throw std::length_error("Empty set"); // Cannot find min in empty set
  }

  // In a sorted ascending list, the minimum is always at the head
  return head->element;
}

// MinNRemove: Returns and removes the smallest element atomically
// Efficient O(1) operation since minimum is at head of sorted list
template <typename Data>
Data SetLst<Data>::MinNRemove() {
  if (size == 0) {
    throw std::length_error("Empty set"); // Cannot remove from empty set
  }

  // Store the minimum element value before removal
  Data min = head->element;
  
  // Remove the head node (which contains the minimum)
  auto temp = head;
  head = head->next; // Update head to next node
  delete temp;       // Free the old head node
  
  size--; // Decrement size counter
  
  // Handle the case where we removed the last element
  if (size == 0) {
    tail = nullptr; // Set tail to null for empty list
  }
  
  return min; // Return the removed minimum value
}

// RemoveMin: Removes the smallest element without returning it
// Efficient O(1) operation for head removal in sorted list
template <typename Data>
void SetLst<Data>::RemoveMin() {
  if (size == 0) {
    throw std::length_error("Empty set"); // Cannot remove from empty set
  }
  
  // Remove the head node which contains the minimum
  auto temp = head;
  head = head->next; // Update head to next node
  delete temp;       // Free the old head node
  
  size--; // Decrement size counter
  
  // Handle the case where we removed the last element
  if (size == 0) {
    tail = nullptr; // Set tail to null for empty list
  }
}

// Max: Returns the largest element in the set
// Since elements are sorted in ascending order, maximum is always at the tail
template <typename Data>
const Data& SetLst<Data>::Max() const {
  if (size == 0) {
    throw std::length_error("Empty set"); // Cannot find max in empty set
  }

  // In a sorted ascending list, the maximum is always at the tail
  return tail->element;
}

// MaxNRemove: Returns and removes the largest element atomically
// O(n) operation since we need to find the node before tail to update links
template <typename Data>
Data SetLst<Data>::MaxNRemove() {
  if (size == 0) {
    throw std::length_error("Empty set"); // Cannot remove from empty set
  }

  // Store the maximum element value before removal
  Data max = tail->element;
  
  if (head == tail) {
    // Special case: only one element in the set
    delete head;
    head = nullptr;
    tail = nullptr;
  } else {
    // Find the node before the tail (O(n) operation in singly-linked list)
    auto current = head;
    while (current->next != tail) {
      current = current->next;
    }
    
    // Remove the tail node
    delete tail;
    tail = current;        // Update tail to previous node
    tail->next = nullptr;  // Ensure new tail points to null
  }
  
  size--; // Decrement size counter
  return max; // Return the removed maximum value
}

// RemoveMax: Removes the largest element without returning it
// O(n) operation due to the need to traverse to find the node before tail
template <typename Data>
void SetLst<Data>::RemoveMax() {
  if (size == 0) {
    throw std::length_error("Empty set"); // Cannot remove from empty set
  }
  
  if (head == tail) {
    // Special case: only one element in the set
    delete head;
    head = nullptr;
    tail = nullptr;
  } else {
    // Find the node before the tail (O(n) operation in singly-linked list)
    auto current = head;
    while (current->next != tail) {
      current = current->next;
    }
    
    // Remove the tail node
    delete tail;
    tail = current;        // Update tail to previous node
    tail->next = nullptr;  // Ensure new tail points to null
  }
  
  size--; // Decrement size counter
}

// ORDERED DICTIONARY IMPLEMENTATION - PREDECESSOR OPERATIONS
// These operations find elements that are immediately smaller than a given value
// They implement the mathematical concept of predecessor in an ordered set

// Predecessor: Finds the largest element smaller than the given data
// Returns a reference to the predecessor element
// Time complexity: O(n) - must traverse list to find largest element < data
template <typename Data>
const Data& SetLst<Data>::Predecessor(const Data& data) const {
  if (size == 0) {
    throw std::length_error("Empty set"); // Cannot find predecessor in empty set
  }

  // Search for the largest element that is smaller than data
  auto current = head;
  typename List<Data>::Node* pred = nullptr; // Will hold the best predecessor found
  
  // Traverse the entire list to find the largest element < data
  while (current != nullptr) {
    if (current->element < data) {
      // This element is smaller than data, check if it's the best predecessor so far
      if (pred == nullptr || current->element > pred->element) {
        pred = current; // Update best predecessor
      }
    }
    current = current->next;
  }
  
  if (pred == nullptr) {
    // No element found that is smaller than data
    throw std::length_error("No predecessor found");
  }
  
  return pred->element; // Return the predecessor element
}

// PredecessorNRemove: Finds, returns, and removes the predecessor atomically
// This is a compound operation that ensures consistency
template <typename Data>
Data SetLst<Data>::PredecessorNRemove(const Data& data) {
  if (size == 0) {
    throw std::length_error("Empty set"); // Cannot find predecessor in empty set
  }

  // Find the predecessor and track the nodes needed for removal
  auto current = head;
  typename List<Data>::Node* pred = nullptr;     // The predecessor node
  typename List<Data>::Node* prevPred = nullptr; // Node before predecessor
  typename List<Data>::Node* prev = nullptr;     // Previous node in traversal
  
  // Traverse to find the largest element < data and its previous node
  while (current != nullptr) {
    if (current->element < data) {
      // This element is smaller than data, check if it's the best predecessor
      if (pred == nullptr || current->element > pred->element) {
        pred = current;     // Update best predecessor
        prevPred = prev;    // Update predecessor's previous node
      }
    }
    prev = current;         // Update previous node for next iteration
    current = current->next;
  }
  
  if (pred == nullptr) {
    // No predecessor found
    throw std::length_error("No predecessor found");
  }
  
  // Store the predecessor value before removal
  Data result = pred->element;
  
  // Remove the predecessor node from the list
  if (prevPred == nullptr) {
    // Predecessor is at the head of the list
    head = pred->next;
    
    if (head == nullptr) {
      // List becomes empty after removal
      tail = nullptr;
    }
  } else {
    // Predecessor is in the middle or at the end
    prevPred->next = pred->next;
    
    if (pred == tail) {
      // Predecessor was the tail, update tail pointer
      tail = prevPred;
    }
  }
  
  delete pred; // Free the predecessor node
  size--;      // Decrement size counter
  
  return result; // Return the removed predecessor value
}

// RemovePredecessor: Removes the predecessor without returning its value
// More efficient when the value is not needed
template <typename Data>
void SetLst<Data>::RemovePredecessor(const Data& data) {
  if (size == 0) {
    throw std::length_error("Empty set"); // Cannot find predecessor in empty set
  }

  // Find the predecessor and track the nodes needed for removal
  auto current = head;
  typename List<Data>::Node* pred = nullptr;     // The predecessor node
  typename List<Data>::Node* prevPred = nullptr; // Node before predecessor
  typename List<Data>::Node* prev = nullptr;     // Previous node in traversal
  
  // Traverse to find the largest element < data and its previous node
  while (current != nullptr) {
    if (current->element < data) {
      // This element is smaller than data, check if it's the best predecessor
      if (pred == nullptr || current->element > pred->element) {
        pred = current;     // Update best predecessor
        prevPred = prev;    // Update predecessor's previous node
      }
    }
    prev = current;         // Update previous node for next iteration
    current = current->next;
  }
  
  if (pred == nullptr) {
    // No predecessor found
    throw std::length_error("No predecessor found");
  }
  
  // Remove the predecessor node from the list
  if (prevPred == nullptr) {
    // Predecessor is at the head of the list
    head = pred->next;
    
    if (head == nullptr) {
      // List becomes empty after removal
      tail = nullptr;
    }
  } else {
    // Predecessor is in the middle or at the end
    prevPred->next = pred->next;
    
    if (pred == tail) {
      // Predecessor was the tail, update tail pointer
      tail = prevPred;
    }
  }
  
  delete pred; // Free the predecessor node
  size--;      // Decrement size counter
}

// ORDERED DICTIONARY IMPLEMENTATION - SUCCESSOR OPERATIONS
// These operations find elements that are immediately larger than a given value
// They implement the mathematical concept of successor in an ordered set

// Successor: Finds the smallest element larger than the given data
// Returns a reference to the successor element
// Time complexity: O(n) - must traverse list to find smallest element > data
template <typename Data>
const Data& SetLst<Data>::Successor(const Data& data) const {
  if (size == 0) {
    throw std::length_error("Empty set"); // Cannot find successor in empty set
  }

  // Search for the smallest element that is larger than data
  auto current = head;
  typename List<Data>::Node* succ = nullptr; // Will hold the best successor found
  
  // Traverse the entire list to find the smallest element > data
  while (current != nullptr) {
    if (current->element > data) {
      // This element is larger than data, check if it's the best successor so far
      if (succ == nullptr || current->element < succ->element) {
        succ = current; // Update best successor
      }
    }
    current = current->next;
  }
  
  if (succ == nullptr) {
    // No element found that is larger than data
    throw std::length_error("No successor found");
  }
  
  return succ->element; // Return the successor element
}

// SuccessorNRemove: Finds, returns, and removes the successor atomically
// This is a compound operation that ensures consistency
template <typename Data>
Data SetLst<Data>::SuccessorNRemove(const Data& data) {
  if (size == 0) {
    throw std::length_error("Empty set"); // Cannot find successor in empty set
  }

  // Find the successor and track the nodes needed for removal
  auto current = head;
  typename List<Data>::Node* succ = nullptr;     // The successor node
  typename List<Data>::Node* prevSucc = nullptr; // Node before successor
  typename List<Data>::Node* prev = nullptr;     // Previous node in traversal
  
  // Traverse to find the smallest element > data and its previous node
  while (current != nullptr) {
    if (current->element > data) {
      // This element is larger than data, check if it's the best successor
      if (succ == nullptr || current->element < succ->element) {
        succ = current;     // Update best successor
        prevSucc = prev;    // Update successor's previous node
      }
    }
    prev = current;         // Update previous node for next iteration
    current = current->next;
  }
  
  if (succ == nullptr) {
    // No successor found
    throw std::length_error("No successor found");
  }
  
  // Store the successor value before removal
  Data result = succ->element;
  
  // Remove the successor node from the list
  if (prevSucc == nullptr) {
    // Successor is at the head of the list
    head = succ->next;
    
    if (head == nullptr) {
      // List becomes empty after removal
      tail = nullptr;
    }
  } else {
    // Successor is in the middle or at the end
    prevSucc->next = succ->next;
    
    if (succ == tail) {
      // Successor was the tail, update tail pointer
      tail = prevSucc;
    }
  }
  
  delete succ; // Free the successor node
  size--;      // Decrement size counter
  
  return result; // Return the removed successor value
}

// RemoveSuccessor: Removes the successor without returning its value
// More efficient when the value is not needed
template <typename Data>
void SetLst<Data>::RemoveSuccessor(const Data& data) {
  if (size == 0) {
    throw std::length_error("Empty set"); // Cannot find successor in empty set
  }

  // Find the successor and track the nodes needed for removal
  auto current = head;
  typename List<Data>::Node* succ = nullptr;     // The successor node
  typename List<Data>::Node* prevSucc = nullptr; // Node before successor
  typename List<Data>::Node* prev = nullptr;     // Previous node in traversal
  
  // Traverse to find the smallest element > data and its previous node
  while (current != nullptr) {
    if (current->element > data) {
      // This element is larger than data, check if it's the best successor
      if (succ == nullptr || current->element < succ->element) {
        succ = current;     // Update best successor
        prevSucc = prev;    // Update successor's previous node
      }
    }
    prev = current;         // Update previous node for next iteration
    current = current->next;
  }
  
  if (succ == nullptr) {
    // No successor found
    throw std::length_error("No successor found");
  }
  
  // Remove the successor node from the list
  if (prevSucc == nullptr) {
    // Successor is at the head of the list
    head = succ->next;
    
    if (head == nullptr) {
      // List becomes empty after removal
      tail = nullptr;
    }
  } else {
    // Successor is in the middle or at the end
    prevSucc->next = succ->next;
    
    if (succ == tail) {
      // Successor was the tail, update tail pointer
      tail = prevSucc;
    }
  }
  
  delete succ; // Free the successor node
  size--;      // Decrement size counter
}

/* ************************************************************************** */

// TEMPLATE INSTANTIATION
// Explicit template instantiation for commonly used data types
// This ensures the template code is compiled for these specific types
// and can improve compilation times and reduce code bloat

// Instantiate SetLst template for fundamental integer type
template class SetLst<int>;

// Instantiate SetLst template for fundamental floating-point type
template class SetLst<double>;

// Instantiate SetLst template for standard string type
template class SetLst<std::string>;

/* ************************************************************************** */

}