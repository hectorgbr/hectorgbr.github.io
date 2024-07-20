// User.cpp
#include "User.h"

// Constructor to initialize the user with a username and password
User::User(const std::string& u, const std::string& p) : username(u), password(p), isLoggedIn(false) {}

// Method to log in the user
bool User::login(const std::string& u, const std::string& p) {
    // This code checks if the provided credentials match the stored ones
    if (u == username && p == password) {
        isLoggedIn = true; // Set the login status to true
        return true;
    }
    return false;
}

// Method to log out the user
void User::logout() {
    isLoggedIn = false; // Set the login status to false
}

// Method to get the username
std::string User::getUsername() const {
    return username;
}

// Method to get the login status
bool User::getLoginStatus() const {
    return isLoggedIn;
}
