/*
 * Corner Grocer App
 * Date: 07/20/2024
 * Author: Hector Banos Ramos
 * This is the enhanced version of the item-tracking program for the Corner Grocer
 */
// Update: Enhancement One: Software design and engineering
// To meet this requirement I added options to create user accounts with username and password.
// I implemented login and logout functionality. I integrated a database to store user accounts
// and items. I implemented CRUD operations for managing list items. I added two menus one to
// manage user accounts and the other is for options once the user gains access to his account.

// Main_Project_CS-499.cpp
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
    return true;
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
        cout << "Enter choice: ";
        cin >> userChoiceVar;

        // Error handling for invalid input
        try {
            if (cin.fail()) {
                throw runtime_error("Invalid input. Please enter a number.");
            }
            if (userChoiceVar < 1 || userChoiceVar > 2) {
                throw runtime_error("Invalid choice. Please enter 1 or 2.");
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
            }
        }
        else if (userChoiceVar == 2) {
            if (!loginUser(db, currentUser)) {
                continue; // Go back to the account management options
            }
        }
        break; // Exit the loop if an account is successfully created or user logs in
    }

    // Main menu loop - Displays options for item management
    while (!exitRequest) {
        cout << "\n1. Search for an item" << endl;
        cout << "2. Display all item frequencies" << endl;
        cout << "3. Display histogram of items" << endl;
        cout << "4. Add an item" << endl;
        cout << "5. Add items from text file" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice (1-6): ";
        cin >> userChoiceVar;

        try {
            if (cin.fail()) {
                throw runtime_error("Invalid input. Please enter a number.");
            }

            if (userChoiceVar < 1 || userChoiceVar > 6) {
                throw runtime_error("Invalid choice. Please enter a number between 1 and 6.");
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
