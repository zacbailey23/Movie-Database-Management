#include <iostream>
#include <vector>
#include "DirectorSkipList.hpp"
using namespace std;

// Constructor for DirectorSkipList with default capacity and levels
DirectorSkipList::DirectorSkipList()
{

    capacity = DEFAULT_CAPACITY;
    levels = DEFAULT_LEVELS;
    head = new DirectorSLNode("", levels);
    size = 0;
}

// Constructor for DirectorSkipList with given capacity and levels
DirectorSkipList::DirectorSkipList(int _cap, int _levels)
{

    capacity = _cap;
    levels = _levels;
    head = new DirectorSLNode("", levels);
    size = 0;
}

// Destructor for DirectorSkipList that deletes all nodes in the skip list (but not the MovieNodes, which are shared with other data structures)
DirectorSkipList::~DirectorSkipList()
{

  
    DirectorSLNode *current = head;
    DirectorSLNode *toDelete;

    // Loop through all levels of the skip list
    while (current != nullptr)
    {
        // Save the next node at the current level before deleting the current node
        toDelete = current;
        current = current->next[0];

        // Delete the current node
        delete toDelete;
    }
}

// Inserts a movie node into the skip list with the specified director
void DirectorSkipList::insert(string director, MovieNode *_movie)
{

     // Start at the head of the skip list
    DirectorSLNode* current = head;
    // Create an array to store the nodes that need to be updated
    DirectorSLNode* update[levels];
    int i, level;

    // Find the node with the specified director or the position where it should be inserted
    for (i = levels-1; i >= 0; i--) {
        // Move to the right until the next node's director string is greater than or equal to the target director string
        while (current->next[i] != nullptr && current->next[i]->director<director)
            current = current->next[i];
        // Store the current node in the update array at this level
        update[i] = current;
    }
    // Move to the next node at the lowest level
    current = current->next[0];

    if (current != nullptr && current->director == director) {
        // If the node already exists, add the movie to its vector of movies
        current->addMovie(_movie);
    } else {
        // Otherwise, create a new node and insert it into the skip list
        // Randomly determine the level of the new node
        DirectorSLNode* newNode = new DirectorSLNode(director, rand() % levels + 1);
        // Add the movie to the new node's vector of movies
        newNode->addMovie(_movie);

        // Insert the new node at each level up to its maximum level
        for (level = 0; level < newNode->next.size(); level++) {
            // Set the new node's pointers to the same nodes as the current node at this level
            newNode->next[level] = update[level]->next[level];
            // Set the current node's pointers to the new node at this level
            update[level]->next[level] = newNode;
        }

        // Update the size of the skip list
        size++;
    }

}

// Searches for a node in the skip list with the specified director
DirectorSLNode *DirectorSkipList::search(string director)
{
    // Start at the head of the skip list
    DirectorSLNode *current = head;

    // Traverse the skip list at the highest level
    for (int i = levels - 1; i >= 0; i--)
    {
        // Move to the right until the next node's director string is greater than or equal to the target director string
        while (current->next[i] != nullptr && current->next[i]->director < director)
        {
            current = current->next[i];
        }
    }

    // Move to the next node at the lowest level
    current = current->next[0];

    // Check if the current node matches the target director string
    if (current != nullptr && current->director == director)
    {
        // If so, return a pointer to the current node
        return current;
    }
    else
    {
        // Otherwise, the target director string was not found in the skip list
        return nullptr;
    }
}

// Pretty-prints the skip list
void DirectorSkipList::prettyPrint()
{
    cout << "Skip List: " << endl;

    for (int level = levels - 1; level >= 0; level--)
    {
        cout << "Level " << level+1 << ": ";
        DirectorSLNode *current = head->next[level];

        while (current != nullptr)
        {
            // Print out the director and their movies
            cout << current->director << " (";
            // for (auto movie : current->movies) {
            //     cout << movie->title << ", " << movie->year << " ";
            // }
            cout << ") ";

            // Print out arrows to represent left and right nodes
            if (current->next[level] != nullptr)
            {
                cout << "-->";
            }
            else
            {
                cout << "-- END";
            }

            // Move to the next node at the current level
            current = current->next[level];
        }

        cout << endl;
    }
}
