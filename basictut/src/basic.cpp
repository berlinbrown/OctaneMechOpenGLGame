/// basic cpp
#include <iostream>
#include "DriverBots.hpp"

// Basic main program
int main() {
    std::cout << "Hello from C++ on macOS with clang!" << std::endl;

    // Instantiate an object
    DriverBots bot;  // Calls the constructor with val=42
    bot.wanderCommand();
    return 0;
}