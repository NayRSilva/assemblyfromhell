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


typedef struct instructions {
  string opcode;
  int size;
} instructions;

void executeCode(string originalFile, string finalDocument);
void splitToVector(string line, vector<string> &vector); 
void createInstructions(unordered_map<string, instructions> &set);
bool isOnInstructions(const unordered_map<string, instructions> &set, string aux);
int whereIsData(const vector<string> OpVector, unordered_map<string, instructions> &instructionlist);
void printInst(int acc, int pc, string instrucao);
void getData(int position, unordered_map<string, int> &dataTable, vector<string>enterVector);
void DoIt(vector<string> &vectorMy, unordered_map<string, int> dataTable, int position, ofstream &finalFile);
#endif