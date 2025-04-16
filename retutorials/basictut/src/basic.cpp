/// basic cpp

#include "DriverBots.hpp"
#include "list.hpp"
#include "plist.hpp"

#include <iostream>

// Basic main program
int main() {
    std::cout << "Hello from C++ on macOS with clang!" << std::endl;

    // Instantiate an object
    DriverBots bot;  // Calls the constructor with val=42
    bot.wanderCommand();

    // Use list and plist
    List list;

    list.insertFront(100);
    list.insertFront(200);
    list.insertFront(300);

    list.printList();

    list.deleteNode(200);
    std::cout << "After deletion:\n";
    list.printList();

    // Usage plist
    PtrList list;

    auto a = new int(10);
    auto b = new int(20);
    auto c = new int(30);

    list.insertFront(a);
    list.insertFront(b);
    list.insertFront(c);

    std::cout << "Int list:\n";
    list.printListAsInt();

    list.deleteNode(b);
    std::cout << "After deleting 20:\n";
    list.printListAsInt();

    auto removed = static_cast<int*>(list.removeFront());
    std::cout << "Removed front: " << *removed << "\n";
    delete removed; // don't forget to free the int

    std::cout << "Remaining:\n";
    list.printListAsInt();

    // Clean up
    delete a;
    delete b;
    delete c;

    return 0;
}