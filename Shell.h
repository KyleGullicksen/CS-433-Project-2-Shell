//
// Created by stoffel on 10/8/17.
//

#ifndef INC_2_SHELL_H
#define INC_2_SHELL_H

#include <iostream>

#include <vector>
#include <deque>
#include <sys/wait.h>
#include <chrono>
#include <thread>
#include <string.h>
#include <unistd.h>
#include <cstring>
#include <regex>

#include "CommandAndOptions.h"

using namespace std;

class Shell
{
private:
    deque<CommandAndOptions> commandHistory;
    int processCount = 0;
    int maxProcessCount = 50;
    int maxSleepCount = 5;
    regex nHistoryItemsMatcher;

protected:
    bool strContains(string &str, string string1);
    string trim(string &str);
    char *convert(string &str);
    void displayHistory();
    bool handleBuiltInCommands(string &commandLine);
    CommandAndOptions parseCommandAndOptions(string &commandLine);
    void executeCommand(CommandAndOptions &commandWithOptions);
    string removeNewlines(string &commandLine);

public:
    void processCommandLine(string &commandLine);
    void startNewShellSession();

    Shell();
};


#endif //INC_2_SHELL_H
