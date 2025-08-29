#include <stdexcept>

namespace lasd {

/* ************************************************************************** */
// LinearContainer Implementation - Comparison Operators
/* ************************************************************************** */

// operator==() - Checks if two linear containers have identical elements in the same order
// This performs element-by-element comparison after checking size compatibility
template<typename Data>
bool LinearContainer<Data>::operator==(const LinearContainer<Data> & con) const noexcept {
  // First check if sizes are equal - if not, containers cannot be equal
  if (size == con.size) {
    // Compare each element at the same position in both containers
    for (ulong index = 0; index < size; ++index) {
      if (operator[](index) != con[index]) {
        return false; // Found a mismatch, containers are not equal
      }
    }
    return true; // All elements match, containers are equal
  } else {
    return false; // Different sizes, containers cannot be equal
  }
}

// operator!=() - Checks if two linear containers are different
// Simply negates the result of operator== for consistency
template<typename Data>
bool LinearContainer<Data>::operator!=(const LinearContainer<Data> & con) const noexcept {
  return !(*this == con);
}

/* ************************************************************************** */
// LinearContainer Implementation - Access Functions
/* ************************************************************************** */

// Front() - Access the first element in the container (read-only version)
// Provides semantic clarity compared to operator[](0)
template <typename Data>
const Data & LinearContainer<Data>::Front() const {
  // Check if container is empty and throw appropriate exception
  if (size == 0) {
    throw std::length_error("Access to an empty linear container.");
  }
  return operator[](0); // Return first element
}

// Back() - Access the last element in the container (read-only version)
// Provides semantic clarity compared to operator[](size-1)
template <typename Data>
const Data & LinearContainer<Data>::Back() const {
  // Check if container is empty and throw appropriate exception
  if (size == 0) {
    throw std::length_error("Access to an empty linear container.");
  }
  return operator[](size - 1); // Return last element
}

/* ************************************************************************** */
// LinearContainer Implementation - Traversal Functions
/* ************************************************************************** */

// Traverse() - Default traversal implementation using pre-order traversal
// For linear containers, this means front-to-back iteration
template<typename Data>
void LinearContainer<Data>::Traverse(TraverseFun fun) const {
  // Delegate to PreOrderTraverse for consistency
  PreOrderTraverse(fun);
}

// PreOrderTraverse() - Traverse elements from front to back (left to right)
// For linear containers, pre-order means natural sequential order
template<typename Data>
void LinearContainer<Data>::PreOrderTraverse(TraverseFun fun) const {
  // Iterate through elements from index 0 to size-1
  for (ulong index = 0; index < size; ++index) {
    fun(operator[](index)); // Apply function to each element
  }
}

// PostOrderTraverse() - Traverse elements from back to front (right to left)
// For linear containers, post-order means reverse sequential order
template<typename Data>
void LinearContainer<Data>::PostOrderTraverse(TraverseFun fun) const {
  // Iterate through elements from last to first using reverse iteration
  ulong index = size;
  while (index > 0) {
    fun(operator[](--index)); // Apply function to each element in reverse order
  }
}

/* ************************************************************************** */
// MutableLinearContainer Implementation - Mapping Functions
/* ************************************************************************** */

// Map() - Default mapping implementation using pre-order mapping
// Applies a function to modify each element from front to back
template<typename Data>
void MutableLinearContainer<Data>::Map(MapFun fun) {
  // Delegate to PreOrderMap for consistency
  PreOrderMap(fun);
}

// PreOrderMap() - Apply a function to modify elements from front to back
// For linear containers, this means natural sequential modification
template<typename Data>
void MutableLinearContainer<Data>::PreOrderMap(MapFun fun) {
  // Iterate through elements from index 0 to size-1, allowing modification
  for (ulong index = 0; index < LinearContainer<Data>::size; ++index) {
    fun(operator[](index)); // Apply function to potentially modify each element
  }
}

// PostOrderMap() - Apply a function to modify elements from back to front
// For linear containers, this means reverse sequential modification
template<typename Data>
void MutableLinearContainer<Data>::PostOrderMap(MapFun fun) {
  // Iterate through elements from last to first, allowing modification
  ulong index = LinearContainer<Data>::size;
  while (index > 0) {
    fun(operator[](--index)); // Apply function to potentially modify each element in reverse
  }
}

/* ************************************************************************** */
// SortableLinearContainer Implementation - Sorting Functions
/* ************************************************************************** */

// Sort() - Public interface for sorting the container in ascending order
// Uses QuickSort algorithm for efficient O(n log n) average-case performance
template<typename Data>
void SortableLinearContainer<Data>::Sort() {
  // Only sort if there are at least 2 elements to sort
  if (this->size > 1) {
    QuickSort(0, this->size - 1); // Sort the entire range
  }
}

/* ************************************************************************** */
// SortableLinearContainer Implementation - QuickSort Algorithm
/* ************************************************************************** */

// QuickSort() - Recursive implementation of the QuickSort algorithm
// Divides the array into partitions around a pivot and recursively sorts each partition
template<typename Data>
void SortableLinearContainer<Data>::QuickSort(ulong p, ulong r) {
  // Base case: if p >= r, the subarray has 0 or 1 elements and is already sorted
  if (p < r) {
    // Partition the array and get the pivot position
    ulong q = Partition(p, r);
    
    // Recursively sort the left partition (elements smaller than pivot)
    if (q > 0) {
      QuickSort(p, q);
    }
    
    // Recursively sort the right partition (elements larger than pivot)
    QuickSort(q + 1, r);
  }
}

// Partition() - Partitions the array around a pivot element
// Rearranges elements so that smaller elements are on the left, larger on the right
// Returns the final position of the pivot
template<typename Data>
ulong SortableLinearContainer<Data>::Partition(ulong p, ulong r) {
  // Choose the first element as the pivot
  Data x = this->operator[](p);
  ulong i = p - 1; // Index for smaller elements
  ulong j = r + 1; // Index for larger elements
  
  do {
    // Move j leftward to find an element <= pivot
    do {
      j--;
    } while (this->operator[](j) > x);
    
    // Move i rightward to find an element >= pivot
    do {
      i++;
    } while (this->operator[](i) < x);
    
    // If pointers haven't crossed, swap the misplaced elements
    if (i < j) {
      // Create copies of the values for the SwapAt call
      Data temp_i = this->operator[](i);
      Data temp_j = this->operator[](j);
      
      // Use the virtual SwapAt function (implemented by derived classes)
      // This allows different container types to handle swapping appropriately
      SwapAt(i, j, temp_i, temp_j);
    }
  } while (i < j); // Continue until pointers cross
  
  return j; // Return the partition point
}

/* ************************************************************************** */

}
