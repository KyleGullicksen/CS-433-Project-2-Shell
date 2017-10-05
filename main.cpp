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

}

void createHistory(char command)
{
    commandHistory.push_back(command);
}

//exit, history, !!, !n (!1 executes the most recent, !2 executes the 2 most recent commands

void handleBuiltInCommands(string commandLine)
{
    string cleanCommandLine = trim(commandLine);

    if(cleanCommandLine == "history")
    {
        //Show the most recent command is executed
        displayHistory();
    }
    else if(cleanCommandLine == "exit")
    {
        exit(1);
    }
    else if(cleanCommandLine[0] == "!")
    {
        if (cleanCommandLine == "!!")
        {
            if(commandHistory.empty())
            {
                cout << "History is emtpy." << endl;
                return;
            }
            //grab and store the previous command
            CommandAndHistory previousCommand = commandHistory.pop();
        }
        else //command is "!N"
        {
           char N = cleanCommandLine[1];
           int numOfCommands = (int)N - 48;

           if(commandHistory.size() < numOfCommands)
           {
            cout << "Command unavailable or does not exist." << endl;
            return;
           }

           for(int i = 0; i < commandHistory.size() && i < numOfCommands; i++)
           {
            commandHistory.pop();
           }
           CommandAndHistory previousCommand = commandHistory.front();
        }   
        cout << previousCommand << endl;
        processCommand(previousCommand);
    }

        return;
}


CommandAndOptions parseCommandAndOptions(string commandLine)
{

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


void processCommand(string commandLine)
{
    pid_t pid = fork();

    if(pid == 0)
    {
        execvp()
    }
}

void processCommandLine(string commandLine)
{
    if(!boost::contains(commandLine, "|"))
        processPipedCommand(commandLine);
    else
        processCommand(commandLine);


}


int main()
{
    /*
     * // ls -la | grep "Shit"
     */

    string commandString = R"(ls -la | grep "Shit")";




    //split on |

    /*
     * for each command in the
     */

    vector<string> commandLines;
    boost::split(commandLines, commandString, boost::is_any_of("\n"));

    for(int index = 0; index < commandLines.size(); ++index)
    {
        processCommandLine(commandLines[index]);
    }
}