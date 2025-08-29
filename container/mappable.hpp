#ifndef MAPPABLE_HPP
#define MAPPABLE_HPP

/* ************************************************************************** */

#include <functional>

/* ************************************************************************** */

#include "traversable.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

// MappableContainer Class
// ----------------------
// A container interface that allows applying a function to modify each element
// Unlike TraversableContainer which only reads elements, this allows mutations
// It extends TraversableContainer with the ability to change element values
template <typename Data>
class MappableContainer : virtual public TraversableContainer<Data> {

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~MappableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  MappableContainer & operator=(const MappableContainer &) = delete;

  // Move assignment - deleted for abstract class
  MappableContainer & operator=(MappableContainer &&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const MappableContainer &) const noexcept = delete;
  bool operator!=(const MappableContainer &) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  // Function type for mapping operations (allows modification of elements)
  using MapFun = std::function<void(Data &)>;

  // Map() - Apply a function to modify each element in the container
  // This is a pure virtual function that must be implemented by derived classes
  // Parameter: A function that will be called to potentially modify each element
  virtual void Map(MapFun) = 0;

};

/* ************************************************************************** */

// PreOrderMappableContainer Class
// ------------------------------
// A container that supports applying functions to elements in pre-order
// Combines the traversal pattern of PreOrderTraversableContainer with mapping capabilities
// For linear structures: front-to-back modification; For trees: node before children
template <typename Data>
class PreOrderMappableContainer : virtual public MappableContainer<Data>,
  virtual public PreOrderTraversableContainer<Data> {

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~PreOrderMappableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  PreOrderMappableContainer & operator=(const PreOrderMappableContainer &) = delete;

  // Move assignment - deleted for abstract class
  PreOrderMappableContainer & operator=(PreOrderMappableContainer &&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const PreOrderMappableContainer &) const noexcept = delete;
  bool operator!=(const PreOrderMappableContainer &) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  // Import the MapFun type from the base class
  using typename MappableContainer<Data>::MapFun;

  // PreOrderMap() - Apply a function to modify elements in pre-order
  // Must be implemented by derived classes to define specific pre-order mapping behavior
  virtual void PreOrderMap(MapFun) = 0;

  /* ************************************************************************ */

  // Specific member function (inherited from MappableContainer)

  // Map() - Default implementation uses pre-order mapping
  inline void Map(MapFun) override;

};

/* ************************************************************************** */

// PostOrderMappableContainer Class
// -------------------------------
// A container that supports applying functions to elements in post-order
// Combines the traversal pattern of PostOrderTraversableContainer with mapping capabilities
// For linear structures: back-to-front modification; For trees: children before node
template <typename Data>
class PostOrderMappableContainer : virtual public MappableContainer<Data>,
  virtual public PostOrderTraversableContainer<Data> {

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~PostOrderMappableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  PostOrderMappableContainer & operator=(const PostOrderMappableContainer &) = delete;

  // Move assignment - deleted for abstract class
  PostOrderMappableContainer & operator=(PostOrderMappableContainer &&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const PostOrderMappableContainer &) const noexcept = delete;
  bool operator!=(const PostOrderMappableContainer &) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  // Import the MapFun type from the base class
  using typename MappableContainer<Data>::MapFun;

  // PostOrderMap() - Apply a function to modify elements in post-order
  // Must be implemented by derived classes to define specific post-order mapping behavior
  virtual void PostOrderMap(MapFun) = 0;

  /* ************************************************************************ */

  // Specific member function (inherited from MappableContainer)

  // Map() - Default implementation uses post-order mapping
  inline void Map(MapFun) override;

};

/* ************************************************************************** */

// InOrderMappableContainer Class
// -----------------------------
// A container that supports applying functions to elements in in-order
// Combines the traversal pattern of InOrderTraversableContainer with mapping capabilities
// Primarily used for binary search trees where in-order maintains sorted property
template <typename Data>
class InOrderMappableContainer : virtual public MappableContainer<Data>,
  virtual public InOrderTraversableContainer<Data> {

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~InOrderMappableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  InOrderMappableContainer & operator=(const InOrderMappableContainer &) = delete;

  // Move assignment - deleted for abstract class
  InOrderMappableContainer & operator=(InOrderMappableContainer &&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const InOrderMappableContainer &) const noexcept = delete;
  bool operator!=(const InOrderMappableContainer &) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  // Import the MapFun type from the base class
  using typename MappableContainer<Data>::MapFun;

  // InOrderMap() - Apply a function to modify elements in in-order
  // Must be implemented by derived classes to define specific in-order mapping behavior
  virtual void InOrderMap(MapFun) = 0;

  /* ************************************************************************ */

  // Specific member function (inherited from MappableContainer)

  // Map() - Default implementation uses in-order mapping
  inline void Map(MapFun) override;

};

/* ************************************************************************** */

// BreadthMappableContainer Class
// -----------------------------
// A container that supports applying functions to elements in breadth-first order
// Combines the traversal pattern of BreadthTraversableContainer with mapping capabilities
// Modifies elements level by level from top to bottom
template <typename Data>
class BreadthMappableContainer : virtual public MappableContainer<Data>,
  virtual public BreadthTraversableContainer<Data> {

private:

protected:

public:

  // Virtual destructor to ensure proper cleanup in derived classes
  virtual ~BreadthMappableContainer() = default;

  /* ************************************************************************ */

  // Copy assignment - deleted to prevent shallow copying
  BreadthMappableContainer & operator=(const BreadthMappableContainer &) = delete;

  // Move assignment - deleted for abstract class
  BreadthMappableContainer & operator=(BreadthMappableContainer &&) noexcept = delete;

  /* ************************************************************************ */

  // Comparison operators - deleted as different container types shouldn't be compared directly
  bool operator==(const BreadthMappableContainer &) const noexcept = delete;
  bool operator!=(const BreadthMappableContainer &) const noexcept = delete;

  /* ************************************************************************ */

  // Specific member function

  // Import the MapFun type from the base class
  using typename MappableContainer<Data>::MapFun;

  // BreadthMap() - Apply a function to modify elements in breadth-first order
  // Must be implemented by derived classes to define specific breadth-first mapping behavior
  virtual void BreadthMap(MapFun) = 0;

  /* ************************************************************************ */

  // Specific member function (inherited from MappableContainer)

  // Map() - Default implementation uses breadth-first mapping
  inline void Map(MapFun) override;

};

/* ************************************************************************** */

}

#include "mappable.cpp"

#endif
