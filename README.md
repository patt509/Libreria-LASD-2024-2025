Libreria di Algoritmi e Strutture Dati (LASD)
Descrizione
Questa è una libreria C++ di strutture dati implementata per il corso di Laboratorio di Algoritmi e Strutture Dati, tenuto dalla Prof.ssa Anna Corazza per l'anno accademico 2024/2025. Il progetto include implementazioni di container generici, strutture dati lineari, dizionari, insiemi, heap e code di priorità.

La libreria è progettata per essere sia funzionale che didattica, mostrando l'implementazione di algoritmi e strutture dati fondamentali con un'attenzione particolare alla correttezza, all'efficienza e al design del software.

Strutture Dati Implementate
La libreria offre una gamma completa di strutture dati, ciascuna con le proprie caratteristiche e casi d'uso:

Strutture Lineari
Vector: Un array dinamico che offre accesso in tempo O(1) e gestione automatica della memoria. Esiste anche in versione SortableVector con algoritmi di ordinamento integrati.

List: Una lista concatenata singolarmente con puntatori a testa e coda, che consente inserimenti e rimozioni efficienti (O(1)) da entrambe le estremità.

Insiemi (Set)
SetVec: Un'implementazione di un insieme basata su un vettore ordinato. Sfrutta la ricerca binaria per operazioni efficienti di ricerca (O(log n)).

SetLst: Un'implementazione di un insieme che utilizza una lista ordinata, adatta per insiemi di dimensioni moderate dove l'efficienza della memoria è una priorità.

Heap e Code di Priorità
HeapVec: Un'implementazione di uno heap binario basato su vettore, che supporta in modo efficiente le operazioni di heap e l'algoritmo di heapsort.

PQHeap: Una coda di priorità costruita sopra HeapVec, ideale per algoritmi che richiedono un accesso rapido all'elemento con la massima priorità.

Come Iniziare
Per compilare e utilizzare la libreria e i test inclusi, segui questi passaggi.

Prerequisiti
Assicurati di avere un compilatore C++ che supporti lo standard C++20. Il makefile fornito utilizza g++.

g++ (versione che supporta C++20)

make

Compilazione
Per compilare il progetto, esegui semplicemente il comando make dalla directory principale:

Bash

make
Questo comando compilerà tutti i file sorgente e creerà un eseguibile chiamato main.

Per rimuovere i file compilati e l'eseguibile, puoi usare il comando:

Bash

make clean
Esecuzione dei Test
L'eseguibile main avvia una suite di test che ti permette di verificare la correttezza delle strutture dati implementate. Puoi scegliere tra i test forniti dal corso (zlasdtest) e una suite di test personalizzata (zmytest).

Avvia l'applicazione con:

Bash

./main
Ti verrà presentato un menu per scegliere quale suite di test eseguire:

Esegui zlasdtest: Esegue la suite di test standard fornita per il corso.

Esegui tutti i test personalizzati: Esegue tutti i test personalizzati che hai scritto.

Esegui solo test su List, Vector e Set: Limita i test alle strutture dati di base.

Esegui solo test su Heap e Priority Queue: Esegue i test per le strutture dati più avanzate.

Termina: Esce dal programma.

Struttura del Progetto
Il codice è organizzato nelle seguenti directory:

container/: Contiene le interfacce astratte per tutte le strutture dati (Container, LinearContainer, DictionaryContainer, ecc.).

vector/: Implementazione della struttura dati Vector.

list/: Implementazione della struttura dati List.

set/: Implementazioni delle strutture dati SetVec e SetLst.

heap/: Implementazione della struttura dati HeapVec.

pq/: Implementazione della PQHeap (coda di priorità).

zlasdtest/: La suite di test fornita per il corso.

zmytest/: La tua suite di test personalizzata.

Contributi
Questo progetto è stato sviluppato come parte di un corso universitario. Sebbene non si accettino contributi esterni, feedback e suggerimenti sono sempre benvenuti.

Licenza
Questo progetto è distribuito sotto la licenza MIT. Vedi il file LICENSE per maggiori dettagli.
