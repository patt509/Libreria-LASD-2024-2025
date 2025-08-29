#ifndef LIST_HPP
#define LIST_HPP

/* ************************************************************************** */

#include "../container/linear.hpp"
#include "../container/dictionary.hpp"

/* ************************************************************************** */

namespace lasd {

/* ************************************************************************** */

// List Class
// ----------
// A doubly-ended singly-linked list implementation that combines linear container functionality
// with dictionary operations. This provides efficient insertion/removal at both ends
// and supports all standard linear container operations like indexed access and traversal.
// The list maintains both head and tail pointers for O(1) operations at both ends.
template <typename Data>
class List : virtual public ClearableContainer, 
             virtual public MutableLinearContainer<Data>,
             virtual public DictionaryContainer<Data> {

private:

protected:

  // Inherit size tracking from the base Container class
  using Container::size;

  // Node Structure
  // --------------
  // Internal node structure for the singly-linked list
  // Each node contains data and a pointer to the next node in the sequence
  struct Node {

    Data element;        // The actual data stored in this node
    Node* next = nullptr; // Pointer to the next node in the list (null for tail)

    /* ********************************************************************** */

    // Node Constructors

    // Default constructor - creates a node with default-constructed element
    inline Node() = default;

    // Copy constructor for element - creates node with copied data
    inline Node(const Data& dat) : element(dat) {}

    // Move constructor for element - creates node with moved data (more efficient)
    inline Node(Data&& dat) noexcept : element(std::move(dat)) {}

    /* ********************************************************************** */

    // Node Copy and Move Constructors

    // Copy constructor - creates a copy of another node (doesn't copy the link)
    inline Node(const Node& other) : element(other.element) {}

    // Move constructor - moves data from another node and steals its next pointer
    inline Node(Node&& other) noexcept : element(std::move(other.element)), next(other.next) {
      other.next = nullptr; // Leave the moved-from node in a valid state
    }

    /* ********************************************************************** */

    // Node Destructor
    virtual ~Node() = default;

    /* ********************************************************************** */

    // Node Comparison Operators

    // Equality comparison - compares only the element data, not the structure
    bool operator==(const Node& other) const noexcept {
      return element == other.element;
    }
    
    // Inequality comparison - logical negation of equality
    inline bool operator!=(const Node& other) const noexcept {
      return !(*this == other);
    }

    /* ********************************************************************** */

    // Node Specific Functions

    // Clone() - Creates a deep copy of this node and potentially links it to another
    // Used for implementing list copy operations
    virtual Node* Clone(Node* next = nullptr);

  };

  Node* head = nullptr; // Pointer to the first node in the list (null when empty)
  Node* tail = nullptr; // Pointer to the last node in the list (null when empty)

public:

  // List Constructors

  // Default constructor - creates an empty list
  List() = default;

  /* ************************************************************************ */

  // Specific constructors

  // Constructor from TraversableContainer - copies all elements from another container
  List(const TraversableContainer<Data>&);

  // Constructor from MappableContainer - moves all elements from another container
  List(MappableContainer<Data>&&);

  /* ************************************************************************ */

  // Copy constructor - creates a deep copy of another list
  List(const List&);

  // Move constructor - transfers ownership from another list (efficient)
  List(List&&) noexcept;

  /* ************************************************************************ */

  // Destructor - properly deallocates all nodes in the list
  virtual ~List();

  /* ************************************************************************ */

  // Assignment Operators

  // Copy assignment - replaces current content with a copy of another list
  List& operator=(const List&);

  // Move assignment - replaces current content by transferring from another list
  List& operator=(List&&) noexcept;

  /* ************************************************************************ */

  // Comparison operators

  // Equality comparison - checks if two lists have the same elements in the same order
  bool operator==(const List&) const noexcept;

  // Inequality comparison - logical negation of equality
  bool operator!=(const List&) const noexcept;

  /* ************************************************************************ */

  // Specific member functions for list operations

  // Front insertion operations
  // InsertAtFront() - Add a new element at the beginning of the list (O(1))
  void InsertAtFront(const Data&); // Copy version
  void InsertAtFront(Data&&);      // Move version (more efficient)

  // RemoveFromFront() - Remove the first element from the list (O(1))
  // Throws std::length_error when called on an empty list
  void RemoveFromFront();

  // FrontNRemove() - Get and remove the first element in one operation (O(1))
  // Returns the removed element; throws std::length_error when list is empty
  Data FrontNRemove();

  // Back insertion operations
  // InsertAtBack() - Add a new element at the end of the list (O(1))
  void InsertAtBack(const Data&); // Copy version
  void InsertAtBack(Data&&);      // Move version (more efficient)

  // RemoveFromBack() - Remove the last element from the list (O(n))
  // Note: This is O(n) due to singly-linked structure requiring traversal to update tail
  // Throws std::length_error when called on an empty list
  void RemoveFromBack();

  // BackNRemove() - Get and remove the last element in one operation (O(n))
  // Returns the removed element; throws std::length_error when list is empty
  Data BackNRemove();

  /* ************************************************************************ */

  // Specific member functions (inherited from MutableLinearContainer)

  // operator[]() - Access element at specified index with modification capability
  // Throws std::out_of_range if index >= size; O(n) operation due to traversal
  Data& operator[](ulong) override;

  // Front() - Access the first element for modification (O(1))
  // Throws std::length_error when list is empty
  Data& Front() override;

  // Back() - Access the last element for modification (O(1))
  // Throws std::length_error when list is empty
  Data& Back() override;

  /* ************************************************************************ */

  // Specific member functions (inherited from LinearContainer)

  // operator[]() - Access element at specified index (read-only) (O(n))
  // Throws std::out_of_range if index >= size
  const Data& operator[](ulong) const override;

  // Front() - Access the first element (read-only) (O(1))
  // Throws std::length_error when list is empty
  const Data& Front() const override;

  // Back() - Access the last element (read-only) (O(1))
  // Throws std::length_error when list is empty
  const Data& Back() const override;

  /* ************************************************************************ */

  // Specific member function (inherited from MappableContainer)

  // Import the MapFun type for consistency
  using typename MappableContainer<Data>::MapFun;

  // Map() - Apply a function to modify each element in the list
  // Uses pre-order traversal (front to back)
  void Map(MapFun) override;

  /* ************************************************************************ */

  // Specific member function (inherited from PreOrderMappableContainer)

  // PreOrderMap() - Apply a function to modify elements from front to back
  void PreOrderMap(MapFun) override;

  /* ************************************************************************ */

  // Specific member function (inherited from PostOrderMappableContainer)

  // PostOrderMap() - Apply a function to modify elements from back to front
  void PostOrderMap(MapFun) override;

  /* ************************************************************************ */

  // Specific member function (inherited from TraversableContainer)

  // Import the TraverseFun type for consistency
  using typename TraversableContainer<Data>::TraverseFun;

  // Traverse() - Process each element in the list with a function (read-only)
  // Uses pre-order traversal (front to back)
  void Traverse(TraverseFun) const override;

  /* ************************************************************************ */

  // Specific member function (inherited from PreOrderTraversableContainer)

  // PreOrderTraverse() - Process elements from front to back (read-only)
  void PreOrderTraverse(TraverseFun) const override;

  /* ************************************************************************ */

  // Specific member function (inherited from PostOrderTraversableContainer)

  // PostOrderTraverse() - Process elements from back to front (read-only)
  void PostOrderTraverse(TraverseFun) const override;
  
  /* ************************************************************************ */

  // Specific member function (inherited from ClearableContainer)

  // Clear() - Remove all elements from the list, making it empty
  // After this operation, size becomes 0 and both head and tail become nullptr
  void Clear() override;

  /* ************************************************************************ */

  // Specific member functions (inherited from DictionaryContainer)

  // Insert() - Add an element to the list if it doesn't already exist
  // Returns true if element was inserted, false if it already existed
  // Uses InsertAtBack for actual insertion
  bool Insert(const Data&) override; // Copy version
  bool Insert(Data&&) override;      // Move version

  // Remove() - Remove the first occurrence of a specific element
  // Returns true if element was found and removed, false if not found
  bool Remove(const Data&) override;

  // Bulk operations inherited from DictionaryContainer
  // These use the base class implementations that delegate to Insert/Remove

  // InsertAll() - Attempt to insert all elements from another container
  bool InsertAll(const TraversableContainer<Data>&) override;
  bool InsertAll(MappableContainer<Data>&&) override;

  // RemoveAll() - Attempt to remove all specified elements
  bool RemoveAll(const TraversableContainer<Data>&) override;

  // InsertSome() - Insert elements, succeeding if at least one is inserted
  bool InsertSome(const TraversableContainer<Data>&) override;
  bool InsertSome(MappableContainer<Data>&&) override;

  // RemoveSome() - Remove elements, succeeding if at least one is removed
  bool RemoveSome(const TraversableContainer<Data>&) override;

protected:

  // Auxiliary member functions for recursive traversal operations
  
  // PreOrderTraverse() - Recursive helper for front-to-back traversal
  void PreOrderTraverse(TraverseFun, const Node*) const;

  // PostOrderTraverse() - Recursive helper for back-to-front traversal
  void PostOrderTraverse(TraverseFun, const Node*) const;

  // Auxiliary member functions for recursive mapping operations
  
  // PreOrderMap() - Recursive helper for front-to-back mapping
  void PreOrderMap(MapFun, Node*);

  // PostOrderMap() - Recursive helper for back-to-front mapping
  void PostOrderMap(MapFun, Node*);

};

/* ************************************************************************** */

}

#include "list.cpp"

#endif
