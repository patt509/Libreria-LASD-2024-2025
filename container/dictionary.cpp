namespace lasd {

/* ************************************************************************** */
// DictionaryContainer Implementation - Bulk Operations
/* ************************************************************************** */

// InsertAll() - Attempt to insert all elements from a TraversableContainer
// Returns true only if ALL elements were successfully inserted (none already existed)
// Uses copy semantics for element insertion
template<typename Data>
bool DictionaryContainer<Data>::InsertAll(const TraversableContainer<Data> & con) {
  bool all = true;
  // Traverse through all elements in the source container
  con.Traverse(
    [this, &all](const Data & dat) {
      // Use logical AND assignment to track if all insertions succeed
      // If any insertion fails (element already exists), all becomes false
      all &= Insert(dat);
    }
  );
  return all; // Return true only if every single element was inserted
}

// InsertAll() - Attempt to move-insert all elements from a MappableContainer
// Returns true only if ALL elements were successfully inserted (none already existed)
// Uses move semantics for efficient element transfer
template<typename Data>
bool DictionaryContainer<Data>::InsertAll(MappableContainer<Data> && con) {
  bool all = true;
  // Map through all elements in the source container, allowing modification
  con.Map(
    [this, &all](Data & dat) {
      // Use logical AND assignment with move semantics
      // Elements are moved from source container to this dictionary
      all &= Insert(std::move(dat));
    }
  );
  return all; // Return true only if every single element was inserted
}

// RemoveAll() - Attempt to remove all specified elements from the dictionary
// Returns true only if ALL specified elements were found and successfully removed
template<typename Data>
bool DictionaryContainer<Data>::RemoveAll(const TraversableContainer<Data> & con) {
  bool all = true;
  // Traverse through all elements to be removed
  con.Traverse(
    [this, &all](const Data & dat) {
      // Use logical AND assignment to track if all removals succeed
      // If any element is not found, all becomes false
      all &= Remove(dat);
    }
  );
  return all; // Return true only if every specified element was removed
}

// InsertSome() - Attempt to insert elements, succeeding if at least one is inserted
// Returns true if at least ONE element was successfully inserted
// Uses copy semantics for element insertion
template<typename Data>
bool DictionaryContainer<Data>::InsertSome(const TraversableContainer<Data> & con) {
  bool some = false;
  // Traverse through all elements in the source container
  con.Traverse(
    [this, &some](const Data & dat) {
      // Use logical OR assignment to track if any insertion succeeds
      // Once one insertion succeeds, some remains true
      some |= Insert(dat);
    }
  );
  return some; // Return true if at least one element was inserted
}

// InsertSome() - Attempt to move-insert elements, succeeding if at least one is inserted
// Returns true if at least ONE element was successfully inserted
// Uses move semantics for efficient element transfer
template<typename Data>
bool DictionaryContainer<Data>::InsertSome(MappableContainer<Data> && con) {
  bool some = false;
  // Map through all elements in the source container
  con.Map(
    [this, &some](Data & dat) {
      // Use logical OR assignment with move semantics
      // Elements are moved from source container when successfully inserted
      some |= Insert(std::move(dat));
    }
  );
  return some; // Return true if at least one element was inserted
}

// RemoveSome() - Attempt to remove elements, succeeding if at least one is removed
// Returns true if at least ONE specified element was found and successfully removed
template<typename Data>
bool DictionaryContainer<Data>::RemoveSome(const TraversableContainer<Data> & con) {
  bool some = false;
  // Traverse through all elements to be potentially removed
  con.Traverse(
    [this, &some](const Data & dat) {
      // Use logical OR assignment to track if any removal succeeds
      // Once one removal succeeds, some remains true
      some |= Remove(dat);
    }
  );
  return some; // Return true if at least one element was removed
}

/* ************************************************************************** */

}
