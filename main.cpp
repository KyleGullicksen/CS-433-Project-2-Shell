/*
Created by Kyle Gullicksen and Benjamin Siegel
Date Written: 9/25/2017
Course: CS433
Assignment #2

This program will imititate a shell. This user will input a shell
    command, such as ls, or mkdir, etc. Then, the program will
    process the user inputed command and execute it.

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

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <deque>
#include <sys/wait.h>

#include "CommandAndOptions.h"

using std::string;
using std::vector;
using boost::split;
using std::cout;
using std::endl;
using std::deque;

//Initialization of the deque
deque<CommandAndOptions> commandHistory;

void processCommand(CommandAndOptions commandWithOptions);

string trim(string str)
{
    int startingIndex = 0, endingIndex = str.length() - 1;

    for(int index = 0; index < str.length(); ++index)
    {
        if(!isspace(str[index]))
        {
            startingIndex = index;
            break;
        }
    }

    for(int index = str.length() - 1; index > -1; --index)
    {
        if(!isspace(str[index]))
        {
            endingIndex = index;
            break;
        }
    }

    return str.substr(startingIndex, (endingIndex - startingIndex) + 1);
}


//displayHistory will verify that commands have been entered
//then display all commands stored in the deque commandHistory.
void displayHistory()
{
    if(commandHistory.empty())
    {
        cout << "Command history is empty or does not exist." << endl;
        return;
    }
    for (int i = 0; i < commandHistory.size(); i++)
    {
        cout << i << " " << commandHistory[i].command << endl;
    }
}

//createHistory pushes the most recently entered command
//by the user onto the deque commandHistory.
void createHistory(char command[])
{
    commandHistory.push_back(command);
}

//exit, history, !!, !n (!1 executes the most recent, !2 executes the 2 most recent commands

/*
handleBuiltInCommands is intended to work with commands
that are defined in the homework problem, these commands
are:
    exit- exits the program and brings the user back to their
        program or shell
    !!- displays the most recently used user inputed command
        and executes it.
    !N- displays the Nth used user inputed command and executs it

*/
bool handleBuiltInCommands(string commandLine)
{
    string cleanCommandLine = trim(commandLine);
    CommandAndOptions previousCommand;

    if(cleanCommandLine == "exit")
    {
        exit(0xBAD);
    } else if(cleanCommandLine == "!!")
    {
        if(commandHistory.empty())
        {
            cout << "History is empty." << endl;
            return true;
        }
        //grab and store the previous command
        //previousCommand = commandHistory.pop_front();
        previousCommand = commandHistory.front();
        commandHistory.pop_front();

        return true;

    } else if(cleanCommandLine[0] == '!') //command is "!N"
    {
        //These next two lines grab the number from !N
        //and converts it into the ascii number for N
        char N = cleanCommandLine[1];
        int numOfCommands = (int) N - 48;

        if(commandHistory.size() < numOfCommands)
        {
            cout << "Command unavailable or does not exist." << endl;
            return
                true;
        }

        for(int i = 0; i < commandHistory.size() && i < numOfCommands; i++)
        {
            commandHistory.pop_front();
        }

        previousCommand = commandHistory.front();
        return true;
    } else
        return false;
}



//ls-la

CommandAndOptions parseCommandAndOptions(string commandLine)
{
    CommandAndOptions commandWithOptions;

    std::size_t pos = commandLine.find_first_of('-');

    //That's a command with no options
    if(pos == -1)
    {
        //commandWithOptions.command = &commandLine[0u];
        commandWithOptions.command = commandLine;
        return commandWithOptions;
    }

    string command = commandLine.substr(0, pos);

    string allOptions = commandLine.substr(pos, commandLine.length() - pos);

    vector<string> options;
    boost::split(options, allOptions, boost::is_any_of("-"));
    string individualOption;

    for(int index = 0; index < options.size(); ++index)
    {
        individualOption = trim(options[index]);

        if(individualOption.empty())
            continue;

        commandWithOptions.options.push_back(individualOption);
    }

//    commandWithOptions.command = &command[0u];
//    commandWithOptions.options = &options[0u];

    commandWithOptions.command = command;
    //commandWithOptions.options = options;

    return commandWithOptions;
}

char * convert(string & str)
{
    char * cStyleString = new char[str.length()];

    for(int index = 0; index < str.length(); ++index)
    {
        cStyleString[index] = str[index];
    }

    return cStyleString;
}

void build_array(char* strings[], vector<string> & vector)
{
    for(int index = 0; index < vector.size(); ++index)
    {
        strings[index] = convert(vector[index]);
    }
}



void processCommand(CommandAndOptions commandWithOptions)
{
    pid_t pid = fork();

    char* strings[] = new char*[commandWithOptions.options.size()];


    if(pid == 0)
    {
        execvp(convert(commandWithOptions.command), build_array(commandWithOptions.options));
        cout << "Command does not exist." << endl;
        exit(1);
    }

    else if (pid > 0)
    {
        if(!commandWithOptions.amp)
        {
            wait(NULL);
        }
    }

    else
    {
        cout << "Fork failed." << endl;
        exit(1);
    }
}

void processCommandLine(string commandLine)
{
//    if(!boost::contains(commandLine, "|"))
//        processPipedCommand(commandLine);
//    else if(!handleBuiltInCommands(commandLine))
//    {
//        CommandAndOptions commandAndOptions = parseCommandAndOptions(commandLine);
//        processCommand(commandAndOptions);
//    }

    if(!handleBuiltInCommands(commandLine))
    {
        CommandAndOptions commandAndOptions = parseCommandAndOptions(commandLine);
        processCommand(commandAndOptions);
    }
}


int main()
{
    /*
     * // ls -la | grep "Shit"
     */

//    string commandString = R"(ls -la | grep "Shit")";
//
//
//    //split on |
//
//    /*
//     * for each command in the
//     */
//

    string commandString = "ls -la -lst -moreoptions -evenmoreoptions";

//    vector<string> commandLines;
//    boost::split(commandLines, commandString, boost::is_any_of("\n"));
//
//    for(int index = 0; index < commandLines.size(); ++index)
//    {
//        processCommandLine(commandLines[index]);
//    }


    processCommandLine(commandString);
}