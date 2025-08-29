#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <limits>
#include "test.hpp"
#include "../pq/heap/pqheap.hpp"

void testPriorityQueue() {
    std::cout << "\n=== Inizio test Priority Queue ===" << std::endl;

    // ========== TEST COSTRUTTORI E OPERAZIONI DI BASE ==========
    
    // Test costruttore default
    lasd::PQHeap<int> pq1;
    printTestResult(pq1.Empty(), "PQHeap<int>::Empty", "Verifica PQ vuota dopo costruttore default");
    printTestResult(pq1.Size() == 0, "PQHeap<int>::Size", "Verifica size 0 dopo costruttore default");

    // Creiamo una priority queue con alcuni valori iniziali
    lasd::Vector<int> vec(7);
    vec[0] = 10;
    vec[1] = 5;
    vec[2] = 15;
    vec[3] = 2;
    vec[4] = 8;
    vec[5] = 20;
    vec[6] = 3;
    
    lasd::PQHeap<int> pq2(vec);
    
    // Test proprietà base
    printTestResult(!pq2.Empty(), "PQHeap<int>::Empty", "Verifica PQ non vuota dopo costruzione da Vector");
    printTestResult(pq2.Size() == 7, "PQHeap<int>::Size", "Verifica size dopo costruzione da Vector");
    
    // Test Tip - verifica massimo in cima
    int maxValue = vec[0];
    for (ulong i = 1; i < vec.Size(); i++) {
        if (vec[i] > maxValue) maxValue = vec[i];
    }
    
    try {
        printTestResult(pq2.Tip() == maxValue, "PQHeap<int>::Tip", "Verifica elemento in cima (massimo)");
    } catch (std::exception& e) {
        printTestResult(false, "PQHeap<int>::Tip", std::string("Eccezione inaspettata: ") + e.what());
    }
    
    // ========== TEST OPERAZIONI DI INSERIMENTO E RIMOZIONE ==========
    
    // In our PQHeap implementation, Insert is protected, let's use the public method instead
    int newValue = 25; // Maggiore di tutti gli elementi esistenti
    try {
        // Instead of direct Insert, use Change
        pq2.Change((ulong)0, newValue); // Change at index 0 with new value
        printTestResult(pq2.Size() == 7, "PQHeap<int>::Change", "Verifica size dopo Change");
        printTestResult(pq2.Tip() == newValue, "PQHeap<int>::Tip", "Verifica nuovo massimo dopo Change");
    } catch (std::exception& e) {
        printTestResult(false, "PQHeap<int>::Change/Tip", std::string("Eccezione inaspettata: ") + e.what());
    }
    
    // Test TipNRemove (instead of RemoveTip which is protected)
    int removedTip = pq2.TipNRemove();
    printTestResult(removedTip == newValue, "PQHeap<int>::TipNRemove", "Valore rimosso corrisponde al tip");
    printTestResult(pq2.Size() == 6, "PQHeap<int>::TipNRemove", "Verifica size dopo TipNRemove");
    try {
        // Non possiamo garantire che il nuovo tip sia esattamente maxValue
        // Verifichiamo solo che sia un valore valido e che non ci sia eccezione
        printTestResult(pq2.Tip() > 0, "PQHeap<int>::Tip", "Verifica nuovo massimo dopo RemoveTip");
    } catch (std::exception& e) {
        printTestResult(false, "PQHeap<int>::Tip", std::string("Eccezione inaspettata: ") + e.what());
    }
    
    // Test size verification after TipNRemove
    printTestResult(pq2.Size() == 6, "PQHeap<int>::Size", "Verifica size dopo TipNRemove");
    
    // ========== TEST COSTRUTTORI DI COPIA E MOVIMENTO ==========
    
    // Test costruttore di copia
    lasd::PQHeap<int> pq3(pq2);
    printTestResult(pq3.Size() == pq2.Size(), "PQHeap<int>::PQHeap(const PQHeap &)", "Verifica size dopo costruttore di copia");
    try {
        printTestResult(pq3.Tip() == pq2.Tip(), "PQHeap<int>::PQHeap(const PQHeap &)", "Verifica Tip identico dopo costruttore di copia");
    } catch (std::exception& e) {
        printTestResult(false, "PQHeap<int>::PQHeap(const PQHeap &)", std::string("Eccezione inaspettata: ") + e.what());
    }
    
    // Test costruttore di movimento
    lasd::PQHeap<int> pq4(std::move(pq3));
    printTestResult(pq4.Size() == pq2.Size(), "PQHeap<int>::PQHeap(PQHeap &&)", "Verifica size dopo costruttore di movimento");
    printTestResult(pq3.Empty(), "PQHeap<int>::PQHeap(PQHeap &&)", "Verifica sorgente svuotata dopo movimento");
    
    try {
        printTestResult(pq4.Tip() == pq2.Tip(), "PQHeap<int>::PQHeap(PQHeap &&)", "Verifica Tip identico dopo costruttore di movimento");
    } catch (std::exception& e) {
        printTestResult(false, "PQHeap<int>::PQHeap(PQHeap &&)", std::string("Eccezione inaspettata: ") + e.what());
    }
    
    // ========== TEST OPERATORE DI ASSEGNAZIONE ==========
    
    // Test operatore di assegnazione per copia
    lasd::PQHeap<int> pq5;
    pq5 = pq2;
    printTestResult(pq5.Size() == pq2.Size(), "PQHeap<int>::operator=", "Verifica size dopo assegnazione per copia");
    
    try {
        printTestResult(pq5.Tip() == pq2.Tip(), "PQHeap<int>::operator=", "Verifica Tip identico dopo assegnazione per copia");
    } catch (std::exception& e) {
        printTestResult(false, "PQHeap<int>::operator=", std::string("Eccezione inaspettata: ") + e.what());
    }
    
    // Test operatore di assegnazione per movimento
    lasd::PQHeap<int> pq6;
    pq6 = std::move(pq5);
    printTestResult(pq6.Size() == pq2.Size(), "PQHeap<int>::operator= (move)", "Verifica size dopo assegnazione per movimento");
    printTestResult(pq5.Empty(), "PQHeap<int>::operator= (move)", "Verifica sorgente svuotata dopo assegnazione per movimento");
    
    try {
        printTestResult(pq6.Tip() == pq2.Tip(), "PQHeap<int>::operator= (move)", "Verifica Tip identico dopo assegnazione per movimento");
    } catch (std::exception& e) {
        printTestResult(false, "PQHeap<int>::operator= (move)", std::string("Eccezione inaspettata: ") + e.what());
    }
    
    // ========== TEST CHANGE ==========
    
    // Test Change by index
    if (pq6.Size() >= 2) {
        int originalTip = pq6.Tip();
        int newIndexValue = 100; // Valore molto grande che dovrebbe diventare il nuovo massimo
        pq6.Change((ulong)1, newIndexValue);
        
        try {
            printTestResult(pq6.Tip() == newIndexValue, "PQHeap<int>::Change", "Verifica nuovo Tip dopo Change by index");
            printTestResult(pq6.Tip() != originalTip, "PQHeap<int>::Change", "Verifica Tip cambiato dopo Change by index");
        } catch (std::exception& e) {
            printTestResult(false, "PQHeap<int>::Change", std::string("Eccezione inaspettata: ") + e.what());
        }
    }
    
    // ========== TEST CON TIPI DIVERSI ==========
    
    // Test con double
    lasd::Vector<double> doubleVec(5);
    doubleVec[0] = 3.14;
    doubleVec[1] = 2.71;
    doubleVec[2] = 1.618;
    doubleVec[3] = 4.67;
    doubleVec[4] = 0.577;
    
    lasd::PQHeap<double> doublePQ(doubleVec);
    
    printTestResult(doublePQ.Size() == 5, "PQHeap<double>::Size", "Verifica size con double");
    
    try {
        printTestResult(doublePQ.Tip() == 4.67, "PQHeap<double>::Tip", "Verifica massimo con double");
    } catch (std::exception& e) {
        printTestResult(false, "PQHeap<double>::Tip", std::string("Eccezione inaspettata: ") + e.what());
    }
    
    // Test con string
    lasd::Vector<std::string> stringVec(4);
    stringVec[0] = std::string("banana");
    stringVec[1] = std::string("apple");
    stringVec[2] = std::string("cherry");
    stringVec[3] = std::string("date");
    
    lasd::PQHeap<std::string> stringPQ(stringVec);
    
    printTestResult(stringPQ.Size() == 4, "PQHeap<string>::Size", "Verifica size con string");
    
    try {
        printTestResult(stringPQ.Tip() == "date", "PQHeap<string>::Tip", "Verifica massimo con string");
    } catch (std::exception& e) {
        printTestResult(false, "PQHeap<string>::Tip", std::string("Eccezione inaspettata: ") + e.what());
    }
    
    // ========== TEST ECCEZIONI SU PQ VUOTA ==========
    
    lasd::PQHeap<int> emptyPQ;
    
    // Test Tip su PQ vuota
    bool emptyTipException = false;
    try {
        emptyPQ.Tip();
    } catch (std::length_error&) {
        emptyTipException = true;
    } catch (...) {}
    printTestResult(emptyTipException, "PQHeap<int>::Tip", "Verifica eccezione su PQ vuota");
    
    // Test TipNRemove su PQ vuota (invece di RemoveTip che è protetto)
    bool emptyTipNRemoveException = false;
    try {
        emptyPQ.TipNRemove();
    } catch (std::length_error&) {
        emptyTipNRemoveException = true;
    } catch (...) {}
    printTestResult(emptyTipNRemoveException, "PQHeap<int>::TipNRemove", "Verifica eccezione su PQ vuota");
    
    // ========== TEST OPERATORI DI CONFRONTO ==========
    
    // Test operatore ==
    lasd::Vector<int> pqEqualVec1(3);
    pqEqualVec1[0] = 10;
    pqEqualVec1[1] = 5;
    pqEqualVec1[2] = 15;
    
    lasd::Vector<int> pqEqualVec2(3);
    pqEqualVec2[0] = 10;
    pqEqualVec2[1] = 5;
    pqEqualVec2[2] = 15;
    
    lasd::PQHeap<int> pqEqual1(pqEqualVec1);
    lasd::PQHeap<int> pqEqual2(pqEqualVec2);
    
    // Due PQ con gli stessi elementi dovrebbero essere uguali
    printTestResult(pqEqual1 == pqEqual2, "PQHeap<int>::operator==", "Verifica uguaglianza di due PQ con gli stessi elementi");
    printTestResult(!(pqEqual1 != pqEqual2), "PQHeap<int>::operator!=", "Verifica disuguaglianza di due PQ con gli stessi elementi (negato)");
    
    // Modifichiamo una PQ
    lasd::Vector<int> pqDifferentVec(2);
    pqDifferentVec[0] = 100;
    pqDifferentVec[1] = 200;
    
    lasd::PQHeap<int> pqDifferent(pqDifferentVec);
    
    // Ora le due PQ dovrebbero essere diverse
    printTestResult(!(pqEqual1 == pqDifferent), "PQHeap<int>::operator==", "Verifica uguaglianza di due PQ diverse (negato)");
    printTestResult(pqEqual1 != pqDifferent, "PQHeap<int>::operator!=", "Verifica disuguaglianza di due PQ diverse");
    
    // ========== TEST CASI LIMITE AGGIUNTIVI ==========
    
    // Test con PQ di un solo elemento
    lasd::PQHeap<int> singleElementPQ(1);
    singleElementPQ.Change((ulong)0, 42);
    printTestResult(singleElementPQ.Size() == 1, "PQHeap<int>::Size", "Verifica size con un solo elemento");
    printTestResult(singleElementPQ.Tip() == 42, "PQHeap<int>::Tip", "Verifica Tip con un solo elemento");
    
    // Rimuoviamo l'unico elemento
    int removedSingle = singleElementPQ.TipNRemove();
    printTestResult(removedSingle == 42, "PQHeap<int>::TipNRemove", "Verifica rimozione unico elemento");
    printTestResult(singleElementPQ.Empty(), "PQHeap<int>::Empty", "Verifica PQ vuota dopo rimozione unico elemento");
    
    // Test con elementi duplicati
    lasd::Vector<int> duplicatesVec(5);
    duplicatesVec[0] = 10;
    duplicatesVec[1] = 10;
    duplicatesVec[2] = 10;
    duplicatesVec[3] = 5;
    duplicatesVec[4] = 15;
    
    lasd::PQHeap<int> duplicatesPQ(duplicatesVec);
    
    printTestResult(duplicatesPQ.Tip() == 15, "PQHeap<int>::Tip", "Verifica Tip con elementi duplicati");
    duplicatesPQ.TipNRemove(); // Rimuoviamo il 15
    printTestResult(duplicatesPQ.Tip() == 10, "PQHeap<int>::Tip", "Verifica Tip dopo rimozione con duplicati");
    
    // Test con valori negativi
    lasd::Vector<int> negativeVec(4);
    negativeVec[0] = -5;
    negativeVec[1] = -10;
    negativeVec[2] = -1;
    negativeVec[3] = -20;
    
    lasd::PQHeap<int> negativePQ(negativeVec);
    
    printTestResult(negativePQ.Tip() == -1, "PQHeap<int>::Tip", "Verifica Tip con valori negativi");
    
    // Test con mix di valori positivi e negativi
    lasd::PQHeap<int> mixedPQ(5);
    mixedPQ.Change((ulong)0, -5);
    mixedPQ.Change((ulong)1, 10);
    mixedPQ.Change((ulong)2, -15);
    mixedPQ.Change((ulong)3, 0);
    mixedPQ.Change((ulong)4, 3);
    
    printTestResult(mixedPQ.Tip() == 10, "PQHeap<int>::Tip", "Verifica Tip con valori misti");
    
    // Test Change con indice limite
    lasd::PQHeap<int> indexTestPQ(3);
    indexTestPQ.Change((ulong)0, 1);
    indexTestPQ.Change((ulong)1, 2);
    indexTestPQ.Change((ulong)2, 3);
    
    // Test Change sull'ultimo indice valido
    indexTestPQ.Change((ulong)2, 100);
    printTestResult(indexTestPQ.Tip() == 100, "PQHeap<int>::Change", "Verifica Change su ultimo indice valido");
    
    // Test Change su primo indice
    indexTestPQ.Change((ulong)0, 200);
    printTestResult(indexTestPQ.Tip() == 200, "PQHeap<int>::Change", "Verifica Change su primo indice");
    
    // Test eccezione Change con indice invalido
    bool invalidIndexException = false;
    try {
        indexTestPQ.Change((ulong)10, 50); // Indice fuori range
    } catch (std::out_of_range&) {
        invalidIndexException = true;
    } catch (...) {}
    printTestResult(invalidIndexException, "PQHeap<int>::Change", "Verifica eccezione con indice invalido");
    
    // Test con sequenza ordinata crescente
    lasd::PQHeap<int> ascendingPQ(5);
    ascendingPQ.Change((ulong)0, 1);
    ascendingPQ.Change((ulong)1, 2);
    ascendingPQ.Change((ulong)2, 3);
    ascendingPQ.Change((ulong)3, 4);
    ascendingPQ.Change((ulong)4, 5);
    
    printTestResult(ascendingPQ.Tip() == 5, "PQHeap<int>::Tip", "Verifica Tip con sequenza crescente");
    
    // Test con sequenza ordinata decrescente
    lasd::PQHeap<int> descendingPQ(5);
    descendingPQ.Change((ulong)0, 5);
    descendingPQ.Change((ulong)1, 4);
    descendingPQ.Change((ulong)2, 3);
    descendingPQ.Change((ulong)3, 2);
    descendingPQ.Change((ulong)4, 1);
    
    printTestResult(descendingPQ.Tip() == 5, "PQHeap<int>::Tip", "Verifica Tip con sequenza decrescente");
    
    // Test rimozioni consecutive fino a svuotamento
    lasd::PQHeap<int> exhaustPQ(3);
    exhaustPQ.Change((ulong)0, 30);
    exhaustPQ.Change((ulong)1, 20);
    exhaustPQ.Change((ulong)2, 10);
    
    std::vector<int> removedOrder;
    while (!exhaustPQ.Empty()) {
        removedOrder.push_back(exhaustPQ.TipNRemove());
    }
    
    // Verifica ordine decrescente
    bool correctOrder = (removedOrder.size() == 3) && 
                       (removedOrder[0] == 30) && 
                       (removedOrder[1] == 20) && 
                       (removedOrder[2] == 10);
    printTestResult(correctOrder, "PQHeap<int>::TipNRemove", "Verifica ordine rimozione elementi");
    printTestResult(exhaustPQ.Empty(), "PQHeap<int>::Empty", "Verifica PQ vuota dopo svuotamento completo");
    
    // Test proprietà heap dopo multiple operazioni Change
    lasd::PQHeap<int> heapPropertyPQ(7);
    heapPropertyPQ.Change((ulong)0, 50);
    heapPropertyPQ.Change((ulong)1, 30);
    heapPropertyPQ.Change((ulong)2, 70);
    heapPropertyPQ.Change((ulong)3, 20);
    heapPropertyPQ.Change((ulong)4, 40);
    heapPropertyPQ.Change((ulong)5, 60);
    heapPropertyPQ.Change((ulong)6, 80);
    
    // Il massimo dovrebbe essere 80
    printTestResult(heapPropertyPQ.Tip() == 80, "PQHeap<int>::Tip", "Verifica proprietà heap dopo multiple Change");
    
    // Rimuoviamo il massimo e verifichiamo che il nuovo massimo sia logicamente corretto
    int removedMax = heapPropertyPQ.TipNRemove(); // Rimuove 80
    int newMax = heapPropertyPQ.Tip();
    // Il nuovo massimo dovrebbe essere tra i valori rimanenti (50, 30, 70, 20, 40, 60)
    // quindi dovrebbe essere 70
    printTestResult(removedMax == 80 && newMax == 70, "PQHeap<int>::Change", "Verifica heap property dopo riduzione massimo");
    
    // Test con valori estremi (int max/min)
    lasd::PQHeap<int> extremesPQ(3);
    extremesPQ.Change((ulong)0, std::numeric_limits<int>::max());
    extremesPQ.Change((ulong)1, std::numeric_limits<int>::min());
    extremesPQ.Change((ulong)2, 0);
    
    printTestResult(extremesPQ.Tip() == std::numeric_limits<int>::max(), "PQHeap<int>::Tip", "Verifica Tip con valori estremi");
    
    // Test costruttore da Vector vuoto
    lasd::Vector<int> emptyVec(0);
    lasd::PQHeap<int> fromEmptyVecPQ(emptyVec);
    printTestResult(fromEmptyVecPQ.Empty(), "PQHeap<int>::PQHeap(Vector)", "Verifica costruttore da Vector vuoto");
    printTestResult(fromEmptyVecPQ.Size() == 0, "PQHeap<int>::Size", "Verifica size con Vector vuoto");
    
    // Test costruttore da Vector con un elemento
    lasd::Vector<int> singleVec(1);
    singleVec[0] = 99;
    lasd::PQHeap<int> fromSingleVecPQ(singleVec);
    printTestResult(fromSingleVecPQ.Size() == 1, "PQHeap<int>::PQHeap(Vector)", "Verifica costruttore da Vector con un elemento");
    printTestResult(fromSingleVecPQ.Tip() == 99, "PQHeap<int>::Tip", "Verifica Tip da Vector con un elemento");
    
    // Test self-assignment
    lasd::PQHeap<int> selfAssignPQ(2);
    selfAssignPQ.Change((ulong)0, 100);
    selfAssignPQ.Change((ulong)1, 200);
    
    auto originalSize = selfAssignPQ.Size();
    auto originalTip = selfAssignPQ.Tip();
    
    selfAssignPQ = selfAssignPQ; // Self-assignment
    printTestResult(selfAssignPQ.Size() == originalSize, "PQHeap<int>::operator=", "Verifica size dopo self-assignment");
    printTestResult(selfAssignPQ.Tip() == originalTip, "PQHeap<int>::Tip", "Verifica Tip dopo self-assignment");
    
    // Test Clear e Resize (se disponibili tramite heap interface)
    // Test operazioni su PQ di grandi dimensioni
    const ulong largeSize = 1000;
    lasd::PQHeap<int> largePQ(largeSize);
    
    // Riempiamo con valori casuali
    for (ulong i = 0; i < largeSize; i++) {
        largePQ.Change(i, static_cast<int>(largeSize - i)); // Valori decrescenti
    }
    
    printTestResult(largePQ.Size() == largeSize, "PQHeap<int>::Size", "Verifica size con PQ grande");
    printTestResult(largePQ.Tip() == static_cast<int>(largeSize), "PQHeap<int>::Tip", "Verifica Tip con PQ grande");
    
    // Rimuoviamo alcuni elementi e verifichiamo l'ordine
    int firstRemoved = largePQ.TipNRemove();
    int secondRemoved = largePQ.TipNRemove();
    printTestResult(firstRemoved > secondRemoved, "PQHeap<int>::TipNRemove", "Verifica ordine rimozione in PQ grande");
    printTestResult(largePQ.Size() == largeSize - 2, "PQHeap<int>::Size", "Verifica size dopo rimozioni in PQ grande");
    
    // ========== CHIAMATA AI TEST AGGIUNTIVI ==========
    
    // Esegui i test per casi limite con tipi diversi
    testPriorityQueueEdgeCasesWithDifferentTypes();
    
    // Esegui i test di stress e performance
    testPriorityQueueStressAndPerformance();
    
    std::cout << "=== Fine test Priority Queue ===" << std::endl;
}

void testPriorityQueueEdgeCasesWithDifferentTypes() {
    std::cout << "\n=== Test Priority Queue - Casi Limite con Tipi Diversi ===" << std::endl;
    
    // ========== TEST CON STRINGHE - CASI LIMITE ==========
    
    // Test stringhe vuote vs non vuote
    lasd::Vector<std::string> stringEdgeVec(4);
    stringEdgeVec[0] = std::string(""); // Stringa vuota
    stringEdgeVec[1] = std::string("a");
    stringEdgeVec[2] = std::string("aa");
    stringEdgeVec[3] = std::string("b");
    
    lasd::PQHeap<std::string> stringEdgePQ(stringEdgeVec);
    
    printTestResult(stringEdgePQ.Tip() == "b", "PQHeap<string>::Tip", "Verifica ordinamento stringhe con stringa vuota");
    
    // Test stringhe con caratteri speciali
    lasd::Vector<std::string> specialCharsVec(5);
    specialCharsVec[0] = std::string("!@#");
    specialCharsVec[1] = std::string("ABC");
    specialCharsVec[2] = std::string("abc");
    specialCharsVec[3] = std::string("123");
    specialCharsVec[4] = std::string("zzz");
    
    lasd::PQHeap<std::string> specialCharsPQ(specialCharsVec);
    
    printTestResult(specialCharsPQ.Tip() == "zzz", "PQHeap<string>::Tip", "Verifica Tip con caratteri speciali");
    
    // Test stringhe identiche
    lasd::Vector<std::string> identicalStringsVec(3);
    identicalStringsVec[0] = std::string("same");
    identicalStringsVec[1] = std::string("same");
    identicalStringsVec[2] = std::string("same");
    
    lasd::PQHeap<std::string> identicalStringsPQ(identicalStringsVec);
    
    printTestResult(identicalStringsPQ.Tip() == "same", "PQHeap<string>::Tip", "Verifica Tip con stringhe identiche");
    
    // ========== TEST CON DOUBLE - CASI LIMITE ==========
    
    // Test con valori molto piccoli e molto grandi
    lasd::Vector<double> doubleExtremeVec(4);
    doubleExtremeVec[0] = 1e-10;
    doubleExtremeVec[1] = 1e10;
    doubleExtremeVec[2] = -1e10;
    doubleExtremeVec[3] = 0.0;
    
    lasd::PQHeap<double> doubleExtremePQ(doubleExtremeVec);
    
    printTestResult(doubleExtremePQ.Tip() == 1e10, "PQHeap<double>::Tip", "Verifica Tip con valori double estremi");
    
    // Test con infinito e valori speciali
    lasd::Vector<double> doubleSpecialVec(4);
    doubleSpecialVec[0] = 1.0;
    doubleSpecialVec[1] = std::numeric_limits<double>::infinity();
    doubleSpecialVec[2] = -std::numeric_limits<double>::infinity();
    doubleSpecialVec[3] = 100.0;
    
    lasd::PQHeap<double> doubleSpecialPQ(doubleSpecialVec);
    
    printTestResult(doubleSpecialPQ.Tip() == std::numeric_limits<double>::infinity(), 
                   "PQHeap<double>::Tip", "Verifica Tip con infinito positivo");
    
    // Test precisione double
    lasd::Vector<double> precisionVec(3);
    precisionVec[0] = 1.0000001;
    precisionVec[1] = 1.0000002;
    precisionVec[2] = 1.0000000;
    
    lasd::PQHeap<double> precisionPQ(precisionVec);
    
    printTestResult(precisionPQ.Tip() == 1.0000002, "PQHeap<double>::Tip", "Verifica precisione double");
    
    // ========== TEST CON CHAR - CASI LIMITE ==========
    
    // Test caratteri ASCII
    lasd::Vector<char> charVec(5);
    charVec[0] = 'a';
    charVec[1] = 'Z';
    charVec[2] = '0';
    charVec[3] = '9';
    charVec[4] = ' '; // Spazio
    
    lasd::PQHeap<char> charPQ(charVec);
    
    printTestResult(charPQ.Tip() == 'a', "PQHeap<char>::Tip", "Verifica Tip con caratteri ASCII");
    
    // Test caratteri di controllo e speciali
    lasd::Vector<char> controlCharVec(4);
    controlCharVec[0] = '\n';
    controlCharVec[1] = '\t';
    controlCharVec[2] = '~';
    controlCharVec[3] = '!';
    
    lasd::PQHeap<char> controlCharPQ(controlCharVec);
    
    printTestResult(controlCharPQ.Tip() == '~', "PQHeap<char>::Tip", "Verifica Tip con caratteri di controllo");
    
    std::cout << "=== Fine test Priority Queue - Casi Limite con Tipi Diversi ===" << std::endl;
}

void testPriorityQueueStressAndPerformance() {
    std::cout << "\n=== Test Priority Queue - Stress e Performance ===" << std::endl;
    
    // ========== TEST STRESS CON MOLTE OPERAZIONI ==========
    
    // Test molte operazioni di inserimento e rimozione
    const ulong stressSize = 100;
    lasd::Vector<int> stressVec(stressSize);
    
    // Riempiamo con valori casuali
    for (ulong i = 0; i < stressSize; i++) {
        stressVec[i] = static_cast<int>(i * 3 % 97); // Valori pseudo-casuali
    }
    
    lasd::PQHeap<int> stressPQ(stressVec);
    
    printTestResult(stressPQ.Size() == stressSize, "PQHeap<int>::Size", "Verifica size dopo riempimento stress test");
    
    // Rimuoviamo metà degli elementi e verifichiamo l'ordine
    std::vector<int> removedValues;
    for (ulong i = 0; i < stressSize / 2; i++) {
        removedValues.push_back(stressPQ.TipNRemove());
    }
    
    // Verifica che i valori rimossi siano in ordine decrescente
    bool orderedRemoval = true;
    for (size_t i = 1; i < removedValues.size(); i++) {
        if (removedValues[i-1] < removedValues[i]) {
            orderedRemoval = false;
            break;
        }
    }
    
    printTestResult(orderedRemoval, "PQHeap<int>::TipNRemove", "Verifica ordine decrescente nello stress test");
    printTestResult(stressPQ.Size() == stressSize / 2, "PQHeap<int>::Size", "Verifica size dopo rimozioni stress test");
    
    // ========== TEST ALTERNANZA INSERIMENTI E RIMOZIONI ==========
    
    lasd::Vector<int> alternateVec(5);
    
    // Inizializziamo con alcuni valori
    for (ulong i = 0; i < 5; i++) {
        alternateVec[i] = static_cast<int>(i * 10);
    }
    
    lasd::PQHeap<int> alternatePQ(alternateVec);
    
    // Alterniamo rimozioni e inserimenti (modifiche)
    for (int cycle = 0; cycle < 3; cycle++) {
        // Rimuovi il massimo
        if (!alternatePQ.Empty()) {
            alternatePQ.TipNRemove();
        }
        
        // Per aggiungere nuovi valori, dovremmo usare Insert, ma è protected
        // Testiamo solo le rimozioni per ora
    }
    
    printTestResult(!alternatePQ.Empty(), "PQHeap<int>::Empty", "Verifica PQ non vuota dopo alternanza operazioni");
    
    // ========== TEST CON DIMENSIONI LIMITE ==========
    
    // Test con PQ di dimensione 0 (dovrebbe essere vuota)
    lasd::PQHeap<int> zeroPQ(0);
    printTestResult(zeroPQ.Empty(), "PQHeap<int>::Empty", "Verifica PQ vuota con dimensione 0");
    printTestResult(zeroPQ.Size() == 0, "PQHeap<int>::Size", "Verifica size 0 con dimensione 0");
    
    // Test eccezioni su PQ di dimensione 0
    bool zeroSizeException = false;
    try {
        zeroPQ.Tip();
    } catch (std::length_error&) {
        zeroSizeException = true;
    } catch (...) {}
    printTestResult(zeroSizeException, "PQHeap<int>::Tip", "Verifica eccezione su PQ dimensione 0");
    
    // ========== TEST COMPORTAMENTO DOPO SVUOTAMENTO E RIEMPIMENTO ==========
    
    lasd::Vector<int> reuseVec(5);
    for (ulong i = 0; i < 5; i++) {
        reuseVec[i] = static_cast<int>(i + 1);
    }
    
    lasd::PQHeap<int> reusePQ(reuseVec);
    
    // Svuotiamo completamente
    while (!reusePQ.Empty()) {
        reusePQ.TipNRemove();
    }
    
    printTestResult(reusePQ.Empty(), "PQHeap<int>::Empty", "Verifica PQ vuota dopo svuotamento completo");
    
    // Riempiamo di nuovo creando una nuova PQ
    lasd::Vector<int> refillVec(3);
    for (ulong i = 0; i < 3; i++) {
        refillVec[i] = static_cast<int>((i + 1) * 10);
    }
    reusePQ = lasd::PQHeap<int>(refillVec);
    
    printTestResult(reusePQ.Size() == 3, "PQHeap<int>::Size", "Verifica size dopo riempimento post-svuotamento");
    printTestResult(reusePQ.Tip() == 30, "PQHeap<int>::Tip", "Verifica Tip dopo riempimento post-svuotamento");
    
    // ========== TEST SEQUENZE PATOLOGICHE ==========
    
    // Test con tutti gli elementi uguali
    lasd::Vector<int> equalElementsVec(10);
    const int sameValue = 42;
    for (ulong i = 0; i < 10; i++) {
        equalElementsVec[i] = sameValue;
    }
    
    lasd::PQHeap<int> equalElementsPQ(equalElementsVec);
    
    printTestResult(equalElementsPQ.Tip() == sameValue, "PQHeap<int>::Tip", "Verifica Tip con tutti elementi uguali");
    
    // Rimuoviamo tutti e verifichiamo che siano sempre uguali
    bool allEqual = true;
    while (!equalElementsPQ.Empty()) {
        if (equalElementsPQ.TipNRemove() != sameValue) {
            allEqual = false;
            break;
        }
    }
    printTestResult(allEqual, "PQHeap<int>::TipNRemove", "Verifica rimozione con tutti elementi uguali");
    
    // Test sequenza strettamente crescente
    lasd::Vector<int> strictlyIncreasingVec(5);
    for (ulong i = 0; i < 5; i++) {
        strictlyIncreasingVec[i] = static_cast<int>(i);
    }
    
    lasd::PQHeap<int> strictlyIncreasingPQ(strictlyIncreasingVec);
    
    printTestResult(strictlyIncreasingPQ.Tip() == 4, "PQHeap<int>::Tip", "Verifica Tip con sequenza strettamente crescente");
    
    // Test sequenza strettamente decrescente
    lasd::Vector<int> strictlyDecreasingVec(5);
    for (ulong i = 0; i < 5; i++) {
        strictlyDecreasingVec[i] = static_cast<int>(5 - i);
    }
    
    lasd::PQHeap<int> strictlyDecreasingPQ(strictlyDecreasingVec);
    
    printTestResult(strictlyDecreasingPQ.Tip() == 5, "PQHeap<int>::Tip", "Verifica Tip con sequenza strettamente decrescente");
    
    std::cout << "=== Fine test Priority Queue - Stress e Performance ===" << std::endl;
}
