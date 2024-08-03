/*
 * Corner Grocer App
 * Date: 08/03/2024
 * Author: Hector Banos Ramos
 * This is the enhanced version of the item-tracking program for Corner Grocer
 */

// Update: Enhancement One: Software design and engineering
// To meet this requirement I added options to create user accounts with username and password.
// I implemented login and logout functionality. I integrated a database to store user accounts
// and items. I implemented CRUD operations for managing list items. I added two menus one to
// manage user accounts and the other is for options once the user gains access to his account.

// Update: Enhancement Two: Databases
// This requirement is met by using efficient data structures such as std::unordered_map
// for in-memory item tracking.Additionally, SQL's GROUP BY and COUNT functions are used
// for optimized data retrieval and aggregation from the database, eliminating redundancy
// and enhancing performance.

// Update: Enhancement Three: Databases
// I met the database requirement by setting up a database using SQLite to store user accounts
// and items entered by users.I also included CRUD operations such as options to create, log in,
// and delete user accounts. I also allow users to read a text file with items, add an item
// manually, search for a specific item, and delete items.

// Main_Project_Three.cpp
#include <iostream>
#include <iomanip>
#include <limits>
#include <fstream>
#include "Database.h"
#include "User.h"

using namespace std;

// This function creates a new user account
bool createUser(Database& db) {
    string username, password;
    while (true) {
        // Prompt the user for a new username and password
        cout << "Enter new username (or type 'exit' to go back): ";
        cin >> username;
        if (username == "exit") {
            return false;
        }
        if (db.userExists(username)) {
            // Check if the username already exists
            cout << "Username already exists. Please try a different username." << endl;
        }
        else {
            break;
        }
    }
    cout << "Enter new password: ";
    cin >> password;
    // Add the new user to the database
    db.addUser(username, password);
    cout << "Account created successfully!" << endl;
    return false; // The user is taken back to the account management menu
}

// Function to log in an existing user
bool loginUser(Database& db, User& currentUser) {
    string username, password;
    // Prompt the user for their username and password
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    if (db.authenticateUser(username, password)) {
        // Authenticate user and set current user
        currentUser = User(username, password);
        cout << "Login successful!" << endl;
        return true;
    }
    else {
        cout << "Invalid credentials!" << endl;
        return false;
    }
}

// Function to delete a user account
bool deleteUser(Database& db) {
    string username, password;
    // Prompt the user for their username and password
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    // Authenticate user before deleting the account
    if (db.authenticateUser(username, password)) {
        if (db.deleteUser(username)) {
            cout << "Account and all associated items deleted successfully!" << endl;
            return false; // The user is taken back to the account management menu
        }
    }
    cout << "Failed to delete account. Please check your credentials." << endl;
    return false;
}

// Function to add items from a text file to the user's account
void addItemsFromFile(Database& db, const User& currentUser) {
    string fileName = "CS210_Project_Three_Input_File.txt";
    ifstream inputFile(fileName);
    string item;

    if (!inputFile) {
        cerr << "Error opening file: " << fileName << endl;
        return;
    }

    while (getline(inputFile, item)) {
        db.addItem(currentUser.getUsername(), item);
    }

    inputFile.close();
    cout << "Items from the file have been added successfully!" << endl;
}

// Function to delete an individual item from the user's account
bool deleteItem(Database& db, const User& currentUser) {
    string item;
    cout << "Enter the item you wish to delete: ";
    cin >> item;
    // Check if the item exists before attempting to delete
    if (db.searchItem(currentUser.getUsername(), item)) {
        if (db.deleteItem(currentUser.getUsername(), item)) {
            cout << "Item deleted successfully!" << endl;
            return true;
        }
    }
    cout << "Failed to delete item. Item not found." << endl;
    return false;
}

int main() {
    int userChoiceVar;
    bool exitRequest = false;

    // Initialize database and user objects
    Database db("corner_grocer.db");
    User currentUser("", "");

    while (true) {
        // Display account management options
        cout << "1. Create account" << endl;
        cout << "2. Login" << endl;
        cout << "3. Delete account" << endl; // I added this new option to delete account
        cout << "4. Exit" << endl;
        cout << "Enter choice: ";
        cin >> userChoiceVar;

        // Error handling for invalid input
        try {
            if (cin.fail()) {
                throw runtime_error("Invalid input. Please enter a number.");
            }
            if (userChoiceVar < 1 || userChoiceVar > 4) {
                throw runtime_error("Invalid choice. Please enter 1 and 4.");
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (userChoiceVar == 1) {
            if (!createUser(db)) {
                continue; // Go back to the account management options
                currentUser.logout(); // Clear the current user
                break;
            }
        }
        else if (userChoiceVar == 2) {
            if (!loginUser(db, currentUser)) {
                continue; // Go back to the account management options
            }
        }
        else if (userChoiceVar == 3) {
            if (!deleteUser(db)) {
                //currentUser.logout(); // Clear the current user
                continue; // Go back to the account management options
                //break;
            }
        }
        else if (userChoiceVar == 4) { // Exit the program
            return 0;
        }
        break; // Exit the loop if an account is successfully created or
        // user logs in or deletes an account
    }

   // Main menu loop - Displays options for item management
    while (!exitRequest) {
        cout << "\n1. Search for an item" << endl;
        cout << "2. Display all item frequencies" << endl;
        cout << "3. Display histogram of items" << endl;
        cout << "4. Add an item" << endl;
        cout << "5. Add items from text file" << endl;
        cout << "6. Delete an item" << endl; // New option to delete an item
        cout << "7. Exit" << endl;
        cout << "Enter your choice (1-7): ";
        cin >> userChoiceVar;

        try {
            if (cin.fail()) {
                throw runtime_error("Invalid input. Please enter a number.");
            }

            if (userChoiceVar < 1 || userChoiceVar > 7) {
                throw runtime_error("Invalid choice. Please enter a number between 1 and 7.");
            }

            if (userChoiceVar == 1) {
                string searchItem;
                cout << "Enter the item you wish to look for: ";
                cin >> searchItem;
                // Search for item in the user's account
                if (db.searchItem(currentUser.getUsername(), searchItem)) {
                    cout << "Item found in your account!" << endl;
                }
                else {
                    cout << "Item not found." << endl;
                }
            }
            else if (userChoiceVar == 2) {
                // Display all item frequencies
                auto items = db.getItems(currentUser.getUsername());
                // items is an std::unordered_map<std::string, int>.
                for (const auto& item : items) {
                    cout << item.first << ": " << item.second << endl;
                }
            }
            else if (userChoiceVar == 3) {
                // Display histogram of items
                auto items = db.getItems(currentUser.getUsername());
                for (const auto& item : items) {
                    cout << item.first << ": ";
                    for (int i = 0; i < item.second; ++i) {
                        cout << "*";
                    }
                    cout << endl;
                }
            }
            else if (userChoiceVar == 4) {
                // Add a new item to the user's account
                string newItem;
                cout << "Enter the item to add: ";
                cin >> newItem;
                db.addItem(currentUser.getUsername(), newItem);
                cout << "Item added successfully!" << endl;
            }
            else if (userChoiceVar == 5) {
                // Add items from text file to the user's account
                addItemsFromFile(db, currentUser);
            }
            else if (userChoiceVar == 6) {
                // Delete an item from the user's account
                deleteItem(db, currentUser);
            }
            else if (userChoiceVar == 7) {
                // Exit the program
                exitRequest = true;
            }
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Log out the current user
    currentUser.logout();
    return 0;
}
