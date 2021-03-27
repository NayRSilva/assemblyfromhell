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



void createDirectiveTable(unordered_map<string, int> &set);
void createInstructions(unordered_map<string, instructions> &set);
int main(int argc, char *argv[]);
void firstpass(string newdocument, const unordered_map<string, instructions> &set, const unordered_map<string, int> &directives, unordered_map<string, int> &symb );
void splitToVector(string line, vector<string> &vector);
void placeLabel(int numberline, int *position, vector<string> vector, unordered_map<string, int> &symbTabel, const unordered_map<string, instructions> &set);
bool isOnTable(string label, unordered_map<string, int> &symbTabel);
bool isTokenValid(string token);
bool isLabel(vector<string> vector);
void isRestOfLabelValid(vector<string>tokenVector,int numberLine);
bool isOnInstructions(const unordered_map<string, instructions> &set, string aux);
void addPositiontoInstruction(const unordered_map<string, instructions> &set, string inst, int *position);
bool isOnDirectives(const unordered_map<string, int> &directives, string aux);
void organizingLabelsInstructionsDirectives(const unordered_map<string, instructions> &set, const unordered_map<string, int> &directives, unordered_map<string, int> &symb, vector<string>tokenVector, int *position, int *numberLine);

#endif