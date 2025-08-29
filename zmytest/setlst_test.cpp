#include "test.hpp"
#include "../set/lst/setlst.hpp"
#include "../vector/vector.hpp" // For constructing SetLst from Vector
#include "../list/list.hpp"     // For constructing SetLst from List
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector> // For std::vector in tests
#include <functional> // For std::function

void testSetLst() {
    std::cout << "\n=== Inizio test SetLst ===" << std::endl;

    // ========== TEST COSTRUTTORI E OPERAZIONI DI BASE ==========
    
    // Test costruttore default
    lasd::SetLst<int> s1;
    printTestResult(s1.Empty(), "SetLst<int>::Empty", "Verifica set vuoto dopo costruttore default");
    printTestResult(s1.Size() == 0, "SetLst<int>::Size", "Verifica size 0 dopo costruttore default");
    
    // Test Insert e Exists
    s1.Insert(10);
    s1.Insert(20);
    s1.Insert(30);
    printTestResult(s1.Size() == 3, "SetLst<int>::Size", "Verifica size dopo inserimenti");
    printTestResult(!s1.Empty(), "SetLst<int>::Empty", "Verifica non vuoto dopo inserimenti");
    printTestResult(s1.Exists(10), "SetLst<int>::Exists", "Verifica elemento esistente");
    printTestResult(s1.Exists(20), "SetLst<int>::Exists", "Verifica elemento esistente");
    printTestResult(!s1.Exists(15), "SetLst<int>::Exists", "Verifica elemento non esistente");
    
    // Test Insert duplicato (non deve essere inserito)
    s1.Insert(10);
    printTestResult(s1.Size() == 3, "SetLst<int>::Insert", "Verifica size non cambiata dopo inserimento duplicato");
    
    // Test Remove
    s1.Remove(20);
    printTestResult(s1.Size() == 2, "SetLst<int>::Remove", "Verifica size dopo rimozione");
    printTestResult(!s1.Exists(20), "SetLst<int>::Remove", "Verifica elemento rimosso");
    printTestResult(s1.Exists(10) && s1.Exists(30), "SetLst<int>::Remove", "Verifica altri elementi intatti");
    
    // Test Remove elemento non esistente
    s1.Remove(25);
    printTestResult(s1.Size() == 2, "SetLst<int>::Remove", "Verifica size invariata dopo tentativo rimozione non esistente");
    
    // ========== TEST COSTRUTTORI AVANZATI ==========
    
    // Crea un insieme con alcuni elementi
    lasd::SetLst<int> s2;
    s2.Insert(5);
    s2.Insert(10);
    s2.Insert(15);
    s2.Insert(20);
    s2.Insert(25);
    printTestResult(s2.Size() == 5, "SetLst<int>::Insert multipli", "Verifica size dopo inserimenti multipli");
    printTestResult(s2.Exists(5) && s2.Exists(15) && s2.Exists(25), "SetLst<int>::Exists", "Verifica elementi inseriti");
    
    // Costruttore di copia
    lasd::SetLst<int> s3(s2);
    printTestResult(s3.Size() == s2.Size(), "SetLst<int>::SetLst(const SetLst &)", "Verifica size dopo costruttore di copia");
    bool copyCorrect = true;
    for(int val : {5, 10, 15, 20, 25}) {
        if(!s3.Exists(val)) {
            copyCorrect = false;
            break;
        }
    }
    printTestResult(copyCorrect, "SetLst<int>::SetLst(const SetLst &)", "Verifica elementi dopo costruttore di copia");
    
    // Costruttore di movimento
    lasd::SetLst<int> s4(std::move(s3));
    printTestResult(s4.Size() == 5, "SetLst<int>::SetLst(SetLst &&)", "Verifica size dopo costruttore di movimento");
    printTestResult(s3.Empty(), "SetLst<int>::SetLst(SetLst &&)", "Verifica sorgente svuotato dopo movimento");
    copyCorrect = true;
    for(int val : {5, 10, 15, 20, 25}) {
        if(!s4.Exists(val)) {
            copyCorrect = false;
            break;
        }
    }
    printTestResult(copyCorrect, "SetLst<int>::SetLst(SetLst &&)", "Verifica elementi dopo costruttore di movimento");
    
    // ========== TEST OPERATOR= (COPIA E MOVIMENTO) ==========
    lasd::SetLst<int> setA;
    setA.Insert(1);
    setA.Insert(2);
    setA.Insert(3);
    setA.Insert(4);
    setA.Insert(5);
    
    lasd::SetLst<int> setB;
    setB.Insert(4);
    setB.Insert(5);
    setB.Insert(6);
    setB.Insert(7);
    setB.Insert(8);
    
    // Test operator= di copia
    lasd::SetLst<int> setC;
    setC = setA;  // operator= di copia
    printTestResult(setC.Size() == setA.Size(), "SetLst<int>::operator=", "Verifica size dopo operator= di copia");
    bool equalSets = true;
    for(int val : {1, 2, 3, 4, 5}) {
        if(!setC.Exists(val)) {
            equalSets = false;
            break;
        }
    }
    printTestResult(equalSets, "SetLst<int>::operator=", "Verifica elementi dopo operator= di copia");
    
    // Test operator= di movimento
    lasd::SetLst<int> setD;
    setD = std::move(setC);  // operator= di movimento
    printTestResult(setD.Size() == setA.Size(), "SetLst<int>::operator=", "Verifica size dopo operator= di movimento");
    printTestResult(setC.Empty(), "SetLst<int>::operator=", "Verifica sorgente svuotato dopo operator= di movimento");
    
    // ========== TEST OPERAZIONI SU SET ==========
    
    // Nota: gli operatori +, *, -, <=, >= non sono definiti direttamente in SetLst
    // quindi utilizziamo metodi equivalenti o costruiamo manualmente i risultati
    
    // Test equivalente a unione (setA + setB)
    lasd::SetLst<int> unionSet(setA);
    // Aggiungiamo gli elementi di setB a unionSet
    for(int val : {4, 5, 6, 7, 8}) {
        unionSet.Insert(val);
    }
    printTestResult(unionSet.Size() == 8, "SetLst<int>::Insert (unione manuale)", "Verifica size dopo unione");
    bool unionCorrect = true;
    for(int val : {1, 2, 3, 4, 5, 6, 7, 8}) {
        if(!unionSet.Exists(val)) {
            unionCorrect = false;
            break;
        }
    }
    printTestResult(unionCorrect, "SetLst<int>::Insert (unione manuale)", "Verifica elementi dopo unione");
    
    // Test equivalente a intersezione (setA * setB)
    lasd::SetLst<int> intersectSet;
    // Aggiungiamo solo gli elementi comuni a setA e setB
    for(int val : {4, 5}) {
        if(setA.Exists(val) && setB.Exists(val)) {
            intersectSet.Insert(val);
        }
    }
    printTestResult(intersectSet.Size() == 2, "SetLst<int>::Insert (intersezione manuale)", "Verifica size dopo intersezione");
    printTestResult(intersectSet.Exists(4) && intersectSet.Exists(5), "SetLst<int>::Insert (intersezione manuale)", "Verifica elementi dopo intersezione");
    
    // Test equivalente a differenza (setA - setB)
    lasd::SetLst<int> diffSet;
    // Aggiungiamo solo gli elementi di setA che non sono in setB
    for(int val : {1, 2, 3, 4, 5}) {
        if(setA.Exists(val) && !setB.Exists(val)) {
            diffSet.Insert(val);
        }
    }
    printTestResult(diffSet.Size() == 3, "SetLst<int>::Insert (differenza manuale)", "Verifica size dopo differenza");
    printTestResult(diffSet.Exists(1) && diffSet.Exists(2) && diffSet.Exists(3), "SetLst<int>::Insert (differenza manuale)", "Verifica elementi dopo differenza");
    printTestResult(!diffSet.Exists(4) && !diffSet.Exists(5), "SetLst<int>::Insert (differenza manuale)", "Verifica elementi non in differenza");
    
    // ========== TEST ORDINAMENTO DEGLI ELEMENTI ==========
    std::cout << "\n=== Test ordinamento elementi ===" << std::endl;
    
    lasd::SetLst<int> orderedSet;
    // Inserisco in ordine non sequenziale per verificare che vengano comunque ordinati
    orderedSet.Insert(30);
    orderedSet.Insert(10);
    orderedSet.Insert(50);
    orderedSet.Insert(20);
    orderedSet.Insert(40);
    
    // Creo una lista ordinata usando traversal
    std::vector<int> collectedElements;
    orderedSet.Traverse([&collectedElements](const int& value) {
        collectedElements.push_back(value);
    });
    
    // Verifico che gli elementi siano stati traversati in ordine
    bool isOrdered = true;
    for (size_t i = 1; i < collectedElements.size(); i++) {
        if (collectedElements[i-1] > collectedElements[i]) {
            isOrdered = false;
            break;
        }
    }
    printTestResult(isOrdered, "SetLst<int>::Traverse", "Verifica elementi traversati in ordine");
    printTestResult(collectedElements[0] == 10 && collectedElements[4] == 50, "SetLst<int>::Traverse", "Verifica elementi min/max corretti");

    // ========== TEST METODI Min/Max E VARIANTI ==========
    std::cout << "\n=== Test Min/Max e varianti ===" << std::endl;
    
    // Test Min e Max
    printTestResult(orderedSet.Min() == 10, "SetLst<int>::Min", "Verifica minimo");
    printTestResult(orderedSet.Max() == 50, "SetLst<int>::Max", "Verifica massimo");
    
    // Test MinNRemove
    int minValue = orderedSet.MinNRemove();
    printTestResult(minValue == 10, "SetLst<int>::MinNRemove", "Verifica valore minimo rimosso");
    printTestResult(!orderedSet.Exists(10), "SetLst<int>::MinNRemove", "Verifica rimozione dell'elemento minimo");
    printTestResult(orderedSet.Size() == 4, "SetLst<int>::MinNRemove", "Verifica size dopo rimozione minimo");
    
    // Test RemoveMin
    orderedSet.RemoveMin();
    printTestResult(!orderedSet.Exists(20), "SetLst<int>::RemoveMin", "Verifica rimozione del nuovo minimo");
    printTestResult(orderedSet.Size() == 3, "SetLst<int>::RemoveMin", "Verifica size dopo RemoveMin");
    
    // Test MaxNRemove
    int maxValue = orderedSet.MaxNRemove();
    printTestResult(maxValue == 50, "SetLst<int>::MaxNRemove", "Verifica valore massimo rimosso");
    printTestResult(!orderedSet.Exists(50), "SetLst<int>::MaxNRemove", "Verifica rimozione dell'elemento massimo");
    
    // Test RemoveMax
    orderedSet.RemoveMax();
    printTestResult(!orderedSet.Exists(40), "SetLst<int>::RemoveMax", "Verifica rimozione del nuovo massimo");
    printTestResult(orderedSet.Size() == 1, "SetLst<int>::RemoveMax", "Verifica size dopo RemoveMax");
    
    // ========== TEST METODI Predecessor/Successor E VARIANTI ==========
    std::cout << "\n=== Test Predecessor/Successor e varianti ===" << std::endl;
    
    lasd::SetLst<int> seqSet;
    // Inserisco elementi in sequenza
    for (int i = 10; i <= 50; i += 10) {
        seqSet.Insert(i);
    }
    
    // Test Predecessor
    printTestResult(seqSet.Predecessor(30) == 20, "SetLst<int>::Predecessor", "Verifica predecessore di un elemento");
    printTestResult(seqSet.Predecessor(40) == 30, "SetLst<int>::Predecessor", "Verifica predecessore di un elemento");
    
    // Test Successor
    printTestResult(seqSet.Successor(20) == 30, "SetLst<int>::Successor", "Verifica successore di un elemento");
    printTestResult(seqSet.Successor(30) == 40, "SetLst<int>::Successor", "Verifica successore di un elemento");
    
    // Test PredecessorNRemove
    int predValue = seqSet.PredecessorNRemove(30);
    printTestResult(predValue == 20, "SetLst<int>::PredecessorNRemove", "Verifica valore predecessore rimosso");
    printTestResult(!seqSet.Exists(20), "SetLst<int>::PredecessorNRemove", "Verifica rimozione del predecessore");
    
    // Test SuccessorNRemove
    int succValue = seqSet.SuccessorNRemove(30);
    printTestResult(succValue == 40, "SetLst<int>::SuccessorNRemove", "Verifica valore successore rimosso");
    printTestResult(!seqSet.Exists(40), "SetLst<int>::SuccessorNRemove", "Verifica rimozione del successore");
    
    // Test RemovePredecessor
    seqSet.Insert(20);  // Reinseriamo 20 per avere un predecessore di 30
    seqSet.RemovePredecessor(30);
    printTestResult(!seqSet.Exists(20), "SetLst<int>::RemovePredecessor", "Verifica rimozione del predecessore");
    
    // Test RemoveSuccessor
    seqSet.Insert(40);  // Reinseriamo 40 per avere un successore di 30
    seqSet.RemoveSuccessor(30);
    printTestResult(!seqSet.Exists(40), "SetLst<int>::RemoveSuccessor", "Verifica rimozione del successore");
    
    // ========== TEST ECCEZIONI PER SET VUOTO ==========
    std::cout << "\n=== Test eccezioni per set vuoto ===" << std::endl;
    
    lasd::SetLst<int> emptySet;
    bool exceptionThrown;
    
    // Test Min su set vuoto
    exceptionThrown = false;
    try {
        int val = emptySet.Min();
        (void)val; // Per evitare warning
    } catch (std::length_error&) {
        exceptionThrown = true;
    } catch (...) {
        // Cattura altre eccezioni
    }
    printTestResult(exceptionThrown, "SetLst<int>::Min", "Verifica eccezione su set vuoto");
    
    // Test Max su set vuoto
    exceptionThrown = false;
    try {
        int val = emptySet.Max();
        (void)val; // Per evitare warning
    } catch (std::length_error&) {
        exceptionThrown = true;
    } catch (...) {
        // Cattura altre eccezioni
    }
    printTestResult(exceptionThrown, "SetLst<int>::Max", "Verifica eccezione su set vuoto");
    
    // Test MinNRemove su set vuoto
    exceptionThrown = false;
    try {
        int val = emptySet.MinNRemove();
        (void)val; // Per evitare warning
    } catch (std::length_error&) {
        exceptionThrown = true;
    } catch (...) {
        // Cattura altre eccezioni
    }
    printTestResult(exceptionThrown, "SetLst<int>::MinNRemove", "Verifica eccezione su set vuoto");
    
    // ========== TEST Clear ==========
    std::cout << "\n=== Test Clear ===" << std::endl;
    
    lasd::SetLst<int> setToClear;
    for (int i = 1; i <= 10; i++) {
        setToClear.Insert(i);
    }
    
    setToClear.Clear();
    printTestResult(setToClear.Empty(), "SetLst<int>::Clear", "Verifica set vuoto dopo Clear");
    printTestResult(setToClear.Size() == 0, "SetLst<int>::Clear", "Verifica size 0 dopo Clear");
    
    // ========== TEST COSTRUTTORE DA CONTAINER TRAVERSABILE ==========
    std::cout << "\n=== Test costruttore da TraversableContainer ===" << std::endl;
    
    lasd::Vector<int> vec(5);
    vec[0] = 50; vec[1] = 30; vec[2] = 10; vec[3] = 40; vec[4] = 20;
    
    lasd::SetLst<int> setFromTraversable(vec);
    printTestResult(setFromTraversable.Size() == 5, "SetLst<int>::SetLst(TraversableContainer)", "Verifica dimensione dopo costruzione da TraversableContainer");
    
    // Verifico che tutti gli elementi siano stati copiati
    bool allElementsCopied = true;
    for (int i = 10; i <= 50; i += 10) {
        if (!setFromTraversable.Exists(i)) {
            allElementsCopied = false;
            break;
        }
    }
    printTestResult(allElementsCopied, "SetLst<int>::SetLst(TraversableContainer)", "Verifica elementi dopo costruzione da TraversableContainer");
    
    // Verifico che sia stato mantenuto l'ordinamento
    std::vector<int> traversalOrder;
    setFromTraversable.Traverse([&traversalOrder](const int& value) {
        traversalOrder.push_back(value);
    });
    
    bool isSortedTraversal = true;
    for (size_t i = 1; i < traversalOrder.size(); i++) {
        if (traversalOrder[i-1] > traversalOrder[i]) {
            isSortedTraversal = false;
            break;
        }
    }
    printTestResult(isSortedTraversal, "SetLst<int>::SetLst(TraversableContainer)", "Verifica ordine degli elementi");
    
    // ========== TEST COSTRUTTORE DA CONTAINER MAPPABILE ==========
    std::cout << "\n=== Test costruttore da MappableContainer ===" << std::endl;
    
    lasd::List<int> listToMove;
    listToMove.InsertAtBack(15);
    listToMove.InsertAtBack(5);
    listToMove.InsertAtBack(25);
    listToMove.InsertAtBack(5);  // Elemento duplicato
    
    lasd::SetLst<int> setFromMappable(std::move(listToMove));
    printTestResult(setFromMappable.Size() == 3, "SetLst<int>::SetLst(MappableContainer&&)", "Verifica size dopo costruzione da MappableContainer (duplicati eliminati)");
    printTestResult(setFromMappable.Exists(5) && setFromMappable.Exists(15) && setFromMappable.Exists(25), 
                   "SetLst<int>::SetLst(MappableContainer&&)", "Verifica elementi dopo costruzione da MappableContainer");
    printTestResult(listToMove.Empty(), "SetLst<int>::SetLst(MappableContainer&&)", "Verifica sorgente svuotata dopo costruzione");
    
    // ========== TEST CON TIPO STRING ==========
    std::cout << "\n=== Test con std::string ===" << std::endl;
    
    lasd::SetLst<std::string> stringSet;
    stringSet.Insert("banana");
    stringSet.Insert("mela");
    stringSet.Insert("arancia");
    stringSet.Insert("kiwi");
    stringSet.Insert("mela");  // Duplicato, non dovrebbe essere inserito
    
    printTestResult(stringSet.Size() == 4, "SetLst<string>::Size", "Verifica size dopo inserimenti string");
    printTestResult(stringSet.Exists("mela"), "SetLst<string>::Exists", "Verifica esistenza elemento string");
    printTestResult(stringSet.Min() == "arancia", "SetLst<string>::Min", "Verifica elemento minimo string");
    printTestResult(stringSet.Max() == "mela", "SetLst<string>::Max", "Verifica elemento massimo string");
    
    // Test ordinamento alfabetico
    std::vector<std::string> stringOrder;
    stringSet.Traverse([&stringOrder](const std::string& value) {
        stringOrder.push_back(value);
    });
    
    printTestResult(stringOrder[0] == "arancia" && stringOrder[1] == "banana" && 
                   stringOrder[2] == "kiwi" && stringOrder[3] == "mela", 
                   "SetLst<string>::Traverse", "Verifica ordine alfabetico degli elementi string");
    
    // ========== TEST FUNZIONI Map e Fold ==========
    std::cout << "\n=== Test Map e Fold ===" << std::endl;
    
    lasd::SetLst<int> mapSet;
    mapSet.Insert(1);
    mapSet.Insert(2);
    mapSet.Insert(3);
    
    // Test Map (raddoppio tutti i valori)
    mapSet.Map([](int& x) { x *= 2; });
    
    printTestResult(mapSet.Exists(2) && mapSet.Exists(4) && mapSet.Exists(6), 
                   "SetLst<int>::Map", "Verifica elementi dopo Map");
    // Ensure original elements are not present if mapping was in-place and changed values
    // This depends on SetLst::Map implementation. If it creates a new set, this test is different.
    // Assuming Map modifica in place:
    printTestResult(!mapSet.Exists(1) && !mapSet.Exists(3), // Element 5 was not in the set {1,2,3}
                   "SetLst<int>::Map", "Verifica elementi originali non più presenti dopo Map (se Map è in-place)");
    
    // Test Fold (somma)
    using FoldFunctionInt = std::function<int(const int&, const int&)>;
    int sum = mapSet.Fold(FoldFunctionInt([](const int& val, const int& acc) { return acc + val; }), 0);
    printTestResult(sum == 12, "SetLst<int>::Fold", "Verifica risultato Fold (somma)");
    
    // Test Fold (prodotto)
    int product = mapSet.Fold(FoldFunctionInt([](const int& val, const int& acc) { return acc * val; }), 1);
    printTestResult(product == (2*4*6), "SetLst<int>::Fold", "Verifica risultato Fold (prodotto)");

    // Test su SetLst vuoto per Min, Max, Successor, Predecessor
    lasd::SetLst<int> emptySetForOps;
    exceptionThrown = false;
    try { [[maybe_unused]] auto val = emptySetForOps.Min(); } catch (const std::length_error&) { exceptionThrown = true; } catch (...) {}
    printTestResult(exceptionThrown, "SetLst<int>::Min", "Test eccezione Min su set vuoto");
    exceptionThrown = false;
    try { [[maybe_unused]] auto val = emptySetForOps.Max(); } catch (const std::length_error&) { exceptionThrown = true; } catch (...) {}
    printTestResult(exceptionThrown, "SetLst<int>::Max", "Test eccezione Max su set vuoto");
    exceptionThrown = false;
    try { [[maybe_unused]] auto val = emptySetForOps.Successor(1); } catch (const std::length_error&) { exceptionThrown = true; } catch (...) {}
    printTestResult(exceptionThrown, "SetLst<int>::Successor", "Test eccezione Successor su set vuoto");
    exceptionThrown = false;
    try { [[maybe_unused]] auto val = emptySetForOps.Predecessor(1); } catch (const std::length_error&) { exceptionThrown = true; } catch (...) {}
    printTestResult(exceptionThrown, "SetLst<int>::Predecessor", "Test eccezione Predecessor su set vuoto");

    // Test con elemento di tipo personalizzato (rimosso se non definito)
    // std::cout << "Test con elemento di tipo personalizzato non implementato." << std::endl;
    
    std::cout << "=== Fine test SetLst ===\n" << std::endl;

    // ========== TEST Aggiuntivi per copertura completa SetLst ==========
    
    std::cout << "\n=== Test aggiuntivi per copertura completa SetLst ===" << std::endl;

    // === Test SetLst con double (funzioni avanzate non testate) ===
    {
      lasd::SetLst<double> doubleSetAdv;
      doubleSetAdv.Insert(1.5);
      doubleSetAdv.Insert(3.7);
      doubleSetAdv.Insert(2.1);
      doubleSetAdv.Insert(4.9);
      doubleSetAdv.Insert(0.8);

      // Test MinNRemove con double
      double minVal = doubleSetAdv.MinNRemove();
      printTestResult(minVal == 0.8 && doubleSetAdv.Size() == 4, "SetLst<double>::MinNRemove", "Verifica MinNRemove con double");

      // Test MaxNRemove con double
      double maxVal = doubleSetAdv.MaxNRemove();
      printTestResult(maxVal == 4.9 && doubleSetAdv.Size() == 3, "SetLst<double>::MaxNRemove", "Verifica MaxNRemove con double");

      // Test Predecessor con double
      double pred = doubleSetAdv.Predecessor(3.7);
      printTestResult(pred == 2.1, "SetLst<double>::Predecessor", "Verifica Predecessor con double");

      // Test Successor con double
      double succ = doubleSetAdv.Successor(2.1);
      printTestResult(succ == 3.7, "SetLst<double>::Successor", "Verifica Successor con double");

      // Test PredecessorNRemove con double
      double predRem = doubleSetAdv.PredecessorNRemove(3.7);
      printTestResult(predRem == 2.1 && doubleSetAdv.Size() == 2, "SetLst<double>::PredecessorNRemove", "Verifica PredecessorNRemove con double");

      // Test SuccessorNRemove con double
      double succRem = doubleSetAdv.SuccessorNRemove(1.5);
      printTestResult(succRem == 3.7 && doubleSetAdv.Size() == 1, "SetLst<double>::SuccessorNRemove", "Verifica SuccessorNRemove con double");

      // Test RemovePredecessor con double
      doubleSetAdv.Insert(0.5);
      doubleSetAdv.Insert(2.0);
      doubleSetAdv.RemovePredecessor(2.0);
      printTestResult(doubleSetAdv.Size() == 2 && !doubleSetAdv.Exists(1.5), "SetLst<double>::RemovePredecessor", "Verifica RemovePredecessor con double");

      // Test RemoveSuccessor con double
      doubleSetAdv.RemoveSuccessor(0.5);
      printTestResult(doubleSetAdv.Size() == 1 && !doubleSetAdv.Exists(2.0), "SetLst<double>::RemoveSuccessor", "Verifica RemoveSuccessor con double");
    }

    // === Test SetLst con string (funzioni avanzate non testate) ===
    {
      lasd::SetLst<std::string> stringSetAdv;
      stringSetAdv.Insert("apple");
      stringSetAdv.Insert("banana");
      stringSetAdv.Insert("cherry");
      stringSetAdv.Insert("date");

      // Test MinNRemove con string
      std::string minVal = stringSetAdv.MinNRemove();
      printTestResult(minVal == "apple" && stringSetAdv.Size() == 3, "SetLst<string>::MinNRemove", "Verifica MinNRemove con string");

      // Test MaxNRemove con string
      std::string maxVal = stringSetAdv.MaxNRemove();
      printTestResult(maxVal == "date" && stringSetAdv.Size() == 2, "SetLst<string>::MaxNRemove", "Verifica MaxNRemove con string");

      // Test Predecessor con string
      std::string pred = stringSetAdv.Predecessor("cherry");
      printTestResult(pred == "banana", "SetLst<string>::Predecessor", "Verifica Predecessor con string");

      // Test Successor con string
      std::string succ = stringSetAdv.Successor("banana");
      printTestResult(succ == "cherry", "SetLst<string>::Successor", "Verifica Successor con string");

      // Test PredecessorNRemove con string
      std::string predRem = stringSetAdv.PredecessorNRemove("cherry");
      printTestResult(predRem == "banana" && stringSetAdv.Size() == 1, "SetLst<string>::PredecessorNRemove", "Verifica PredecessorNRemove con string");

      // Aggiungiamo elementi per testare SuccessorNRemove
      stringSetAdv.Insert("elderberry");
      stringSetAdv.Insert("fig");

      // Test SuccessorNRemove con string
      std::string succRem = stringSetAdv.SuccessorNRemove("cherry");
      printTestResult(succRem == "elderberry" && stringSetAdv.Size() == 2, "SetLst<string>::SuccessorNRemove", "Verifica SuccessorNRemove con string");

      // Test RemovePredecessor con string
      stringSetAdv.RemovePredecessor("fig");
      printTestResult(stringSetAdv.Size() == 1 && !stringSetAdv.Exists("cherry"), "SetLst<string>::RemovePredecessor", "Verifica RemovePredecessor con string");

      // Test RemoveSuccessor con string
      stringSetAdv.Insert("grape");
      stringSetAdv.RemoveSuccessor("fig");
      printTestResult(stringSetAdv.Size() == 1 && !stringSetAdv.Exists("grape"), "SetLst<string>::RemoveSuccessor", "Verifica RemoveSuccessor con string");
    }

    // === Test InsertSome e RemoveSome per SetLst ===
    {
      // InsertSome e RemoveSome sono metodi casuali che inseriscono/rimuovono solo alcuni elementi
      // Testiamo più volte per aumentare la probabilità di successo
      bool someInserted = false;
      bool someRemoved = false;
      
      for(int attempt = 0; attempt < 10 && (!someInserted || !someRemoved); attempt++) {
        lasd::SetLst<int> insertSomeSet;
        lasd::Vector<int> insertVector(4);
        insertVector[0] = 10;
        insertVector[1] = 20;
        insertVector[2] = 30;
        insertVector[3] = 40;

        // Test InsertSome (metodo casuale)
        bool insertResult = insertSomeSet.InsertSome(insertVector);
        if(insertResult && insertSomeSet.Size() > 0) {
          someInserted = true;
        }

        // Se abbiamo inserito qualcosa, testiamo RemoveSome
        if(insertSomeSet.Size() > 0) {
          lasd::Vector<int> removeVector(2);
          removeVector[0] = 10;
          removeVector[1] = 20;

          // Test RemoveSome (metodo casuale)
          bool removeResult = insertSomeSet.RemoveSome(removeVector);
          if(removeResult) {
            someRemoved = true;
          }
        }
      }
      
      printTestResult(someInserted, "SetLst<int>::InsertSome", "Verifica InsertSome (comportamento casuale)");
      printTestResult(someRemoved, "SetLst<int>::RemoveSome", "Verifica RemoveSome (comportamento casuale)");
    }
}
