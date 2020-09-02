/* 
 * File:   main.cpp
 * Author: Peter Hjalmarsson 
 *
 * Created on September 3, 2015, 9:29 PM
 */

#include <cstdlib>
#include <string>
#include "Interface.h"

/*
 * 
 */
int main(int argc, char** argv) {
    Interface *interface = new Interface();
    if (argc > 1) {
        interface->openFile(argv[1]);
        return 0;
    }
    std::string command = "";
    while (command != "exit" && command != "quit") {
        char com[128];
        std::cin.getline(com, 128);
        command.assign(com);
        interface->command(command);
    }
    return 0;
}

