#ifndef CONTAINER_HPP
#define CONTAINER_HPP

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

// Container Class
// --------------
// The Container class serves as the base abstract class for all data structures in this library.
// It defines the basic properties and behaviors that all containers must implement.
// This is the root of the container hierarchy from which all other data structures inherit.
class Container {

private:

protected:
  // Tracks the number of elements currently stored in the container
  ulong size = 0;

  /* ************************************************************************ */

  // Default constructor - creates an empty container with size 0
  // Protected to prevent direct instantiation of this abstract class
  Container() = default;

public:

  // Virtual destructor ensures proper cleanup of derived classes
  virtual ~Container() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying of containers
  Container& operator=(const Container&) = delete;

  // Move assignment - deleted to prevent moving of abstract container
  Container& operator=(Container&&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const Container&) const noexcept = delete;
  bool operator!=(const Container&) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member functions

  // Empty() - Checks if the container is empty (contains no elements)
  // Returns: true if the container has no elements (size == 0), false otherwise
  inline virtual bool Empty() const noexcept {
    return (size == 0);
  }

  // Size() - Returns the current number of elements in the container
  // Returns: The count of elements currently stored in the container
  inline virtual ulong Size() const noexcept {
    return size;
  }

};

/* ************************************************************************** */

// ClearableContainer Class
// -----------------------
// ClearableContainer extends Container with the ability to remove all elements
// This abstract class introduces the Clear operation which empties the container
// Any container that can be emptied should inherit from this class
class ClearableContainer : virtual public Container {
  // Using virtual inheritance to avoid the "diamond problem" with multiple inheritance

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~ClearableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  ClearableContainer& operator=(const ClearableContainer&) = delete;

  // Move assignment - deleted as abstract class
  ClearableContainer& operator=(ClearableContainer&&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be directly compared
  bool operator==(const ClearableContainer&) const noexcept = delete;
  bool operator!=(const ClearableContainer&) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member functions

  // Clear() - Pure virtual function that removes all elements from the container
  // Must be implemented by all derived classes
  // After calling Clear(), the container should be empty and Size() should return 0
  virtual void Clear() = 0;

};

/* ************************************************************************** */

// ResizableContainer Class
// -----------------------
// ResizableContainer extends ClearableContainer with the ability to change its size
// This abstract class defines containers that can grow or shrink on demand
// Examples include dynamic arrays and vectors
class ResizableContainer : virtual public ClearableContainer {
  // Using virtual inheritance to avoid the "diamond problem" with multiple inheritance

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~ResizableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  ResizableContainer& operator=(const ResizableContainer&) = delete;

  // Move assignment - deleted as abstract class
  ResizableContainer& operator=(ResizableContainer&&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be directly compared
  bool operator==(const ResizableContainer&) const noexcept = delete;
  bool operator!=(const ResizableContainer&) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member functions

  // Resize() - Changes the container's size to the specified new size
  // Must be implemented by all derived classes
  // Parameter: The new desired size for the container
  // If the new size is smaller, some elements may be lost
  // If the new size is larger, new default-constructed elements may be added
  virtual void Resize(ulong) = 0;

  /* ************************************************************************ */

  // Clear() - Implementation of the Clear method from ClearableContainer
  // Clears the container by resizing it to 0 elements
  // This provides a default implementation for derived classes
  inline void Clear() override {
    Resize(0);
  }

};

/* ************************************************************************** */

}

#endif
