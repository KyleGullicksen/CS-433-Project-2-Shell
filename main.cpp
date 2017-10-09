/*
Created by Kyle Gullicksen and Benjamin Siegel
Date Written: 9/25/2017
Course: CS433
Assignment #2

This program will imitate a shell. This user will input a shell
    command, such as ls, or mkdir, etc. Then, the program will
    process the user inputted command and execute it.

    If the user enters a preset unique command, such as "!!"
    or "!N", where N is an integer then the program will handle
    those inputs as if they were built in.

    !! will display the previously entered command and execute it.
    !N will display the Nth previously entered command and execute
        that command, as long a there are N previously entered
        commands at minimum.

    All commands that the user enters into this shell will be
    stored in commandHistory. The command history can be accessed
    via the command "history".
*/

#include <iostream>
#include "Shell.h"

using namespace std;


int main()
{
    Shell shell;
    shell.startNewShellSession();
}