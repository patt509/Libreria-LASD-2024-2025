#ifndef MYTEST_HPP
#define MYTEST_HPP

/* ************************************************************************** */

#include <string>
#include <iostream>

// Contatori per i test
extern uint testNumber;
extern uint testsPassed;
extern uint testsFailed;

// Funzione helper per stampare il risultato del test
void printTestResult(bool condition, const std::string& testName, const std::string& description);

/* ************************************************************************** */

// Main functions to execute tests - called from main.cpp
void executeMyTests(); // Execute all tests
void executeDataStructureTests(); // Execute tests for List, Vector and Set
void executeHeapPQTests(); // Execute tests for Heap and Priority Queue

// Declarations for tests in separate files - Tests for specific data structures
void testList();
void testVector();
void testSetLst();
void testSetVec();
void testHeap();
void testHeapEdgeCases();
void testHeapDataTypes();
void testHeapStress();
void testPriorityQueue();
void testPriorityQueueEdgeCasesWithDifferentTypes();
void testPriorityQueueStressAndPerformance();

#endif
