/*
 * Vector Implementation File
 * 
 * This file contains the complete implementation of Vector and SortableVector classes.
 * The Vector class provides a dynamic array with contiguous memory allocation,
 * efficient random access, and automatic memory management. SortableVector extends
 * this functionality with integrated sorting capabilities.
 * 
 * Implementation highlights:
 * - RAII-compliant memory management
 * - Exception-safe operations with strong guarantee
 * - Move semantics for performance optimization
 * - Template specialization for different data types
 */

namespace lasd {

/* ************************************************************************** */

// VECTOR CLASS IMPLEMENTATION
// Provides dynamic array functionality with automatic memory management

// Specific constructors for different initialization scenarios

// Constructor with initial size: Creates vector with specified number of default-constructed elements
template <typename Data>
Vector<Data>::Vector(const ulong newSize) {
  Elements = new Data[newSize]{}; // Allocate array and default-construct all elements
  size = newSize; // Set the container size
}

// Constructor from TraversableContainer: Creates vector by copying elements from any traversable container
// This allows creation from lists, other vectors, etc.
template <typename Data>
Vector<Data>::Vector(const TraversableContainer<Data>& container) {
  size = container.Size(); // Get the size of source container
  Elements = new Data[size]{}; // Allocate array for elements
  
  // Use traversal to copy elements sequentially
  ulong i = 0;
  container.Traverse([this, &i](const Data& data) {
    Elements[i++] = data; // Copy each element to vector
  });
}

// Constructor from MappableContainer: Creates vector by moving elements for efficiency
// Uses move semantics when possible to avoid unnecessary copying
template <typename Data>
Vector<Data>::Vector(MappableContainer<Data>&& container) {
  size = container.Size(); // Get the size of source container
  Elements = new Data[size]{}; // Allocate array for elements

  // Use mapping to move elements efficiently
  ulong i = 0;
  container.Map([this, &i](Data& data) {
    std::swap(Elements[i++], data); // Move data from source to vector position
                                   // Source element becomes default-constructed
  });
  // Note: Source container is not explicitly cleared to maintain compatibility
}

// Copy constructor: Creates a deep copy of another vector
// Provides the strong exception safety guarantee
template <typename Data>
Vector<Data>::Vector(const Vector<Data>& vector) {
  size = vector.size; // Copy the size
  Elements = new Data[size]{}; // Allocate new memory
  
  // Copy all elements individually
  for(ulong i = 0; i < size; ++i) {
    Elements[i] = vector.Elements[i];
  }
}

// Move constructor: Efficiently transfers ownership from another vector
// Uses swap to avoid memory allocation/deallocation
template <typename Data>
Vector<Data>::Vector(Vector<Data>&& vector) noexcept {
  std::swap(Elements, vector.Elements); // Transfer ownership of array
  std::swap(size, vector.size); // Transfer size information
  // The moved-from vector will be left in a valid but unspecified state
}

// Destructor: Automatically frees allocated memory
// Follows RAII principles for automatic cleanup
template <typename Data>
Vector<Data>::~Vector() {
  delete[] Elements; // Free the dynamically allocated array
  // Elements pointer becomes invalid, but that's fine as object is being destroyed
}

// Copy assignment: Safely assigns the contents of another vector
// Uses copy-and-swap idiom for exception safety
template <typename Data>
Vector<Data>& Vector<Data>::operator=(const Vector<Data>& vector) {
  if (this != &vector) { // Protect against self-assignment
    // Allocate new memory first (exception-safe approach)
    Data* tempElements = new Data[vector.size]{};
    
    // Copy elements to new memory
    for(ulong i = 0; i < vector.size; ++i) {
      tempElements[i] = vector.Elements[i];
    }
    
    // Only after successful copy, replace our data
    delete[] Elements; // Free old memory
    Elements = tempElements; // Assign new memory
    size = vector.size; // Update size
  }
  
  return *this; // Return reference for chaining
}

// Move assignment: Efficiently transfers ownership from another vector
// Avoids memory allocation by simply swapping resources
template <typename Data>
Vector<Data>& Vector<Data>::operator=(Vector<Data>&& vector) noexcept {
  if (this != &vector) { // Protect against self-assignment
    std::swap(Elements, vector.Elements); // Swap array pointers
    std::swap(size, vector.size); // Swap size values
    // The moved-from vector will clean up our old data in its destructor
  }
  
  return *this; // Return reference for chaining
}

// Comparison operators for structural equality testing

// Equality operator: Returns true if vectors have same size and identical elements
template <typename Data>
bool Vector<Data>::operator==(const Vector<Data>& vector) const noexcept {
  if (size != vector.size) {
    return false; // Different sizes means vectors cannot be equal
  }
  
  // Compare each element pair
  for(ulong i = 0; i < size; ++i) {
    if (Elements[i] != vector.Elements[i]) {
      return false; // Found differing elements
    }
  }
  
  return true; // All elements matched
}

// Inequality operator: Returns true if vectors differ in size or content
template <typename Data>
bool Vector<Data>::operator!=(const Vector<Data>& vector) const noexcept {
  return !(*this == vector); // Simply negate the equality result
}

// Specific member functions (inherited from MutableLinearContainer)
// These provide mutable (read-write) access to vector elements with bounds checking

// Mutable element access by index: Returns a reference that can be modified
// Time complexity: O(1) - direct array access
template <typename Data>
Data& Vector<Data>::operator[](ulong index) {
  if (index >= size) {
    throw std::out_of_range("Access at index " + std::to_string(index) + " on vector of size " + std::to_string(size));
  }
  
  return Elements[index]; // Direct access to array element
}

// Mutable access to first element: Returns a reference to the front element
template <typename Data>
Data& Vector<Data>::Front() {
  if (size == 0) {
    throw std::length_error("Access to an empty vector");
  }
  
  return Elements[0]; // First element is always at index 0
}

// Mutable access to last element: Returns a reference to the back element
template <typename Data>
Data& Vector<Data>::Back() {
  if (size == 0) {
    throw std::length_error("Access to an empty vector");
  }
  
  return Elements[size - 1]; // Last element is at index (size-1)
}

// Specific member functions (inherited from LinearContainer)
// These provide immutable (read-only) access to vector elements with bounds checking

// Immutable element access by index: Returns a const reference for read-only access
// Time complexity: O(1) - direct array access
template <typename Data>
const Data& Vector<Data>::operator[](ulong index) const {
  if (index >= size) {
    throw std::out_of_range("Access at index " + std::to_string(index) + " on vector of size " + std::to_string(size));
  }
  
  return Elements[index]; // Direct const access to array element
}

// Immutable access to first element: Returns a const reference to the front element
template <typename Data>
const Data& Vector<Data>::Front() const {
  if (size == 0) {
    throw std::length_error("Access to an empty vector");
  }
  
  return Elements[0]; // First element is always at index 0
}

// Immutable access to last element: Returns a const reference to the back element
template <typename Data>
const Data& Vector<Data>::Back() const {
  if (size == 0) {
    throw std::length_error("Access to an empty vector");
  }
  
  return Elements[size - 1]; // Last element is at index (size-1)
}

// Specific member function (inherited from ResizableContainer)
// Dynamically changes the vector size while preserving existing elements when possible

template <typename Data>
void Vector<Data>::Resize(ulong newSize) {
  if (newSize == size) {
    return; // No change needed - optimization for same size
  }
  
  if (newSize == 0) {
    // Special case: resizing to empty vector
    delete[] Elements; // Free existing memory
    Elements = nullptr; // Reset pointer to null
    size = 0; // Update size to zero
    return;
  }
  
  // General case: create new array with new size
  Data* tempElements = new Data[newSize]{}; // Allocate and default-construct elements
  
  // Copy existing elements up to the minimum of old and new sizes
  ulong minSize = (size < newSize) ? size : newSize;
  for(ulong i = 0; i < minSize; ++i) {
    tempElements[i] = Elements[i]; // Preserve existing data
  }
  // Note: If newSize > size, new elements are default-constructed
  // If newSize < size, excess elements are discarded
  
  delete[] Elements; // Free old memory
  Elements = tempElements; // Assign new memory
  size = newSize; // Update size
}

/* ************************************************************************** */

// SORTABLE VECTOR CLASS IMPLEMENTATION
// Extends Vector with sorting capabilities through multiple inheritance

// Default constructor: Creates an empty sortable vector
// Delegates to Vector's default constructor
template <typename Data>
SortableVector<Data>::SortableVector() : Vector<Data>() {}

// Specific constructors: Initialize sortable vector with various data sources
// All delegate to corresponding Vector constructors for the actual data management

// Constructor with initial size: Creates sortable vector with specified number of elements
template <typename Data>
SortableVector<Data>::SortableVector(const ulong n) : Vector<Data>(n) {}

// Constructor from TraversableContainer: Creates sortable vector by copying from any traversable container
template <typename Data>
SortableVector<Data>::SortableVector(const TraversableContainer<Data>& con) : Vector<Data>(con) {}

// Constructor from MappableContainer: Creates sortable vector by moving from any mappable container
template <typename Data>
SortableVector<Data>::SortableVector(MappableContainer<Data>&& con) : Vector<Data>(std::move(con)) {}

// Copy constructor: Creates a deep copy of another sortable vector
template <typename Data>
SortableVector<Data>::SortableVector(const SortableVector<Data>& sv) : Vector<Data>(sv) {}

// Move constructor: Efficiently transfers ownership from another sortable vector
template <typename Data>
SortableVector<Data>::SortableVector(SortableVector<Data>&& sv) noexcept : Vector<Data>(std::move(sv)) {}

// Copy assignment: Assigns contents of another sortable vector with deep copying
template <typename Data>
SortableVector<Data>& SortableVector<Data>::operator=(const SortableVector<Data>& sv) {
  Vector<Data>::operator=(sv); // Delegate to Vector's copy assignment
  return *this; // Return reference for chaining
}

// Move assignment: Efficiently transfers ownership from another sortable vector
template <typename Data>
SortableVector<Data>& SortableVector<Data>::operator=(SortableVector<Data>&& sv) noexcept {
  Vector<Data>::operator=(std::move(sv)); // Delegate to Vector's move assignment
  return *this; // Return reference for chaining
}

// Implementation of SwapAt method from SortableLinearContainer
// This method is called during sorting operations to exchange elements at specific positions
// The temporary values are provided to optimize the swapping process
template <typename Data>
void SortableVector<Data>::SwapAt(ulong i, ulong j, const Data& temp_i, const Data& temp_j) {
  // Direct assignment using the pre-computed temporary values
  // This is more efficient than using std::swap as it avoids extra copies
  this->Elements[i] = temp_j; // Place element j's value at position i
  this->Elements[j] = temp_i; // Place element i's value at position j
  
  // Note: The QuickSort algorithm in SortableLinearContainer handles the
  // temporary value management and calls this method for the actual swapping
}

/* ************************************************************************** */

}
