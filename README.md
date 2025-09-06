# LASD Library - Laboratorio di Algoritmi e Strutture Dati

[![Language](https://img.shields.io/badge/Language-C%2B%2B20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)](https://github.com/patt509/Libreria-LASD-2024-2025)
[![License](https://img.shields.io/badge/License-Academic-orange.svg)](https://github.com/patt509/Libreria-LASD-2024-2025)

---

*[English version below](#english-version)*

## Versione Italiana

### Descrizione

Implementazione delle strutture dati e algoritmi richiesti per il corso di **Laboratorio di Algoritmi e Strutture Dati** presso l'Università degli Studi di Napoli "Federico II", tenuto dalla Prof.ssa Anna Corazza.

Il progetto consiste nell'implementazione di una libreria di strutture dati utilizzando C++20, basata su template e specifiche fornite dal corso. L'implementazione utilizza ereditarietà virtuale multipla, template generici e design pattern avanzati seguendo le linee guida del corso.

### Architettura del Sistema

La libreria è strutturata secondo una gerarchia di classi che utilizza ereditarietà virtuale multipla per fornire massima flessibilità e riusabilità del codice:

#### Container Base
- **Container**: Classe astratta base per tutti i contenitori
- **ClearableContainer**: Estende Container con funzionalità di svuotamento
- **ResizableContainer**: Aggiunge supporto per ridimensionamento dinamico

#### Container Specializzati
- **TestableContainer**: Base per contenitori con test di esistenza elementi
- **DictionaryContainer**: Implementa collezioni con elementi unici
- **OrderedDictionaryContainer**: Dizionari con operazioni di accesso ordinato

#### Traversal e Mapping
- **TraversableContainer**: Supporta attraversamento read-only con pattern Visitor
- **MappableContainer**: Consente attraversamento con modifica degli elementi
- Implementazioni specializzate per diversi ordini di attraversamento:
  - PreOrder/PostOrder (strutture lineari e alberi)
  - InOrder (alberi binari di ricerca)
  - BreadthFirst (attraversamento per livelli)

#### Container Lineari
- **LinearContainer**: Fornisce accesso indicizzato agli elementi
- **MutableLinearContainer**: Estende con capacità di modifica via indice
- **SortableLinearContainer**: Aggiunge algoritmi di ordinamento integrati

### Strutture Dati Implementate

| Struttura | Implementazione | File |
|-----------|----------------|------|
| **Vector** | Array dinamico | `vector/vector.hpp` |
| **List** | Lista doppiamente collegata | `list/list.hpp` |
| **Set (List-based)** | Set basato su lista | `set/lst/setlst.hpp` |
| **Set (Vector-based)** | Set basato su vettore | `set/vec/setvec.hpp` |
| **Heap** | Heap binario su vettore | `heap/vec/heapvec.hpp` |
| **Priority Queue** | Coda con priorità su heap | `pq/heap/pqheap.hpp` |

### Build e Installazione

#### Prerequisiti
- Compilatore GCC con supporto completo C++20
- Make utility
- Sistema operativo: Linux, macOS, Windows (MinGW/WSL)

#### Compilazione
```bash
# Clona la repository
git clone https://github.com/patt509/Libreria-LASD-2024-2025.git
cd Libreria-LASD-2024-2025

# Compila il progetto
make

# Esegui i test
./main
```

#### Configurazione Avanzata
Il Makefile supporta le seguenti operazioni:
```bash
# Compila tutto il progetto
make

# Pulisce i file oggetto e l'eseguibile
make clean
```

**Nota**: I target `debug` e `release` menzionati non sono implementati nel Makefile corrente.

### Testing

Il progetto include una suite di test organizzata su due livelli:

1. **zlasdtest**: Test suite ufficiale fornita dal corso per la validazione
2. **zmytest**: Test aggiuntivi implementati per testing approfondito

#### Esecuzione Test
L'applicazione fornisce un'interfaccia interattiva per l'esecuzione selettiva dei test:
```
1: Esegui zlasdtest
2: Esegui tutti i test personalizzati  
3: Esegui solo test su List, Vector e Set
4: Esegui solo test su Heap e Priority Queue
0: Termina
```

### Specifiche Tecniche

#### Requisiti di Sistema
- **Compilatore**: GCC con supporto C++20 (il progetto usa `-std=c++20`)
- **Memoria**: Minimo 1GB RAM per compilazione (include AddressSanitizer)
- **Spazio disco**: 50MB per sorgenti e binari

#### Caratteristiche Implementate
- **Template generici** per supporto multi-tipo
- **Exception safety** con garanzie strong/basic
- **Memory management** automatico via RAII
- **Iterator patterns** per attraversamento sicuro
- **Move semantics** per ottimizzazioni di performance
- **AddressSanitizer** abilitato per debug della memoria (`-fsanitize=address`)

### Analisi delle Performance

| Operazione | Vector | List | Set (List) | Set (Vector) | Heap | Priority Queue |
|------------|--------|------|------------|--------------|------|----------------|
| Inserimento | O(1) amm. | O(1) | O(n) | O(n) | O(log n) | O(log n) |
| Rimozione | O(n) | O(1) | O(n) | O(n) | O(log n) | O(log n) |
| Ricerca | O(n) | O(n) | O(n) | O(n) | O(n) | O(n) |
| Accesso | O(1) | O(n) | O(n) | O(n) | O(1) top | O(1) top |

### Credits

**Studente**: Luca Lucci  
**Corso**: Laboratorio di Algoritmi e Strutture Dati  
**Docente**: Prof.ssa Anna Corazza  
**Università**: Università degli Studi di Napoli "Federico II"  
**Anno Accademico**: 2024/2025

**Nota**: Questo progetto è basato su template e specifiche fornite dal corso.

---

## English Version

### Description

Implementation of data structures and algorithms required for the **Laboratory of Algorithms and Data Structures** course at the University of Naples "Federico II", under the supervision of Prof. Anna Corazza.

The project consists of implementing a data structure library using C++20, based on templates and specifications provided by the course. The implementation uses multiple virtual inheritance, generic templates, and advanced design patterns following course guidelines.

### System Architecture

### System Architecture

The library implements a class hierarchy based on specifications provided by the course, using multiple virtual inheritance to provide flexibility and code reusability:

#### Base Containers
- **Container**: Abstract base class for all containers
- **ClearableContainer**: Extends Container with clearing functionality
- **ResizableContainer**: Adds support for dynamic resizing

#### Specialized Containers
- **TestableContainer**: Base for containers with element existence testing
- **DictionaryContainer**: Implements collections with unique elements
- **OrderedDictionaryContainer**: Dictionaries with ordered access operations

#### Traversal and Mapping
- **TraversableContainer**: Supports read-only traversal with Visitor pattern
- **MappableContainer**: Enables traversal with element modification
- Specialized implementations for different traversal orders:
  - PreOrder/PostOrder (linear structures and trees)
  - InOrder (binary search trees)
  - BreadthFirst (level-order traversal)

#### Linear Containers
- **LinearContainer**: Provides indexed access to elements
- **MutableLinearContainer**: Extends with index-based modification capabilities
- **SortableLinearContainer**: Adds integrated sorting algorithms

### Implemented Data Structures

| Structure | Implementation | File |
|-----------|----------------|------|
| **Vector** | Dynamic array | `vector/vector.hpp` |
| **List** | Doubly linked list | `list/list.hpp` |
| **Set (List-based)** | List-based set | `set/lst/setlst.hpp` |
| **Set (Vector-based)** | Vector-based set | `set/vec/setvec.hpp` |
| **Heap** | Binary heap on vector | `heap/vec/heapvec.hpp` |
| **Priority Queue** | Heap-based priority queue | `pq/heap/pqheap.hpp` |

### Build and Installation

#### Prerequisites
- GCC compiler with full C++20 support
- Make utility
- Operating system: Linux, macOS, Windows (MinGW/WSL)

#### Compilation
```bash
# Clone the repository
git clone https://github.com/patt509/Libreria-LASD-2024-2025.git
cd Libreria-LASD-2024-2025

# Build the project
make

# Run tests
./main
```

#### Advanced Configuration
The Makefile supports the following operations:
```bash
# Build the entire project
make

# Clean object files and executable
make clean
```

**Note**: The `debug` and `release` targets mentioned are not implemented in the current Makefile.

### Testing

The project includes a test suite organized on two levels:

1. **zlasdtest**: Official test suite provided by the course for validation
2. **zmytest**: Additional tests implemented for thorough testing

#### Test Execution
The application provides an interactive interface for selective test execution:
```
1: Run zlasdtest
2: Run all custom tests
3: Run only List, Vector and Set tests
4: Run only Heap and Priority Queue tests
0: Exit
```

### Technical Specifications

#### System Requirements
- **Compiler**: GCC with C++20 support (project uses `-std=c++20`)
- **Memory**: Minimum 1GB RAM for compilation (includes AddressSanitizer)
- **Disk Space**: 50MB for sources and binaries

#### Implemented Features
- **Generic templates** for multi-type support
- **Exception safety** with strong/basic guarantees
- **Automatic memory management** via RAII
- **Iterator patterns** for safe traversal
- **Move semantics** for performance optimizations
- **AddressSanitizer** enabled for memory debugging (`-fsanitize=address`)

### Performance Analysis

| Operation | Vector | List | Set (List) | Set (Vector) | Heap | Priority Queue |
|-----------|--------|------|------------|--------------|------|----------------|
| Insertion | O(1) amm. | O(1) | O(n) | O(n) | O(log n) | O(log n) |
| Removal | O(n) | O(1) | O(n) | O(n) | O(log n) | O(log n) |
| Search | O(n) | O(n) | O(n) | O(n) | O(n) | O(n) |
| Access | O(1) | O(n) | O(n) | O(n) | O(1) top | O(1) top |

### Credits

**Student**: Luca Lucci  
**Course**: Laboratory of Algorithms and Data Structures  
**Professor**: Prof. Anna Corazza  
**University**: University of Naples "Federico II"  
**Academic Year**: 2024/2025

**Note**: This project is based on templates and specifications provided by the course.

---

### License

This project is developed for educational purposes as part of the university curriculum.

### Contributing

This is an academic project based on course templates and specifications. For questions or suggestions regarding the implementation, please refer to the course materials.

### Contact

For technical questions about this implementation, please contact through the appropriate academic channels.

---

*Implementation of LASD course requirements at University of Naples "Federico II"*
