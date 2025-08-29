namespace lasd {

/* ************************************************************************** */
// PreOrderMappableContainer Implementation
/* ************************************************************************** */

// Map() - Implementation of the generic Map function using pre-order traversal
// This provides a default implementation that delegates to PreOrderMap
// Allows containers to support both the generic Map interface and specific pre-order mapping
template <typename Data>
inline void PreOrderMappableContainer<Data>::Map(MapFun fun) {
  // Implement the Map function in terms of PreOrderMap
  // This ensures that when Map() is called, it uses pre-order traversal
  PreOrderMap(fun);
}

/* ************************************************************************** */
// PostOrderMappableContainer Implementation
/* ************************************************************************** */

// Map() - Implementation of the generic Map function using post-order traversal
// This provides a default implementation that delegates to PostOrderMap
// Allows containers to support both the generic Map interface and specific post-order mapping
template <typename Data>
inline void PostOrderMappableContainer<Data>::Map(MapFun fun) {
  // Implement the Map function in terms of PostOrderMap
  // This ensures that when Map() is called, it uses post-order traversal
  PostOrderMap(fun);
}

/* ************************************************************************** */
// InOrderMappableContainer Implementation
/* ************************************************************************** */

// Map() - Implementation of the generic Map function using in-order traversal
// This provides a default implementation that delegates to InOrderMap
// Particularly useful for binary search trees where in-order maintains sorted property
template <typename Data>
inline void InOrderMappableContainer<Data>::Map(typename MappableContainer<Data>::MapFun fun) {
  // Implement the Map function in terms of InOrderMap
  // This ensures that when Map() is called, it uses in-order traversal
  InOrderMap(fun);
}

/* ************************************************************************** */
// BreadthMappableContainer Implementation
/* ************************************************************************** */

// Map() - Implementation of the generic Map function using breadth-first traversal
// This provides a default implementation that delegates to BreadthMap
// Useful for tree structures where level-by-level modification is desired
template <typename Data>
inline void BreadthMappableContainer<Data>::Map(typename MappableContainer<Data>::MapFun fun) {
  // Implement the Map function in terms of BreadthMap
  // This ensures that when Map() is called, it uses breadth-first traversal
  BreadthMap(fun);
}

/* ************************************************************************** */

}
