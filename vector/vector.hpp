/*
 * Vector Implementation
 * 
 * This file defines Vector and SortableVector classes that provide dynamic array
 * functionality with contiguous memory layout. The Vector class implements a
 * resizable array with random access, while SortableVector extends it with
 * sorting capabilities.
 * 
 * Key Features:
 * - Contiguous memory storage for cache efficiency
 * - O(1) random access via index operator
 * - Dynamic resizing capabilities
 * - Integration with the container hierarchy
 * - Sorting functionality in SortableVector
 */

#ifndef VECTOR_HPP
#define VECTOR_HPP

/* ************************************************************************** */

#include "../container/linear.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

/*
 * Vector Class
 * 
 * A dynamic array implementation that provides:
 * - Contiguous memory storage for elements
 * - O(1) access time for elements by index
 * - Dynamic resizing with automatic memory management
 * - Integration with MutableLinearContainer and ResizableContainer interfaces
 * 
 * The Vector maintains an array of elements in contiguous memory, allowing
 * for efficient random access and cache-friendly memory usage patterns.
 * It automatically manages memory allocation and deallocation.
 */
template <typename Data>
class Vector : virtual public MutableLinearContainer<Data>,
               virtual public ResizableContainer {

private:

protected:

  using Container::size; // Inherit size from base Container class

  Data* Elements = nullptr; // Pointer to dynamically allocated array of elements

public:

  // Default constructor: Creates an empty vector with no allocated memory
  Vector() = default;

  /* ************************************************************************ */

  // Specific constructors for different initialization scenarios
  
  Vector(const ulong); // Creates a vector with specified initial size (elements default-constructed)
  Vector(const TraversableContainer<Data>&); // Creates vector by copying elements from any traversable container
  Vector(MappableContainer<Data>&&); // Creates vector by moving elements from any mappable container

  /* ************************************************************************ */

  // Copy constructor: Creates a deep copy of another vector
  Vector(const Vector&);

  // Move constructor: Transfers ownership of resources from another vector
  Vector(Vector&&) noexcept;

  /* ************************************************************************ */

  // Destructor: Automatically frees allocated memory and destroys elements
  virtual ~Vector();

  /* ************************************************************************ */

  // Assignment operators for copying and moving vector contents
  
  Vector& operator=(const Vector&); // Copy assignment: Deep copies another vector
  Vector& operator=(Vector&&) noexcept; // Move assignment: Transfers ownership efficiently

  /* ************************************************************************ */

  // Comparison operators for structural equality testing
  bool operator==(const Vector&) const noexcept; // Returns true if vectors have same size and elements
  bool operator!=(const Vector&) const noexcept; // Returns true if vectors differ in size or elements

  /* ************************************************************************ */

  // Specific member functions (inherited from MutableLinearContainer)
  // These provide mutable access to vector elements with bounds checking

  Data& operator[](ulong) override; // Mutable access to element at index (throws out_of_range if invalid)
  Data& Front() override; // Mutable access to first element (throws length_error if empty)
  Data& Back() override; // Mutable access to last element (throws length_error if empty)

  /* ************************************************************************ */

  // Specific member functions (inherited from LinearContainer)
  // These provide immutable (const) access to vector elements with bounds checking

  const Data& operator[](ulong) const override; // Const access to element at index (throws out_of_range if invalid)
  const Data& Front() const override; // Const access to first element (throws length_error if empty)
  const Data& Back() const override; // Const access to last element (throws length_error if empty)

  /* ************************************************************************ */

  // Specific member function (inherited from ResizableContainer)
  // Allows dynamic resizing of the vector with automatic memory management

  void Resize(ulong) override; // Changes vector size, preserving existing elements when possible

  /* ************************************************************************ */

  // Specific member function (inherited from ClearableContainer)
  // Clear() functionality is inherited from ResizableContainer (resize to 0)

protected:

  // Auxiliary functions could be added here for internal operations

};

/* ************************************************************************** */

/*
 * SortableVector Class
 * 
 * An extension of Vector that adds sorting capabilities through inheritance
 * from SortableLinearContainer. This class combines the efficient random
 * access of Vector with powerful sorting algorithms.
 * 
 * Key Features:
 * - All Vector functionality (random access, resizing, etc.)
 * - Built-in QuickSort implementation from SortableLinearContainer
 * - Efficient element swapping for sorting operations
 * - Maintains contiguous memory layout during sorting
 * 
 * The sorting functionality uses the QuickSort algorithm implemented in the
 * parent SortableLinearContainer class, with optimized swapping operations
 * that take advantage of the vector's contiguous memory structure.
 */
template <typename Data>
class SortableVector : public Vector<Data>,
                       public SortableLinearContainer<Data> {

private:

protected:

public:

  // Default constructor: Creates an empty sortable vector
  SortableVector();

  /* ************************************************************************ */

  // Specific constructors for different initialization scenarios
  
  SortableVector(const ulong); // Creates a sortable vector with specified initial size
  SortableVector(const TraversableContainer<Data>&); // Creates sortable vector from any traversable container
  SortableVector(MappableContainer<Data>&&); // Creates sortable vector by moving from any mappable container

  /* ************************************************************************ */

  // Copy constructor: Creates a deep copy of another sortable vector
  SortableVector(const SortableVector&);

  // Move constructor: Transfers ownership efficiently from another sortable vector
  SortableVector(SortableVector&&) noexcept;

  /* ************************************************************************ */

  // Destructor: Uses default behavior (Vector's destructor handles cleanup)
  virtual ~SortableVector() = default;

  /* ************************************************************************ */

  // Assignment operators for copying and moving sortable vector contents
  
  SortableVector& operator=(const SortableVector&); // Copy assignment with deep copying
  SortableVector& operator=(SortableVector&&) noexcept; // Move assignment with resource transfer

  /* ************************************************************************ */

  // Note: Sort functionality is inherited from SortableLinearContainer
  // The Sort() method uses QuickSort algorithm for efficient O(n log n) average-case sorting

protected:

  // Implementation of virtual method from SortableLinearContainer
  // This method performs the actual element swapping during sorting operations
  virtual void SwapAt(ulong, ulong, const Data&, const Data&) override;

  // Additional auxiliary functions could be added here for sorting optimizations

};

/* ************************************************************************** */

}

#include "vector.cpp"

#endif
