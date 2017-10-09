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

Shell.cpp; contains the implementations of all of our functions
*/



#include "Shell.h"

//strContains return true if str contains string 
//Parameters: string &str, string string 1
//Input: str, and string 1
//Output: bool: true / false
bool Shell::strContains(string &str, string string1)
{
    unsigned long pos = str.find(string1);

    return
            pos != -1;
}

//trim removes white space before and after str
//Parameters: string &str
//Input: inputed string
//Output: str[index]
string Shell::trim(string &str)
{
    int startingIndex = 0, endingIndex = str.length() - 1;

    for(int index = 0; index < str.length(); ++index) {
        if(!isspace(str[index])) {
            startingIndex = index;
            break;
        }
    }

    for(int index = str.length() - 1; index > -1; --index) {
        if(!isspace(str[index])) {
            endingIndex = index;
            break;
        }
    }

    return str.substr(startingIndex, (endingIndex - startingIndex) + 1);
}

//convert a C++ string into a Cstyle string
//Parameters: string &str
//Input: C++ string
//Output: Cstyle string
char *Shell::convert(string &str)
{
    string cleanString = trim(str);

    char *cStyleString = new char[cleanString.length()];
    std::strcpy(cStyleString, cleanString.c_str());

    return cStyleString;
}

//displayHistory all of the user inputed commands 
//Parameters: none
//Input: User command
//Output: All of the user input commands
void Shell::displayHistory()
{
    if(commandHistory.empty())
        cout << "No commands have been executed!" << endl;
    else
        for(int i = commandHistory.size() - 1; i > -1; i--)
            cout << i << " " << commandHistory[i].orginalCommandLine << endl;
}

//handleBuiltInCommands will handle the history, !N, and !! commands when the user inputs those
//Parameters: string &commandLine
//Input: user command
//Output: execute command or display history
bool Shell::handleBuiltInCommands(string &commandLine)
{
    string cleanCommandLine = trim(commandLine);

    if(cleanCommandLine == "!!") {
        if(commandHistory.empty()) {
            cout << "No commands have been executed!" << endl;
            return true;
        }

        CommandAndOptions mostRecentCommand = commandHistory.front();
        cout << "Executing [" << mostRecentCommand.orginalCommandLine << "]" << endl;
        executeCommand(mostRecentCommand);

        return true;

    } else if(regex_match(cleanCommandLine, nHistoryItemsMatcher)) //command is "!N"
    {
        //These next two lines grab the number from !N
        //and converts it into the ascii number for N
        char N = cleanCommandLine[1];
        int numOfCommands = (int) N - 48;

        if(commandHistory.size() < numOfCommands) {
            cout << numOfCommands << " commands have not yet been executed!" << endl;
            return true;
        }

        CommandAndOptions desiredCommand = commandHistory.at(numOfCommands - 1);
        cout << "Executing [" << desiredCommand.orginalCommandLine << "]" << endl;
        executeCommand(desiredCommand);

        return true;

    } else if(strContains(cleanCommandLine, "history")) {
        displayHistory();
        return true;
    } else
        return false;
}

//parseCommandAndOptions
//Parses through the user input and grabs all of the options
//Parameters: string &commandLine
//Input: user command
//Output: commandWithOptions
CommandAndOptions Shell::parseCommandAndOptions(string &commandLine)
{
    CommandAndOptions commandWithOptions;
    commandWithOptions.orginalCommandLine = commandLine;

    char *commandLineCStyle = convert(commandLine);
    int i = 0;

    char *p = strtok(commandLineCStyle, " ");

    while(p != nullptr) {
        commandWithOptions.args[i] = p;
        p = strtok(nullptr, " ");
        i++;
    }

    //Is there am amp?
    std::size_t ampPos = commandLine.find('&');

    if(ampPos != -1) {
        commandWithOptions.amp = true;
        commandWithOptions.args[i - 1] = nullptr;
    } else {
        commandWithOptions.args[i] = nullptr;
        commandWithOptions.amp = false;
    }

    commandWithOptions.command = commandWithOptions.args[0];

    return commandWithOptions;
}

//executeCommand will fork and grab the pid. Based off the pid, it will determine if the command is valid
//Parameters: CommandAndOptions &commandWithOptions
//Input: no input
//Output: Appropritate output for user command
void Shell::executeCommand(CommandAndOptions &commandWithOptions)
{
    //Don't execute a new command until some of the others have finished executing
    for(int currSleepCount = 0; processCount >= maxProcessCount && currSleepCount < maxSleepCount; currSleepCount++)
        std::this_thread::sleep_for(std::chrono::seconds(2));

    //Add the command to the command history history
    commandHistory.push_front(commandWithOptions);

    pid_t pid = fork();

    if(pid == 0) {
        execvp(commandWithOptions.command, commandWithOptions.args);
        processCount++;
        cout << "This command does not exit or invalid command." << endl;
        exit(1);
    } else if(pid > 0) {
        if(!commandWithOptions.amp) {
            wait(nullptr);
            processCount--;
        }
    } else {
        cout << "Fork failed." << endl;
        exit(1);
    }
}

//removeNewLines will check to make sure that there are not any accidental new lines in the user input
//Parameters: string &commandLine
//Input: string of user command
//Output: string of user command - new lines
string Shell::removeNewlines(string &commandLine)
{
    string str = "";

    for(char c : commandLine) {
        if(c == '\n' || c == '\r')
            continue;

        str.push_back(c);
    }

    return str;
}

//processCommandLine: given a line from the shell, it parses the line and executes the 
//Parameters: string &commandLine
//Input: user command
//Output: executeCommand
void Shell::processCommandLine(string &commandLine)
{
    string cleanString = removeNewlines(commandLine);

    if(!handleBuiltInCommands(commandLine)) {
        CommandAndOptions commandAndOptions = parseCommandAndOptions(commandLine);
        executeCommand(commandAndOptions);
    }
}

Shell::Shell() : nHistoryItemsMatcher("!(\\+)?[[:digit:]]+") {}

//startNewShellSesion is our gui for this project
//Parameters: none
//Input: User command
//Output: processCommandLine
void Shell::startNewShellSession()
{
    string userInput = "";
    bool shouldAskForMoreInput = true;

    while(shouldAskForMoreInput) {
        cout << "osh> ";
        getline(cin, userInput);

        //Did the user exit the current terminal session?
        if(strContains(userInput, "exit")) {
            shouldAskForMoreInput = false;
            exit(0xBAD);
        } else
            processCommandLine(userInput);
    }
}

Shell::~Shell()
{
}
