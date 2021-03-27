#ifndef _LIMPATEXTO_H_
#define _LIMPATEXTO_H_


#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cctype>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>
#include <sstream>      // std::istringstream
#include <string> 

using namespace std;

void removeSpaceBeginEnd(string &line);
void removeExtraSpace(string &line);

void  writeline(string &line, ofstream &newfile);
void removeComments(string &line);
void limpatexto(string filename, string newfilename);


#endif