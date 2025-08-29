#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include <algorithm>
#include "test.hpp"
#include "../heap/vec/heapvec.hpp"

void testHeap() {
    std::cout << "\n=== Inizio test Heap ===" << std::endl;

    // ========== TEST COSTRUTTORI E OPERAZIONI DI BASE ==========
    
    // Test costruttore default
    lasd::HeapVec<int> heap1;
    printTestResult(heap1.Empty(), "HeapVec<int>::Empty", "Verifica heap vuoto dopo costruttore default");
    printTestResult(heap1.Size() == 0, "HeapVec<int>::Size", "Verifica size 0 dopo costruttore default");

    // Creiamo un heap con alcuni valori iniziali
    lasd::Vector<int> vec(7);
    vec[0] = 10;
    vec[1] = 5;
    vec[2] = 15;
    vec[3] = 2;
    vec[4] = 8;
    vec[5] = 20;
    vec[6] = 3;
    
    lasd::HeapVec<int> heap2(vec);
    
    // Test proprietà base
    printTestResult(!heap2.Empty(), "HeapVec<int>::Empty", "Verifica heap non vuoto dopo costruzione da Vector");
    printTestResult(heap2.Size() == 7, "HeapVec<int>::Size", "Verifica size dopo costruzione da Vector");
    
    // Test IsHeap - verifica che la proprietà heap sia mantenuta
    bool isHeap = true;
    for (ulong i = 0; i < heap2.Size(); i++) {
        ulong leftChild = 2 * i + 1;
        ulong rightChild = 2 * i + 2;
        
        if (leftChild < heap2.Size() && heap2[i] < heap2[leftChild]) {
            isHeap = false;
            break;
        }
        
        if (rightChild < heap2.Size() && heap2[i] < heap2[rightChild]) {
            isHeap = false;
            break;
        }
    }
    printTestResult(isHeap, "HeapVec<int>::IsHeap", "Verifica proprietà heap dopo costruzione");
    
    // ========== TEST COSTRUTTORI DI COPIA E MOVIMENTO ==========
    
    // Test costruttore di copia
    lasd::HeapVec<int> heap3(heap2);
    printTestResult(heap3.Size() == heap2.Size(), "HeapVec<int>::HeapVec(const HeapVec &)", "Verifica size dopo costruttore di copia");
    bool equalElements = true;
    for (ulong i = 0; i < heap2.Size() && i < heap3.Size(); i++) {
        if (heap2[i] != heap3[i]) {
            equalElements = false;
            break;
        }
    }
    printTestResult(equalElements, "HeapVec<int>::HeapVec(const HeapVec &)", "Verifica elementi identici dopo costruttore di copia");
    
    // Test costruttore di movimento
    lasd::HeapVec<int> heap4(std::move(heap3));
    printTestResult(heap4.Size() == heap2.Size(), "HeapVec<int>::HeapVec(HeapVec &&)", "Verifica size dopo costruttore di movimento");
    printTestResult(heap3.Empty(), "HeapVec<int>::HeapVec(HeapVec &&)", "Verifica sorgente svuotata dopo movimento");
    
    equalElements = true;
    for (ulong i = 0; i < heap2.Size() && i < heap4.Size(); i++) {
        if (heap2[i] != heap4[i]) {
            equalElements = false;
            break;
        }
    }
    printTestResult(equalElements, "HeapVec<int>::HeapVec(HeapVec &&)", "Verifica elementi identici dopo costruttore di movimento");
    
    // ========== TEST OPERATORE DI ASSEGNAZIONE ==========
    
    // Test operatore di assegnazione per copia
    lasd::HeapVec<int> heap5;
    heap5 = heap2;
    printTestResult(heap5.Size() == heap2.Size(), "HeapVec<int>::operator=", "Verifica size dopo assegnazione per copia");
    equalElements = true;
    for (ulong i = 0; i < heap2.Size() && i < heap5.Size(); i++) {
        if (heap2[i] != heap5[i]) {
            equalElements = false;
            break;
        }
    }
    printTestResult(equalElements, "HeapVec<int>::operator=", "Verifica elementi identici dopo assegnazione per copia");
    
    // Test operatore di assegnazione per movimento
    lasd::HeapVec<int> heap6;
    heap6 = std::move(heap5);
    printTestResult(heap6.Size() == heap2.Size(), "HeapVec<int>::operator= (move)", "Verifica size dopo assegnazione per movimento");
    printTestResult(heap5.Empty(), "HeapVec<int>::operator= (move)", "Verifica sorgente svuotata dopo assegnazione per movimento");
    
    // ========== TEST SORT ==========
    
    heap6.Sort();
    bool isSorted = true;
    for (ulong i = 0; i < heap6.Size() - 1; i++) {
        if (heap6[i] > heap6[i + 1]) {
            isSorted = false;
            break;
        }
    }
    printTestResult(isSorted, "HeapVec<int>::Sort", "Verifica ordinamento dopo Sort");
    
    // ========== TEST CON TIPI DIVERSI ==========
    
    // Test con double
    lasd::HeapVec<double> doubleHeap;
    lasd::Vector<double> doubleVec(5);
    doubleVec[0] = 3.14;
    doubleVec[1] = 2.71;
    doubleVec[2] = 1.618;
    doubleVec[3] = 4.67;
    doubleVec[4] = 0.577;
    
    lasd::HeapVec<double> doubleHeap2(doubleVec);
    printTestResult(doubleHeap2.Size() == 5, "HeapVec<double>::Size", "Verifica size con double");
    
    // Test IsHeap per double
    isHeap = true;
    for (ulong i = 0; i < doubleHeap2.Size(); i++) {
        ulong leftChild = 2 * i + 1;
        ulong rightChild = 2 * i + 2;
        
        if (leftChild < doubleHeap2.Size() && doubleHeap2[i] < doubleHeap2[leftChild]) {
            isHeap = false;
            break;
        }
        
        if (rightChild < doubleHeap2.Size() && doubleHeap2[i] < doubleHeap2[rightChild]) {
            isHeap = false;
            break;
        }
    }
    printTestResult(isHeap, "HeapVec<double>::IsHeap", "Verifica proprietà heap con double");
    
    // Test con string
    lasd::Vector<std::string> stringVec(4);
    stringVec[0] = "banana";
    stringVec[1] = "apple";
    stringVec[2] = "cherry";
    stringVec[3] = "date";
    
    lasd::HeapVec<std::string> stringHeap(stringVec);
    printTestResult(stringHeap.Size() == 4, "HeapVec<string>::Size", "Verifica size con string");
    
    // Test IsHeap per string
    isHeap = true;
    for (ulong i = 0; i < stringHeap.Size(); i++) {
        ulong leftChild = 2 * i + 1;
        ulong rightChild = 2 * i + 2;
        
        if (leftChild < stringHeap.Size() && stringHeap[i] < stringHeap[leftChild]) {
            isHeap = false;
            break;
        }
        
        if (rightChild < stringHeap.Size() && stringHeap[i] < stringHeap[rightChild]) {
            isHeap = false;
            break;
        }
    }
    printTestResult(isHeap, "HeapVec<string>::IsHeap", "Verifica proprietà heap con string");
    
    // ========== TEST CASE LIMITE ==========
    
    // Test heap con un elemento
    lasd::HeapVec<int> singleElementHeap;
    lasd::Vector<int> singleElementVec(1);
    singleElementVec[0] = 42;
    
    lasd::HeapVec<int> singleElementHeap2(singleElementVec);
    printTestResult(singleElementHeap2.Size() == 1, "HeapVec<int>::Size", "Verifica size con un elemento");
    printTestResult(singleElementHeap2[0] == 42, "HeapVec<int>::operator[]", "Verifica valore con un elemento");
    printTestResult(singleElementHeap2.IsHeap(), "HeapVec<int>::IsHeap", "Verifica proprietà heap con un elemento");
    
    // ========== TEST OPERATORI DI CONFRONTO ==========
    
    // Test operatore ==
    lasd::HeapVec<int> heapEqual1;
    lasd::HeapVec<int> heapEqual2;
    
    // Popoliamo con gli stessi valori
    lasd::Vector<int> vecEqual(5);
    vecEqual[0] = 10;
    vecEqual[1] = 5;
    vecEqual[2] = 15;
    vecEqual[3] = 3;
    vecEqual[4] = 7;
    
    heapEqual1 = lasd::HeapVec<int>(vecEqual);
    heapEqual2 = lasd::HeapVec<int>(vecEqual);
    
    // Due heap con gli stessi elementi in origine dovrebbero essere uguali 
    // (anche se potrebbero avere strutture interne diverse a seconda dell'ordine di inserimento)
    printTestResult(heapEqual1 == heapEqual2, "HeapVec<int>::operator==", "Verifica uguaglianza di due heap con gli stessi elementi");
    printTestResult(!(heapEqual1 != heapEqual2), "HeapVec<int>::operator!=", "Verifica disuguaglianza di due heap con gli stessi elementi (negato)");
    
    // Modifichiamo uno degli heap
    lasd::HeapVec<int> heapDifferent(heapEqual1);
    lasd::Vector<int> vecDifferent(1);
    vecDifferent[0] = 100;
    heapDifferent = lasd::HeapVec<int>(vecDifferent);
    
    // Ora i due heap dovrebbero essere diversi
    printTestResult(!(heapEqual1 == heapDifferent), "HeapVec<int>::operator==", "Verifica uguaglianza di due heap diversi (negato)");
    printTestResult(heapEqual1 != heapDifferent, "HeapVec<int>::operator!=", "Verifica disuguaglianza di due heap diversi");
    
    // ========== ESECUZIONE TEST AGGIUNTIVI ==========
    
    // Esegui test per casi limite
    testHeapEdgeCases();
    
    // Esegui test per tipi di dati diversi
    testHeapDataTypes();
    
    // Esegui test di stress
    testHeapStress();
    
    std::cout << "=== Fine test Heap ===" << std::endl;
}

// Funzione per testare casi limite dell'heap
void testHeapEdgeCases() {
    std::cout << "\n=== Test Heap - Casi Limite ===" << std::endl;
    
    // ========== TEST HEAP VUOTO ==========
    
    lasd::HeapVec<int> emptyHeap;
    
    // Test operazioni su heap vuoto
    printTestResult(emptyHeap.Empty(), "HeapVec<int>::Empty", "Heap vuoto - verifica Empty()");
    printTestResult(emptyHeap.Size() == 0, "HeapVec<int>::Size", "Heap vuoto - verifica Size()");
    
    // Test eccezioni su heap vuoto
    bool exceptionThrown = false;
    try {
        emptyHeap[0]; // Accesso fuori range
    } catch (std::out_of_range&) {
        exceptionThrown = true;
    } catch (...) {}
    printTestResult(exceptionThrown, "HeapVec<int>::operator[]", "Heap vuoto - eccezione su accesso indice 0");
    
    // ========== TEST HEAP CON ELEMENTI DUPLICATI ==========
    
    lasd::Vector<int> duplicateVec(6);
    duplicateVec[0] = 5;
    duplicateVec[1] = 5;
    duplicateVec[2] = 5;
    duplicateVec[3] = 3;
    duplicateVec[4] = 3;
    duplicateVec[5] = 7;
    
    lasd::HeapVec<int> duplicateHeap(duplicateVec);
    
    printTestResult(duplicateHeap.Size() == 6, "HeapVec<int>::Size", "Heap con duplicati - verifica size");
    
    // Verifica proprietà heap con duplicati
    bool isHeapWithDuplicates = true;
    for (ulong i = 0; i < duplicateHeap.Size(); i++) {
        ulong leftChild = 2 * i + 1;
        ulong rightChild = 2 * i + 2;
        
        if (leftChild < duplicateHeap.Size() && duplicateHeap[i] < duplicateHeap[leftChild]) {
            isHeapWithDuplicates = false;
            break;
        }
        
        if (rightChild < duplicateHeap.Size() && duplicateHeap[i] < duplicateHeap[rightChild]) {
            isHeapWithDuplicates = false;
            break;
        }
    }
    printTestResult(isHeapWithDuplicates, "HeapVec<int>::IsHeap", "Heap con duplicati - verifica proprietà heap");
    
    // ========== TEST HEAP GIÀ ORDINATO (CASO MIGLIORE) ==========
    
    lasd::Vector<int> sortedVec(5);
    sortedVec[0] = 1;
    sortedVec[1] = 2;
    sortedVec[2] = 3;
    sortedVec[3] = 4;
    sortedVec[4] = 5;
    
    lasd::HeapVec<int> sortedHeap(sortedVec);
    printTestResult(sortedHeap.Size() == 5, "HeapVec<int>::Size", "Heap da array ordinato - verifica size");
    
    // Verifica che sia ancora un heap valido
    bool isSortedHeap = true;
    for (ulong i = 0; i < sortedHeap.Size(); i++) {
        ulong leftChild = 2 * i + 1;
        ulong rightChild = 2 * i + 2;
        
        if (leftChild < sortedHeap.Size() && sortedHeap[i] < sortedHeap[leftChild]) {
            isSortedHeap = false;
            break;
        }
        
        if (rightChild < sortedHeap.Size() && sortedHeap[i] < sortedHeap[rightChild]) {
            isSortedHeap = false;
            break;
        }
    }
    printTestResult(isSortedHeap, "HeapVec<int>::IsHeap", "Heap da array ordinato - verifica proprietà heap");
    
    // ========== TEST HEAP INVERSAMENTE ORDINATO (CASO PEGGIORE) ==========
    
    lasd::Vector<int> reverseSortedVec(5);
    reverseSortedVec[0] = 5;
    reverseSortedVec[1] = 4;
    reverseSortedVec[2] = 3;
    reverseSortedVec[3] = 2;
    reverseSortedVec[4] = 1;
    
    lasd::HeapVec<int> reverseSortedHeap(reverseSortedVec);
    printTestResult(reverseSortedHeap.Size() == 5, "HeapVec<int>::Size", "Heap da array inverso - verifica size");
    
    // Verifica che sia un heap valido
    bool isReverseSortedHeap = true;
    for (ulong i = 0; i < reverseSortedHeap.Size(); i++) {
        ulong leftChild = 2 * i + 1;
        ulong rightChild = 2 * i + 2;
        
        if (leftChild < reverseSortedHeap.Size() && reverseSortedHeap[i] < reverseSortedHeap[leftChild]) {
            isReverseSortedHeap = false;
            break;
        }
        
        if (rightChild < reverseSortedHeap.Size() && reverseSortedHeap[i] < reverseSortedHeap[rightChild]) {
            isReverseSortedHeap = false;
            break;
        }
    }
    printTestResult(isReverseSortedHeap, "HeapVec<int>::IsHeap", "Heap da array inverso - verifica proprietà heap");
    
    // ========== TEST VALORI ESTREMI ==========
    
    lasd::Vector<int> extremeVec(4);
    extremeVec[0] = std::numeric_limits<int>::max();
    extremeVec[1] = std::numeric_limits<int>::min();
    extremeVec[2] = 0;
    extremeVec[3] = -1;
    
    lasd::HeapVec<int> extremeHeap(extremeVec);
    printTestResult(extremeHeap.Size() == 4, "HeapVec<int>::Size", "Heap con valori estremi - verifica size");
    
    // Verifica che il massimo sia in cima
    printTestResult(extremeHeap[0] == std::numeric_limits<int>::max(), "HeapVec<int>::operator[]", "Heap con valori estremi - massimo in cima");
    
    // ========== TEST SORT SU CASI PARTICOLARI ==========
    
    // Test sort su heap con un elemento
    lasd::HeapVec<int> singleElementHeap;
    lasd::Vector<int> singleVec(1);
    singleVec[0] = 42;
    singleElementHeap = lasd::HeapVec<int>(singleVec);
    
    singleElementHeap.Sort();
    printTestResult(singleElementHeap.Size() == 1, "HeapVec<int>::Sort", "Sort su heap con un elemento - verifica size");
    printTestResult(singleElementHeap[0] == 42, "HeapVec<int>::Sort", "Sort su heap con un elemento - verifica valore");
    
    // Test sort su heap vuoto
    lasd::HeapVec<int> emptyHeapForSort;
    emptyHeapForSort.Sort(); // Non dovrebbe causare eccezioni
    printTestResult(emptyHeapForSort.Empty(), "HeapVec<int>::Sort", "Sort su heap vuoto - rimane vuoto");
    
    // Test sort su heap con duplicati
    lasd::HeapVec<int> duplicateHeapForSort(duplicateVec);
    duplicateHeapForSort.Sort();
    
    bool isSortedWithDuplicates = true;
    for (ulong i = 0; i < duplicateHeapForSort.Size() - 1; i++) {
        if (duplicateHeapForSort[i] > duplicateHeapForSort[i + 1]) {
            isSortedWithDuplicates = false;
            break;
        }
    }
    printTestResult(isSortedWithDuplicates, "HeapVec<int>::Sort", "Sort su heap con duplicati - verifica ordinamento");
    
    std::cout << "=== Fine test Heap - Casi Limite ===" << std::endl;
}

// Funzione per testare diversi tipi di dati con casi limite
void testHeapDataTypes() {
    std::cout << "\n=== Test Heap - Tipi di Dati Diversi ===" << std::endl;
    
    // ========== TEST CON DOUBLE E PRECISIONE ==========
    
    lasd::Vector<double> precisionVec(4);
    precisionVec[0] = 1.0000001;
    precisionVec[1] = 1.0000002;
    precisionVec[2] = 1.0;
    precisionVec[3] = 0.9999999;
    
    lasd::HeapVec<double> precisionHeap(precisionVec);
    printTestResult(precisionHeap.Size() == 4, "HeapVec<double>::Size", "Heap con double precisione - verifica size");
    
    // Verifica ordinamento corretto con precisione
    precisionHeap.Sort();
    bool correctPrecisionOrder = true;
    for (ulong i = 0; i < precisionHeap.Size() - 1; i++) {
        if (precisionHeap[i] > precisionHeap[i + 1]) {
            correctPrecisionOrder = false;
            break;
        }
    }
    printTestResult(correctPrecisionOrder, "HeapVec<double>::Sort", "Heap con double precisione - ordinamento corretto");
    
    // ========== TEST CON STRING VUOTE E SPECIALI ==========
    
    lasd::Vector<std::string> specialStringVec(5);
    specialStringVec[0] = "";           // String vuota
    specialStringVec[1] = " ";          // Spazio
    specialStringVec[2] = "a";          // Singolo carattere
    specialStringVec[3] = "aa";         // Caratteri ripetuti
    specialStringVec[4] = "z";          // Carattere finale alfabeto
    
    lasd::HeapVec<std::string> specialStringHeap(specialStringVec);
    printTestResult(specialStringHeap.Size() == 5, "HeapVec<string>::Size", "Heap con string speciali - verifica size");
    
    // Test sort
    specialStringHeap.Sort();
    bool correctStringOrder = true;
    for (ulong i = 0; i < specialStringHeap.Size() - 1; i++) {
        if (specialStringHeap[i] > specialStringHeap[i + 1]) {
            correctStringOrder = false;
            break;
        }
    }
    printTestResult(correctStringOrder, "HeapVec<string>::Sort", "Heap con string speciali - ordinamento lessicografico");
    
    // ========== TEST CON CHAR ==========
    
    lasd::Vector<char> charVec(5);
    charVec[0] = 'z';
    charVec[1] = 'a';
    charVec[2] = 'M';  // Maiuscola
    charVec[3] = 'm';  // Minuscola
    charVec[4] = '1';  // Numero
    
    lasd::HeapVec<char> charHeap(charVec);
    printTestResult(charHeap.Size() == 5, "HeapVec<char>::Size", "Heap con char - verifica size");
    
    // Test sort con caratteri
    charHeap.Sort();
    bool correctCharOrder = true;
    for (ulong i = 0; i < charHeap.Size() - 1; i++) {
        if (charHeap[i] > charHeap[i + 1]) {
            correctCharOrder = false;
            break;
        }
    }
    printTestResult(correctCharOrder, "HeapVec<char>::Sort", "Heap con char - ordinamento ASCII");
    
    std::cout << "=== Fine test Heap - Tipi di Dati Diversi ===" << std::endl;
}

// Funzione per testare performance e stress
void testHeapStress() {
    std::cout << "\n=== Test Heap - Stress e Performance ===" << std::endl;
    
    // ========== TEST CON MOLTI ELEMENTI ==========
    
    const ulong largeSize = 1000;
    lasd::Vector<int> largeVec(largeSize);
    
    // Riempi con valori casuali simulati
    for (ulong i = 0; i < largeSize; i++) {
        largeVec[i] = (int)(largeSize - i); // Ordine decrescente per test peggiore
    }
    
    lasd::HeapVec<int> largeHeap(largeVec);
    printTestResult(largeHeap.Size() == largeSize, "HeapVec<int>::Size", "Heap grande (1000 elementi) - verifica size");
    
    // Verifica proprietà heap su heap grande
    bool isLargeHeap = true;
    for (ulong i = 0; i < std::min(largeHeap.Size(), (ulong)100); i++) { // Controllo solo primi 100 per performance
        ulong leftChild = 2 * i + 1;
        ulong rightChild = 2 * i + 2;
        
        if (leftChild < largeHeap.Size() && largeHeap[i] < largeHeap[leftChild]) {
            isLargeHeap = false;
            break;
        }
        
        if (rightChild < largeHeap.Size() && largeHeap[i] < largeHeap[rightChild]) {
            isLargeHeap = false;
            break;
        }
    }
    printTestResult(isLargeHeap, "HeapVec<int>::IsHeap", "Heap grande - verifica proprietà heap (primi 100 elementi)");
    
    // Test sort su heap grande
    largeHeap.Sort();
    
    // Verifica ordinamento (controllo ogni 100 elementi per performance)
    bool isLargeSorted = true;
    for (ulong i = 0; i < largeHeap.Size() - 100; i += 100) {
        if (largeHeap[i] > largeHeap[i + 100]) {
            isLargeSorted = false;
            break;
        }
    }
    printTestResult(isLargeSorted, "HeapVec<int>::Sort", "Heap grande - verifica ordinamento (ogni 100 elementi)");
    
    // ========== TEST OPERAZIONI MULTIPLE ==========
    
    lasd::HeapVec<int> multiOpHeap;
    lasd::Vector<int> baseVec(10);
    for (ulong i = 0; i < 10; i++) {
        baseVec[i] = (int)(i * 3 + 1);
    }
    multiOpHeap = lasd::HeapVec<int>(baseVec);
    
    // Test copia multipla
    lasd::HeapVec<int> copy1(multiOpHeap);
    lasd::HeapVec<int> copy2(copy1);
    lasd::HeapVec<int> copy3(copy2);
    
    printTestResult(copy3.Size() == multiOpHeap.Size(), "HeapVec<int>::HeapVec(const HeapVec &)", "Copia multipla - verifica size finale");
    printTestResult(copy3 == multiOpHeap, "HeapVec<int>::operator==", "Copia multipla - verifica uguaglianza");
    
    // Test movimento multiplo
    lasd::HeapVec<int> move1(std::move(copy1));
    lasd::HeapVec<int> move2(std::move(move1));
    lasd::HeapVec<int> move3(std::move(move2));
    
    printTestResult(move3.Size() == multiOpHeap.Size(), "HeapVec<int>::HeapVec(HeapVec &&)", "Movimento multiplo - verifica size finale");
    printTestResult(copy1.Empty() && move1.Empty() && move2.Empty(), "HeapVec<int>::HeapVec(HeapVec &&)", "Movimento multiplo - sorgenti vuote");
    
    std::cout << "=== Fine test Heap - Stress e Performance ===" << std::endl;
}
