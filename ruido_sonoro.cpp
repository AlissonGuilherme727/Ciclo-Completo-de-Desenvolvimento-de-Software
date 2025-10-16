#include <iostream>
#include <unistd.h>
#include <cmath>
#include <fstream>
#include <string> 
#include <ctime>
#include <iomanip>
using namespace std;

 struct capitador_sonoro{
    float fixos[1200] = {0};
    float moveis[500] = {0};
    float portateis[300] = {0};
    int total;

    int num_fixos;
    int num_moveis;
    int num_port;
};
capitador_sonoro sensor;

int hh,mm;
void iniciar_rd() {
    srand(time(NULL));
}

float rd_float(float min, float max) {
    return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

int ler_BD(){
    ifstream arquivo("BD_sons.txt");
if (!arquivo){
    cout << "Erro ao abrir ao acessar banco de dados!" << endl;
    return 1;}

    arquivo >> sensor.total;
    arquivo >> sensor.num_fixos;
    arquivo >> sensor.num_moveis;
    arquivo >> sensor.num_port;

    if( sensor.total != (sensor.num_fixos + sensor.num_moveis + sensor.num_port))
        {
            cout << "Banco de dados corrompido, valores não condizentes!" << endl;
            return 1;
        }
    arquivo.close();
    return 0;
}

void registrar_dados(){
    ofstream saida("nivel_de_ruido_sonoros.txt",ios::app);
    float mediaM = 0;
    float mediaP = 0;
    float mediaF = 0;
    if((mm%10) ==0)
    {
    for(int i = 0; i < sensor.num_fixos;i++)
        mediaF = mediaF + sensor.fixos[i];
    mediaF = mediaF/sensor.num_fixos;
    
    for(int i = 0; i < sensor.num_moveis;i++)
        mediaM = mediaM + sensor.moveis[i];
    mediaM = mediaM / sensor.num_moveis;

    for (int i = 0; i < sensor.num_port;i++)
        mediaP = mediaP + sensor.portateis[i];
    mediaP = mediaP / sensor.num_port;

    
    saida << fixed << setprecision(2) << ((mediaF + mediaP + mediaM) / 3) << " ";

    saida.close();
    }
}

void clear(){
    system("clear");
}

void simulation_hr(){
cout << "Digite o horario que essa simulação deve ocorrer" << endl;
cin >> hh >> mm;
if (hh > 23 || hh < 0)
    hh = 0;
if(mm >= 60 || mm <= -1)
    mm = 0;
}

void relogio(){
if(mm < 59)
mm ++;
    else{
    hh = (hh + 1)% 24;
    mm = 0;
    }

cout << hh << " " << mm << endl;
sleep(1);
clear();

}

void verifica_sensores(){
if((mm%10) == 0)
    {
    for(int i = 0; i < sensor.num_fixos; i++)
        sensor.fixos[i] = rd_float(45, 85);

    for(int i = 0; i < sensor.num_moveis; i++)
        sensor.moveis[i] = rd_float(45,85); 

    for(int i = 0; i < sensor.num_port; i++)
        sensor.portateis[i] = rd_float(45,85);
    }
    

}



int main(){
//pré iniciando o codigo
ler_BD();
iniciar_rd();
simulation_hr();

//funcionalidade do codigo
while(true){
relogio();

verifica_sensores();


registrar_dados();
}

return  0;}