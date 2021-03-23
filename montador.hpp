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
  int opcode;
  int size;
} instructions;

void createInstructions();
int main(int argc, char *argv[]);
void firstpass(string newdocument, unordered_set<instructions> &set);


#endif