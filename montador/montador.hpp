#ifndef _MONTADOR_H_
#define _MONTADOR_H_


#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cctype>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>
#include <bits/stdc++.h>


using namespace std;

typedef struct instructions {
  string opcode;
  int size;
} instructions;

typedef struct usecases{
  string appearances;
} usecases;


void changeGlobalError();
void createDirectiveTable(unordered_map<string, int> &set);
void createInstructions(unordered_map<string, instructions> &set);
int main(int argc, char *argv[]);
void firstpass(string newdocument, const unordered_map<string, instructions> &set, const unordered_map<string, int> &directives, unordered_map<string, int> &symb, string firstfile, unordered_map<string, int> &deftable, unordered_map<string, usecases> &usetable );
void splitToVector(string line, vector<string> &vector);
void placeLabel(int *position, vector<string> vector, unordered_map<string, int> &symbTabel, const unordered_map<string, instructions> &set, string originalFile, unordered_map<string, int> &defTable, unordered_map<string, usecases> &useTable);
bool isOnTable(string label, unordered_map<string, int> &symbTabel);
void isTokenValid(string token,string filename, vector<string> tokenVector);
bool isLabel(vector<string> vector);
void isRestOfLabelValid(vector<string>tokenVector, string originalfile);
bool isOnInstructions(const unordered_map<string, instructions> &set, string aux);
void addPositiontoInstruction(const unordered_map<string, instructions> &set, string inst, int *position);
bool isOnDirectives(const unordered_map<string, int> &directives, string aux);
void organizingLabelsInstructionsDirectives(const unordered_map<string, instructions> &set, const unordered_map<string, int> &directives, unordered_map<string, int> &symb, vector<string>tokenVector, int *position, string firstfile, unordered_map<string, int> &deftable, unordered_map<string, usecases> &usetable);
bool isonDefinitions(string label, unordered_map<string, int> &defTable);
void secondpass(string newdocument, const unordered_map<string, instructions> &set, const unordered_map<string, int> &directives, unordered_map<string, int> &symb,string firstfile, vector<int> bitmap, unordered_map<string, usecases> useTable, unordered_map<string, int> defTable);
bool isOnUse(unordered_map<string, int> usage, string aux);
void verifyusage(string newdocument, const unordered_map<string, instructions> &set, unordered_map<string, int> &usage);


vector<string> recoverCodesandPositions(vector<string>tokenVector, unordered_map<string, int> &symb, const unordered_map<string, instructions> &set);
bool isItNumber(string token, string firstfile);
void verifyOperands(vector<string> tokenVector, unordered_map<string, int> &symb, ofstream &newfile, const unordered_map<string, instructions> &set, string originalFile);
int findLine(vector<string> tokenVector, string originalFile);
void findBeginEnd(string newdocument );
void findSize(string newdocument );
bool isDefinedTable(string label, unordered_map<string, int> &defTable);



#endif