#include<iostream>
#include<fstream>
#include<regex>
#include<list>
#include<string>
#include <algorithm>
//Jadson Costa de Amorim
//A calculadora apenas aceita expressoes simples sem parenteses,
//para expressoes compostas ela realiza os calculos da direita para a 
//esquerda indo da expressao mais interna para a mais externa

//estruturas
struct Token{
       std::string valor;
       std::string token;
};
struct Expressao{
       std::string num1;
       std::string num2;
       std::string operador;
};
using namespace std;
//regex
bool is_numero(std::string s){
    std::regex digito = std::regex("[0-9]+|[0-9]+[.][0-9]+");
    if(std::regex_match(s, digito))
        return true;
    return false;
}
bool is_operacao(std::string s){
    std::regex caracter = std::regex("[0-9]+[+][0-9]+|[0-9]+[-][0-9]+|[0-9]+[*][0-9]+|[0-9]+[/][0-9]+");
    if(std::regex_match(s, caracter))
        return true;
    return false;
}
bool is_inicio_operacao(std::string s){
    std::regex caracter = std::regex("[0-9]+[+]|[0-9]+[-]|[0-9]+[*]|[0-9]+[/]");
    if(std::regex_match(s, caracter))
        return true;
    return false;
}
bool is_fim_operacao(std::string s){
    std::regex caracter = std::regex("[+][0-9]+|[-][0-9]+|[*][0-9]+|[/][0-9]+");
    if(std::regex_match(s, caracter))
        return true;
    return false;
}
bool is_operador_aritmetico(std::string s){
    std::regex digito = std::regex("[*]|[/]|[-]|[+]");
    if(std::regex_match(s, digito))
        return true;
    return false;
}
bool tem_parenteses(std::string s){
    int i;
    for(i=0;i<s.length()-1;i++){
        if(s[i]=='('){
            return true;
        }
    }
    return false;
}
//gerador de tokens
std::string tokenExpression(std::string expressao){
            if(expressao == "+"){
                return "<operador>";
            }else if(expressao == "-"){
                return "<operador>";
            }else if(expressao == "*"){
                return "<operador>";
            }else if(expressao == "/"){
                return "<operador>";
            }else if(is_numero(expressao)){
                return "<numero>";
            }else{
                return "";
            }
}
//separacao de expressao operador e numero
int limite_expressao(std::string s){
    int i;
    std::string aux;
    for(i = s.length()-1 ; i>=0; i--){
        aux = s[i];
        if(is_operador_aritmetico(aux)){
            return i;
        }
    }
    return -1;
}
//gera tokens
void geraToken(std::string expressao, list<Token>*resultados){
      std::string procura_expressao = "";
      struct Token token;
      list<Token>::iterator it;
      bool possui_expressao_composta = false;
      std::string aux;
      std::string numero_corrente = "";
      int i;
      int limite;
      if(!is_operacao(expressao)){
          limite = limite_expressao(expressao);
          for(i=0; i<expressao.length(); i++){
            if(i < limite){
               procura_expressao = procura_expressao+expressao[i];
            }else{
                if(i == limite){
                   token.token = "<expressao>";
                   token.valor = procura_expressao;
                   resultados->push_back(token);
                   aux = expressao[i];
                   token.token = tokenExpression(aux);
                   token.valor = aux;
                   resultados->push_back(token);
                   procura_expressao = "";
                }else{
                   procura_expressao = procura_expressao+expressao[i];
                }
            }
        }
        token.token = tokenExpression(procura_expressao);
        token.valor = procura_expressao;
        resultados->push_back(token);
    }else{
        for(i=0; i<expressao.length(); i++){
             if(tokenExpression(procura_expressao+expressao[i]) == ""){
                token.token = tokenExpression(procura_expressao);
                token.valor = procura_expressao;
                resultados->push_back(token);
                aux = expressao[i];
                token.token = tokenExpression(aux);
                token.valor = aux;
                resultados->push_back(token);
                procura_expressao = "";
             }else{
                procura_expressao = procura_expressao+expressao[i];
             }

        }
        token.token = tokenExpression(procura_expressao);
        token.valor = procura_expressao;
        resultados->push_back(token);
    }
}
//gera string de tokens
std::string codifica(list<Token>resultados){
     std::string tokens;
     list<Token>::iterator it;
     for (it = resultados.begin(); it != resultados.end(); ++it){
           tokens = tokens+it->token;
     }
     return tokens;
}
//retorna fim de tag
std::string end_token(std::string token){
     if(token == "<expressao>"){
        return "</expressao>";
     }else if(token == "<numero>"){
        return "</numero>";
     }else if(token == "<operador>"){
        return "</operador>";
     }else {
        return "";
     }
}
//faz as operacoes
float opera(std::string numero1, std::string operador, std::string numero2){
    float n1,n2;
    n1 = std::stof(numero1);
    n2 = std::stof(numero2);
    if(operador == "+"){
        return n1+n2;
    }
    else if(operador == "-"){
        return n1-n2;
    }
    else if(operador == "*"){
        return n1*n2;
    }
    else if(operador == "/"){
        return n1/n2;
    }else{
        return -1;
    }
}
//calculadora analise semantica
float calculadora(std::string s){
     std::string tokens = "";
     std::string codificacao;
     struct Expressao expressao;
     list<Token>resultados;
     list<Token>::iterator it;
     geraToken(s,&resultados);
     codificacao = codifica(resultados);
     if(codificacao == "<numero><operador><numero>"){
       for (it = resultados.begin(); it != resultados.end(); ++it){
          if(it->token == "<numero>" && tokens == ""){
             expressao.num1 = it->valor;
             tokens = tokens+it->token;
          }else if(it->token == "<numero>" && tokens == "<numero><operador>"){
             expressao.num2 = it->valor;
             return opera(expressao.num1,expressao.operador,expressao.num2);
          }else if(it->token == "<operador>"){
             expressao.operador = it->valor;
             tokens = tokens+it->token;
          }
       }
     }else{
         for (it = resultados.begin(); it != resultados.end(); ++it){
              if(it->token == "<numero>"){
                   if(tokens == ""){
                     expressao.num1 = it->valor;
                     tokens = tokens+it->token;
                   }else{
                     expressao.num2 = it->valor;
                     return opera(expressao.num1,expressao.operador,expressao.num2);
                   }
              }else if(it->token == "<expressao>"){
                   if(tokens == ""){
                      expressao.num1 = std::to_string(calculadora(it->valor));
                      tokens = tokens+it->valor;
                   }else{
                      expressao.num2 = std::to_string(calculadora(it->valor));
                      return opera(expressao.num1,expressao.operador,expressao.num2);
                   }
              }else if(it->token == "<operador>"){
                   expressao.operador = it->valor;
                   tokens = tokens+it->token;
              }
         }
     }
     return -1;
}
//geardor da arvore
void gera_arvore(std::string expressao, std::string *conteudo){
     list<Token>resultados;
     list<Token>::iterator it;
     std::string token;
     std::string valor;
     std::string tab = "\t";
     std::string quebra ="\n";

     geraToken(expressao,&resultados);
     for (it = resultados.begin(); it != resultados.end(); ++it){
          token = it->token;
          valor = it->valor;
          if(it->token == "<numero>" || it->token == "<operador>"){
             *conteudo = *conteudo+tab+tab+token+valor+end_token(token)+quebra;
          }else{
             *conteudo = *conteudo+tab+token+quebra;
             gera_arvore(valor, conteudo);
             *conteudo = *conteudo+tab+end_token(token)+quebra;
          }
     }
}
//imprime arvore no arquivo
void imprimir_arvore(std::string expressao){
     ofstream arquivo;
     std::string conteudo = "";
     arquivo.open("calculadora.xml");

     arquivo<<"<?xml version='1.0' encoding='UTF-8'?>"<<endl;
     arquivo<<"<expressao>"<<endl;
     gera_arvore(expressao, &conteudo);
     arquivo<<conteudo;
     arquivo<<"</expressao>"<<endl;

     arquivo.close();
}
//validacoes analise sintatica
bool valida_caracteres(std::string expressao, int *p){
    int i;
    std::string aux;
    for(i = 0;i < expressao.length(); i++){
        aux = expressao[i];
        if(!is_numero(aux) && !is_operador_aritmetico(aux)){
           *p = i;
           return false;
        }
    }
    return true;
}
bool valida_forma(std::string expressao){
     int i;
     if(is_inicio_operacao(expressao) || is_fim_operacao(expressao)){
        return false;
     }
     return true;
}
void valida_expressao(std::string expressao){
    int c = -1;
    if(!valida_caracteres(expressao, &c)){
       cout<<"erro:caractere "<<expressao[c]<<" invalido!"<<endl;
       exit(1);
    }
    if(!valida_forma(expressao)){
       cout<<"erro:forma invalida!"<<endl;
       exit(1);
    }
    if(is_numero(expressao)){
        cout<<"Apenas um numero nao e uma expressao!"<<endl;
        exit(1);
    }
}
//programa
int main(){
    std::string expressao;
    cout<<"Informe uma expressao:"<<endl;
    cin>>expressao;
    valida_expressao(expressao);
    cout<<calculadora(expressao)<<endl;
    imprimir_arvore(expressao);
    return 0;
}