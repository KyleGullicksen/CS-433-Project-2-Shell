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

using std::string;
using std::vector;
using boost::split;
using std::cout;
using std::endl;

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

    vector<string> results;
    boost::split(results, commandString, boost::is_any_of("|"));
    string command;
    int resultCode;

    for(int index = 0; index < results.size(); ++index)
    {
        command = trim(results[index]);
        resultCode = system(command);

        cout << "ResultCode: " << resultCode << endl;
    }
}