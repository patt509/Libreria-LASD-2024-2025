#ifndef TESTABLE_HPP
#define TESTABLE_HPP

/* ************************************************************************** */

#include "container.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

// TestableContainer Class
// ---------------------
// A container interface for types that can be queried to check if specific elements exist
// This serves as the foundation for any container that needs to support membership tests
// The Exists() method allows checking whether a specific value is present in the container
template <typename Data>
class TestableContainer : virtual public Container {

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~TestableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  TestableContainer& operator=(const TestableContainer&) = delete;

  // Move assignment - deleted as abstract class
  TestableContainer& operator=(TestableContainer&&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const TestableContainer&) const noexcept = delete;
  bool operator!=(const TestableContainer&) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  // Exists() - Checks if a specified value exists in the container
  // Parameter: The data value to search for
  // Returns: true if the value exists in the container, false otherwise
  // This is a pure virtual function that must be implemented by derived classes
  virtual bool Exists(const Data&) const noexcept = 0;

};

/* ************************************************************************** */

}

#endif
