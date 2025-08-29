#include "test.hpp"
#include "../list/list.hpp"
#include "../vector/vector.hpp" // For testing constructor from TraversableContainer
#include <iostream>
#include <string>      // For std::to_string and lasd::List<std::string>
#include <stdexcept>   // For std::length_error and std::out_of_range
#include <functional>  // For std::function in Fold

// void testList(); // Declaration is in test.hpp

void testList() {
    std::cout << "\n=== Inizio test List ===" << std::endl;

    // Test costruttore di default
    lasd::List<int> l1;
    printTestResult(l1.Empty(), "List<int>::Empty", "Test Empty() su lista appena creata (default)");
    printTestResult(l1.Size() == 0, "List<int>::Size", "Test Size() su lista appena creata (default)");

    // Test InsertAtFront e Front
    l1.InsertAtFront(10);
    printTestResult(!l1.Empty(), "List<int>::Empty", "Test Empty() dopo InsertAtFront");
    printTestResult(l1.Size() == 1, "List<int>::Size", "Test Size() dopo InsertAtFront");
    printTestResult(l1.Front() == 10, "List<int>::Front", "Test Front() dopo InsertAtFront");

    l1.InsertAtFront(20);
    printTestResult(l1.Size() == 2, "List<int>::Size", "Test Size() dopo secondo InsertAtFront");
    printTestResult(l1.Front() == 20, "List<int>::Front", "Test Front() dopo secondo InsertAtFront");
    printTestResult(l1.Back() == 10, "List<int>::Back", "Test Back() dopo secondo InsertAtFront");

    // Test InsertAtBack e Back
    lasd::List<std::string> l2;
    l2.InsertAtBack("hello");
    printTestResult(!l2.Empty(), "List<string>::Empty", "Test Empty() dopo InsertAtBack");
    printTestResult(l2.Size() == 1, "List<string>::Size", "Test Size() dopo InsertAtBack");
    printTestResult(l2.Back() == "hello", "List<string>::Back", "Test Back() dopo InsertAtBack");
    printTestResult(l2.Front() == "hello", "List<string>::Front", "Test Front() dopo InsertAtBack (single element)");

    l2.InsertAtBack("world");
    printTestResult(l2.Size() == 2, "List<string>::Size", "Test Size() dopo secondo InsertAtBack");
    printTestResult(l2.Back() == "world", "List<string>::Back", "Test Back() dopo secondo InsertAtBack");
    printTestResult(l2.Front() == "hello", "List<string>::Front", "Test Front() dopo secondo InsertAtBack");

    // Test costruttore da TraversableContainer (usando un Vector)
    lasd::Vector<int> vec(3);
    vec[0] = 1; vec[1] = 2; vec[2] = 3;
    lasd::List<int> l3(vec);
    printTestResult(!l3.Empty(), "List<int>::List(TraversableContainer)", "Test Empty() dopo costr. da Vector");
    printTestResult(l3.Size() == 3, "List<int>::Size", "Test Size() dopo costr. da Vector");
    printTestResult(l3.Front() == 1, "List<int>::Front", "Test Front() dopo costr. da Vector");
    printTestResult(l3.Back() == 3, "List<int>::Back", "Test Back() dopo costr. da Vector");
    printTestResult(l3[1] == 2, "List<int>::operator[]", "Test operator[] dopo costr. da Vector");

    // Test costruttore di copia
    lasd::List<int> l4(l3);
    printTestResult(!l4.Empty(), "List<int>::List(const List&)", "Test Empty() dopo costr. di copia");
    printTestResult(l4.Size() == 3, "List<int>::Size", "Test Size() dopo costr. di copia");
    printTestResult(l4.Front() == 1, "List<int>::Front", "Test Front() dopo costr. di copia");
    printTestResult(l4.Back() == 3, "List<int>::Back", "Test Back() dopo costr. di copia");
    printTestResult(l4[1] == 2, "List<int>::operator[]", "Test operator[] dopo costr. di copia");
    // Modifico l'originale per assicurarmi che la copia sia profonda
    l3.Front() = 100;
    printTestResult(l4.Front() == 1, "List<int>::List(const List&)", "Test deep copy (Front) dopo modifica originale");
    printTestResult(l3.Front() == 100, "List<int>::List(const List&)", "Test original list's Front is modified");


    // Test costruttore di movimento
    lasd::List<int> l5(std::move(l4));
    printTestResult(!l5.Empty(), "List<int>::List(List&&)", "Test Empty() dopo costr. di movimento");
    printTestResult(l5.Size() == 3, "List<int>::Size", "Test Size() dopo costr. di movimento");
    printTestResult(l5.Front() == 1, "List<int>::Front", "Test Front() dopo costr. di movimento");
    printTestResult(l5.Back() == 3, "List<int>::Back", "Test Back() dopo costr. di movimento");
    printTestResult(l4.Empty(), "List<int>::List(List&&)", "Test Empty() su lista sorgente dopo costr. di movimento");
    printTestResult(l4.Size() == 0, "List<int>::Size", "Test Size() su lista sorgente dopo costr. di movimento");
    
    // Test costruttore da TraversableContainer
    std::cout << "\n=== Test costruttore da TraversableContainer ===" << std::endl;
    lasd::Vector<int> vecInt(5);
    for (ulong i = 0; i < vecInt.Size(); ++i) { // Loop completion
        vecInt[i] = static_cast<int>(i) + 10;
    }
    lasd::List<int> l6(vecInt);
    printTestResult(l6.Size() == 5, "List<int>::List(const TraversableContainer&)", "Test Size() dopo costr. da Vector");
    printTestResult(l6.Front() == 10, "List<int>::Front", "Test Front() dopo costr. da Vector");
    printTestResult(l6.Back() == 14, "List<int>::Back", "Test Back() dopo costr. da Vector");
    for (ulong i = 0; i < vecInt.Size(); ++i) { // Loop completion
        printTestResult(l6[i] == vecInt[i], "List<int>::operator[]", "Test operator[" + std::to_string(i) + "] dopo costr. da Vector");
    }
    
    // Test costruttore da MappableContainer con move
    std::cout << "\n=== Test costruttore da MappableContainer con move ===" << std::endl;
    lasd::Vector<std::string> vecStr(3);
    vecStr[0] = "Hello"; vecStr[1] = "World"; vecStr[2] = "Test";
    lasd::List<std::string> l7(std::move(vecStr));
    printTestResult(l7.Size() == 3, "List<string>::Size", "Size after move construction from Vector");    // Test that the source vector is NOT empty, but retains its size, though elements are moved-from.
    printTestResult(!vecStr.Empty(), "Vector<string>::!Empty", "Test sorgente NON svuotato dopo costr. da MappableContainer con move");
    printTestResult(vecStr.Size() == 3, "Vector<string>::Size", "Size of source vector after move (should be unchanged)");

    // Test operazioni di copy assignment
    std::cout << "\n=== Test copy assignment ===" << std::endl;
    lasd::List<int> l8;
    l8 = l5; // Copy assignment
    printTestResult(l8.Size() == l5.Size(), "List<int>::operator=", "Test Size() dopo copy assignment");
    printTestResult(l8.Front() == l5.Front(), "List<int>::operator=", "Test Front() dopo copy assignment");
    printTestResult(l8.Back() == l5.Back(), "List<int>::operator=", "Test Back() dopo copy assignment");
    // Modifica per verificare la deep copy
    l5.Front() = 999; // l5 becomes {999, 2, 3}
    printTestResult(l8.Front() == 1, "List<int>::operator=", "Test deep copy (Front era 1) dopo modifica originale");
    printTestResult(l5.Front() == 999, "List<int>::operator=", "Test original list's Front is modified after assignment");
    
    // Test operazioni di move assignment
    std::cout << "\n=== Test move assignment ===" << std::endl;
    lasd::List<int> l9;
    l9 = std::move(l8); // Move assignment
    printTestResult(l9.Size() == 3, "List<int>::operator=", "Test Size() dopo move assignment");
    printTestResult(l9.Front() == 1, "List<int>::operator=", "Test Front() dopo move assignment");
    printTestResult(l9.Back() == 3, "List<int>::operator=", "Test Back() dopo move assignment");
    printTestResult(l8.Empty(), "List<int>::Empty", "Test Empty() su lista sorgente dopo move assignment");
    
    // Test RemoveFromFront
    std::cout << "\n=== Test RemoveFromFront ===" << std::endl;
    // l9 contiene {1, 2, 3}
    ulong oldSize_l9 = l9.Size();
    int oldBack_l9 = l9.Back(); 
    l9.RemoveFromFront(); // l9 diventa {2, 3}
    printTestResult(l9.Size() == oldSize_l9 - 1, "List<int>::RemoveFromFront", "Test Size() dopo RemoveFromFront");
    if (!l9.Empty()) {
        printTestResult(l9.Front() == 2, "List<int>::Front", "Test Front() dopo RemoveFromFront");
        printTestResult(l9.Back() == oldBack_l9, "List<int>::Back", "Test Back() dopo RemoveFromFront (era 3)");
    } else {
        printTestResult(false, "List<int>::Front/Back", "Test Front/Back dopo RemoveFromFront - LISTA VUOTA INASPETTATAMENTE");
    }
    
    // Test RemoveFromBack
    std::cout << "\n=== Test RemoveFromBack ===" << std::endl;
    // l9 contiene {2, 3}
    ulong oldSize_l9_v2 = l9.Size();
    int oldFront_l9 = l9.Front(); 
    l9.RemoveFromBack(); // l9 diventa {2}
    printTestResult(l9.Size() == oldSize_l9_v2 - 1, "List<int>::RemoveFromBack", "Test Size() dopo RemoveFromBack");
    if (!l9.Empty()) {
        printTestResult(l9.Front() == oldFront_l9, "List<int>::Front", "Test Front() dopo RemoveFromBack (era 2)");
        printTestResult(l9.Back() == 2, "List<int>::Back", "Test Back() dopo RemoveFromBack (dovrebbe essere 2)");
    } else {
        printTestResult(false, "List<int>::Front/Back", "Test Front/Back dopo RemoveFromBack - LISTA VUOTA INASPETTATAMENTE");
    }
        
    // Test FrontNRemove
    std::cout << "\n=== Test FrontNRemove ===" << std::endl;
    lasd::List<int> l10;
    l10.InsertAtBack(10);
    l10.InsertAtBack(20);
    l10.InsertAtBack(30);
    int frontVal = l10.Front();
    oldSize_l9 = l10.Size();
    int removedVal = l10.FrontNRemove();
    printTestResult(removedVal == frontVal, "List<int>::FrontNRemove", "Test valore rimosso");
    printTestResult(l10.Size() == oldSize_l9 - 1, "List<int>::FrontNRemove", "Test Size() dopo FrontNRemove");
    printTestResult(l10.Front() == 20, "List<int>::Front", "Test nuovo Front() dopo FrontNRemove");
    
    // Test BackNRemove
    std::cout << "\n=== Test BackNRemove ===" << std::endl;
    // l10 = {20, 30}
    int backVal = l10.Back(); 
    ulong oldSize_l10 = l10.Size(); 
    removedVal = l10.BackNRemove(); // removedVal = 30, l10 = {20}
    printTestResult(removedVal == backVal, "List<int>::BackNRemove", "Test valore rimosso (30)");
    printTestResult(l10.Size() == oldSize_l10 - 1, "List<int>::BackNRemove", "Test Size() dopo BackNRemove (dovrebbe essere 1)");
    if(!l10.Empty()){
        printTestResult(l10.Front() == 20, "List<int>::Front", "Test Front() dopo BackNRemove (dovrebbe essere 20)");
        printTestResult(l10.Back() == 20, "List<int>::Back", "Test Back() dopo BackNRemove (dovrebbe essere 20)");
    } else {
        printTestResult(false, "List<int>::Front/Back", "Test Front/Back dopo BackNRemove - LISTA VUOTA INASPETTATAMENTE");
    }
    
    // Test caso limite: rimuovi l'ultimo elemento
    std::cout << "\n=== Test caso limite: rimuovi l'ultimo elemento ===" << std::endl;
    l10.FrontNRemove();
    printTestResult(l10.Empty(), "List<int>::Empty", "Test Empty() dopo rimozione unico elemento");
    printTestResult(l10.Size() == 0, "List<int>::Size", "Test Size() dopo rimozione unico elemento");
    
    // Test eccezioni su lista vuota
    std::cout << "\n=== Test eccezioni su lista vuota ===" << std::endl;
    lasd::List<int> emptyList;
    // Test Front() su lista vuota
    try {
        [[maybe_unused]] int val = emptyList.Front(); 
        printTestResult(false, "List<int>::Front", "Test eccezione length_error su lista vuota (DOVEVA LANCIARE)");
    } catch (const std::length_error&) {
        printTestResult(true, "List<int>::Front", "Test eccezione length_error su lista vuota (CATTURATA)");
    } catch (...) {
        printTestResult(false, "List<int>::Front", "Test eccezione length_error su lista vuota (TIPO ERRATO)");
    }
    
    // Test Back() su lista vuota
    try {
        [[maybe_unused]] int val = emptyList.Back(); 
        printTestResult(false, "List<int>::Back", "Test eccezione length_error su lista vuota (DOVEVA LANCIARE)");
    } catch (const std::length_error&) {
        printTestResult(true, "List<int>::Back", "Test eccezione length_error su lista vuota (CATTURATA)");
    } catch (...) {
        printTestResult(false, "List<int>::Back", "Test eccezione length_error su lista vuota (TIPO ERRATO)");
    }
    
    // Test RemoveFromFront() su lista vuota
    try {
        emptyList.RemoveFromFront();
        printTestResult(false, "List<int>::RemoveFromFront", "Test eccezione length_error su lista vuota (DOVEVA LANCIARE)");
    } catch (const std::length_error&) {
        printTestResult(true, "List<int>::RemoveFromFront", "Test eccezione length_error su lista vuota (CATTURATA)");
    } catch (...) {
        printTestResult(false, "List<int>::RemoveFromFront", "Test eccezione length_error su lista vuota (TIPO ERRATO)");
    }
    
    // Test RemoveFromBack() su lista vuota
    try {
        emptyList.RemoveFromBack();
        printTestResult(false, "List<int>::RemoveFromBack", "Test eccezione length_error su lista vuota (DOVEVA LANCIARE)");
    } catch (const std::length_error&) {
        printTestResult(true, "List<int>::RemoveFromBack", "Test eccezione length_error su lista vuota (CATTURATA)");
    } catch (...) {
        printTestResult(false, "List<int>::RemoveFromBack", "Test eccezione length_error su lista vuota (TIPO ERRATO)");
    }

    // Test FrontNRemove() su lista vuota
    try {
        [[maybe_unused]] int val = emptyList.FrontNRemove();
        printTestResult(false, "List<int>::FrontNRemove", "Test eccezione length_error su lista vuota (DOVEVA LANCIARE)");
    } catch (const std::length_error&) {
        printTestResult(true, "List<int>::FrontNRemove", "Test eccezione length_error su lista vuota (CATTURATA)");
    } catch (...) {
        printTestResult(false, "List<int>::FrontNRemove", "Test eccezione length_error su lista vuota (TIPO ERRATO)");
    }

    // Test BackNRemove() su lista vuota
    try {
        [[maybe_unused]] int val = emptyList.BackNRemove();
        printTestResult(false, "List<int>::BackNRemove", "Test eccezione length_error su lista vuota (DOVEVA LANCIARE)");
    } catch (const std::length_error&) {
        printTestResult(true, "List<int>::BackNRemove", "Test eccezione length_error su lista vuota (CATTURATA)");
    } catch (...) {
        printTestResult(false, "List<int>::BackNRemove", "Test eccezione length_error su lista vuota (TIPO ERRATO)");
    }
    
    // Test operator[] su indice fuori range
    std::cout << "\n=== Test operator[] su indice fuori range ===" << std::endl;
    lasd::List<int> smallList;
    smallList.InsertAtBack(42);
    bool exceptionThrown = false;
    try {
        smallList[1]; // Indice fuori range
    } catch (const std::out_of_range&) {
        exceptionThrown = true;
    } catch (...) {
        exceptionThrown = false;
    }
    printTestResult(exceptionThrown, "List<int>::operator[]", "Test eccezione out_of_range per indice non valido");
    
    // Test operator[] scrittura fuori range
    try {
        smallList[1] = 99; // Indice 1 è fuori range
        printTestResult(false, "List<int>::operator[]", "Test scrittura operator[] out_of_range (DOVEVA LANCIARE)");
    } catch (const std::out_of_range& ) { // Removed variable name e
        printTestResult(true, "List<int>::operator[]", "Test scrittura operator[] out_of_range (eccezione std::out_of_range catturata)");
    } catch (...) {
        printTestResult(false, "List<int>::operator[]", "Test scrittura operator[] out_of_range (eccezione errata catturata)");
    }
    // Test operator[] su lista vuota
    try {
        [[maybe_unused]] int val = emptyList[0];
        printTestResult(false, "List<int>::operator[]", "Test operator[] su lista vuota (DOVEVA LANCIARE)");
    } catch (const std::out_of_range& ) { // Removed variable name e
        printTestResult(true, "List<int>::operator[]", "Test operator[] su lista vuota (eccezione std::out_of_range catturata)");
    } catch (...) {
        printTestResult(false, "List<int>::operator[]", "Test operator[] su lista vuota (eccezione errata catturata)");
    }

    
    // Test Clear
    std::cout << "\n=== Test Clear ===" << std::endl;
    lasd::List<int> l11;
    for (int i = 0; i < 5; i++) {
        l11.InsertAtBack(i);
    }
    printTestResult(l11.Size() == 5, "List<int>::Size", "Test Size() prima di Clear");
    l11.Clear();
    printTestResult(l11.Empty(), "List<int>::Clear", "Test Empty() dopo Clear");
    printTestResult(l11.Size() == 0, "List<int>::Size", "Test Size() dopo Clear");
    
    // Test operatori di confronto
    std::cout << "\n=== Test operatori di confronto ===" << std::endl;
    lasd::List<int> l12;
    l12.InsertAtBack(1);
    l12.InsertAtBack(2);
    l12.InsertAtBack(3);
    
    lasd::List<int> l13;
    l13.InsertAtBack(1);
    l13.InsertAtBack(2);
    l13.InsertAtBack(3);
    
    lasd::List<int> l14;
    l14.InsertAtBack(1);
    l14.InsertAtBack(5); // Valore diverso
    l14.InsertAtBack(3);
    
    lasd::List<int> l15;
    l15.InsertAtBack(1);
    l15.InsertAtBack(2); // Mancante l'elemento 3
    
    printTestResult(l12 == l13, "List<int>::operator==", "Test operatore == con liste uguali");
    printTestResult(!(l12 == l14), "List<int>::operator==", "Test operatore == con liste di valori diversi");
    printTestResult(!(l12 == l15), "List<int>::operator==", "Test operatore == con liste di dimensioni diverse");
    
    printTestResult(!(l12 != l13), "List<int>::operator!=", "Test operatore != con liste uguali");
    printTestResult(l12 != l14, "List<int>::operator!=", "Test operatore != con liste di valori diversi");
    printTestResult(l12 != l15, "List<int>::operator!=", "Test operatore != con liste di dimensioni diverse");
    
    // Test Map e funzioni di attraversamento
    std::cout << "\n=== Test Map (modificante) e Traverse/Fold ===" << std::endl; 
    lasd::List<int> l16;
    for (int i = 1; i <= 3; i++) {
        l16.InsertAtBack(i); // l16 = {1, 2, 3}
    }
    
    // Test Map (modificante)
    l16.Map([](int& value) { value *= 2; }); // l16 = {2, 4, 6}
    printTestResult(l16.Size() ==3, "List<int>::Map (modificante)", "Test Size dopo Map (*2)");
    if (l16.Size() == 3) {
        printTestResult(l16[0] == 2, "List<int>::Map (modificante)", "Test elemento 0 dopo Map (*2)");
        printTestResult(l16[1] == 4, "List<int>::Map (modificante)", "Test elemento 1 dopo Map (*2)");
        printTestResult(l16[2] == 6, "List<int>::Map (modificante)", "Test elemento 2 dopo Map (*2)");
    }
    
    // Test PreOrderMap (su {2,4,6})
    l16.PreOrderMap([](int& value) { value += 1; }); // l16 = {3, 5, 7}
    if (l16.Size() == 3) {
        printTestResult(l16[0] == 3, "List<int>::PreOrderMap", "Test elemento 0 dopo PreOrderMap (+1)");
        printTestResult(l16[1] == 5, "List<int>::PreOrderMap", "Test elemento 1 dopo PreOrderMap (+1)");
        printTestResult(l16[2] == 7, "List<int>::PreOrderMap", "Test elemento 2 dopo PreOrderMap (+1)");
    }
    
    // Test PostOrderMap (su {3,5,7})
    l16.PostOrderMap([](int& value) { value -= 2; }); // l16 = {1, 3, 5} (ordine di applicazione: 7->5, 5->3, 3->1)
    if (l16.Size() == 3) {
        printTestResult(l16[0] == 1, "List<int>::PostOrderMap", "Test elemento 0 dopo PostOrderMap (-2)");
        printTestResult(l16[1] == 3, "List<int>::PostOrderMap", "Test elemento 1 dopo PostOrderMap (-2)");
        printTestResult(l16[2] == 5, "List<int>::PostOrderMap", "Test elemento 2 dopo PostOrderMap (-2)");
    }
        
    // Test Traverse e funzioni di attraversamento (su {1,3,5})
    std::cout << "\n=== Test Traverse e Fold ===" << std::endl;
    std::string result_traverse = ""; 
    l16.Traverse([&result_traverse](const int& value) { result_traverse += std::to_string(value) + " "; });
    printTestResult(result_traverse == "1 3 5 ", "List<int>::Traverse", "Test risultato Traverse (1 3 5 )");
    
    result_traverse = "";
    l16.PreOrderTraverse([&result_traverse](const int& value) { result_traverse += std::to_string(value) + " "; });
    printTestResult(result_traverse == "1 3 5 ", "List<int>::PreOrderTraverse", "Test risultato PreOrderTraverse (1 3 5 )");
    
    result_traverse = "";
    l16.PostOrderTraverse([&result_traverse](const int& value) { result_traverse += std::to_string(value) + " "; });
    printTestResult(result_traverse == "5 3 1 ", "List<int>::PostOrderTraverse", "Test risultato PostOrderTraverse (5 3 1 )");

    // Test Fold (su {1,3,5})
    // Definiamo esplicitamente il tipo della funzione per Fold
    using FoldFunctionInt = std::function<int(const int&, const int&)>;

    int sum_fold_val = l16.Fold(
        FoldFunctionInt([](const int& dat, const int& acc_val) { 
            return acc_val + dat;
        }),
        0 // Inizializza a 0 per la somma
    );
    printTestResult(sum_fold_val == 9, "List<int>::Fold", "Test Fold per somma elementi (1+3+5=9)");

    int initial_acc_val = 100;
    sum_fold_val = l16.Fold(
        FoldFunctionInt([](const int& dat, const int& acc_val) { 
            return acc_val + dat;
        }),
        initial_acc_val // Passa l'accumulatore iniziale direttamente
    );
    printTestResult(sum_fold_val == 109, "List<int>::Fold", "Test Fold per somma elementi con valore iniziale (100+1+3+5=109)");
    
    // Test Insert, InsertAll, Remove, RemoveAll (Dictionary)
    std::cout << "\n=== Test funzioni Dictionary ===" << std::endl;
    lasd::List<int> l17;
    bool inserted = l17.Insert(10);
    printTestResult(inserted, "List<int>::Insert", "Test inserimento in lista vuota");
    printTestResult(l17.Size() == 1, "List<int>::Size", "Test Size() dopo Insert");
    printTestResult(l17.Front() == 10, "List<int>::Front", "Test Front() dopo Insert");
    
    inserted = l17.Insert(10); // Duplicato
    printTestResult(!inserted, "List<int>::Insert", "Test inserimento duplicato");
    
    // Test InsertAll - implementazione corretta
    lasd::Vector<int> vecInsert(3);
    vecInsert[0] = 20; vecInsert[1] = 30; vecInsert[2] = 10; // Include un duplicato
    bool insertAllResult = l17.InsertAll(vecInsert);
    printTestResult(!insertAllResult, "List<int>::InsertAll", "Test InsertAll con alcuni duplicati");
    printTestResult(l17.Size() == 3, "List<int>::Size", "Test Size() dopo InsertAll");
    printTestResult(l17.Exists(10), "List<int>::Exists", "Test Exists(10) dopo InsertAll");
    printTestResult(l17.Exists(20), "List<int>::Exists", "Test Exists(20) dopo InsertAll");
    printTestResult(l17.Exists(30), "List<int>::Exists", "Test Exists(30) dopo InsertAll");
    
    // Test Remove
    bool removed = l17.Remove(20);
    printTestResult(removed, "List<int>::Remove", "Test Remove elemento esistente");
    printTestResult(l17.Size() == 2, "List<int>::Size", "Test Size() dopo Remove");
    printTestResult(!l17.Exists(20), "List<int>::Exists", "Test Exists() dopo Remove");
    
    removed = l17.Remove(99); // Non esiste
    printTestResult(!removed, "List<int>::Remove", "Test Remove elemento non esistente");
    
    // Test RemoveAll
    lasd::Vector<int> vecRemove(2);
    vecRemove[0] = 10; vecRemove[1] = 30;
    bool allRemoved = l17.RemoveAll(vecRemove);
    printTestResult(allRemoved, "List<int>::RemoveAll", "Test RemoveAll tutti elementi esistenti");
    printTestResult(l17.Empty(), "List<int>::Empty", "Test Empty() dopo RemoveAll");
    
    // Test casi limite con liste di dimensione 1
    std::cout << "\n=== Test casi limite con liste di dimensione 1 ===" << std::endl;
    lasd::List<int> singleList;
    singleList.InsertAtFront(42);
    printTestResult(singleList.Front() == singleList.Back(), "List<int>::Front/Back", "Test Front() == Back() su lista di dimensione 1");
    
    singleList.RemoveFromFront();
    printTestResult(singleList.Empty(), "List<int>::RemoveFromFront", "Test Empty() dopo RemoveFromFront su lista di dimensione 1");
    
    singleList.InsertAtBack(42);
    singleList.RemoveFromBack();
    printTestResult(singleList.Empty(), "List<int>::RemoveFromBack", "Test Empty() dopo RemoveFromBack su lista di dimensione 1");
    
    // Test InsertSome e RemoveSome
    std::cout << "\n=== Test InsertSome e RemoveSome ===" << std::endl;
    lasd::List<int> l18;
    l18.Insert(100);
    l18.Insert(200);
    
    lasd::Vector<int> vecInsertSome(3);
    vecInsertSome[0] = 100; // Duplicato
    vecInsertSome[1] = 300; // Nuovo
    vecInsertSome[2] = 400; // Nuovo
    
    bool someInserted = l18.InsertSome(vecInsertSome);
    printTestResult(someInserted, "List<int>::InsertSome", "Test InsertSome con alcuni elementi inseriti");
    printTestResult(l18.Size() == 4, "List<int>::Size", "Test Size() dopo InsertSome");
    printTestResult(l18.Exists(300) && l18.Exists(400), "List<int>::Exists", "Test nuovi elementi dopo InsertSome");
    
    lasd::Vector<int> vecRemoveSome(3);
    vecRemoveSome[0] = 100;
    vecRemoveSome[1] = 999; // Non esiste
    vecRemoveSome[2] = 300;
    
    bool someRemoved = l18.RemoveSome(vecRemoveSome);
    printTestResult(someRemoved, "List<int>::RemoveSome", "Test RemoveSome con alcuni elementi rimossi");
    printTestResult(l18.Size() == 2, "List<int>::Size", "Test Size() dopo RemoveSome");
    printTestResult(!l18.Exists(100) && !l18.Exists(300), "List<int>::Exists", "Test elementi rimossi dopo RemoveSome");
    printTestResult(l18.Exists(200) && l18.Exists(400), "List<int>::Exists", "Test elementi rimasti dopo RemoveSome");

    std::cout << "=== Fine test List ===" << std::endl;
}