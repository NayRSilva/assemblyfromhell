
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
#include "limpatexto.hpp"
#include "montador.hpp"

using namespace std;

//Preciso de um grupo de diretivas

void firstpass(string newdocument, unordered_set<instructions> &set){
  
  //linhas com SECTION não contam na passagem de linha
  //identifica erros lexicos
  //monta a tabela de símbolos

};


int main(int argc, char *argv[]) {

    //tem que verificar se tem um arquivo na linha de comando
    if(argc < 2){
    cout << "Insira um arquivo." << endl;
    exit(1);
  }
//pega o nome do arquivo original
  string originalFile = argv[1];
  //cria nome de um arquivo temporario que vai sair sem comentarios ou espaços extras
  size_t dotPosition= originalFile.find_last_of(".");
  if(dotPosition==string::npos){
    cout<<"É necessário a extensão do arquivo";
    exit(1);
  }
  string tempdocument = originalFile;
  tempdocument = tempdocument.erase(dotPosition+1, string::npos) + "txt";
  // cout<<originalFile<<"\n";
  // cout<<tempdocument<<"\n";

//função que cria o arquivo temporário limpo
  limpatexto(originalFile, tempdocument);
  unordered_set<instructions> instructionsSet;
  //firstpass
  firstpass(tempdocument, instructionsSet);
  //tokenizar e erros lexicos




}