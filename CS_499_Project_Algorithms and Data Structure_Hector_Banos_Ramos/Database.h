#pragma once
// Database.h
// I used efficient data structures for managing user sessions and item tracking.
// The Database class uses an std::unordered_map to handle item counts in-memory,
// providing average constant-time complexity for operations such as insertion, lookup,
// and updates.

#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <utility>
#include <sqlite3.h>
#include <unordered_map> // I included this for unordered_map

// This class handles all database operations for the application
class Database {
private:
    sqlite3* db; // SQLite database connection
    bool executeQuery(const std::string& query); // Helper function to execute SQL queries

public:
    Database(const std::string& dbName); // Constructor to initialize the database
    ~Database(); // Destructor to close the database connection
    void addUser(const std::string& username, const std::string& password); // Add a new user to the database
    bool authenticateUser(const std::string& username, const std::string& password); // Check if user credentials are correct
    void addItem(const std::string& username, const std::string& item); // Add a new item for the user
    bool searchItem(const std::string& username, const std::string& item); // Check if an item exists for the user
    // I used a hash table data structure
    std::unordered_map<std::string, int> getItems(const std::string& username); // Get all items and their counts for the user
    bool userExists(const std::string& username); // Check if a username already exists
};

#endif // DATABASE_H
