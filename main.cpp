//
// Created by stoffel on 10/4/17.
//

#include <iostream>

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <deque>

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
    int startingIndex, endingIndex;

    for(int index = 0; index < str.length(); ++index)
    {
        if(!isspace(str[index]))
        {
            startingIndex = index;
            break;
        }
    }

    for(int index = str.length() - 1; index > -1; ++index)
    {
        if(!isspace(str[index]))
        {
            endingIndex = index;
            break;
        }
    }

    return str.substr(startingIndex, (endingIndex - startingIndex) + 1);
}


void displayHistory()
{
    if(commandHistory.emty())
    {
        cout << "Command history is empty or does not exist." << endl;
        return;
    }
    for (int i = 0; i < commandHistory.size(); i++)
    {
        cout << i++ << " " << commandHistory[i] << endl;
    }
}

void createHistory(char command[])
{
    commandHistory.push_back(command);
}

//exit, history, !!, !n (!1 executes the most recent, !2 executes the 2 most recent commands

bool handleBuiltInCommands(string commandLine)
{
    string cleanCommandLine = trim(commandLine);

    if(cleanCommandLine == "!!")
    {
        //Show the most recent command is executed

        return true;
    } else if(cleanCommandLine == "exit")
    {
        //Show

        return true;
    } else if(cleanCommandLine == "exit")
    {
        CommandAndOptions previousCommand;

        if(cleanCommandLine == "!!")
        {
            if(commandHistory.empty())
            {
                cout << "History is emtpy." << endl;
                return;
            }
            //grab and store the previous command
            previousCommand = commandHistory.pop_front();
        } else //command is "!N"
        {
            char N = cleanCommandLine[1];
            int numOfCommands = (int) N - 48;

            if(commandHistory.size() < numOfCommands)
            {
                cout << "Command unavailable or does not exist." << endl;
                return;
            }

            for(int i = 0; i < commandHistory.size() && i < numOfCommands; i++)
            {
                commandHistory.pop_front();
            }

            previousCommand = commandHistory.front();


        }


        cout << previousCommand.command << endl;
        processCommand(previousCommand);
    }

    return true;
}



//ls-la

CommandAndOptions parseCommandAndOptions(string commandLine)
{
    CommandAndOptions commandAndHistory;

    std::size_t pos = commandLine.find("-");

    //That's a command with no options
    if(pos == -1)
    {
        commandAndHistory.command = &commandLine[0u];
        return commandAndHistory;
    }

    string command = commandLine.substr(0, pos);
    string options = commandLine.substr(pos, commandLine.size() - 1);

    commandAndHistory.command = &command[0u];
    commandAndHistory.options = &options[0u];

    return commandAndHistory;
}

void processPipedCommand(string commandLine)
{
    vector<string> pipedCommands;
    boost::split(pipedCommands, commandLine, boost::is_any_of("|"));
    string command;

    for(int index = 0; index < pipedCommands.size(); ++index)
    {
        command = trim(pipedCommands[index]);
        cout << "command: " << command << endl;
    }
}


void processCommand(CommandAndOptions commandWithOptions)
{
    pid_t pid = fork();

    if(pid == 0)
    {
        //execvp(commandWithOptions.command, commandWithOptions.options);
    }
}

void processCommandLine(string commandLine)
{
    if(!boost::contains(commandLine, "|"))
        processPipedCommand(commandLine);
    else if(!handleBuiltInCommands(commandLine))
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
//    vector<string> commandLines;
//    boost::split(commandLines, commandString, boost::is_any_of("\n"));
//
//    for(int index = 0; index < commandLines.size(); ++index)
//    {
//        processCommandLine(commandLines[index]);
//    }




    //split on |

    CommandAndOptions commandAndOptions = parseCommandAndOptions(testCommandLine);
    cout << "Command: " << commandAndOptions.command << ", Options: " << commandAndOptions.options << endl;
}