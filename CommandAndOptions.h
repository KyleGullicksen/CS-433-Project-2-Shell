//
// Created by stoffel on 10/4/17.
//

#ifndef INC_2_COMMANDANDOPTIONS_H
#define INC_2_COMMANDANDOPTIONS_H

#include <string>
#include <vector>

using std::string;
using std::vector;


class CommandAndOptions
{
public:
    string command;
    vector<string> options;

    char * cachedCommand = nullptr;

    bool amp = false;
};


#endif //INC_2_COMMANDANDOPTIONS_H
