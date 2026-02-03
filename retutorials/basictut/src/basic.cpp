/// basic cpp

#include <iostream>

#include "DriverBots.hpp"
#include "list.hpp"
#include "plist.hpp"

// Basic main program
int main()
{
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

  std::cout << "**** Continue to PLIST test **** \n";
  std::cout << "**** //// **** \n\n";

  // Usage plist
  PtrList plist;

  auto a = new int(10);
  auto b = new int(20);
  auto c = new int(30);

  plist.insertFront(a);
  plist.insertFront(b);
  plist.insertFront(c);

  std::cout << "Int list:\n";
  plist.printListAsInt();

  plist.deleteNode(b);
  std::cout << "After deleting 20:\n";
  plist.printListAsInt();

  auto removed = static_cast<int*>(plist.removeFront());
  std::cout << "Removed front: " << *removed << "\n";
  delete removed;  // don't forget to free the int

  std::cout << "Remaining:\n";
  plist.printListAsInt();

  std::cout << "Doing last clean up \n";

  return 0;
}