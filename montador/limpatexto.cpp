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
  //primeiras e segundas palavras
  size_t lineposition;
  lineposition = line.find_first_of(regex);//espaço depois do texto
  size_t beforeTextPosition= (line.find_first_not_of(regex, lineposition))- lineposition;//acha o primeiro que não é espaço, dado a posição de lineposition
  
  if(lineposition <= line.length()){
    
    line.replace(lineposition,beforeTextPosition, " ");
  }
  //segundas e terceiras coisinhas
  lineposition = line.find_first_of(regex);
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
    vector<string> vaux;

      lineposition = line.find_first_not_of(" \t\f\v\n\r");
      int lastCharPosition = line.length()-1;
      //Vou criar um vetor para verificar o caso de
      /*LABEL:
      ALGUMA COISA
       */
      istringstream auxString(line);
        string token;   


      while(auxString >> token){
          vaux.push_back(token);
        }
      if((vaux.size()==1) &&(vaux[0]!="STOP")){
        if(line[lastCharPosition]==':'){//lembrar de tirar isso daqui... eu acho
            // cout<<"Probably a label";
        }
        if(lineposition!=(string::npos)){
        // cout<< line <<"\n";
        newfile << line << ' ';
        }
      }else if(lineposition!=(string::npos)){
        // cout<< line <<"\n";
        newfile << line << '\n';
      }
    
  }
/*função principal que vai criar meu arquivo sem coisas que me atrapalhem */
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
     
      //tirando comentarios

      removeComments(line);

      //tirando os espaços começo e final
      removeSpaceBeginEnd(line);

      //tira o resto dos espaços p deixar so o que eu precio
      removeExtraSpace(line); 
      //escreve no de saida
      writeline(line, newfile);

    }
    newfile.close();
    originalfile.close();
  }
}
