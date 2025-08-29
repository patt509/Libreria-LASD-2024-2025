#include "test.hpp"
#include "../vector/vector.hpp"
#include "../list/list.hpp"
#include <iostream>
#include <stdexcept>
#include <cmath> // For std::abs

void testVector() {
    std::cout << "\nInizio test Vector" << std::endl;

    // ========== TEST COSTRUTTORI E OPERATORI DI BASE ==========
    
    // Test costruttore con dimensione
    lasd::Vector<int> v1(5);
    printTestResult(v1.Size() == 5, "Vector<int>::Size", "Verifica dimensione dopo costruttore con dimensione");
    printTestResult(!v1.Empty(), "Vector<int>::Empty", "Verifica non vuoto dopo costruttore con dimensione");
    
    // Inizializzazione elementi
    for(ulong i = 0; i < v1.Size(); i++) {
        v1[i] = i * 10;
    }
    
    // Test accesso a elementi
    bool accessCorrect = true;
    for(ulong i = 0; i < v1.Size(); i++) {
        if(v1[i] != static_cast<int>(i) * 10) {
            accessCorrect = false;
            break;
        }
    }
    printTestResult(accessCorrect, "Vector<int>::operator[]", "Verifica accesso corretto agli elementi");
    
    // Test Front e Back
    printTestResult(v1.Front() == 0, "Vector<int>::Front", "Verifica accesso al primo elemento");
    printTestResult(v1.Back() == 40, "Vector<int>::Back", "Verifica accesso all'ultimo elemento");
    
    // Test costruttore di copia
    lasd::Vector<int> v2(v1);
    printTestResult(v2.Size() == v1.Size(), "Vector<int>::Vector(const Vector &)", "Verifica dimensione dopo costruttore di copia");
    
    bool copyCorrect = true;
    for(ulong i = 0; i < v1.Size(); i++) {
        if(v2[i] != v1[i]) {
            copyCorrect = false;
            break;
        }
    }
    printTestResult(copyCorrect, "Vector<int>::Vector(const Vector &)", "Verifica contenuto dopo copia");
    
    // Test di copia profonda (modifico v1 e verifico che v2 non cambi)
    v1[0] = 100;
    printTestResult(v2[0] == 0, "Vector<int>::Vector(const Vector &)", "Verifica deep copy");
    
    // Test costruttore di movimento
    lasd::Vector<int> v3(std::move(v2));
    printTestResult(v3.Size() == 5, "Vector<int>::Vector(Vector &&)", "Verifica dimensione dopo costruttore di movimento");
    printTestResult(v2.Empty(), "Vector<int>::Vector(Vector &&)", "Verifica source svuotato dopo movimento");
    
    bool moveCorrect = true;
    for(ulong i = 0; i < v3.Size(); i++) {
        if(v3[i] != static_cast<int>(i) * 10) {  // v2 conteneva i valori originali di v1
            moveCorrect = false;
            break;
        }
    }
    printTestResult(moveCorrect, "Vector<int>::Vector(Vector &&)", "Verifica contenuto dopo movimento");
    
    // ========== TEST OPERAZIONI SPECIFICHE ==========
    
    // Test Resize
    v1.Resize(8);
    printTestResult(v1.Size() == 8, "Vector<int>::Resize", "Verifica aumento dimensione");
    printTestResult(v1[0] == 100, "Vector<int>::Resize", "Verifica elementi originali conservati");
    
    v1.Resize(3);
    printTestResult(v1.Size() == 3, "Vector<int>::Resize", "Verifica riduzione dimensione");
    printTestResult(v1[0] == 100 && v1[1] == 10 && v1[2] == 20, "Vector<int>::Resize", "Verifica primi elementi conservati");
    
    // Test Clear
    v1.Clear();
    printTestResult(v1.Empty(), "Vector<int>::Clear", "Verifica svuotamento");
    printTestResult(v1.Size() == 0, "Vector<int>::Clear", "Verifica dimensione 0 dopo Clear");
    
    // ========== TEST ECCEZIONI ==========
    
    // Test eccezione per out-of-bounds
    lasd::Vector<int> v4(3);
    bool exceptionThrown = false;
    try {
        int val = v4[5]; // Questo dovrebbe lanciare un'eccezione
        (void)val;  // Per evitare warning di unused variable
    } catch(std::out_of_range&) {
        exceptionThrown = true;
    } catch(...) {
        // Cattura altre eccezioni
    }
    printTestResult(exceptionThrown, "Vector<int>::operator[]", "Verifica eccezione out-of-bounds");
    
    // Test eccezioni su vettore vuoto
    lasd::Vector<int> v5(0);
    exceptionThrown = false;
    try {
        int val = v5.Front();
        (void)val;
    } catch(std::length_error&) {
        exceptionThrown = true;
    } catch(...) {
        // Cattura altre eccezioni
    }
    printTestResult(exceptionThrown, "Vector<int>::Front", "Verifica eccezione Front su vettore vuoto");
    
    exceptionThrown = false;
    try {
        int val = v5.Back();
        (void)val;
    } catch(std::length_error&) {
        exceptionThrown = true;
    } catch(...) {
        // Cattura altre eccezioni
    }
    printTestResult(exceptionThrown, "Vector<int>::Back", "Verifica eccezione Back su vettore vuoto");
    
    // ========== TEST OPERATORE DI ASSEGNAZIONE ==========
    
    std::cout << "\n=== Test operatore di assegnazione ===" << std::endl;
    
    // Test assegnazione per copia
    lasd::Vector<int> v6(3);
    for(ulong i = 0; i < v6.Size(); i++) {
        v6[i] = i * 5;
    }
    
    lasd::Vector<int> v7;
    v7 = v6; // Operatore di assegnazione di copia
    
    printTestResult(v7.Size() == v6.Size(), "Vector<int>::operator=", "Verifica dimensione dopo assegnazione per copia");
    
    bool assignmentCorrect = true;
    for(ulong i = 0; i < v6.Size(); i++) {
        if(v7[i] != v6[i]) {
            assignmentCorrect = false;
            break;
        }
    }
    printTestResult(assignmentCorrect, "Vector<int>::operator=", "Verifica contenuto dopo assegnazione per copia");
    
    // Verifica deep copy
    v6[0] = 999;
    printTestResult(v7[0] == 0, "Vector<int>::operator=", "Verifica deep copy dopo assegnazione");
    
    // Test assegnazione per movimento
    lasd::Vector<int> v8;
    v8 = std::move(v7);
    
    printTestResult(v8.Size() == 3, "Vector<int>::operator= (move)", "Verifica dimensione dopo assegnazione per movimento");
    printTestResult(v7.Empty(), "Vector<int>::operator= (move)", "Verifica source vuoto dopo assegnazione per movimento");
    
    bool moveAssignCorrect = true;
    for(ulong i = 0; i < v8.Size(); i++) {
        if(v8[i] != static_cast<int>(i) * 5) {
            moveAssignCorrect = false;
            break;
        }
    }
    printTestResult(moveAssignCorrect, "Vector<int>::operator= (move)", "Verifica contenuto dopo assegnazione per movimento");
    
    // ========== TEST DI TRAVERSABLE CONTAINER ==========
    
    std::cout << "\n=== Test TraversableContainer ===" << std::endl;
    
    // Test Exists
    lasd::Vector<int> v9(4);
    v9[0] = 10; v9[1] = 20; v9[2] = 30; v9[3] = 40;
    
    printTestResult(v9.Exists(20), "Vector<int>::Exists", "Verifica elemento esistente");
    printTestResult(!v9.Exists(25), "Vector<int>::Exists", "Verifica elemento non esistente");
    printTestResult(!lasd::Vector<int>(0).Exists(10), "Vector<int>::Exists", "Verifica su vettore vuoto");
    
    // Test Fold
    // Definiamo esplicitamente il tipo della funzione per Fold
    using FoldFunctionInt = std::function<int(const int&, const int&)>;

    int sum = v9.Fold(
        FoldFunctionInt([](const int& x, const int& y) { return x + y; }), 
        0
    );
    printTestResult(sum == 100, "Vector<int>::Fold", "Verifica somma di tutti gli elementi");
    
    int product = v9.Fold(
        FoldFunctionInt([](const int& x, const int& y) { return x * y; }), 
        1
    );
    printTestResult(product == 240000, "Vector<int>::Fold", "Verifica prodotto di tutti gli elementi");
    
    // ========== TEST DI MAPPABLE CONTAINER ==========
    
    std::cout << "\n=== Test MappableContainer ===" << std::endl;
    
    // Test Map
    lasd::Vector<int> v10(3);
    v10[0] = 1; v10[1] = 2; v10[2] = 3;
    
    v10.Map([](int& x) { x *= 2; });
    
    bool mapCorrect = true;
    if(v10[0] != 2 || v10[1] != 4 || v10[2] != 6) {
        mapCorrect = false;
    }
    printTestResult(mapCorrect, "Vector<int>::Map", "Verifica mapping di tutti gli elementi");
    
    // Test PreOrderMap (dovrebbe essere identico a Map per Vector)
    v10.PreOrderMap([](int& x) { x += 1; });
    
    bool preOrderMapCorrect = true;
    if(v10[0] != 3 || v10[1] != 5 || v10[2] != 7) {
        preOrderMapCorrect = false;
    }
    printTestResult(preOrderMapCorrect, "Vector<int>::PreOrderMap", "Verifica PreOrderMap di tutti gli elementi");
    
    // Test PostOrderMap
    v10.PostOrderMap([](int& x) { x *= 10; });
    
    bool postOrderMapCorrect = true;
    if(v10[0] != 30 || v10[1] != 50 || v10[2] != 70) {
        postOrderMapCorrect = false;
    }
    printTestResult(postOrderMapCorrect, "Vector<int>::PostOrderMap", "Verifica PostOrderMap di tutti gli elementi");
    
    // ========== TEST DI TRAVERSABLE CONTAINER (TRAVERSE) ==========
    
    std::cout << "\n=== Test Traverse ===" << std::endl;
    
    lasd::Vector<int> v11(4);
    v11[0] = 10; v11[1] = 20; v11[2] = 30; v11[3] = 40;
    
    std::string traverseResult = "";
    v11.Traverse([&traverseResult](const int& x) { traverseResult += std::to_string(x) + " "; });
    printTestResult(traverseResult == "10 20 30 40 ", "Vector<int>::Traverse", "Verifica attraversamento standard");
    
    std::string preOrderTraverseResult = "";
    v11.PreOrderTraverse([&preOrderTraverseResult](const int& x) { preOrderTraverseResult += std::to_string(x) + " "; });
    printTestResult(preOrderTraverseResult == "10 20 30 40 ", "Vector<int>::PreOrderTraverse", "Verifica attraversamento in pre-ordine");
    
    std::string postOrderTraverseResult = "";
    v11.PostOrderTraverse([&postOrderTraverseResult](const int& x) { postOrderTraverseResult += std::to_string(x) + " "; });
    printTestResult(postOrderTraverseResult == "40 30 20 10 ", "Vector<int>::PostOrderTraverse", "Verifica attraversamento in post-ordine");
    
    // ========== TEST CONFRONTO TRA VETTORI ==========
    
    std::cout << "\n=== Test operatori di confronto ===" << std::endl;
    
    lasd::Vector<int> v12(3);
    v12[0] = 1; v12[1] = 2; v12[2] = 3;
    
    lasd::Vector<int> v13(3);
    v13[0] = 1; v13[1] = 2; v13[2] = 3;
    
    lasd::Vector<int> v14(3);
    v14[0] = 1; v14[1] = 9; v14[2] = 3;
    
    lasd::Vector<int> v15(4);
    v15[0] = 1; v15[1] = 2; v15[2] = 3; v15[3] = 4;
    
    printTestResult(v12 == v13, "Vector<int>::operator==", "Verifica uguaglianza vettori identici");
    printTestResult(!(v12 == v14), "Vector<int>::operator==", "Verifica disuguaglianza vettori con valori diversi");
    printTestResult(!(v12 == v15), "Vector<int>::operator==", "Verifica disuguaglianza vettori con dimensioni diverse");
    
    printTestResult(!(v12 != v13), "Vector<int>::operator!=", "Verifica non disuguaglianza vettori identici");
    printTestResult(v12 != v14, "Vector<int>::operator!=", "Verifica disuguaglianza vettori con valori diversi");
    printTestResult(v12 != v15, "Vector<int>::operator!=", "Verifica disuguaglianza vettori con dimensioni diverse");
    
    // ========== TEST COSTRUTTORE DA TRAVERSABLE CONTAINER ==========
    
    std::cout << "\n=== Test costruttore da TraversableContainer ===" << std::endl;
    
    // Creo una lista per testare il costruttore da TraversableContainer
    lasd::List<int> list;
    list.InsertAtBack(100);
    list.InsertAtBack(200);
    list.InsertAtBack(300);
    
    lasd::Vector<int> v16(list);
    
    printTestResult(v16.Size() == 3, "Vector<int>::Vector(const TraversableContainer&)", "Verifica dimensione da TraversableContainer");
    printTestResult(v16[0] == 100 && v16[1] == 200 && v16[2] == 300, "Vector<int>::Vector(const TraversableContainer&)", "Verifica contenuto da TraversableContainer");
    
    // ========== TEST COSTRUTTORE DA MAPPABLE CONTAINER CON MOVE ==========
    
    std::cout << "\n=== Test costruttore da MappableContainer (move) ===" << std::endl;
    
    lasd::List<int> listToMove;
    listToMove.InsertAtBack(10);
    listToMove.InsertAtBack(20);
    listToMove.InsertAtBack(30);
    
    ulong originalSize = listToMove.Size(); // Store original size

    lasd::Vector<int> v17(std::move(listToMove));
    
    printTestResult(v17.Size() == 3, "Vector<int>::Vector(MappableContainer&&)", "Verifica dimensione da MappableContainer con move");
    printTestResult(v17[0] == 10 && v17[1] == 20 && v17[2] == 30, "Vector<int>::Vector(MappableContainer&&)", "Verifica contenuto da MappableContainer con move");
    
    // Verify that the source List is NOT empty and retains its original size, as per simpletest.cpp logic
    printTestResult(!listToMove.Empty(), "List<int>::!Empty", "Test sorgente (List<int>) NON svuotato dopo costr. Vector da MappableContainer con move");
    printTestResult(listToMove.Size() == originalSize, "List<int>::Size", "Test dimensione sorgente (List<int>) invariata dopo costr. Vector da MappableContainer con move");
        
    // ========== TEST CON TIPI DIVERSI ==========
    
    std::cout << "\n=== Test con std::string ===" << std::endl;
    
    lasd::Vector<std::string> vstr(3);
    vstr[0] = "Ciao";
    vstr[1] = "Mondo";
    vstr[2] = "Test";
    
    printTestResult(vstr.Size() == 3, "Vector<string>::Size", "Verifica dimensione vector di stringhe");
    printTestResult(vstr.Front() == "Ciao", "Vector<string>::Front", "Verifica Front su vector di stringhe");
    printTestResult(vstr.Back() == "Test", "Vector<string>::Back", "Verifica Back su vector di stringhe");
    
    std::string strCat = "";
    vstr.Traverse([&strCat](const std::string& s) { strCat += s.substr(0, 1); });
    printTestResult(strCat == "CMT", "Vector<string>::Traverse", "Verifica Traverse su vector di stringhe");
    
    // ========== TEST CON VETTORI ANNIDATI ==========
    
    std::cout << "\n=== Test con Vector<Vector<int>> ===" << std::endl;
    
    lasd::Vector<lasd::Vector<int>> vvec(2);
    
    vvec[0] = lasd::Vector<int>(2);
    vvec[0][0] = 1; vvec[0][1] = 2;
    
    vvec[1] = lasd::Vector<int>(3);
    vvec[1][0] = 10; vvec[1][1] = 20; vvec[1][2] = 30;
    
    printTestResult(vvec[0].Size() == 2, "Vector<Vector<int>>::Size", "Verifica dimensione primo vettore annidato");
    printTestResult(vvec[1].Size() == 3, "Vector<Vector<int>>::Size", "Verifica dimensione secondo vettore annidato");
    printTestResult(vvec[0][1] == 2, "Vector<Vector<int>>::operator[]", "Verifica accesso a elemento di vettore annidato");
    printTestResult(vvec[1][2] == 30, "Vector<Vector<int>>::operator[]", "Verifica accesso a elemento di vettore annidato");
    
    // ========== TEST SORTABLE VECTOR ==========
    
    std::cout << "\n=== Test SortableVector ===" << std::endl;
    
    lasd::SortableVector<int> sv1(5);
    sv1[0] = 5; sv1[1] = 3; sv1[2] = 1; sv1[3] = 4; sv1[4] = 2;
    
    // Testiamo la funzione Sort
    sv1.Sort();
    
    bool sortCorrect = true;
    for(ulong i = 0; i < sv1.Size() - 1; i++) {
        if(sv1[i] > sv1[i+1]) {
            sortCorrect = false;
            break;
        }
    }
    printTestResult(sortCorrect, "SortableVector<int>::Sort", "Verifica ordinamento corretto");
    printTestResult(sv1[0] == 1 && sv1[1] == 2 && sv1[2] == 3 && sv1[3] == 4 && sv1[4] == 5, 
                   "SortableVector<int>::Sort", "Verifica valori dopo ordinamento");
    
    // Test sort su vettore già ordinato
    lasd::SortableVector<int> sv2(4);
    sv2[0] = 1; sv2[1] = 2; sv2[2] = 3; sv2[3] = 4;
    sv2.Sort();
    
    bool sortedCorrect = true;
    for(ulong i = 0; i < sv2.Size() - 1; i++) {
        if(sv2[i] > sv2[i+1]) {
            sortedCorrect = false;
            break;
        }
    }
    printTestResult(sortedCorrect, "SortableVector<int>::Sort", "Verifica ordinamento su vettore già ordinato");
    
    // Test sort su vettore vuoto
    lasd::SortableVector<int> sv3(0);
    sv3.Sort(); // Non dovrebbe causare errori
    printTestResult(sv3.Size() == 0, "SortableVector<int>::Sort", "Verifica sort su vettore vuoto");
    
    // Test sort su vettore con un elemento
    lasd::SortableVector<int> sv4(1);
    sv4[0] = 42;
    sv4.Sort(); // Non dovrebbe causare errori
    printTestResult(sv4[0] == 42, "SortableVector<int>::Sort", "Verifica sort su vettore con un elemento");
    
    // ========== TEST CASI LIMITE E SPECIALI ==========
    
    std::cout << "\n=== Test casi speciali ===" << std::endl;
    
    // Test con vettore di dimensione 1
    lasd::Vector<int> v18(1);
    v18[0] = 42;
    
    printTestResult(v18.Size() == 1, "Vector<int>::Size", "Verifica dimensione di vettore con un elemento");
    printTestResult(v18.Front() == 42, "Vector<int>::Front", "Verifica Front su vettore con un elemento");
    printTestResult(v18.Back() == 42, "Vector<int>::Back", "Verifica Back su vettore con un elemento");
    printTestResult(v18.Front() == v18.Back(), "Vector<int>::Front/Back", "Verifica Front == Back su vettore con un elemento");
    
    // Test resize con valore 0
    v18.Resize(0);
    printTestResult(v18.Empty(), "Vector<int>::Resize", "Verifica resize a 0");
    printTestResult(v18.Size() == 0, "Vector<int>::Size", "Verifica Size() dopo resize a 0");
    
    // Test vettore con floating point
    lasd::Vector<double> v19(3);
    v19[0] = 1.5; v19[1] = 2.5; v19[2] = 3.5;
    
    printTestResult(std::abs(v19[0] - 1.5) < 0.0001, "Vector<double>::operator[]", "Verifica accesso con double");
    
    v19.Map([](double& x) { x *= 2; });
    
    bool doubleMapCorrect = true;
    if(std::abs(v19[0] - 3.0) > 0.0001 || std::abs(v19[1] - 5.0) > 0.0001 || std::abs(v19[2] - 7.0) > 0.0001) {
        doubleMapCorrect = false;
    }
    printTestResult(doubleMapCorrect, "Vector<double>::Map", "Verifica mapping con double");
    
    std::cout << "Fine test Vector\n" << std::endl;
}

// Definizione di functor personalizzato per test addizionali
class MultiplyBy {
private:
    int factor;
    
public:
    MultiplyBy(int f) : factor(f) {}
    
    void operator()(int& value) const {
        value *= factor;
    }
};

class SummarizeInt {
private:
    std::string& result;
    
public:
    SummarizeInt(std::string& res) : result(res) {}
    
    void operator()(const int& value) const {
        result += "[" + std::to_string(value) + "]";
    }
};

void testVectorExtended() {
    std::cout << "\n=== Test Estesi per Vector ===" << std::endl;
    
    // Test PreOrderFold e PostOrderFold
    std::cout << "\\n=== Test PreOrderFold e PostOrderFold ===" << std::endl;
    
    lasd::Vector<int> vExtFold(4);
    vExtFold[0] = 1; vExtFold[1] = 2; vExtFold[2] = 3; vExtFold[3] = 4;
    
    // Definiamo esplicitamente il tipo della funzione per Fold
    using FoldFunctionIntExt = std::function<int(const int&, const int&)>;

    int sumPreOrder = vExtFold.PreOrderFold(
        FoldFunctionIntExt([](const int& x, const int& y) { return x + y; }), 
        0
    );
    printTestResult(sumPreOrder == 10, "Vector<int>::PreOrderFold", "Verifica somma con PreOrderFold");
    
    int productPreOrder = vExtFold.PreOrderFold(
        FoldFunctionIntExt([](const int& x, const int& y) { return x * y; }), 
        1
    );
    printTestResult(productPreOrder == 24, "Vector<int>::PreOrderFold", "Verifica prodotto con PreOrderFold");
    
    int sumPostOrder = vExtFold.PostOrderFold(
        FoldFunctionIntExt([](const int& x, const int& y) { return x + y; }), 
        0
    );
    printTestResult(sumPostOrder == 10, "Vector<int>::PostOrderFold", "Verifica somma con PostOrderFold");
    
    int productPostOrder = vExtFold.PostOrderFold(
        FoldFunctionIntExt([](const int& x, const int& y) { return x * y; }), 
        1
    );
    printTestResult(productPostOrder == 24, "Vector<int>::PostOrderFold", "Verifica prodotto con PostOrderFold");
    
    // Test con functor personalizzati
    std::cout << "\n=== Test functor personalizzati ===" << std::endl;
    
    lasd::Vector<int> v2(3);
    v2[0] = 1; v2[1] = 2; v2[2] = 3;
    
    MultiplyBy mult3(3);
    v2.Map(mult3);
    
    printTestResult(v2[0] == 3 && v2[1] == 6 && v2[2] == 9, 
                   "Vector<int>::Map con functor", "Verifica mapping con functor personalizzato");
    
    MultiplyBy mult2(2);
    v2.PreOrderMap(mult2);
    
    printTestResult(v2[0] == 6 && v2[1] == 12 && v2[2] == 18, 
                   "Vector<int>::PreOrderMap con functor", "Verifica PreOrderMap con functor personalizzato");
    
    std::string traverseResult = "";
    SummarizeInt summarize(traverseResult);
    v2.Traverse(summarize);
    
    printTestResult(traverseResult == "[6][12][18]", 
                   "Vector<int>::Traverse con functor", "Verifica Traverse con functor personalizzato");
    
    // Test casi limite
    std::cout << "\n=== Test casi limite ===" << std::endl;
    
    // Test con vettore che viene ridimensionato più volte
    lasd::Vector<int> v3(2);
    v3[0] = 10; v3[1] = 20;
    
    v3.Resize(5);
    v3[2] = 30; v3[3] = 40; v3[4] = 50;
    printTestResult(v3.Size() == 5 && v3[0] == 10 && v3[4] == 50, 
                   "Vector<int>::Resize", "Verifica multiple resize operazioni");
    
    v3.Resize(3); // Ridurre la dimensione
    printTestResult(v3.Size() == 3 && v3[0] == 10 && v3[2] == 30, 
                   "Vector<int>::Resize", "Verifica riduzione dimensione conservando elementi");
    
    v3.Resize(0); // Pulire completamente il vettore
    printTestResult(v3.Size() == 0 && v3.Empty(), 
                   "Vector<int>::Resize", "Verifica resize a zero");
    
    // Test con tipi personalizzati
    struct Point {
        int x, y;
        
        bool operator==(const Point& p) const {
            return x == p.x && y == p.y;
        }
        
        bool operator!=(const Point& p) const {
            return !(*this == p);
        }
    };
    
    lasd::Vector<Point> v4(2);
    v4[0] = Point{1, 2};
    v4[1] = Point{3, 4};
    
    printTestResult(v4[0].x == 1 && v4[0].y == 2, 
                   "Vector<custom>", "Verifica vettore con tipo personalizzato");
    
    // Test copia con tipo personalizzato
    lasd::Vector<Point> v5(v4);
    printTestResult(v5[0] == v4[0] && v5[1] == v4[1], 
                   "Vector<custom>::Vector(const Vector&)", "Verifica copia di tipo personalizzato");

    // Test di ordinamento con tipi diversi
    lasd::SortableVector<double> sv1(4);
    sv1[0] = 3.5;
    sv1[1] = 1.2;
    sv1[2] = 4.7;
    sv1[3] = 2.1;
    
    sv1.Sort();
    
    printTestResult(sv1[0] <= sv1[1] && sv1[1] <= sv1[2] && sv1[2] <= sv1[3],
                   "SortableVector<double>::Sort", "Verifica ordinamento con double");
    
    // Verifica singoli valori con tolleranza per floating point
    bool sortedDoubleCorrect = 
        std::abs(sv1[0] - 1.2) < 0.0001 && 
        std::abs(sv1[1] - 2.1) < 0.0001 && 
        std::abs(sv1[2] - 3.5) < 0.0001 && 
        std::abs(sv1[3] - 4.7) < 0.0001;
        
    printTestResult(sortedDoubleCorrect, 
                   "SortableVector<double>::Sort", "Verifica valori specifici dopo ordinamento double");
                   
    // Test con stringhe
    lasd::SortableVector<std::string> sv2(3);
    sv2[0] = "banana";
    sv2[1] = "apple";
    sv2[2] = "cherry";
    
    sv2.Sort();
    
    printTestResult(sv2[0] == "apple" && sv2[1] == "banana" && sv2[2] == "cherry",
                   "SortableVector<string>::Sort", "Verifica ordinamento con stringhe");
                   
    std::cout << "Fine test estesi Vector\n" << std::endl;
}