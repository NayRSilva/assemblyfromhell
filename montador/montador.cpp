
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
bool isThereError= false;
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
int findLine(vector<string> tokenVector, string originalFile){
  int linenum=1;
  ifstream mydocument; //arquivo original, leitura
  mydocument.open(originalFile);
  string line;
  while (getline (mydocument, line)){
    transform(line.begin(), line.end(), line.begin(), ::toupper);

    removeSpaceBeginEnd(line);
    removeComments(line);
    removeExtraSpace(line);
    // cout<<line<<"\n";
    // getchar();
    vector<string> auxVector;
    //   // cout<< line<<"\n";
      splitToVector(line, auxVector);

      if(auxVector==tokenVector){
        return linenum;
      }
      linenum++;
  }
  return 0;
}
void changeGlobalError(){
  if(!isThereError){
    isThereError=true;
  }
}
void splitToVector(string line, vector<string> &vector){
  //no firstpass eu so quero os labels e identificar as sections
  //meu vetor pode ser algo como ["LABEL:", "OPERACAO", "OPERANDO"] ou ["OPERAÇAO", "OPERANDO"]

  //Primeiro, pegar o que tem `:`
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
}

bool isOnTable(string label, unordered_map<string, int> &symbTabel){
  if(symbTabel.empty()){
    return false;
  }else{
    if(symbTabel.find(label)!=symbTabel.end()){//achou
      return true;
    }
  }
  return false;
}

void isTokenValid(string token, string filename, vector<string> tokenVector){
    if(token.length()>50){
      changeGlobalError();
        changeGlobalError();
  int errorLine= findLine(tokenVector, filename);
        cout<<"ERRO LEXICO LINHA:"<< errorLine << "token invalido, maior que 50 caracteres\n";
      }

    if((isdigit(token.at(0)))){
      changeGlobalError();
        int errorLine = findLine(tokenVector, filename);

        cout<<"ERRO LEXICO LINHA:"<< errorLine << "token invalido, não pode começar com número\n";

    }
    size_t stringposition;
    stringposition= token.find_first_not_of(" ABCDEFGHIJKLMNOPQRSTUVWYXZ0123456789_");

   if(stringposition!=string::npos){
      changeGlobalError();
        int errorLine = findLine(tokenVector, filename);

      cout<<"ERRO LEXICO LINHA:"<< errorLine << "token "<<token<<" contém caracteres inválidos\n";
   }   
}

bool isLabel(vector<string> vector){
  string first= vector[0];

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

void placeLabel(int *position, vector<string> vector, unordered_map<string, int> &symbTabel, const unordered_map<string, instructions> &set, string originalFile){
  string first= vector[0];//First é o label
    // size_t stringposition;
  int aux= *position;
  if(isOnTable(first, symbTabel)){
    changeGlobalError();
    int errorLine= findLine(vector, originalFile);
    cout<<"ERRO SEMÂNTICO LINHA "<< errorLine <<" SIMBOLO "<<first<<"  REDEFINIDO, PRIMEIRA POSIÇÃO SERÁ MANTIDA\n";
  }
  else if(isOnInstructions(set,first)){
    changeGlobalError();
    int errorLine= findLine(vector, originalFile);
    cout<<"ERRO SINTATICO LINHA "<< errorLine <<" USO DE PALAVRA RESERVADA COMO LABEL\n";
    }
  else{//se não estiver na tabela, basta colocar
    isTokenValid(first, originalFile, vector);
    symbTabel[first] = aux;//bota na chave "LABEL" a posição
  }; 
  if(isOnInstructions(set, vector[1])){//se for label p instruction, a instruction que conta a posição
    addPositiontoInstruction(set, vector[1], position);
  }
  else{
    *position= aux+1;
  }
}

void isRestOfLabelValid(vector<string>tokenVector, string originalfile){
  int sizeVector = tokenVector.size();
  //o maior label é LABEL: COPY N1,N2 o que equivale a um tamanho de 3 no vetor
  int errorLine;
  for(int i=1; i< sizeVector; i++){
    if(i>2){
      changeGlobalError();
      errorLine= findLine(tokenVector, originalfile);
      cout<<"ERRO SINTATICO LINHA:"<< errorLine << "número inválido de tokens para Label\n";
      break;
    }
    if(tokenVector[i-1]=="CONST"){//se o último for a diretiva CONST
      size_t stringposition;
      stringposition= tokenVector[i].find_first_not_of("-1234567890");
      if(stringposition!=string::npos){
        changeGlobalError();
        errorLine = findLine(tokenVector, originalfile);
        cout<<"ERRO SINTATICO LINHA:"<< errorLine << "Tipo de operando inválido para CONST\n";
      }
    }
    else{
      isTokenValid(tokenVector[i], originalfile, tokenVector);
    }
  }
}


void addPositiontoInstruction(const unordered_map<string, instructions> &set, string inst, int *position){
  int lastPosition= *position;
  int add = set.at(inst).size;
  *position = lastPosition + add;
}

bool isOnDirectives(const unordered_map<string, int> &directives, string aux){
    if(directives.find(aux)!= directives.end()){//se achou na tabela de diretivas
    return true;
  }else{
    return false;
  }
}

void organizingLabelsInstructionsDirectives(const unordered_map<string, instructions> &set, const unordered_map<string, int> &directives,unordered_map<string, int> &symb, vector<string>tokenVector, int *position, string firstfile){
  int errorLine;
  if(isLabel(tokenVector)){
    size_t stringposition;
    stringposition = tokenVector[0].find_first_of(":");
    if(stringposition!=string::npos){
      tokenVector[0].erase(stringposition,stringposition+1);
    }
    placeLabel(position, tokenVector, symb, set, firstfile);


    isRestOfLabelValid(tokenVector, firstfile);
  }
  else{//se não é label, é operação ou diretiva
    if(isOnInstructions(set, tokenVector[0])){
        addPositiontoInstruction(set, tokenVector[0], position);
      }
    else{
      if(isOnDirectives(directives, tokenVector[0])){//SE FOR UMA DIRETIVA, Nossas unicas diretivas sao SPACE e CONST e elas precisam de LABELS
        changeGlobalError();
        errorLine= findLine(tokenVector, firstfile);
        cout<<"ERRO SINTÁTICO- LINHA: "<< errorLine<< " LABEL AUSENTE PARADIRETIVA: "<< tokenVector[0]<< "\n";
      }
      else{//Se nao for uma directive e nao for uma instrução, é uma instrução nao existente
        changeGlobalError();
        errorLine= findLine(tokenVector, firstfile);

        cout<<"ERRO SINTÁTICO- LINHA: "<< errorLine<< "  OPERAÇÃO OU DIRETIVA NEXISTENTE: "<< tokenVector[0]<< "\n";
      }
    }
    // *numberLine= lineNumber + 1;//aumenta sempre um em linha
  }
}


void firstpass(string newdocument, const unordered_map<string, instructions> &set, const unordered_map<string, int> &directives, unordered_map<string, int> &symb, string firstfile){
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
//ROTULO: OPERACAO OPERANDOS
  string line; 

  if(!mydocument.is_open()){//se deu erro ao abrir o arquivo
    cout << "Não foi possível abrir o arquivo, erro ao limpar texto \n";
  }
  else{
    while (getline (mydocument, line)){
      // size_t lineposition;
      vector<string> tokenVector;
      // cout<< line<<"\n";
      splitToVector(line, tokenVector);

        //tem sempre que achar um section antes de achar outras coisas
      if(tokenVector[0]=="SECTION"){

        if((tokenVector[1]=="DATA")&&(!didText)){//se DATA ESTIVER NO INICIO DO TEXTO E nao tiver passado por texto primeiro

          dataLine= mydocument.tellg();//guarda a posição de data
          beginDataLine= numberLine; //Guarda a contagem de linha em data
          numberLine++;
        }
          else if((tokenVector[1]=="TEXT")&&(!didText)){//Quando acha texto da primeira vez
              foundText=true;
              numberLine++;
          }else if((tokenVector[1]=="TEXT")&&(didText)){//se voltou em texto e ele acabou uma vez
              mydocument.close();
              break;
              return;
          }
        }
        else{//Quando le algo que nao é section
          if(didText){//se o texto tiver acabado

            if(isLabel(tokenVector)){
              size_t stringposition;
              stringposition = tokenVector[0].find_first_of(":");
              if(stringposition!=string::npos){
                tokenVector[0].erase(stringposition,stringposition+1);
              }
              placeLabel(&position, tokenVector, symb, set, firstfile);
              // cout<<tokenVector[0];
                  // cout<<"\nfrom data\n";
                // getchar();

              isRestOfLabelValid(tokenVector, firstfile);
              
            }
            else{//se não for label e estiver dentro de DADOS é porque falta a label
              changeGlobalError();
              cout<<"ERRO SINTÁTICO- LINHA: "<< numberLine<< " LABEL AUSENTE OU INSTRUÇÃO FORA DE SEÇÃO\n";
            }
              numberLine++;//aumenta sempre um em linha
          }
          else if((!foundText)){//se é data e ainda nao achou o texto, ainda assim conta as linhas
            numberLine++;
          }
          else if(foundText && (!didText)){//se eu ja tiver chegado em texto uma vez mas nao tiver acabado o texto
              organizingLabelsInstructionsDirectives(set, directives, symb, tokenVector, &position, firstfile);
                        
            if(mydocument.peek() == EOF){//se tiver chegado ao final do texto
            // cout<<"Ultimo documento: "<<tokenVector[0]<<" posicao "<< position<<"\n";
              didText= true;
              numberLine = beginDataLine;
              mydocument.clear();
              mydocument.seekg(dataLine);//pula de volta para uma linha após data            
            }//se não tiver chegado ao final do texto procura por label e erros lexicos
            // else{
            //   organizingLabelsInstructionsDirectives(set, directives, symb, tokenVector, &position, &numberLine);              
            // }
            numberLine++;
          }               
      }
    }
  }
}
vector<string> recoverCodesandPositions(vector<string>tokenVector, unordered_map<string, int> &symb, const unordered_map<string, instructions> &set){
  vector<string>auxVector;
  vector<string> returnVector;
  string instructionCode = set.at(tokenVector[0]).opcode;
  string operandMemory = to_string(symb.at(tokenVector[1]));

  auxVector.push_back(instructionCode);
  auxVector.push_back(operandMemory);
  if(tokenVector.size()==3){//se vier de COPY
    string operandMemory2=to_string(symb.at(tokenVector[2]));
    auxVector.push_back(operandMemory2);

  }
  for(auto v: auxVector){
    if(v.at(0)=='0'){//se o primeiro caracter for 0
    v.erase(0,1);
    }
    returnVector.push_back(v);  
  }  
  return returnVector;
}

bool isItNumber(string token, string firstfile, vector<string> tokenVector){
  size_t stringposition;
  stringposition= token.find_first_not_of("1234567890.,_");
  int errorLine;
  if(stringposition==string::npos){//se nao achar nada que nao seja numero
      changeGlobalError();
      errorLine= findLine(tokenVector, firstfile);

      cout<<"\nERRO SINTÁTICO LINHA: "<< errorLine << " OPERANDO"<< token<<"-- TIPO DE OPERANDO INVÁLIDO\n";
      return true;
    }
  return false;

}

void verifyOperands(vector<string> tokenVector, unordered_map<string, int> &symb, ofstream &newfile, const unordered_map<string, instructions> &set, string originalFile){
  if(!isOnTable(tokenVector[1], symb)){//se nao estiver na tabela de simbolo
    changeGlobalError();
    int errorLine= findLine(tokenVector, originalFile);

    cout<<"\nERRO SEMÂNTICO LINHA: "<< errorLine << " SIMBOLO "<< tokenVector[1]<<" NÃO DEFINIDO\n";
  }
  else {//se estiver na tabela
  //verifica se teve erro, se nao teve, imprime no novo documento
    if(!isThereError){
      vector<string> printVector = recoverCodesandPositions(tokenVector, symb, set);
      if(tokenVector[0]=="COPY"){
        for(int i=0; i<3;i++){
          newfile<< printVector[i] << " ";
        }
      }
      else{
        for(int i=0; i<2;i++){
          newfile<< printVector[i] << " ";
        }
      }
    }
  }
}

void secondpass(string auxdocument, const unordered_map<string, instructions> &set, const unordered_map<string, int> &directives, unordered_map<string, int> &symb, string firstfile ){
  string newTitle;
  int numberLine=1;
  int dataLine;
  int beginDataLine;
  bool didText= false; //variavel para controlar se a seção TEXT ja foi lida
  bool foundText=false; //começa falso para pular as linhas de data
  string auxstring= auxdocument;
  size_t dotPosition= auxstring.find_last_of(".");

  newTitle = auxstring.erase(dotPosition+1, string::npos) + "obj";

  ifstream mydocument; //arquivo modificado para leitura
  mydocument.open(auxdocument);

  ofstream newfile; //arquivo novo, escrita
  newfile.open(newTitle);
  string line; 

  if(!mydocument.is_open()){//se deu erro ao abrir o arquivo
    cout << "Não foi possível abrir o arquivo, erro na segunda passagem \n";
  }
  else{
    while (getline (mydocument, line)){
      vector<string> tokenVector;
      splitToVector(line, tokenVector);

        //tem sempre que achar um section antes de achar outras coisas
        if(tokenVector[0]=="SECTION"){
          if((tokenVector[1]=="DATA")&&(!didText)&&(foundText)){//se DATA vier depois de texto
            // getchar();
            didText = true;
          }
          if((tokenVector[1]=="DATA")&&(!didText)){//se nao tiver passado por texto primeiro
            dataLine= mydocument.tellg();//guarda a posição de data
            beginDataLine= numberLine; //Guarda a contagem de linha em data
            numberLine++;
          }
          else if((tokenVector[1]=="TEXT")&&(!didText)){//Quando acha texto da primeira vez
              foundText=true;
              numberLine++;
          }
          else if((tokenVector[1]=="TEXT")&&(didText)){//Quando acha texto da segunda vez
              newfile.close();
              return;
          }
        }
        else{//Quando le algo que nao é section
          if(didText){//Se ja fez o texto uma vez, vai tratar os dados
            if(!isLabel(tokenVector)){//se não for um labeL
                changeGlobalError();

                cout<<"\nERRO SINTÁTICO LINHA: "<< numberLine << " RÓTULO FALTANTE\n";
            }
            else{
              if(tokenVector.size()>3){
                changeGlobalError();

                cout<<"\nERRO SINTÁTICO LINHA: "<< numberLine << " NÚMERO DE OPERANDOS ERRADO\n";
              }
              else{
                if(isOnDirectives(directives, tokenVector[1])){
                  if(tokenVector[1]=="CONST"){
                    if(tokenVector.size()<3){//para const tem que ter tres argumentos
                      changeGlobalError();

                      cout<<"\nERRO SINTÁTICO LINHA: "<< numberLine << " NÚMERO DE OPERANDOS ERRADO\n";
                    }
                    if(!isThereError){//se não houver erro
                      newfile<< tokenVector[2]<<" ";
                    }
                  }
                  else if(tokenVector[1]=="SPACE"){
                    if(tokenVector.size()>2){//para const tem que ter tres argumentos
                      changeGlobalError();

                      cout<<"\nERRO SINTÁTICO LINHA: "<< numberLine << " NÚMERO DE OPERANDOS ERRADO\n";
                    }
                    else{
                      if(!isThereError){//se não houver erro
                        newfile<<"0"<<" ";
                      }
                    }
                  }
                }
                else{
                  changeGlobalError();

                  cout<<"\nERRO SINTÁTICO LINHA: "<< numberLine << " DIRETIVA"<< tokenVector[1]<<"-- TIPO DE DIRETIVA INVÁLIDO\n";                    
                }
              }
            }      
            numberLine++;    
          }
          else if((!foundText)){//se é data e ainda nao achou o texto, ainda assim conta as linhas
            numberLine++;
          }
          else if(foundText && (!didText)){//se eu ja tiver chegado em texto uma vez mas nao tiver acabado o texto, trata o texto
          //ignora o rótulo
            if(isLabel(tokenVector)){//se o primeiro token for Label
              tokenVector.erase(tokenVector.begin());//remove o label do vetor
            }
          //Verify if instructions exists
            if(isOnInstructions(set, tokenVector[0])){
              if(tokenVector[0]=="COPY"){//Se for a instrução COPY
              //nesse momento COPY deve estar com o vetor de tamanho2
                if((tokenVector.size()<2) || (tokenVector.size()>2) ){//Se so tiver 1 no vetor, então nao tem operandos, Se nao for STOP, da erro
                  //se tiver mais de 2 coisas no vetor, tambem ta errado pq as operações so tem um operando
                  changeGlobalError();

                  cout<<"\nERRO SINTÁTICO LINHA: "<< numberLine << " NÚMERO DE OPERANDOS ERRADO\n";
                }
                //verificar se tem a vírgula
                size_t comaposition;
                comaposition= tokenVector[1].find_first_of(",");
                 
                if(comaposition==string::npos){//se não achar nenhuma vírgula
                  changeGlobalError();

                  cout<<"\nERRO SINTÁTICO LINHA: "<< numberLine << " OPERANDO"<< tokenVector[1]<<"-- TIPO DE OPERANDO INVÁLIDO, falta uma vírgula\n";
                }
                else{
                  string auxtoken = tokenVector[1];
                  string auxtoken2= tokenVector[1];
                  tokenVector[1]= auxtoken.erase(comaposition, string::npos);
                  auxtoken2= auxtoken2.erase(0, comaposition+1);
                  tokenVector.push_back(auxtoken2);
                  if(tokenVector[2]==""){//se o token estiver vazio
                    changeGlobalError();

                    cout<<"\nERRO SINTÁTICO LINHA: "<< numberLine << " NÚMERO DE OPERANDOS ERRADO\n";
                  }
                  else{
                    if(!isItNumber(tokenVector[1], firstfile, tokenVector)){//se nao for numero
                      if(!isItNumber(tokenVector[2], firstfile, tokenVector)){//se os dois forem ok
                        verifyOperands(tokenVector, symb, newfile, set, firstfile);
                      }
                    }            
                  }
                }
              }
              else if(tokenVector[0]!="STOP"){
                if((tokenVector.size()<2) || (tokenVector.size()>2) ){//Se so tiver 1 no vetor, então nao tem operandos, Se nao for STOP, da erro
                  //se tiver mais de 2 coisas no vetor, tambem ta errado pq as operações so tem um operando
                  changeGlobalError();

                  cout<<"\nERRO SINTÁTICO LINHA: "<< numberLine << " NÚMERO DE OPERANDOS ERRADO\n";
                }
                //Se não for STOP, possui operandos, vamos catar eles
                //verifyOperands
                size_t stringposition;
                stringposition= tokenVector[1].find_first_not_of("1234567890.,_");
                if(stringposition==string::npos){//se nao achar nada que nao seja numero
                  changeGlobalError();

                  cout<<"\nERRO SINTÁTICO LINHA: "<< numberLine << " OPERANDO"<< tokenVector[1]<<"-- TIPO DE OPERANDO INVÁLIDO\n";

                }
                else{//se for algo diferente de um número
                  verifyOperands(tokenVector, symb, newfile, set, firstfile);
                }
              }
              else{//se for o STOP
                if(tokenVector.size()>1){
                  changeGlobalError();

                  cout<<"\nERRO SINTÁTICO LINHA: "<< numberLine << " NÚMERO DE OPERANDOS ERRADO\n";
                }
                else{
                  string auxprint= set.at(tokenVector[0]).opcode;
                  newfile<< auxprint << " ";
                }
              }
            }
            else{//se nao achouu na tabela de instrução
              isThereError=true;
              changeGlobalError();

              cout<<"ERRO SINTÁTICO LINHA: "<< numberLine << " INSTRUÇÃO: "<< tokenVector[0] <<" Inválida\n";
            }
            // numberLine++;
            if(mydocument.peek() == EOF){//se tiver chegado ao final do texto
              didText= true;
              numberLine = beginDataLine;
              mydocument.clear();
              mydocument.seekg(dataLine);//pula de volta para uma linha após data
          }
          numberLine++;
        }
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
    cout<<"É necessário a extensão do arquivo\n";
    exit(1);
  } 
  string tempdocument = originalFile;
  tempdocument = tempdocument.erase(dotPosition+1, string::npos) + "txt";

  //função que cria o arquivo temporário limpo
  limpatexto(originalFile, tempdocument);
  //criando minha tabela de instrução
  unordered_map<string, instructions> instructionsSet;
  unordered_map<string, int> directives;
  unordered_map<string, int> symbolTable;//tabela de simbolos 

  //firstpass
  createDirectiveTable(directives);
  createInstructions(instructionsSet);
  firstpass(tempdocument, instructionsSet, directives, symbolTable, originalFile);
  
  secondpass(tempdocument, instructionsSet, directives, symbolTable, originalFile);


}