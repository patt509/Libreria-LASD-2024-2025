#ifndef LINEAR_HPP
#define LINEAR_HPP

/* ************************************************************************** */

#include "mappable.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

// LinearContainer Class
// --------------------
// Represents a sequential collection of elements that can be accessed by position (index)
// This is the foundation for all linear data structures like arrays, lists, and vectors
// Elements are organized in a sequence where each element has a specific position
// It supports both pre-order and post-order traversal patterns
template <typename Data>
class LinearContainer : virtual public PreOrderTraversableContainer<Data>,
  virtual public PostOrderTraversableContainer<Data> {

private:

protected:
  // Inherits size variable from Container base class to track number of elements
  using Container::size;

public:
  // Virtual destructor for proper cleanup in derived classes
  virtual ~LinearContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  LinearContainer & operator=(const LinearContainer &) = delete;

  // Move assignment - deleted for abstract class
  LinearContainer & operator=(LinearContainer &&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators

  // operator==() - Check if two linear containers have the same elements in the same order
  // Compares size first, then element-by-element comparison
  bool operator==(const LinearContainer &) const noexcept;

  // operator!=() - Check if two linear containers are different
  bool operator!=(const LinearContainer &) const noexcept;

  /* ************************************************************************ */

  // Specific member functions for indexed access

  // operator[]() - Access element at specified position (read-only)
  // Throws std::out_of_range if position is invalid (>= size)
  // This is the fundamental indexed access operation for linear containers
  virtual const Data & operator[](ulong) const = 0;

  // Front() - Access the first element in the container (read-only)
  // Equivalent to operator[](0) but provides semantic clarity
  // Throws std::length_error if container is empty
  virtual const Data & Front() const;

  // Back() - Access the last element in the container (read-only)
  // Equivalent to operator[](size-1) but provides semantic clarity
  // Throws std::length_error if container is empty
  virtual const Data & Back() const;

  /* ************************************************************************ */

  // Specific member function (inherited from TraversableContainer)

  // Import TraverseFun type from base class for type consistency
  using typename TraversableContainer<Data>::TraverseFun;

  // Traverse() - Process each element in the container using the provided function
  // For linear containers, this typically means front-to-back traversal
  void Traverse(TraverseFun) const override;

  /* ************************************************************************ */

  // Specific member function (inherited from PreOrderTraversableContainer)

  // PreOrderTraverse() - Process each element in pre-order (front to back for linear containers)
  // For linear structures, pre-order is the natural left-to-right order
  void PreOrderTraverse(TraverseFun) const override;

  /* ************************************************************************ */

  // Specific member function (inherited from PostOrderTraversableContainer)

  // PostOrderTraverse() - Process each element in post-order (back to front for linear containers)
  // For linear structures, post-order is the reverse order (right-to-left)
  void PostOrderTraverse(TraverseFun) const override;

};

/* ************************************************************************** */

// MutableLinearContainer Class
// ---------------------------
// Extends LinearContainer with the ability to modify elements in place
// Adds mapping capabilities (applying functions that can change element values)
// This is the foundation for mutable linear data structures
template <typename Data>
class MutableLinearContainer : virtual public LinearContainer<Data>,
  virtual public PreOrderMappableContainer<Data>,
  virtual public PostOrderMappableContainer<Data> {

private:

protected:

public:

  // Virtual destructor for proper cleanup in derived classes
  virtual ~MutableLinearContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  MutableLinearContainer & operator=(const MutableLinearContainer &) = delete;

  // Move assignment - deleted for abstract class
  MutableLinearContainer & operator=(MutableLinearContainer &&) noexcept = delete;

  /* ************************************************************************ */

  // Specific member functions for mutable access

  // Make the const versions from the base class visible in this class
  // This ensures both const and non-const versions are available
  using LinearContainer<Data>::operator[];
  using LinearContainer<Data>::Front;
  using LinearContainer<Data>::Back;

  // operator[]() - Access and modify element at specified position
  // Throws std::out_of_range if position is invalid (>= size)
  // This allows both reading and writing: container[i] = value;
  virtual Data & operator[](ulong) = 0;

  // Front() - Access and modify the first element in the container
  // Throws std::length_error if container is empty
  // Allows modification: container.Front() = value;
  virtual Data & Front() = 0;

  // Back() - Access and modify the last element in the container
  // Throws std::length_error if container is empty
  // Allows modification: container.Back() = value;
  virtual Data & Back() = 0;

  /* ************************************************************************ */

  // Specific member function (inherited from MappableContainer)

  // Import MapFun type from base class for function application
  using typename MappableContainer<Data>::MapFun;

  // Map() - Apply a function to modify each element in the container
  // Uses pre-order traversal by default (front-to-back)
  void Map(MapFun) override;

  /* ************************************************************************ */

  // Specific member function (inherited from PreOrderMappableContainer)

  // PreOrderMap() - Apply a function to modify each element in pre-order (front to back)
  // For linear containers, this is the natural order of modification
  void PreOrderMap(MapFun) override;

  /* ************************************************************************ */

  // Specific member function (inherited from PostOrderMappableContainer)

  // PostOrderMap() - Apply a function to modify each element in post-order (back to front)
  // For linear containers, this modifies elements in reverse order
  void PostOrderMap(MapFun) override;

};

// SortableLinearContainer Class
// ----------------------------
// Extends MutableLinearContainer with sorting capability
// Provides functionality to arrange elements in ascending order using comparison operators
// Uses QuickSort algorithm as the default sorting implementation
template <typename Data>
class SortableLinearContainer : virtual public MutableLinearContainer<Data> {

private:

protected:

public:

  // Virtual destructor for proper cleanup in derived classes
  virtual ~SortableLinearContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  SortableLinearContainer & operator=(const SortableLinearContainer &) = delete;

  // Move assignment - deleted for abstract class
  SortableLinearContainer & operator=(SortableLinearContainer &&) noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  // Sort() - Sorts the elements in the container in ascending order
  // Uses the < operator for comparison, so Data type must support comparison
  // Default implementation uses QuickSort algorithm for O(n log n) average performance
  virtual void Sort();

protected:

  // Auxiliary member functions for sorting implementation

  // QuickSort() - Recursive QuickSort algorithm implementation
  // Sorts elements between indices left and right (inclusive)
  // Parameters: left index, right index
  void QuickSort(ulong, ulong);
  
  // Partition() - Helper function for QuickSort that partitions the array
  // Rearranges elements so that elements smaller than pivot are on the left
  // Returns the final position of the pivot element
  // Parameters: left index, right index
  ulong Partition(ulong, ulong);
  
  // SwapAt() - Swaps elements at the specified positions
  // This is an abstract method that must be implemented by concrete derived classes
  // The implementation depends on how the specific container stores its elements
  // Parameters: position1, position2, reference to element1, reference to element2
  virtual void SwapAt(ulong, ulong, const Data&, const Data&) = 0;

};

/* ************************************************************************** */

}

#include "linear.cpp"

#endif
