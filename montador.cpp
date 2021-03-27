
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

//preciso de uma tabela de diretivas
void createDirectiveTable(unordered_map<string, int> &set){
  set["SPACE"] = 1;
  set["CONST"] = 1;
}
//Preciso de instruções
void createInstructions(unordered_map<string, instructions> &set){
  set["ADD"].opcode = "01";
  set["ADD"].size = 2;
  
  set["SUB"].opcode = "02";
  set["SUB"].size = 2;
  
  set["MUL"].opcode = "03";
  set["MUL"].size = 2;

  set["DIV"].opcode = "04";
  set["DIV"].size = 2;

  set["JMP"].opcode = "05";
  set["JMP"].size = 2;

  set["JMPN"].opcode = "06";
  set["JMPN"].size = 2;

  set["JMPP"].opcode = "07";
  set["JMPP"].size = 2;

  set["JMPZ"].opcode = "08";
  set["JMPZ"].size = 2;

  set["COPY"].opcode = "09";
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

void splitToVector(string line, vector<string> &vector){
  //no firstpass eu so quero os labels e identificar as sections
  //meu vetor pode ser algo como ["LABEL:", "OPERACAO", "OPERANDO"] ou ["OPERAÇAO", "OPERANDO"]

  //Primeiro, pegar o que tem :
  size_t newlinePosition;

  newlinePosition= line.find_first_of(":");
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
};

bool isOnTable(string label, unordered_map<string, int> &symbTabel){
  if(symbTabel.empty()){
    // cout<<"vazio, nao da problema\n";
    return false;
  }else{
    if(symbTabel.find(label)!=symbTabel.end()){//achou
      // cout<<"deu problema\n";
      return true;
    }
  }
  return false;
}

bool isTokenValid(string token, int numLine){
    if(token.length()>50){
        cout<<"ERRO LEXICO LINHA:"<< numLine << "token invalido, maior que 50 caracteres\n";
      }

    if((isdigit(token.at(0)))){
        cout<<"ERRO LEXICO LINHA:"<< numLine << "token invalido, não pode começar com número\n";

    }
    size_t stringposition;
    stringposition= token.find_first_not_of(" ABCDEFGHIJKLMNOPQRSTUVWYXZ0123456789_");

   if(stringposition!=string::npos){
      cout<<"ERRO LEXICO LINHA:"<< numLine << "token contém caracteres inválidos\n";

    //  getchar();
   }

    
      
}

bool isLabel(vector<string> vector){
  string first= vector[0];

    // cout<< first <<"\n";
    size_t stringposition;

  stringposition = first.find_first_of(":");
  if(stringposition!=string::npos){
    return true;
  }
  return false;
}

bool isOnInstructions(const unordered_map<string, instructions> &set, string aux){
  if(set.find(aux)!= set.end()){//se achou na tabela de instruções
    return true;
  }

  return false;


}

void placeLabel(int numberline, int *position, vector<string> vector, unordered_map<string, int> &symbTabel, const unordered_map<string, instructions> &set){
  string first= vector[0];//First é o label

    size_t stringposition;
    int aux= *position;

    // cout<< first <<" posiÇao "<< aux <<"\n";
    if(isOnTable(first, symbTabel)){
      cout<<"ERRO SEMÂNTICO LINHA "<< numberline <<" SIMBOLO REDEFINIDO, PRIMEIRA POSIÇÃO SERÁ MANTIDA\n";
    }else if(isOnInstructions(set,first)){
            cout<<"ERRO SINTATICO LINHA "<< numberline <<" USO DE PALAVRA RESERVADA COMO LABEL\n";
    }else{//se não estiver na tabela, basta colocar
      isTokenValid(first, numberline);
      cout<<"\n colocando"<<first<<" na positoin: "<< aux <<"\n";
      symbTabel[first] = aux;//bota na chave "LABEL" a posição
    }; 
    if(isOnInstructions(set, vector[1])){//se for label p instruction, a instruction que conta a posição
          addPositiontoInstruction(set, vector[1], position);
    }else{

        *position= aux+1;

    }


}

void isRestOfLabelValid(vector<string>tokenVector,int numberLine){
  int sizeVector = tokenVector.size();
  //o maior label é LABEL: COPY N1,N2 o que equivale a um tamanho de 3 no vetor

  for(int i=0; i< sizeVector; i++){
      if(i>2){
      cout<<"ERRO SINTATICO LINHA:"<< numberLine << "número inválido de tokens para Label\n";
      break;
      }
      if(tokenVector[i-1]=="CONST"){//se o último for a diretiva CONST
      size_t stringposition;
      stringposition= tokenVector[i].find_first_not_of("1234567890");
        if(stringposition!=string::npos){
          cout<<"ERRO SINTATICO LINHA:"<< numberLine << "Tipo de operando inválido para CONST\n";
          
        }
      }else{
      isTokenValid(tokenVector[i], numberLine);
      }
      // cout<<tokenVector[i]<<"\n";
      // getchar();
  }
}


void addPositiontoInstruction(const unordered_map<string, instructions> &set, string inst, int *position){
  int lastPosition= *position;
  int add = set.at(inst).size;
  *position = lastPosition + add;


}

bool isOnDirectives(const unordered_map<string, int> &directives, string aux){
  cout<<"Procurando diretiva\n";
    if(directives.find(aux)!= directives.end()){//se achou na tabela de diretivas
    cout<<"achou a diretiva" << aux<<"\n";
    return true;
  }else{
    cout<<"aNAO a diretiva" << aux<<"\n";

    return false;
  }
}

void organizingLabelsInstructionsDirectives(const unordered_map<string, instructions> &set, const unordered_map<string, int> &directives,unordered_map<string, int> &symb, vector<string>tokenVector, int *position, int *numberLine){
                int lineNumber= *numberLine;
                cout<<tokenVector[0]<<"\n";
                getchar();
                // cout<<"fazendo o texto começo na linha:"<<lineNumber<<"\n";
              if(isLabel(tokenVector)){
                // cout<<"Linha: "<<lineNumber<<"\n";
                  size_t stringposition;
                  stringposition = tokenVector[0].find_first_of(":");
                if(stringposition!=string::npos){
                      tokenVector[0].erase(stringposition,stringposition+1);
                      }
                placeLabel(*numberLine, position, tokenVector, symb, set);

                isRestOfLabelValid(tokenVector, *numberLine);
                // cout<<"Position after label "<<*position<<"\n";
                *numberLine= lineNumber + 1;//aumenta sempre um em linha

              }else{//se não é label, é operação ou diretiva

                if(isOnInstructions(set, tokenVector[0])){
                  addPositiontoInstruction(set, tokenVector[0], position);
                  cout<<"Posiçao apos operação "<< *position <<"\n";
                }else {
                    if(isOnDirectives(directives, tokenVector[0])){//SE FOR UMA DIRETIVA, Nossas unicas diretivas sao SPACE e CONST e elas precisam de LABELS
                      cout<<"ERRO SINTÁTICO- LINHA: "<< *numberLine<< " LABEL AUSENTE PARA A DIRETIVA: "<< tokenVector[0]<< "\n";
                  }else{//Se nao for uma directive e nao for uma instrução, é uma instrução nao existente
                      cout<<"ERRO SINTÁTICO- LINHA: "<< *numberLine<< "  OPERAÇÃO OU DIRETIVA NÃO EXISTENTE: "<< tokenVector[0]<< "\n";

                  }
                  // getchar();

                }
                *numberLine= lineNumber + 1;//aumenta sempre um em linha
              }

}


void firstpass(string newdocument, const unordered_map<string, instructions> &set, const unordered_map<string, int> &directives, unordered_map<string, int> &symb){
  int numberLine=1;
  int position = 0;
  int dataLine;
  // int endFile;
  int beginDataLine;
  bool didText= false; //variavel para controlar se a seção TEXT ja foi lida
  bool foundText=false; //começa falso para pular as linhas de data
  ifstream mydocument; //arquivo modificado para leitura
  mydocument.open(newdocument);
//ROTULO: Nao pode começar com numero nem ter caractere especial
// ROTULO: OPERACAO OPERANDOS
  string line;
 

  if (!mydocument.is_open()){//se deu erro ao abrir o arquivo
    cout << "Não foi possível abrir o arquivo, erro ao limpar texto \n";
  }else{
    while (getline (mydocument, line)){
      size_t lineposition;
      vector<string> tokenVector;
      // cout<< line<<"\n";
      splitToVector(line, tokenVector);


        //tem sempre que achar um section antes de achar outras coisas
        if(tokenVector[0]=="SECTION"){
          if((tokenVector[1]=="DATA")&&(!didText)){//se nao tiver passado por texto primeiro
            dataLine= mydocument.tellg();//guarda a posição de data
            cout<<"dataline: "<< dataLine<<"\n";
            beginDataLine= numberLine; //Guarda a contagem de linha em data
            numberLine++;
          }
          else if((tokenVector[1]=="TEXT")&&(!didText)){//Quando acha texto da primeira vez
              foundText=true;
              cout<<line;
              numberLine++;
          }else if((tokenVector[1]=="TEXT")&&(didText)){
              return;
          }

        }else{//Quando le algo que nao é section
          if(didText){
              if(isLabel(tokenVector)){
                // cout<< tokenVector[0];
                  // cout<<"Linha: "<<numberLine<<"\n";
                  size_t stringposition;
                  stringposition = tokenVector[0].find_first_of(":");
                  if(stringposition!=string::npos){
                        tokenVector[0].erase(stringposition,stringposition+1);
                    }
                    placeLabel(numberLine, &position, tokenVector, symb, set);


                  isRestOfLabelValid(tokenVector, numberLine);
                // position++;//aumenta posição em um no label
                // cout<< tokenVector[0]<< "\n Position: "<<position<<"\n";
                // getchar();
                  // cout<<"Position after label "<<position<<"\n";
                  numberLine++;//aumenta sempre um em linha

              }else{//se não for label e estiver dentro de DADOS é porque falta a label
                      cout<<"ERRO SINTÁTICO- LINHA: "<< numberLine<< " LABEL AUSENTE OU INSTRUÇÃO FORA DE SEÇÃO\n";
                 numberLine++;//aumenta sempre um em linha
                  }
          }
          else if((!foundText)){//se é data e ainda nao achou o texto, ainda assim conta as linhas
            numberLine++;
          }
          else if(foundText && (!didText)){//se eu ja tiver chegado em texto uma vez mas nao tiver acabado o texto
                        
            if(mydocument.peek() == EOF){//se tiver chegado ao final do texto
            cout<<"Ultimo documento: "<<tokenVector[0]<<" posicao "<< position<<"\n";
              organizingLabelsInstructionsDirectives(set, directives, symb, tokenVector, &position, &numberLine);

                didText= true;
                numberLine = beginDataLine;
                mydocument.clear();
                mydocument.seekg(dataLine);//pula de volta para uma linha após data
                if((didText)&&(foundText)){
                  // cout<<"tudo true\n";
                }
                
            }//se não tiver chegado ao final do texto procura por label e erros lexicos
            else{
              organizingLabelsInstructionsDirectives(set, directives, symb, tokenVector, &position, &numberLine);
              
            }
          }
          // if((didText)&&(foundText)) {//Finalmente é hora de ler os dados
          // getchar();
          // cout<< tokenVector[0]<<"\n";
                
      }

    }
  }
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
  string tempdocument = originalFile;
  tempdocument = tempdocument.erase(dotPosition+1, string::npos) + "txt";


//função que cria o arquivo temporário limpo
  limpatexto(originalFile, tempdocument);
  //Simulando minha tabela de instrução
  unordered_map<string, instructions> instructionsSet;
  unordered_map<string, int> directives;
  unordered_map<string, int> symbolTable;//tabela de simbolos vazia


  //firstpass
  createDirectiveTable(directives);
  createInstructions(instructionsSet);
  firstpass(tempdocument, instructionsSet, directives, symbolTable);
  //tokenizar e erros lexicos
  // for(auto inst : instructionsSet){
  //   cout<< inst.first << "  " << inst.second.opcode<<"\n";
//   // }
cout<<"Acabou o first, vamos ver a symb:\n";
for(auto inst : symbolTable){
    cout<< inst.first << "  " << inst.second<<"\n";
  }



}