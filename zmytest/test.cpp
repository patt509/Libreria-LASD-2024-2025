#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath> // Per std::abs

#include "test.hpp" // Include delle dichiarazioni

// Global test counters
uint testNumber = 0;
uint testsPassed = 0;
uint testsFailed = 0;

// Helper function to print test results
void printTestResult(bool condition, const std::string& testName, const std::string& description) {
    testNumber++;
    if (condition) {
        testsPassed++;
        std::cout << "Test " << testNumber << " [" << testName << "]: PASSED - " << description << std::endl;
    } else {
        testsFailed++;
        std::cout << "Test " << testNumber << " [" << testName << "]: FAILED - " << description << std::endl;
    }
}

// Execute all tests - this is called from main.cpp
void executeMyTests() {
    // Reset counters
    testNumber = 0;
    testsPassed = 0;
    testsFailed = 0;
    
    // Esegui i test per tutte le strutture dati
    testVector();
    testList();
    testSetVec();
    testSetLst();
    testHeap();
    testPriorityQueue();
    
    // Report total results
    std::cout << "\nTest summary: " << testsPassed << " passed, " 
              << testsFailed << " failed, " << testNumber << " total" << std::endl;
}

// Execute tests only for basic data structures (List, Vector, Set)
void executeDataStructureTests() {
    // Reset counters
    testNumber = 0;
    testsPassed = 0;
    testsFailed = 0;
    
    // Esegui i test per List, Vector e Set
    testVector();
    testList();
    testSetVec();
    testSetLst();
    
    // Report total results
    std::cout << "\nTest di List, Vector e Set - Riepilogo: " << testsPassed << " passati, " 
              << testsFailed << " falliti, " << testNumber << " totali" << std::endl;
}

// Execute tests only for Heap and Priority Queue
void executeHeapPQTests() {
    // Reset counters
    testNumber = 0;
    testsPassed = 0;
    testsFailed = 0;
    
    // Esegui i test per Heap e Priority Queue
    testHeap();
    testPriorityQueue();
    
    // Report total results
    std::cout << "\nTest di Heap e Priority Queue - Riepilogo: " << testsPassed << " passati, " 
              << testsFailed << " falliti, " << testNumber << " totali" << std::endl;
}
