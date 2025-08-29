namespace lasd {

/* ************************************************************************** */
// TraversableContainer Implementation
/* ************************************************************************** */

// Fold() - Implementation of the fold operation for TraversableContainer
// This method applies a binary function to accumulate a single result from all elements
// The function is called for each element with the current element and the accumulator
template <typename Data>
template <typename Accumulator>
Accumulator TraversableContainer<Data>::Fold(FoldFun<Accumulator> fun, Accumulator acc) const {
  // Use Traverse to visit each element and apply the fold function
  Traverse([fun, &acc](const Data & dat) {
    acc = fun(dat, acc); // Apply the fold function and update accumulator
  });
  return acc; // Return the final accumulated value
}

// Exists() - Implementation that checks if a specific value exists in the container
// Uses traversal to search through all elements until the value is found
template <typename Data>
bool TraversableContainer<Data>::Exists(const Data & val) const noexcept {
  bool found = false;
  // Traverse all elements, stopping when we find the target value
  Traverse([&found, &val](const Data & dat) {
    if (!found && dat == val) {
      found = true;
    }
  });
  return found;
}

/* ************************************************************************** */
// PreOrderTraversableContainer Implementation
/* ************************************************************************** */

// PreOrderFold() - Fold operation specifically for pre-order traversal
// Accumulates values by visiting elements in pre-order (node before children)
template <typename Data>
template <typename Accumulator>
Accumulator PreOrderTraversableContainer<Data>::PreOrderFold(FoldFun<Accumulator> fun, Accumulator acc) const {
  // Use PreOrderTraverse to apply the fold function to each element in pre-order
  PreOrderTraverse(
    [fun, &acc](const Data & dat) {
      acc = fun(dat, acc); // Apply the fold function and update accumulator
    }
  );
  return acc; // Return the accumulated value
}

// Traverse() - Override from TraversableContainer, implemented using pre-order traversal
// This makes pre-order the default traversal method for containers that support it
template <typename Data>
void PreOrderTraversableContainer<Data>::Traverse(TraverseFun fun) const {
  // Implement Traverse in terms of PreOrderTraverse
  PreOrderTraverse(fun);
}

/* ************************************************************************** */
// PostOrderTraversableContainer Implementation
/* ************************************************************************** */

// PostOrderFold() - Fold operation specifically for post-order traversal
// Accumulates values by visiting elements in post-order (children before node)
template <typename Data>
template <typename Accumulator>
Accumulator PostOrderTraversableContainer<Data>::PostOrderFold(FoldFun<Accumulator> fun, Accumulator acc) const {
  // Use PostOrderTraverse to apply the fold function to each element in post-order
  PostOrderTraverse(
    [fun, &acc](const Data & dat) {
      acc = fun(dat, acc); // Apply the fold function and update accumulator
    }
  );
  return acc; // Return the accumulated value
}

// Traverse() - Override from TraversableContainer, implemented using post-order traversal
// This makes post-order the default traversal method for containers that support it
template <typename Data>
void PostOrderTraversableContainer<Data>::Traverse(TraverseFun fun) const {
  // Implement Traverse in terms of PostOrderTraverse
  PostOrderTraverse(fun);
}

/* ************************************************************************** */
// InOrderTraversableContainer Implementation
/* ************************************************************************** */

// InOrderFold() - Fold operation specifically for in-order traversal
// Accumulates values by visiting elements in in-order (left, node, right for trees)
template <typename Data>
template <typename Accumulator>
Accumulator InOrderTraversableContainer<Data>::InOrderFold(FoldFun<Accumulator> fun, Accumulator acc) const {
  // Use InOrderTraverse to apply the fold function to each element in in-order
  InOrderTraverse(
    [fun, &acc](const Data & dat) {
      acc = fun(dat, acc); // Apply the fold function and update accumulator
    }
  );
  return acc; // Return the accumulated value
}

// Traverse() - Override from TraversableContainer, implemented using in-order traversal
// This makes in-order the default traversal method for containers that support it
template <typename Data>
void InOrderTraversableContainer<Data>::Traverse(TraverseFun fun) const {
  // Implement Traverse in terms of InOrderTraverse
  InOrderTraverse(fun);
}

/* ************************************************************************** */
// BreadthTraversableContainer Implementation
/* ************************************************************************** */

// BreadthFold() - Fold operation specifically for breadth-first traversal
// Accumulates values by visiting elements level by level (breadth-first order)
template <typename Data>
template <typename Accumulator>
Accumulator BreadthTraversableContainer<Data>::BreadthFold(FoldFun<Accumulator> fun, Accumulator acc) const {
  // Use BreadthTraverse to apply the fold function to each element in breadth-first order
  BreadthTraverse(
    [fun, &acc](const Data & dat) {
      acc = fun(dat, acc); // Apply the fold function and update accumulator
    }
  );
  return acc; // Return the accumulated value
}

// Traverse() - Override from TraversableContainer, implemented using breadth-first traversal
// This makes breadth-first the default traversal method for containers that support it
template <typename Data>
void BreadthTraversableContainer<Data>::Traverse(TraverseFun fun) const {
  // Implement Traverse in terms of BreadthTraverse
  BreadthTraverse(fun);
}

/* ************************************************************************** */
// Template Instantiations
// These explicit instantiations ensure the templates are compiled for common types
/* ************************************************************************** */

// Template class instantiations for basic container types
template class TraversableContainer<int>;
template class TraversableContainer<double>;
template class TraversableContainer<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>;

template class PreOrderTraversableContainer<int>;
template class PreOrderTraversableContainer<double>;
template class PreOrderTraversableContainer<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>;

template class PostOrderTraversableContainer<int>;
template class PostOrderTraversableContainer<double>;
template class PostOrderTraversableContainer<std::string>;

template class InOrderTraversableContainer<int>;
template class InOrderTraversableContainer<double>;
template class InOrderTraversableContainer<std::string>;

template class BreadthTraversableContainer<int>;
template class BreadthTraversableContainer<double>;
template class BreadthTraversableContainer<std::string>;

// Template function instantiations for fold operations with different accumulator types
// These ensure the Fold functions are compiled for common combinations of data and accumulator types
template int TraversableContainer<int>::Fold(FoldFun<int>, int) const;
template double TraversableContainer<int>::Fold(FoldFun<double>, double) const;
template std::string TraversableContainer<int>::Fold(FoldFun<std::string>, std::string) const;

template int TraversableContainer<double>::Fold(FoldFun<int>, int) const;
template double TraversableContainer<double>::Fold(FoldFun<double>, double) const;
template std::string TraversableContainer<double>::Fold(FoldFun<std::string>, std::string) const;

template int TraversableContainer<std::string>::Fold(FoldFun<int>, int) const;
template double TraversableContainer<std::string>::Fold(FoldFun<double>, double) const;
template std::string TraversableContainer<std::string>::Fold(FoldFun<std::string>, std::string) const;

// Additional instantiations would follow the same pattern for other traversable container types...

}
