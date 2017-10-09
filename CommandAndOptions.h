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
