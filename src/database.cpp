#include <string>
#include <unordered_set>
#include <mutex>

#include "include/database.hpp"

Database::Database(): usernames({ "SERVER" })
{
    
}

bool Database::contains_username(const char *username)
{
    std::unique_lock<std::mutex> lock(usernames_mutex);
    return usernames.find(std::string(username)) != usernames.end();
}

void Database::add_username(const char *username)
{
    std::unique_lock<std::mutex> lock(usernames_mutex);
    usernames.insert(std::string(username));
}

void Database::remove_username(const char *username)
{
    std::unique_lock<std::mutex> lock(usernames_mutex);
    usernames.erase(std::string(username));
}