#pragma once
// User.h
#ifndef USER_H
#define USER_H

#include <string>

// This class represents a user in the application
class User {
private:
    std::string username; // Username of the user
    std::string password; // Password of the user
    bool isLoggedIn; // Login status of the user

public:
    User(const std::string& u, const std::string& p); // Constructor to initialize the user
    bool login(const std::string& u, const std::string& p); // Method to log in the user
    void logout(); // Method to log out the user
    std::string getUsername() const; // Get the username
    bool getLoginStatus() const; // Get the login status
};

#endif // USER_H
