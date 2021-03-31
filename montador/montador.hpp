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


void changeGlobalError();
void createDirectiveTable(unordered_map<string, int> &set);
void createInstructions(unordered_map<string, instructions> &set);
int main(int argc, char *argv[]);
void firstpass(string newdocument, const unordered_map<string, instructions> &set, const unordered_map<string, int> &directives, unordered_map<string, int> &symb, string firstfile );
void splitToVector(string line, vector<string> &vector);
void placeLabel(int *position, vector<string> vector, unordered_map<string, int> &symbTabel, const unordered_map<string, instructions> &set, string originalFile);
bool isOnTable(string label, unordered_map<string, int> &symbTabel);
void isTokenValid(string token,string filename, vector<string> tokenVector);
bool isLabel(vector<string> vector);
void isRestOfLabelValid(vector<string>tokenVector, string originalfile);
bool isOnInstructions(const unordered_map<string, instructions> &set, string aux);
void addPositiontoInstruction(const unordered_map<string, instructions> &set, string inst, int *position);
bool isOnDirectives(const unordered_map<string, int> &directives, string aux);
void organizingLabelsInstructionsDirectives(const unordered_map<string, instructions> &set, const unordered_map<string, int> &directives, unordered_map<string, int> &symb, vector<string>tokenVector, int *position, string firstfile);
void secondpass(string newdocument, const unordered_map<string, instructions> &set, const unordered_map<string, int> &directives, unordered_map<string, int> &symb,string firstfile );
vector<string> recoverCodesandPositions(vector<string>tokenVector, unordered_map<string, int> &symb, const unordered_map<string, instructions> &set);
bool isItNumber(string token, string firstfile);
void verifyOperands(vector<string> tokenVector, unordered_map<string, int> &symb, ofstream &newfile, const unordered_map<string, instructions> &set, string originalFile);
int findLine(vector<string> tokenVector, string originalFile);


#endif