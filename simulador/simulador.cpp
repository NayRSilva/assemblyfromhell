
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>
#include <bits/stdc++.h>
#include "simulador.hpp"
#include <iostream>
#include <vector>
using namespace std;

void splitToVector(string line, vector<string> &vector){
  //no firstpass eu so quero os labels e identificar as sections
  //meu vetor pode ser algo como ["LABEL:", "OPERACAO", "OPERANDO"] ou ["OPERAÇAO", "OPERANDO"]

  istringstream auxString(line);

  string token;

  while(auxString >> token){
    vector.push_back(token);
  }
}


bool isOnInstructions(const unordered_map<string, instructions> &set, string aux){
  if(set.find(aux)!= set.end()){//se achou na tabela de instruções
    return true;
  }
  return false;
}
int whereIsData(const vector<string> OpVector, unordered_map<string, instructions> &instructionlist){
    int positionstring=0;
    int indiceVetor=0;
    bool foundData=false;
    int size_vector= OpVector.size();
    int aux;
    string stopOp= instructionlist.at("STOP").opcode;
    string copOp= instructionlist.at("COPY").opcode;

    while((!foundData)|| indiceVetor<size_vector){
      if(OpVector[indiceVetor]==stopOp){//se for a ultima expressão
          aux = instructionlist.at("STOP").size;
          foundData=true;
          indiceVetor= indiceVetor + aux;
          return (positionstring+aux);
          
        }else if(OpVector[indiceVetor]==copOp){
          //se for Copy
          aux= instructionlist.at("COPY").size;
          positionstring = positionstring+ aux;
          indiceVetor= indiceVetor+aux;

        }else{//se nao for nenhum dos dois deve ser outra operação e aí a gente soma 2
          positionstring= positionstring+2;
          indiceVetor= indiceVetor+2;

        }
    }

    if(foundData){

      return positionstring;
    }else{//se nao tiver achado stop antes de while terminar algo ta errado
      return(-1);
    }
   
}

void getData(int position, unordered_map<string, int> &dataTable, vector<string>enterVector){
  int sizeVector= enterVector.size();

  for(int indice=position; indice<sizeVector; indice++){
    //memaddress é o endereço que é a posição no caso o indice
    //data é o dado então
    int aux = stoi(enterVector[indice]);
    string straux = to_string(indice);
    dataTable[straux] = aux;

  }
}

void printInst(int acc, int pc, string instrucao){
  cout<<"depois da instruçao: "<<instrucao<<"\n"<<"PC<- "<<pc<<"\n ACC<- "<<acc<<"\n";
}

void DoIt(vector<string> &vectorMy, unordered_map<string, int> dataTable, int position, ofstream &finalFile){
  int acc =0;
  int  pc = 0;
  string operar;
  int recoveraddress;
  string toBeCopied;
  string toReceive;
  int datacopied;
  int i=0;
  while(i<position){//percorre o vertor
    // cout<<i;
    // getchar();
    int aux = stoi(vectorMy[i]);
    switch(aux){
      case 1://ADD
        operar = vectorMy[i+1];
        recoveraddress= dataTable[operar];
        acc += recoveraddress;
        pc+=2;
        i=i+2;
        printInst(acc, pc, "ADD");

        break;

      case 2://SUB
        operar = vectorMy[i+1];
        recoveraddress= dataTable[operar];
        acc -= recoveraddress;
        pc+=2;
        i=i+2;
        printInst(acc, pc, "SUB");
        break;

      case 3://MUL
        operar = vectorMy[i+1];
        recoveraddress= dataTable[operar];
        acc = acc*recoveraddress;
        pc+=2;
        i=i+2;
        printInst(acc, pc, "MULT");
        break;

      case 4://div
        operar = vectorMy[i+1];
        recoveraddress= dataTable[operar];
        acc = acc/recoveraddress;
        pc+=2;
        i=i+2;
        printInst(acc, pc, "DIV");
        break;
      case 5://JMP  
        pc=stoi(vectorMy[i+1]);//transforma o que ta na fente em int p poder pular
        i=pc;
        printInst(acc, pc, "JMP");
        break;

      case 6://JMPN
        if(acc<0){
          pc=stoi(vectorMy[i+1]);//transforma o que ta na fente em int p poder pular
          i=pc;
          printInst(acc, pc, "JMPN");

        }else{
          pc = pc+2;
          printInst(acc, pc, "JMPN");
          i=i+2;
        }
        break;




      case 7://JMPP
        if(acc>0){
          pc=stoi(vectorMy[i+1]);//transforma o que ta na fente em int p poder pular
          i=pc;
          printInst(acc, pc, "JMPP");

        }else{
          pc = pc+2;
          printInst(acc, pc, "JMPP");
          i=i+2;
        }
        break;

      case 8://JMPZ
        if(acc==0){
          pc=stoi(vectorMy[i+1]);//transforma o que ta na fente em int p poder pular
          i=pc;
          printInst(acc, pc, "JMPP");

        }else{
          pc = pc+2;
          printInst(acc, pc, "JMPP");
          i=i+2;
        }
        break;

      case 9://COPY
        toBeCopied= vectorMy[i+1];
        toReceive = vectorMy[i+2];
        datacopied = dataTable.at(toBeCopied);
        dataTable[toReceive]= datacopied;
        pc+=3;
        i=i+3;
          printInst(acc, pc, "COPY");


        break;

      case 10://LOAD

        operar= vectorMy[i+1];
        acc= dataTable.at(operar);
        pc+=2;
        i+=2;
          printInst(acc, pc, "LOAD");
          break;


      case 11://STORE
        operar= vectorMy[i+1];
        dataTable[operar]= acc;
        pc+=2;
        i+=2;
        printInst(acc, pc, "STORE");
        break;



      case 12://INPUT
        cout<<"insira um numero inteiro:\n";
        cin>>recoveraddress;
        operar= vectorMy[i+1];
        dataTable[operar]= recoveraddress;
        pc+=2;
        i+=2;
        printInst(acc, pc, "INPUT");
        break;

        

      case 13://OUTPUT
        operar= vectorMy[i+1];
        finalFile << to_string(dataTable[operar])<<"\n";
        pc+=2;
        i+=2;
        printInst(acc, pc, "OUTPUT");
        break;

      case 14://STOP
        pc+=1;
        i+=1;
        printInst(acc, pc, "STOP");

        break;





    }

  }

}

void executeCode(string originalFile, string finalDocument, vector<string> &vectorMy){
    string myLine;
    ifstream mydocument; //arquivo modificado para leitura
    mydocument.open(originalFile);
    unordered_map<string, int> dataTable;

  ofstream newfile; //arquivo novo, escrita
  newfile.open(finalDocument);

  unordered_map<string, instructions> instructionsSet;
  createInstructions(instructionsSet);


      if (mydocument.is_open()){
        getline(mydocument, myLine);
        mydocument.close();
    }

    splitToVector(myLine, vectorMy);

    int datapos= whereIsData(vectorMy, instructionsSet);

    if(datapos==(-1)){
      cout<<"Não teve STOP no programa?? Então nao roda\n";
      exit(1);
    }

    getData(datapos, dataTable, vectorMy);


    DoIt(vectorMy, dataTable, datapos, newfile);





}

void createInstructions(unordered_map<string, instructions> &set){
  set["ADD"].opcode = "1";
  set["ADD"].size = 2;
  
  set["SUB"].opcode = "2";
  set["SUB"].size = 2;
  
  set["MUL"].opcode = "3";
  set["MUL"].size = 2;

  set["DIV"].opcode = "4";
  set["DIV"].size = 2;

  set["JMP"].opcode = "5";
  set["JMP"].size = 2;

  set["JMPN"].opcode = "6";
  set["JMPN"].size = 2;

  set["JMPP"].opcode = "7";
  set["JMPP"].size = 2;

  set["JMPZ"].opcode = "8";
  set["JMPZ"].size = 2;

  set["COPY"].opcode = "9";
  set["COPY"].size = 3;

    set["LOAD"].opcode = "10";
  set["LOAD"].size = 2;

  set["STORE"].opcode = "11";
  set["STORE"].size = 2;

    set["INPUT"].opcode = "12";
  set["INPUT"].size = 2;

    set["OUTPUT"].opcode = "13";
  set["OUTPUT"].size = 2;

    set["STOP"].opcode = "14";
  set["STOP"].size = 1;

}


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
  vector<string> instructions;
    
    string tempdocument = originalFile;
    tempdocument = tempdocument.erase(dotPosition+1, string::npos) + "out";

    executeCode(originalFile, tempdocument, instructions);
}