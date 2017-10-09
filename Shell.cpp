//
// Created by stoffel on 10/8/17.
//

#include "Shell.h"

bool Shell::strContains(string &str, string string1)
{
    unsigned long pos = str.find(string1);

    return
            pos != -1;
}

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

char *Shell::convert(string &str)
{
    string cleanString = trim(str);

    char *cStyleString = new char[cleanString.length()];
    std::strcpy(cStyleString, cleanString.c_str());

    return cStyleString;
}

void Shell::displayHistory()
{
    if(commandHistory.empty())
        cout << "No commands have been executed!" << endl;
    else
        for(int i = commandHistory.size() - 1; i > -1; i--)
            cout << i << " " << commandHistory[i].orginalCommandLine << endl;
}

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

void Shell::processCommandLine(string &commandLine)
{
    string cleanString = removeNewlines(commandLine);

    if(!handleBuiltInCommands(commandLine)) {
        CommandAndOptions commandAndOptions = parseCommandAndOptions(commandLine);
        executeCommand(commandAndOptions);
    }
}

Shell::Shell() : nHistoryItemsMatcher("!(\\+)?[[:digit:]]+") {}

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
