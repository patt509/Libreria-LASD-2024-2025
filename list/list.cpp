#include <type_traits>
#include <stdexcept>
#include <string>

namespace lasd {

/* ************************************************************************** */
// Node Implementation
/* ************************************************************************** */

// Clone() - Creates a deep copy of this node with optional next pointer
// This is used for implementing list copy operations
template <typename Data>
typename List<Data>::Node* List<Data>::Node::Clone(Node* next) {
  // Create a new node with the same element data
  Node* temp = new Node(element);
  temp->next = next; // Set the next pointer to the provided value
  return temp;
}

/* ************************************************************************** */
// List Constructors Implementation
/* ************************************************************************** */

// Constructor from TraversableContainer - Copy Semantics
// Creates a new list by copying all elements from any traversable container
template <typename Data>
List<Data>::List(const TraversableContainer<Data>& container) {
  // Use the container's traverse function to visit each element
  // and add it to the back of this list
  container.Traverse(
    [this](const Data& dat) {
      InsertAtBack(dat); // Copy each element to the back of the list
    }
  );
}

// Constructor from MappableContainer - Move Semantics
// Creates a new list by moving elements from a mappable container
template <typename Data>
List<Data>::List(MappableContainer<Data>&& container) {
  // Use the container's map function to access each element for moving
  container.Map(
    [this](Data& dat) {
      Data temp{}; // Create a temporary default-constructed value
      std::swap(temp, dat); // Swap to extract the original value
      InsertAtBack(std::move(temp)); // Move the extracted value into the list
    }
  );
  // Note: The source container maintains its structure but with default-constructed elements
}

/* ************************************************************************** */
// List Copy and Move Constructors
/* ************************************************************************** */

// Copy Constructor - Creates a deep copy of another list
// All nodes and their data are copied, maintaining the same order
template <typename Data>
List<Data>::List(const List& other) {
  // Only proceed if the other list has elements
  if(other.size > 0) {
    // Traverse the other list manually and copy all elements
    Node* curr = other.head;
    while(curr != nullptr) {
      InsertAtBack(curr->element); // Copy each element to maintain order
      curr = curr->next;
    }
  }
}

// Move Constructor - Transfers ownership from another list
// This is very efficient as it just swaps pointers, no copying involved
template <typename Data>
List<Data>::List(List&& other) noexcept {
  // Transfer ownership of all resources using swap
  std::swap(head, other.head);   // Transfer head pointer
  std::swap(tail, other.tail);   // Transfer tail pointer
  std::swap(size, other.size);   // Transfer size count
  // The other list is left in a valid empty state
}

/* ************************************************************************** */
// List Destructor
/* ************************************************************************** */

// Destructor - Properly deallocates all nodes in the list
// Uses Clear() to handle the deallocation process
template <typename Data>
List<Data>::~List() {
  Clear(); // Delegate to Clear() which handles proper node deallocation
}

/* ************************************************************************** */
// List Assignment Operators
/* ************************************************************************** */

// Copy Assignment - Replaces current content with a deep copy of another list
template <typename Data>
List<Data>& List<Data>::operator=(const List& other) {
  // Protect against self-assignment which would be destructive
  if(this != &other) {
    Clear(); // First clear current contents to avoid memory leaks

    // Copy all elements from the other list in order
    Node* curr = other.head;
    while(curr != nullptr) {
      InsertAtBack(curr->element); // Copy each element
      curr = curr->next;
    }
  }
  return *this; // Return reference for chaining
}

// Move Assignment - Efficiently transfers resources from another list
// Avoids the expensive copy-and-delete cycle by simply swapping pointers
template <typename Data>
List<Data>& List<Data>::operator=(List&& other) noexcept {
  // Protect against self-assignment
  if(this != &other) {
    // Swap all member variables - this transfers ownership efficiently
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(size, other.size);
    // The 'other' list will be destroyed with our old data
  }
  return *this; // Return reference for chaining
}

/* ************************************************************************** */

// Comparison operators (List)
// These operators compare the structural content of two lists

// Equality operator - Returns true if both lists contain the same elements in the same order
template <typename Data>
bool List<Data>::operator==(const List& other) const noexcept {
  // Quick check: if sizes differ, lists cannot be equal
  if(size != other.size)
    return false;
  
  // If both lists are empty, they are equal
  if(size == 0)
    return true;
  
  // Compare elements node by node in sequence
  Node* curr1 = head;
  Node* curr2 = other.head;
  
  while(curr1 != nullptr) {
    // If any corresponding elements differ, lists are not equal
    if(curr1->element != curr2->element)
      return false;
      
    curr1 = curr1->next;
    curr2 = curr2->next;
  }
  
  return true; // All elements matched - lists are identical
}

// Inequality operator - Returns true if lists differ in size or content
template <typename Data>
bool List<Data>::operator!=(const List& other) const noexcept {
  return !(*this == other); // Simply negate the equality result
}

/* ************************************************************************** */

// Specific member functions (List)
// These functions provide efficient list-specific operations for front and back insertion/removal

// InsertAtFront - Copy version: Creates a copy of the data and adds it to the front
template <typename Data>
void List<Data>::InsertAtFront(const Data& data) {
  // Create new node with copied data
  Node* newNode = new Node(data);
  
  // Link new node to current head (could be nullptr for empty list)
  newNode->next = head;
  head = newNode;
  
  // If this is the first element, it's also the tail
  if(tail == nullptr)
    tail = head;
    
  size++; // Update size counter
}

// InsertAtFront - Move version: Transfers ownership of data and adds it to the front
template <typename Data>
void List<Data>::InsertAtFront(Data&& data) {
  // Create new node with moved data (avoids unnecessary copy)
  Node* newNode = new Node(std::move(data));
  
  // Link new node to current head
  newNode->next = head;
  head = newNode;
  
  // If this is the first element, it's also the tail
  if(tail == nullptr)
    tail = head;
    
  size++; // Update size counter
}

// RemoveFromFront: Removes the first element from the list
// Throws exception if the list is empty
template <typename Data>
void List<Data>::RemoveFromFront() {
  if(head == nullptr)
    throw std::length_error("Access to an empty list");
    
  Node* temp = head;
  head = head->next; // Move head to next node
  
  // If we removed the last element, update tail
  if(head == nullptr)
    tail = nullptr;
    
  temp->next = nullptr; // Prevent cascade deletion
  delete temp; // Free the removed node
  
  size--; // Update size counter
}

// FrontNRemove: Returns the front element and removes it from the list
// Combines Front() and RemoveFromFront() operations efficiently
template <typename Data>
Data List<Data>::FrontNRemove() {
  if(head == nullptr)
    throw std::length_error("Access to an empty list");
    
  // Store the front element before removal
  Data result = head->element;
  
  // Remove the front node
  Node* temp = head;
  head = head->next;
  
  // If we removed the last element, update tail
  if(head == nullptr)
    tail = nullptr;
    
  temp->next = nullptr; // Prevent cascade deletion
  delete temp; // Free the removed node
  
  size--; // Update size counter
  return result; // Return the removed element
}

// InsertAtBack - Copy version: Creates a copy of the data and adds it to the back
// More efficient than InsertAtFront for maintaining insertion order
template <typename Data>
void List<Data>::InsertAtBack(const Data& data) {
  // Create new node with copied data
  Node* newNode = new Node(data);
  
  // Handle empty list case
  if(head == nullptr) {
    head = newNode;
    tail = newNode;
  } else {
    // Link current tail to new node and update tail pointer
    tail->next = newNode;
    tail = newNode;
  }
  
  size++; // Update size counter
}

// InsertAtBack - Move version: Transfers ownership of data and adds it to the back
template <typename Data>
void List<Data>::InsertAtBack(Data&& data) {
  // Create new node with moved data (avoids unnecessary copy)
  Node* newNode = new Node(std::move(data));
  
  // Handle empty list case
  if(head == nullptr) {
    head = newNode;
    tail = newNode;
  } else {
    // Link current tail to new node and update tail pointer
    tail->next = newNode;
    tail = newNode;
  }
  
  size++; // Update size counter
}

// RemoveFromBack: Removes the last element from the list
// Less efficient than RemoveFromFront due to need to traverse to find previous node
template <typename Data>
void List<Data>::RemoveFromBack() {
  if(head == nullptr)
    throw std::length_error("Access to an empty list");
    
  if(head == tail) {
    // Single element case - both head and tail point to same node
    delete head;
    head = nullptr;
    tail = nullptr;
  } else {
    // Multiple elements - need to find the node before tail
    Node* curr = head;
    while(curr->next != tail) {
      curr = curr->next;
    }
    
    // Remove tail and update pointers
    delete tail;
    tail = curr;
    tail->next = nullptr;
  }
  
  size--; // Update size counter
}

// BackNRemove: Returns the back element and removes it from the list
// Combines Back() and RemoveFromBack() operations
template <typename Data>
Data List<Data>::BackNRemove() {
  if(head == nullptr)
    throw std::length_error("Access to an empty list");
    
  // Store the back element before removal
  Data result = tail->element;
  
  if(head == tail) {
    // Single element case
    delete head;
    head = nullptr;
    tail = nullptr;
  } else {
    // Multiple elements - find the node before tail
    Node* curr = head;
    while(curr->next != tail) {
      curr = curr->next;
    }
    
    // Remove tail and update pointers
    delete tail;
    tail = curr;
    tail->next = nullptr;
  }
  
  size--; // Update size counter
  return result; // Return the removed element
}

/* ************************************************************************** */

// Specific member functions (inherited from MutableLinearContainer)
// These provide mutable (read-write) access to list elements by position

// Mutable access to element at index: Returns a reference that can be modified
// Time complexity: O(n) due to sequential traversal from head
template <typename Data>
Data& List<Data>::operator[](ulong index) {
  if(index >= size)
    throw std::out_of_range("Access at index " + std::to_string(index) + " on list of size " + std::to_string(size));
    
  // Sequential traversal to reach the desired index
  Node* curr = head;
  for(ulong i = 0; i < index; i++)
    curr = curr->next;
    
  return curr->element; // Return modifiable reference
}

// Mutable access to front element: Returns a reference to the first element
template <typename Data>
Data& List<Data>::Front() {
  if(head == nullptr)
    throw std::length_error("Access to an empty list");
    
  return head->element; // Direct access - O(1) complexity
}

// Mutable access to back element: Returns a reference to the last element
template <typename Data>
Data& List<Data>::Back() {
  if(tail == nullptr)
    throw std::length_error("Access to an empty list");
    
  return tail->element; // Direct access via tail pointer - O(1) complexity
}

/* ************************************************************************** */

// Specific member functions (inherited from LinearContainer)
// These provide immutable (read-only) access to list elements by position

// Immutable access to element at index: Returns a const reference for read-only access
template <typename Data>
const Data& List<Data>::operator[](ulong index) const {
  if(index >= size)
    throw std::out_of_range("Access at index " + std::to_string(index) + " on list of size " + std::to_string(size));
    
  // Sequential traversal to reach the desired index
  Node* curr = head;
  for(ulong i = 0; i < index; i++)
    curr = curr->next;
    
  return curr->element; // Return const reference
}

// Immutable access to front element: Returns a const reference to the first element
template <typename Data>
const Data& List<Data>::Front() const {
  if(head == nullptr)
    throw std::length_error("Access to an empty list");
    
  return head->element; // Direct const access
}

// Immutable access to back element: Returns a const reference to the last element
template <typename Data>
const Data& List<Data>::Back() const {
  if(tail == nullptr)
    throw std::length_error("Access to an empty list");
    
  return tail->element; // Direct const access via tail pointer
}

/* ************************************************************************** */

// Specific member function (inherited from MappableContainer)
// Provides the default mapping behavior using pre-order traversal

template <typename Data>
void List<Data>::Map(MapFun fun) {
  PreOrderMap(fun); // Default to pre-order mapping (front to back)
}

/* ************************************************************************** */

// Specific member function (inherited from PreOrderMappableContainer)
// Applies a function to each element in order from front to back

template <typename Data>
void List<Data>::PreOrderMap(MapFun fun) {
  PreOrderMap(fun, head); // Start mapping from the head node
}

// Specific member function (inherited from PostOrderMappableContainer)
// Applies a function to each element in reverse order (back to front)

template <typename Data>
void List<Data>::PostOrderMap(MapFun fun) {
  PostOrderMap(fun, head); // Start post-order mapping from head (recursively handles order)
}

// Specific member function (inherited from TraversableContainer)
// Provides the default traversal behavior using pre-order traversal

template <typename Data>
void List<Data>::Traverse(TraverseFun fun) const {
  PreOrderTraverse(fun); // Default to pre-order traversal (front to back)
}

/* ************************************************************************** */

// Specific member function (inherited from PreOrderTraversableContainer)
// Traverses elements from front to back, applying a read-only function to each

template <typename Data>
void List<Data>::PreOrderTraverse(TraverseFun fun) const {
  PreOrderTraverse(fun, head); // Start traversal from the head node
}

// Specific member function (inherited from PostOrderTraversableContainer)
// Traverses elements from back to front, applying a read-only function to each

template <typename Data>
void List<Data>::PostOrderTraverse(TraverseFun fun) const {
  PostOrderTraverse(fun, head); // Start post-order traversal from head (recursively handles order)
}

/* ************************************************************************** */

// Specific member function (inherited from ClearableContainer)
// Removes all elements from the list and frees associated memory

template <typename Data>
void List<Data>::Clear() {
  if(head != nullptr) {
    Node* curr = head;
    while(curr != nullptr) {
      Node* temp = curr;
      curr = curr->next;
      temp->next = nullptr; // Detach to prevent cascade deletion issues
      delete temp; // Free each node individually
    }
    
    // Reset list to empty state
    head = nullptr;
    tail = nullptr;
    size = 0;
  }
}

/* ************************************************************************** */

// Specific member functions (inherited from DictionaryContainer)
// These functions implement set-like behavior where duplicates are not allowed

// Insert - Copy version: Adds an element only if it doesn't already exist
// Returns true if element was inserted, false if it already existed
template <typename Data>
bool List<Data>::Insert(const Data& data) {
  // Check if the element already exists in the list
  Node* curr = head;
  while(curr != nullptr) {
    if(curr->element == data)
      return false; // Element already exists - no insertion
    curr = curr->next;
  }
  
  // Element doesn't exist - add it to the back
  InsertAtBack(data);
  return true; // Successfully inserted
}

// Insert - Move version: Adds an element only if it doesn't already exist
// More efficient as it avoids copying when the element doesn't exist
template <typename Data>
bool List<Data>::Insert(Data&& data) {
  // Create a copy to compare (since we might move the original)
  Data temp = data;
  Node* curr = head;
  while(curr != nullptr) {
    if(curr->element == temp)
      return false; // Element already exists - no insertion
    curr = curr->next;
  }
  
  // Element doesn't exist - move it to the back
  InsertAtBack(std::move(data));
  return true; // Successfully inserted
}

// Remove: Removes the first occurrence of the specified element
// Returns true if element was found and removed, false otherwise
template <typename Data>
bool List<Data>::Remove(const Data& data) {
  if(head == nullptr)
    return false; // Empty list - nothing to remove
    
  // Special case: remove head element
  if(head->element == data) {
    RemoveFromFront();
    return true;
  }
  
  // Search for the element in the rest of the list
  Node* prev = head;
  Node* curr = head->next;
  
  while(curr != nullptr) {
    if(curr->element == data) {
      // Found the element - remove it
      prev->next = curr->next;
      
      // Update tail if we're removing the last element
      if(curr == tail)
        tail = prev;
        
      curr->next = nullptr; // Prevent cascade deletion
      delete curr;
      size--;
      
      return true; // Successfully removed
    }
    
    prev = curr;
    curr = curr->next;
  }
  
  return false; // Element not found
}

// InsertAll from TraversableContainer: Attempts to insert all elements from another container
// Returns true only if ALL elements were successfully inserted (none were duplicates)
template <typename Data>
bool List<Data>::InsertAll(const TraversableContainer<Data>& container) {
  bool allInserted = true;
  container.Traverse(
    [this, &allInserted](const Data& data) {
      allInserted &= Insert(data); // AND with each insertion result
    }
  );
  return allInserted; // True only if all elements were new
}

// InsertAll from MappableContainer (move version): More efficient for movable elements
template <typename Data>
bool List<Data>::InsertAll(MappableContainer<Data>&& container) {
  bool allInserted = true;
  container.Map(
    [this, &allInserted](Data& data) {
      allInserted &= Insert(data); // Insert by moving when possible
    }
  );
  return allInserted;
}

// RemoveAll: Removes all elements that exist in the given container
// Returns true if at least one element was removed
template <typename Data>
bool List<Data>::RemoveAll(const TraversableContainer<Data>& container) {
  // Record initial size to detect if any removals occurred
  ulong initialSize = size;
  
  // Attempt to remove each element from the container
  container.Traverse(
    [this](const Data& data) {
      Remove(data); // Remove if it exists (ignore return value)
    }
  );
  
  // Return true if the list size decreased (something was removed)
  return size < initialSize;
}

// InsertSome from TraversableContainer: Inserts elements that don't already exist
// Returns true if at least one element was successfully inserted
template <typename Data>
bool List<Data>::InsertSome(const TraversableContainer<Data>& container) {
  // Track whether any insertions succeeded
  bool inserted = false;
  
  // Try inserting all elements
  container.Traverse(
    [this, &inserted](const Data& data) {
      if(Insert(data)) // If insertion succeeded
        inserted = true;
    }
  );
  
  return inserted; // True if at least one element was inserted
}

// InsertSome from MappableContainer (move version): More efficient version
template <typename Data>
bool List<Data>::InsertSome(MappableContainer<Data>&& container) {
  // Track whether any insertions succeeded
  bool inserted = false;
  
  // Try inserting all elements with move semantics
  container.Map(
    [this, &inserted](Data& data) {
      if(Insert(std::move(data))) // Move the data if insertion succeeds
        inserted = true;
    }
  );
  
  return inserted;
}

// RemoveSome: Removes elements that exist in both containers
// Returns true if at least one element was successfully removed
template <typename Data>
bool List<Data>::RemoveSome(const TraversableContainer<Data>& container) {
  // Track whether any removals succeeded
  bool removed = false;
  
  // Try removing all elements
  container.Traverse(
    [this, &removed](const Data& data) {
      if(Remove(data)) // If removal succeeded
        removed = true;
    }
  );
  
  return removed; // True if at least one element was removed
}

/* ************************************************************************** */

// Auxiliary member functions (for MappableContainer)
// These are protected helper methods that perform the actual traversal work

// Protected auxiliary method for PreOrderMap: Applies function from front to back
// Uses iterative approach for better performance and stack safety
template <typename Data>
void List<Data>::PreOrderMap(MapFun fun, Node* curr) {
  while(curr != nullptr) {
    fun(curr->element); // Apply function to current element
    curr = curr->next;   // Move to next node
  }
}

// Protected auxiliary method for PostOrderMap: Applies function from back to front
// Uses recursive approach to achieve reverse order traversal
template <typename Data>
void List<Data>::PostOrderMap(MapFun fun, Node* curr) {
  if(curr != nullptr) {
    PostOrderMap(fun, curr->next); // Recursively process rest of list first
    fun(curr->element);            // Then apply function to current element
  }
}

/* ************************************************************************** */

// Auxiliary member functions (for TraversableContainer)
// These are protected helper methods for const traversal operations

// Protected auxiliary method for PreOrderTraverse: Traverses from front to back
// Uses iterative approach for efficiency
template <typename Data>
void List<Data>::PreOrderTraverse(TraverseFun fun, const Node* curr) const {
  while(curr != nullptr) {
    fun(curr->element); // Apply const function to current element
    curr = curr->next;   // Move to next node
  }
}

// Protected auxiliary method for PostOrderTraverse: Traverses from back to front
// Uses recursive approach to achieve reverse order
template <typename Data>
void List<Data>::PostOrderTraverse(TraverseFun fun, const Node* curr) const {
  if(curr != nullptr) {
    PostOrderTraverse(fun, curr->next); // Recursively process rest of list first
    fun(curr->element);                 // Then apply const function to current element
  }
}

/* ************************************************************************** */

}
