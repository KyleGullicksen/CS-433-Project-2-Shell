/*
Created by Kyle Gullicksen and Benjamin Siegel
Date Written: 9/25/2017
Course: CS433
Assignment #2

CommandAndOptions.h
*/

#ifndef INC_2_COMMANDANDOPTIONS_H
#define INC_2_COMMANDANDOPTIONS_H

#include <string>

using std::string;

class CommandAndOptions
{
public:
    char * command = nullptr;
    char * args[500];
    bool amp = false;
    string orginalCommandLine;

    virtual ~CommandAndOptions();

    CommandAndOptions();
};


#endif //INC_2_COMMANDANDOPTIONS_H
