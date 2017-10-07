//
// Created by stoffel on 10/4/17.
//

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
};


#endif //INC_2_COMMANDANDOPTIONS_H
