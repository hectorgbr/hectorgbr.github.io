// Database.cpp
#include "Database.h"
#include <iostream>
#include <stdexcept>

// Constructor to open the database
Database::Database(const std::string& dbName) {
    // Attempt to open the database with the given name
    if (sqlite3_open(dbName.c_str(), &db)) {
        // If there's an error, throw an exception with the error message
        throw std::runtime_error("Can't open database: " + std::string(sqlite3_errmsg(db)));
    }
}

// Destructor to close the database
Database::~Database() {
    // Close the database connection when the Database object is destroyed
    sqlite3_close(db);
}

// Helper function to execute a query without returning results
bool Database::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    // Execute the SQL query
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        // If there's an error, print it and free the error message memory
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// Function to add a new user to the database
void Database::addUser(const std::string& username, const std::string& password) {
    // Create the SQL query to insert a new user
    std::string sql = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "');";
    // Execute the query and throw an exception if it fails
    if (!executeQuery(sql)) {
        throw std::runtime_error("Failed to add user.");
    }
}

// Function to authenticate a user
bool Database::authenticateUser(const std::string& username, const std::string& password) {
    // Create the SQL query to check if the username and password match
    std::string sql = "SELECT COUNT(*) FROM users WHERE username = '" + username + "' AND password = '" + password + "';";
    sqlite3_stmt* stmt;
    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        // If there's an error, print it and return false
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    // Execute the query and check the result
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return count > 0;
    }
    sqlite3_finalize(stmt);
    return false;
}

// Function to add an item to the user's account
void Database::addItem(const std::string& username, const std::string& item) {
    // Create the SQL query to insert a new item for the user
    std::string sql = "INSERT INTO items (username, item) VALUES ('" + username + "', '" + item + "');";
    // Execute the query and throw an exception if it fails
    if (!executeQuery(sql)) {
        throw std::runtime_error("Failed to add item.");
    }
}

// Function to search for an item in the user's account
bool Database::searchItem(const std::string& username, const std::string& item) {
    // Create the SQL query to count the items matching the username and item
    std::string sql = "SELECT COUNT(*) FROM items WHERE username = '" + username + "' AND item = '" + item + "';";
    sqlite3_stmt* stmt;
    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        // If there's an error, print it and return false
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    // Execute the query and check the result
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return count > 0;
    }
    sqlite3_finalize(stmt);
    return false;
}

// Function to get all items and their counts for the user
std::vector<std::pair<std::string, int>> Database::getItems(const std::string& username) {
    // Create the SQL query to get items and their counts grouped by item name
    std::string sql = "SELECT item, COUNT(item) FROM items WHERE username = '" + username + "' GROUP BY item;";
    sqlite3_stmt* stmt;
    std::vector<std::pair<std::string, int>> items;
    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        // If there's an error, print it and return an empty vector
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return items;
    }
    // Loop through the results and add them to the vector
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string item = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int count = sqlite3_column_int(stmt, 1);
        items.emplace_back(item, count);
    }
    sqlite3_finalize(stmt);
    return items;
}

// Function to check if a username already exists
bool Database::userExists(const std::string& username) {
    // Create the SQL query to count the users with the given username
    std::string sql = "SELECT COUNT(*) FROM users WHERE username = '" + username + "';";
    sqlite3_stmt* stmt;
    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        // If there's an error, print it and return false
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    // Execute the query and check the result
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return count > 0;
    }
    sqlite3_finalize(stmt);
    return false;
}
