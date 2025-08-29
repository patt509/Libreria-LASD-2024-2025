#include <iostream>
#include <limits> // For std::numeric_limits

#include "zlasdtest/test.hpp"
#include "zmytest/test.hpp"

/* ************************************************************************** */
// It's assumed that testsFailed is a global variable (e.g., extern unsigned long testsFailed;)
// declared in one of the included test headers and defined/initialized in their .cpp files.
// This variable will accumulate errors if not reset by the test functions themselves.

int main() {
  
  int choice = -1;

  do {
    std::cout << "\nScegli quale suite di test eseguire:" << std::endl;
    std::cout << "1: Esegui zlasdtest" << std::endl;
    std::cout << "2: Esegui tutti i test personalizzati" << std::endl;
    std::cout << "3: Esegui solo test su List, Vector e Set" << std::endl;
    std::cout << "4: Esegui solo test su Heap e Priority Queue" << std::endl;
    std::cout << "0: Termina" << std::endl;
    std::cout << "Inserisci la tua scelta: ";
    
    std::cin >> choice;

    if (std::cin.fail()) {
        std::cout << "Input non valido. Inserisci un numero." << std::endl;
        std::cin.clear(); // Clear error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        choice = -1; // Ensure loop continues for re-prompt if cin failed
        continue;
    }
    // Consume the rest of the line, in case user entered "1abc" or similar
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
      case 1:
        std::cout << "\nEsecuzione di zlasdtest..." << std::endl;
        lasdtest(); // Esegue i test forniti
        // The global testsFailed will be updated by lasdtest()
        std::cout << "zlasdtest completato." << std::endl;
        break;
      case 2:
        std::cout << "\nEsecuzione di tutti i test personalizzati..." << std::endl;
        executeMyTests(); // Esegue tutti i test personalizzati
        // The global testsFailed will be updated by executeMyTests()
        std::cout << "Test personalizzati completati." << std::endl;
        break;
      case 3:
        std::cout << "\nEsecuzione dei test su List, Vector e Set..." << std::endl;
        executeDataStructureTests(); // Esegue solo i test su List, Vector e Set
        std::cout << "Test su List, Vector e Set completati." << std::endl;
        break;
      case 4:
        std::cout << "\nEsecuzione dei test su Heap e Priority Queue..." << std::endl;
        executeHeapPQTests(); // Esegue solo i test su Heap e Priority Queue
        std::cout << "Test su Heap e Priority Queue completati." << std::endl;
        break;
      case 0:
        std::cout << "Terminazione del programma." << std::endl;
        break;
      default:
        std::cout << "Scelta non valida. Riprova." << std::endl;
        break;
    }
  } while (choice != 0);
  
  // This final message will reflect the cumulative state of testsFailed 
  // across all test suites run during the session, assuming testsFailed accumulates.
  std::cout << "\nProva terminata complessivamente. Esito: " << (testsFailed == 0 ? "Corretto" : "Errore") << std::endl;
  return 0;
}