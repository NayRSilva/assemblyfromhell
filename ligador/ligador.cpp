

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
// #include "ligador.hpp"

using namespace std; 
bool thereisError= false;

typedef struct usecases{
  string appearances;
} usecases;

void splitToVector(string line, vector<string> &vector){
  //no firstpass eu so quero os labels e identificar as sections
  //meu vetor pode ser algo como ["LABEL:", "OPERACAO", "OPERANDO"] ou ["OPERAÇAO", "OPERANDO"]

  //Primeiro, pegar o que tem `:`
  size_t newlinePosition;

  newlinePosition= line.find_first_of(":,");
  string aux;
  if(newlinePosition <= line.length()){
    aux = line.substr(0, (newlinePosition+1));
    vector.push_back(aux);
    line.erase(0, newlinePosition+1);
  } //a Partir daqui as coisas so é ser separadas por espaço

  istringstream auxString(line);

  string token;

  while(auxString >> token){
    vector.push_back(token);
  }
}


void getSizeFile(string myfile, vector<int> &tamanhos){

    ifstream document; //arquivo modificado para leitura
    document.open(myfile);
    string line; 
    int i=1;
     if(!document.is_open()){//se deu erro ao abrir o arquivo
    cout << "Não foi possível abrir o arquivo\n";
  }
  else{
    while (getline (document, line)){
        if(i==2){//a linha do tamanho é smepre 2
            size_t lineposition = line.find_first_of(":");
                if(lineposition <= line.length()){//apaga o H:
                    line.erase(0, lineposition+1);
                }
            int auxint = atoi(line.c_str());//transforma pra int
            tamanhos.push_back(auxint);//passa p array
        }

        i++;

    }
  }
  document.close();
}
void createDefTable(string mydocument, unordered_map<string, int> &defGeralTable, int fatcor){

    ifstream document; //arquivo modificado para leitura
    document.open(mydocument);
    string line; 
     if(!document.is_open()){//se deu erro ao abrir o arquivo
        cout << "Não foi possível abrir o arquivo\n";
    }
  else{
    while (getline (document, line)){
      vector<string> tokenVector;
      splitToVector(line, tokenVector);

      if(tokenVector[0]=="D,"){//se for da tabela de definições
        for(auto t: tokenVector){
            size_t newlinePosition= t.find_first_of(":,");
            string aux;
            if(newlinePosition <= line.length()){
                t = t.erase(newlinePosition, (newlinePosition+1));
              }

        }
        int auxint = atoi(tokenVector[2].c_str());
        int posCor= auxint+fatcor;

        defGeralTable[tokenVector[1]]= posCor;
        

      }

    }
  }

}

bool isOnDef(const unordered_map<string, int> &def, string aux){
  if(def.find(aux)!= def.end()){//se achou na tabela de instruções
    return true;
  }
  return false;
}


void modificaSaida(string mydocument, vector<string>&text,vector<string>&data,unordered_map<string, int> defGeralTable){

    int contador =0;
    ifstream document; //arquivo modificado para leitura
    document.open(mydocument);
    string line; 
    int textsectionSize;
    // int i=1;
    vector<string> codesection;
    vector<string> codesectionT;

    vector<string>bitmap;
    unordered_map<string, usecases> usetable;
     if(!document.is_open()){//se deu erro ao abrir o arquivo
    cout << "Não foi possível abrir o arquivo\n";
  }
  else{
    while (getline (document, line)){//cria a tabela de uso e pega pra mim o codigo
      contador++;
      vector<string> tokenVector;

      splitToVector(line, tokenVector);

        if(contador==3){
            size_t newlinePosition= line.find_first_of(":,");
              if(newlinePosition <= line.length()){
                line = line.erase(0, (newlinePosition+1));
              }

              istringstream st(line);
                string word;
                while(st>>word){//separa a string em palavras
                  bitmap.push_back(word);
                }
                textsectionSize= bitmap.size()-1;
                
        }

        if(tokenVector[0]=="U,"){//definindo a tabela de uso
            for(auto t: tokenVector){//elimina , e :
                size_t newlinePosition= t.find_first_of(":,");
                string aux;
                if(newlinePosition <= line.length()){
                    t = t.erase(newlinePosition, (newlinePosition+1));
                  }
                }
                  if(usetable.find(tokenVector[1])!= usetable.end()){//se ja estiver definido na    tabela de uso
                  string auxstring = usetable.at(tokenVector[1]).appearances;
                  usetable[tokenVector[1]].appearances= auxstring + " "+tokenVector[2];
                  }
                  else{//senao, defina

                    usetable[tokenVector[1]].appearances= tokenVector[2];
                  }
        }

      if(tokenVector[0]=="T:"){//se for codigo coloca no vetor de codigo
      int tokenVectorSize = tokenVector.size();
        for(int j=1; j<tokenVectorSize; j++){
          codesectionT.push_back(tokenVector[j]);
        }

      }

    }
    document.close();
  }
  //hora da substituição

  if(!usetable.empty()){//se houver tabela de uso
    for(auto use: usetable){//quebrando a string em vetores de posiçao
        string name = use.first;
        vector<string> stringpositions;
        vector<int> subposition;
        //passando appearances p vetor de string
        istringstream ss(use.second.appearances);
                string word;
                while(ss>>word){//separa a string em palavras
                  stringpositions.push_back(word);
                }

        //passando vetor de string para vetor de int
        int stringpsize= stringpositions.size();
      for(int i=0; i<stringpsize; i++){
          subposition.push_back(atoi(stringpositions[i].c_str()));
      }

        //verificar se esta definido na tabela de definição

        if(isOnDef(defGeralTable, use.first)){
          // cout<<"para "<<use.first<<" ";
            int correcao = defGeralTable.at(name);
            int subpsize = subposition.size();
          for(int i=0; i<subpsize; i++){//para cada posição da palavra
          // cout<<"posicao: "<<subposition[i]<<"\n";
          // cout<<"modificando: "<< codesectionT[subposition[i]]<<" para"<< correcao<<"\n";
            codesectionT[subposition[i]]= to_string(correcao);
            
            
          }
        }else{//se nao estiver na tabela de definição estamos usando algo que nao esta definido.
            thereisError=true;
        }

    }

  }
  //caso tiver que usar TEXT DATA TEXT DATA
  // for(auto vetor: codesectionT){
  //   text.push_back(vetor);
  // }
  for(int j=0; j< textsectionSize; j++){//adiciona o que é de texto na seção de texto
    text.push_back(codesectionT[j]);
  }
  int codesectionTsize = codesectionT.size();
  for(int j=textsectionSize; j<codesectionTsize; j++){
    data.push_back(codesectionT[j]);
  }
}

void criaArquivo(string myfile, vector<string> textsec, vector<string>datasec){
  ofstream newfile; //arquivo novo, escrita
  newfile.open(myfile);
  for(auto word:textsec){
    newfile<< word<<" ";
  }
  for(auto word:datasec){
    newfile<< word<<" ";
  }

  newfile.close();
}

int main(int argc, char *argv[]) {
  //tem que verificar se tem um arquivo na linha de comando
  if(argc < 2){
    cout << "Insira um arquivo." << endl;
    exit(1);
  }
  unordered_map<string, int> defGeralTable;//tabela de simbolos 
  vector<int>tamanhosArquivos;
  vector<string>filesNames;
  vector<string>textsec;
  vector<string>datasec;
  int fileNameSize;

  //pega o nome do arquivo original


  string originalFile;
  for(int i =1; i<(argc); i++){
      string nameaux= argv[i];
      filesNames.push_back(nameaux);
  }
  fileNameSize= filesNames.size();
  for(int i=0; i<fileNameSize;i++){//criaçao de defgeral e pegar tamanho do arquivo
      getSizeFile(filesNames[i], tamanhosArquivos);
      //adicionando o fator de correção
      if(i==0){//as definições do primeiro arquivo nao possuem fator de correção
        createDefTable(filesNames[i], defGeralTable, 0);

      }else{
        createDefTable(filesNames[i], defGeralTable, tamanhosArquivos[i-1]);

      }

  }

  for(int i=0; i<fileNameSize;i++){
      modificaSaida(filesNames[i], textsec, datasec, defGeralTable );

  }



  criaArquivo("arquivo.obj", textsec, datasec);  




}