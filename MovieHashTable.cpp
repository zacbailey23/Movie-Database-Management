#include<iostream>
#include <vector>
#include "MovieHashTable.hpp"
using namespace std;

// Constructor for MovieHashTable with default size
MovieHashTable::MovieHashTable() {
    // TODO
    table_size= DEFAULT_HTABLE_CAPACITY;
    n_collisions = 0;
    table = new MovieNode*[table_size];
            for (int i = 0; i < table_size; ++i) {
                table[i] = nullptr;
            }
}

// Constructor for MovieHashTable with given size
MovieHashTable::MovieHashTable(int s) {
    // TODO
     table_size= s;
    n_collisions = 0;
    table = new MovieNode*[s];
            for (int i = 0; i < s; ++i) {
                table[i] = nullptr;
            }
}

// Destructor for MovieHashTable that deletes all nodes in the hash table
MovieHashTable::~MovieHashTable() {
    // TODO
     for (int i = 0; i < table_size; ++i) {
                MovieNode* current = table[i];
                while (current != nullptr) {
                    MovieNode* next = current->next;
                    delete current;
                    current = next;
                }
            }
            delete[] table;
}

// Hash function for MovieHashTable that returns an index in the hash table for a given movie title.
// Students must use their identikey to come up with a creative hash function that minimizes collisions
// for the given IMDB-Movie database to ensure efficient insertion and retrieval of movie nodes.



int MovieHashTable::hash(string title) {
    int sum = 0;
    //my personal identikey 
    string key = "zaba2966";
    int salt = 12345; // arbitrary salt value
    for (int i = 0; i < title.length(); i++) {
        //we modular division the key because i can exceed the key length so we keep it circular
        // found this from hash function guide:"A salt is a random value that is added to the input before
        // it is hashed. Adding a salt to the key can help to produce more unique hash values and reduce collisions."
        sum += (int(title[i]) + salt) * int(key[i % key.length()]);
    }
    //we multiply it by a huge prime number to reduce collisions
    int index = (sum * 2654435761) % table_size; // use a prime number for table size to reduce more collisions
    return index;
}

// Inserts a movie node into the hash table with the specified title
void MovieHashTable::insert(string title, MovieNode* movie) {
    // TODO
 int index = hash(title);

    // If the slot is empty, create a new linked list with the movie at the head
    if (table[index] == nullptr) {
        table[index] = movie;
    }
    // If the slot already contains a linked list, insert the new movie at the head
    else {
        movie->next = table[index];
        table[index] = movie;
        setCollisions();
    }
}

// Searches for a node in the hash table with the specified title
MovieNode* MovieHashTable::search(string title) {
    // TODO
     int index = hash(title);

    // Check if the hash table is empty at that index
    if (table[index] == nullptr) {
        return nullptr;
    }

    // Traverse the linked list at the index to find the movie node with the given title
    MovieNode* curr = table[index];
    while (curr != nullptr) {
        if (curr->title == title) {
            return curr;
        }
        curr = curr->next;
    }

    // If the movie node is not found in the linked list, it is not in the hash table
    return nullptr;
}

// Returns the number of collisions that have occurred during insertion into the hash table
int MovieHashTable::getCollisions() {
    // TODO
    return n_collisions;
}

// Increments the number of collisions that have occurred during insertion into the hash table
void MovieHashTable::setCollisions() {
    // TODO
    n_collisions++;
}
  