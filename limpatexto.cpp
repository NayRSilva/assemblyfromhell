#include "limpatexto.hpp"
using namespace std;

void removeSpaceBeginEnd(string &line){
  string regex= " \n\t\f\v\r";
  //apaga qualquer coisa antes de uma caractere
  size_t lineposition;
  lineposition= line.find_first_not_of(regex);
    if(lineposition <= line.length()){
      line.erase(0, lineposition);
    }
  //acha a ultima coisa que nao é um espaço e apaga a partir daí
  lineposition = (line.find_last_not_of(regex))+1;
  if(lineposition <= line.length()){
    line.erase(lineposition, string::npos);
  }

}

void removeExtraSpace(string &line){//pegar o espaço vazio entre textos
  string regex= " \n\t\f\v\r";
  //já não tem espaço no final nem no fim da linha. O próximo espaço em branco está entre textos
  //Colunas 1 e 2
  size_t lineposition;
  lineposition = line.find_first_of(regex);//primeiro espaço pós-texto
  size_t beforeTextPosition= (line.find_first_not_of(regex, lineposition))- lineposition;//acha o primeiro que não é espaço, dado a posição de lineposition
  
  if(lineposition <= line.length()){
    
    line.replace(lineposition,beforeTextPosition, " ");
  }
  //colunas 2 e 3
  //tem um espaço entre coluna 1 e 2 que precisa ser ignorado
  lineposition = line.find_first_of(regex);//primeiro espaço pós-texto  
  size_t afterSecondColumn = line.find_first_of(regex, lineposition+1);
  size_t beforeTextPosition2= (line.find_first_not_of(regex, afterSecondColumn))- afterSecondColumn;
  
  if(afterSecondColumn <= line.length()){
    line.replace(afterSecondColumn,beforeTextPosition2, " ");

  }
}

void removeComments(string &line){
  string regex= ";";

  size_t lineposition;

  lineposition = line.find_first_of(regex);
        if(lineposition <= line.length()){
          line.replace(lineposition, string::npos, "");//tenho mesmo que declarar string?
        }

}
void  writeline(string &line, ofstream &newfile){
    string regex= " \n\t\f\v\r";
    size_t lineposition;

      lineposition = line.find_first_not_of(" \t\f\v\n\r");

      if(lineposition!=(string::npos)){
        cout<< line <<"\n";
        newfile << line << '\n';
      }
    
  }

void limpatexto(string filename, string newfilename){
  string line;
  ifstream originalfile; //arquivo original, leitura
  originalfile.open(filename);
  ofstream newfile; //arquivo novo, escrita
  newfile.open(newfilename);

  //passa por cada linha uma vez
  if (!originalfile.is_open()){//se deu erro ao abrir o arquivo
    cout << "Não foi possível abrir o arquivo, erro ao limpar texto \n";
  }else{
    while (getline (originalfile, line)){//Primeiro: Preciso do texto em maiusculo.  
      transform(line.begin(), line.end(), line.begin(), ::toupper);
     
      //2: Remove comentarios

      removeComments(line);

      //3: remove os espaços do início e do fim
      removeSpaceBeginEnd(line);

      //4: remove espaços enormes entre palavras.
      removeExtraSpace(line); 
      //5: escreve no artigo final
      writeline(line, newfile);

    }
    newfile.close();
    originalfile.close();
  }
}
