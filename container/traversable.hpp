#ifndef TRAVERSABLE_HPP
#define TRAVERSABLE_HPP

/* ************************************************************************** */

#include <functional>

/* ************************************************************************** */

#include "testable.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

// TraversableContainer Class
// -------------------------
// A container interface that allows traversing all elements with a given function
// This is the base class for containers that support element-by-element processing
// It provides both traversal and folding (accumulation) operations
template <typename Data>
class TraversableContainer : virtual public TestableContainer<Data> {

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~TraversableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  TraversableContainer& operator=(const TraversableContainer&) = delete;

  // Move assignment - deleted as abstract class
  TraversableContainer& operator=(TraversableContainer&&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const TraversableContainer&) const noexcept = delete;
  bool operator!=(const TraversableContainer&) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  // Function type for traversing elements (read-only access)
  using TraverseFun = std::function<void(const Data &)>;

  // Traverse() - Apply a function to each element in the container
  // This is a pure virtual function that must be implemented by derived classes
  // Parameter: A function that will be called for each element
  virtual void Traverse(TraverseFun) const = 0;

  // Function type for folding operations (accumulating values)
  template <typename Accumulator>
  using FoldFun = std::function<Accumulator(const Data &, const Accumulator &)>;

  // Fold() - Apply a binary function to accumulate a single value from all elements
  // This performs a reduction operation over the container
  // Parameters: Binary function and initial accumulator value
  // Returns: The final accumulated value
  template <typename Accumulator>
  Accumulator Fold(FoldFun<Accumulator>, Accumulator) const;

  /* ************************************************************************ */

  // Specific member function (inherited from TestableContainer)

  // Exists() - Implementation using traversal to check if an element exists
  // This provides a default implementation by traversing and comparing elements
  bool Exists(const Data &) const noexcept override;

};

/* ************************************************************************** */

// PreOrderTraversableContainer Class
// ---------------------------------
// A container that supports pre-order traversal (root/node first, then children)
// For linear structures, this typically means front-to-back traversal
// For tree structures, this visits the node before its children
template <typename Data>
class PreOrderTraversableContainer : virtual public TraversableContainer<Data> {

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~PreOrderTraversableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  PreOrderTraversableContainer& operator=(const PreOrderTraversableContainer&) = delete;

  // Move assignment - deleted as abstract class
  PreOrderTraversableContainer& operator=(PreOrderTraversableContainer&&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const PreOrderTraversableContainer&) const noexcept = delete;
  bool operator!=(const PreOrderTraversableContainer&) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  // Import the TraverseFun type from the base class
  using typename TraversableContainer<Data>::TraverseFun;

  // PreOrderTraverse() - Traverse elements in pre-order
  // Must be implemented by derived classes to define the specific pre-order behavior
  virtual void PreOrderTraverse(TraverseFun) const = 0;

  // Import the FoldFun type for pre-order folding operations
  template <typename Accumulator>
  using FoldFun = typename TraversableContainer<Data>::FoldFun<Accumulator>;

  // PreOrderFold() - Perform folding operation in pre-order
  // Accumulates values by visiting elements in pre-order
  template <typename Accumulator>
  Accumulator PreOrderFold(FoldFun<Accumulator>, Accumulator) const;

  /* ************************************************************************ */

  // Specific member function (inherited from TraversableContainer)

  // Traverse() - Default implementation uses pre-order traversal
  void Traverse(TraverseFun) const override;

};

/* ************************************************************************** */

// PostOrderTraversableContainer Class
// ----------------------------------
// A container that supports post-order traversal (children first, then root/node)
// For linear structures, this typically means back-to-front traversal
// For tree structures, this visits all children before the node itself
template <typename Data>
class PostOrderTraversableContainer : virtual public TraversableContainer<Data> {

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~PostOrderTraversableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  PostOrderTraversableContainer& operator=(const PostOrderTraversableContainer&) = delete;

  // Move assignment - deleted as abstract class
  PostOrderTraversableContainer& operator=(PostOrderTraversableContainer&&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const PostOrderTraversableContainer&) const noexcept = delete;
  bool operator!=(const PostOrderTraversableContainer&) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  // Import the TraverseFun type from the base class
  using typename TraversableContainer<Data>::TraverseFun;

  // PostOrderTraverse() - Traverse elements in post-order
  // Must be implemented by derived classes to define the specific post-order behavior
  virtual void PostOrderTraverse(TraverseFun) const = 0;

  // Import the FoldFun type for post-order folding operations
  template <typename Accumulator>
  using FoldFun = typename TraversableContainer<Data>::FoldFun<Accumulator>;

  // PostOrderFold() - Perform folding operation in post-order
  // Accumulates values by visiting elements in post-order
  template <typename Accumulator>
  Accumulator PostOrderFold(FoldFun<Accumulator>, Accumulator) const;

  /* ************************************************************************ */

  // Specific member function (inherited from TraversableContainer)

  // Traverse() - Default implementation uses post-order traversal
  void Traverse(TraverseFun) const override;

};

/* ************************************************************************** */

// InOrderTraversableContainer Class
// --------------------------------
// A container that supports in-order traversal (left subtree, node, right subtree)
// This is primarily used for binary search trees where in-order gives sorted output
// For other structures, the exact behavior depends on the implementation
template <typename Data>
class InOrderTraversableContainer : virtual public TraversableContainer<Data> {

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~InOrderTraversableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  InOrderTraversableContainer& operator=(const InOrderTraversableContainer&) = delete;

  // Move assignment - deleted as abstract class
  InOrderTraversableContainer& operator=(InOrderTraversableContainer&&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const InOrderTraversableContainer&) const noexcept = delete;
  bool operator!=(const InOrderTraversableContainer&) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  // Import the TraverseFun type from the base class
  using typename TraversableContainer<Data>::TraverseFun;

  // InOrderTraverse() - Traverse elements in in-order
  // Must be implemented by derived classes to define the specific in-order behavior
  virtual void InOrderTraverse(TraverseFun) const = 0;

  // Import the FoldFun type for in-order folding operations
  template <typename Accumulator>
  using FoldFun = typename TraversableContainer<Data>::FoldFun<Accumulator>;

  // InOrderFold() - Perform folding operation in in-order
  // Accumulates values by visiting elements in in-order
  template <typename Accumulator>
  Accumulator InOrderFold(FoldFun<Accumulator>, Accumulator) const;

  /* ************************************************************************ */

  // Specific member function (inherited from TraversableContainer)

  // Traverse() - Default implementation uses in-order traversal
  void Traverse(TraverseFun) const override;

};

/* ************************************************************************** */

// BreadthTraversableContainer Class
// --------------------------------
// A container that supports breadth-first traversal (level by level)
// This visits all elements at depth n before visiting any elements at depth n+1
// Commonly used for tree and graph structures
template <typename Data>
class BreadthTraversableContainer : virtual public TraversableContainer<Data> {

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~BreadthTraversableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  BreadthTraversableContainer& operator=(const BreadthTraversableContainer&) = delete;

  // Move assignment - deleted as abstract class
  BreadthTraversableContainer& operator=(BreadthTraversableContainer&&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const BreadthTraversableContainer&) const noexcept = delete;
  bool operator!=(const BreadthTraversableContainer&) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  // Import the TraverseFun type from the base class
  using typename TraversableContainer<Data>::TraverseFun;

  // BreadthTraverse() - Traverse elements in breadth-first order
  // Must be implemented by derived classes to define the specific breadth-first behavior
  virtual void BreadthTraverse(TraverseFun) const = 0;

  // Import the FoldFun type for breadth-first folding operations
  template <typename Accumulator>
  using FoldFun = typename TraversableContainer<Data>::FoldFun<Accumulator>;

  // BreadthFold() - Perform folding operation in breadth-first order
  // Accumulates values by visiting elements level by level
  template <typename Accumulator>
  Accumulator BreadthFold(FoldFun<Accumulator>, Accumulator) const;

  /* ************************************************************************ */

  // Specific member function (inherited from TraversableContainer)

  // Traverse() - Default implementation uses breadth-first traversal
  void Traverse(TraverseFun) const override;

};

/* ************************************************************************** */

}

#include "traversable.cpp"

#endif
