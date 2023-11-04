#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "MovieHashTable.hpp"
#include "DirectorSkipList.hpp"

using namespace std;

// Function prototypes
MovieNode *parseMovieLine(string line);
void readMovieCSV(string filename, MovieHashTable &movieTable, DirectorSkipList &directorList);
void display_menu();

//helper function for menu option 4
void printMoviesByDirector(DirectorSLNode *directorNode)
{
    // Check if the director has any movies in the database
    if (directorNode != nullptr && !directorNode->movies.empty())
    {
        cout << directorNode->director << " directed the following movies:\n";
        int i = 0;
        for (int i = 0; i < directorNode->movies.size(); i++)
        {
            MovieNode *movie = directorNode->movies[i];
            cout << i << ":  " << movie->title << " (" << movie->year << ")\n";
        }
    }
    else
    {
        cout << "No movies found for that director.\n";
    }
}
// Main function
int main(int argc, char *argv[])
{
    // TODO

    if (argc < 4)
    {
        cout << "Invalid number of arguments." << endl;
        cout << "Usage: ./<program name> <csv file> <hashTable size> <skipList size>" << endl;
        return 1;
    }

    MovieHashTable movieTable(stoi(argv[2])); //call default constructor
    DirectorSkipList skipList(stoi(argv[3]), 10);
    readMovieCSV(argv[1], movieTable, skipList);
    int option = 0;
    do
    {
        // Display menu options
        cout << "Number of collisions: " << movieTable.getCollisions() << endl;
        display_menu();

        // Get user input
        cout << "Enter an option (1-5): ";
        cin >> option;
        cout << "\n";

        // Check if input is valid
        if (cin.fail() || option < 1 || option > 6)
        {
            cout << "Invalid option. Please enter a number between 1 and 5." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        // Execute chosen option
        switch (option)
        {
        case 1:
        {
            string title;
            cout << "Enter the title of the movie you want to search for: ";
            cin.ignore();
            getline(cin, title);

            // Call the search function to find the movie node with the specified title
            MovieNode *foundMovie = movieTable.search(title);

            if (foundMovie != nullptr)
            {
                cout << "The director of " << title << " is " << foundMovie->director << endl;
            }
            else
            {
                cout << "Movie not found." << endl;
            }
            cout << endl;
            break;
        }
        case 2:
        {
            string directorsName;
            cout << "Enter the director name: ";
            cin.ignore();
            getline(cin, directorsName);

            DirectorSLNode *directorNode = skipList.search(directorsName);
            if (directorNode != nullptr)
            {
                int movieCount = directorNode->getMovieCount();
                cout << directorsName << " directed " << movieCount << " movie(s)." << endl;
            }
            else
            {
                cout << "Director does not exist in the database." << endl;
            }
            cout << endl;
            break;
        }
        case 3:
        {
            string movieTitle;
            cout << "Enter the title of the movie you want to search for: ";
            cin.ignore();
            getline(cin, movieTitle);

            MovieNode *movieNodePtr = movieTable.search(movieTitle);
            if (movieNodePtr == nullptr)
            {
                cout << "Movie not found in database." << endl;
            }
            else
            {
                cout << "Title: " << movieNodePtr->title << endl;
                cout << "Description: " << movieNodePtr->description << endl;
                cout << "Actors: " << movieNodePtr->actors << endl;
                cout << "Year: " << movieNodePtr->year << endl;
                cout << "Genre: " << movieNodePtr->genre << endl;
                cout << endl;
            }
            break;
        }
        case 4:
        {
            string directorssName;
            cout << "Enter director name: ";
            cin.ignore();                 // Ignore any newline characters left in the input buffer
            getline(cin, directorssName); // Get the director name as a string

            // Search for the director in the skip list
            DirectorSLNode *directorNode = skipList.search(directorssName);

            // Print the list of movies by the director
            if (directorNode != nullptr)
            {
                printMoviesByDirector(directorNode);
            }
            else
            {
                cout << "The director does not exist in the database." << endl;
            }

            cout << endl;
            break;
        }
        case 5:
        {
            skipList.prettyPrint();
            break;
        }
        case 6:
        {
            cout << "Goodbye!" << endl;
            break;
        }
        }
    }
    while (option != 6);
    return 0;
}

// Function to parse a CSV line into a MovieNode object
MovieNode *parseMovieLine(string line)
{
    stringstream ss(line);
    vector<string> fields;
    string field;

    // Loop through the characters in the line
    bool in_quotes = false;
    for (size_t i = 0; i < line.length(); ++i)
    {
        char c = line[i];
        if (c == '\"')
        {
            in_quotes = !in_quotes;
        }
        else if (c == ',' && !in_quotes)
        {
            // add field to vector and reset for next field
            fields.push_back(field);
            field.clear();
        }
        else
        {
            field += c;
            // handle double quotes inside a quoted field
            if (in_quotes && c == '\"' && i < line.length() - 1 && line[i + 1] == '\"')
            {
                field += '\"'; // add the second double quote and skip it
                ++i;
            }
        }
    }
    fields.push_back(field);

    if (fields.size() != 12)
    {
        cerr << "Error: Invalid movie line format" << line << endl;
        return nullptr;
    }

    int rank = stoi(fields[0]);
    string title = fields[1];
    string genre = fields[2];
    string description = fields[3];
    string director = fields[4];
    string actors = fields[5];
    int year = stoi(fields[6]);
    int runtime = stoi(fields[7]);
    float rating = stof(fields[8]);
    int votes = stoi(fields[9]);
    float revenue = stof(fields[10]);
    int metascore = stoi(fields[11]);

    // Create a new MovieNode object with the parsed fields
    MovieNode *movie = new MovieNode(rank, title, genre, description, director, actors, year, runtime, rating, votes, revenue, metascore);
    return movie;
}

// Function to read a CSV file into a vector of MovieNode objects
void readMovieCSV(string filename, MovieHashTable &movieTable, DirectorSkipList &directorList)
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    vector<MovieNode *> movies;
    string line;
    getline(file, line);
    while (getline(file, line))
    {
        MovieNode *movie = parseMovieLine(line);
        if (movie)
        {
            movies.push_back(movie);
        }
    }
    // Create a MovieHashTable object and insert each MovieNode

    for (MovieNode *movie : movies)
    {
        movieTable.insert(movie->title, movie);
        directorList.insert(movie->director, movie);
    }
}

// Function to display the menu options
void display_menu()
{
    cout << "Please choose an option:" << endl;
    cout << "1. Find the director of a movie." << endl;
    cout << "2. Find the number of movies by a director." << endl;
    cout << "3. Find the description of a movie" << endl;
    cout << "4. List the movies by director." << endl;
    cout << "5. Pretty Print the dirctors." << endl;
    cout << "6. Quit" << endl;
    
}
