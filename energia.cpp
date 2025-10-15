#include <iostream>
#include <unistd.h>
#include <cmath>
#include <fstream>
#include <string> 
#include <ctime>

//1 min ALTERE PARA SIMULAÇÕES
int tempo_de_verificação = 60;

int movimento = 20;

int hh, mm;

int velocidade_simulação = 1; //reduza para maior velocidade
using namespace std;
//Struct que define todos os tipos de iluminação
typedef struct{
    //total de postes na região
    int total;

    //tamanho max para cada tipo
    //possui o dobro de tamanho de SP
    bool residenciais[900000] = {false};
    bool viarias_principais[500000] = {false};
    bool locais_publicos[160000] = {false};
    bool locais_de_transportes[100000] = {false};

    float consumo_residenciais = 0;
    float consumo_viarias_principais = 0 ;
    float consumo_locais_publicos = 0;
    float consumo_locais_de_transportes = 0;

    int cap_residenciais;
    int cap_viarias_principais;
    int cap_locais_publicos;
    int cap_locais_de_transportes;

    bool ch_resi = false;
    bool ch_via = false;
    bool ch_pub = false;
    bool ch_tr = false;
}BD_postes;
BD_postes postes;

int random_number(int lim){
    return rand() % lim;  // Gera 0 até (lim-1)
}

void clear() { // limpa a tela
    system("clear"); 
}

void iniciar_sensores(){
    if((hh >= 17 && mm >= 30)|| (hh <=22 && mm <= 59))
    {
        postes.ch_resi = true;
        postes.ch_tr = true;
        for(int i = 0; i < postes.cap_residenciais; i++)
        {postes.residenciais[i] = true;
        }

        for(int i = 0; i < postes.cap_locais_de_transportes; i++)
            {postes.locais_de_transportes[i] = true;}

    }   
    
    else if((hh >= 18 && mm >= 0) || (hh <= 22 && mm <= 59))
        {
            postes.ch_via = true;
            postes.ch_pub = true;
        for(int i = 0; i < postes.cap_viarias_principais; i++)
            {postes.viarias_principais[i] = true;}
        for(int i = 0; i < postes.cap_locais_publicos; i++)
            {postes.locais_publicos[i] = true;}
        
        }
}

void verificar_sensores(){
    int lim = 101;
   if((mm % 2) == 1){ 
    
    
    for(int i = 0; i < postes.cap_locais_publicos; i++) {
        int X = random_number(lim);  
        if(X < movimento && postes.locais_publicos[i] == true) {
            postes.locais_publicos[i] = false;
            cout << "Iluminação publica " << i << " foi desativada por inatividade" << endl;
        }
        if(X > movimento && postes.ch_pub == true && postes.locais_publicos[i] == false)
            {postes.locais_publicos[i] = true;
            cout << "Iluminação publica " << i << " foi ativada" << endl;
            }
    }

    for(int i = 0; i < postes.cap_locais_de_transportes; i++) {
        int X = random_number(lim);
        if(X < movimento + 10 && postes.locais_de_transportes[i] == true) {
            postes.locais_de_transportes[i] = false;
            cout << "Iluminação de transporte " << i << " foi desativada por inatividade" << endl;
        }
        if(X > movimento && postes.ch_tr ==  true && postes.locais_de_transportes[i] == false)
        {postes.locais_de_transportes[i] = true;
        cout << "Iluminação de transporte " << i << " foi ativado" << endl;}
    }

    for(int i = 0; i < postes.cap_viarias_principais; i++) {
        int X = random_number(lim);
        if(X < movimento + 20 && postes.viarias_principais[i] == true) {
            postes.viarias_principais[i] = false;
            cout << "Iluminação viaria " << i << " foi desativada por inatividade" << endl;
        }
        if(X > movimento && postes.ch_via == true && postes.viarias_principais[i] == false)
        {postes.viarias_principais[i] = true;
        cout << "Iluminação viaria " << i << " foi ativado" << endl;}
    }
}
}

void simulador_horario(){
cout << "insira a horario da simulação!" << endl;
cin >> hh >> mm;
}

void relogio(){
cout << hh << ":" << mm << endl;

if(mm < 59)
mm ++;
    else{
    hh = (hh + 1)% 24;
    mm = 0;
    }
sleep(velocidade_simulação);
clear();
}

int ler_BD(){
// acessa banco de dados
ifstream arquivo("BD_poste.txt");
if (!arquivo){
    cout << "Erro ao abrir ao acessar banco de dados!" << endl;
    return 1;}
arquivo >> postes.total;
arquivo >> postes.cap_residenciais;
arquivo >> postes.cap_viarias_principais;
arquivo >> postes.cap_locais_publicos;
arquivo >> postes.cap_locais_de_transportes;
arquivo.close();
return 0;
}

void encerrar_sensores(){
    if((hh == 23 && mm == 00) || (hh <= 17 && hh >= 7))
    {
        for(int i = 0; i < postes.cap_viarias_principais; i++)
            {postes.viarias_principais[i] = false;
            postes.ch_via = false;}
            

        for(int i = 0; i < postes.cap_locais_de_transportes; i++)
            {postes.locais_de_transportes[i] = false;
            postes.ch_tr = false;}

    }   
    
    else if((hh <= 16 && mm <= 59)|| (hh <= 17 && hh >=7))
        {
        for(int i = 0; i < postes.cap_viarias_principais; i++)
            {postes.viarias_principais[i] = false;
            postes.ch_via = false;}

        for(int i = 0; i < postes.cap_residenciais; i++)
            {postes.residenciais[i] = false;
            postes.ch_resi = false;}
        
        }
}   

void verifica_horario(){
    if((hh <= 22 && mm <= 59) && hh >= 18)
        movimento = 10;
    else if(hh == 23 || (hh < 2 && mm == 0))
        movimento = 45;
    else if((hh >= 2 && mm >= 1)&&  hh <= 4)
        movimento = 65;
    else if(hh == 5)
        movimento = 10;
}

void consumo_energetico(){
    
    postes.consumo_residenciais = ((random_number(6)/10.0 + 1) * postes.cap_residenciais) * 12;

    postes.consumo_viarias_principais = ((random_number(15)/10.0 + 1) * postes.cap_viarias_principais) * 12;
    
    postes.consumo_locais_publicos = ((random_number(20)/10.0 + 1) * postes.cap_locais_publicos)* 5;

    postes.consumo_locais_de_transportes = ((random_number(40)/10.0 + 1) * postes.cap_locais_de_transportes) * 5;

}
int criar_arquivo_dados(){
    ofstream consumo("consumo.txt");
    if(!consumo)
        return 1;
    return 0;
}

void exportar_dados(){
    postes.consumo_residenciais = 0;
    postes.consumo_viarias_principais = 0 ;
    postes.consumo_locais_publicos = 0;
    postes.consumo_locais_de_transportes = 0;

    consumo_energetico();
    ofstream consumo("consumo.txt", ios::app);

    if(hh == 5 && mm == 30)
        consumo <<  postes.consumo_residenciais << " " ; 

    if(hh == 6 && mm == 0)
        consumo <<  postes.consumo_viarias_principais << " " << endl; 

    if(hh == 23 && mm == 0)
    {
    consumo << postes.consumo_locais_publicos << " " << postes.consumo_locais_de_transportes;
    }
    }

int main(){
ler_BD();
criar_arquivo_dados();
simulador_horario();
while(true){
iniciar_sensores();
verifica_horario();


verificar_sensores();


relogio();
encerrar_sensores();
exportar_dados();
}
return 0;}