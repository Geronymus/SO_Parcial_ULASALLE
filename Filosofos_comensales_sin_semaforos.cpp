/*
Cena de los filosofos sin semaforos.
Durante el tiempo que un filósofo está comiendo o pensando permanecerá
bloqueado un número aleatorio de segundos entre 0 y 7.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
 
#define N 5 //Número de filósofos
#define T_A 7 //tiempo aleatorio
#define COMIDAS 2 //Número de comidas
#define IZQUIERDA(x) ((x) !=0 ? (x)-1 : (N-1)) //Calcula el filosofo de la izqda
#define DERECHA(x) (((x)+1)%N) //Calcula el filosofo de la dcha


typedef enum{
PENSANDO,HAMBRIENTO,COMIENDO
}status;

status estado[N]; //Estado en el que esta cada proceso en cada momento (pensando, hambriento o comiendo)

void pensar(int id);
void comer(int id);
void coger_tenedores(int id);
void Comprobar(int id);
void soltar_tenedores(int id);

int main(){

int v[N]/*para decirle a cada hilo quien es*/,value,i;
pthread_t filosofo[N];
void *Filosofo(void* );


for(i=0;i<N;i++){
v[i]=i;
if(value=pthread_create(&filosofo[i],NULL,Filosofo, (void *) &v[i]))
      exit(value);
}

for(i=0;i<N;i++){
pthread_join(filosofo[i],NULL);//Podemos ver que el Filosofo a muerto,y se acaba el programa.
    printf("\t\tMurió Filosofo %d -> Last status: %d\n", i, estado[i]);
}
return 0;
}

void Filosofo(void* i){
int id,j;
id=*(int *) i;

for(j=0;j<COMIDAS;j++){//For para PENSAR y COMER
pensar(id);
coger_tenedores(id);
comer(id);
soltar_tenedores(id);
}
}

void pensar(int id){
printf("Filosofo %d pensando\n",id);//Crea nuestra aleatoriedad de threads de PENSAR entre 0 a 7 segundos
sleep(random() % T_A);
}

void comer(int id){
printf("Filosofo %d comiendo\n",id);//Crea nuestra aleatoriedad de threads de COMER entre 0 a 7 segundos
sleep(random() % T_A);
}

void coger_tenedores(int id){
estado[id]=HAMBRIENTO;
Comprobar(id);
}

void Comprobar(int id){
if(estado[id]==HAMBRIENTO&&estado[IZQUIERDA(id)]!=COMIENDO&&estado[DERECHA(id)]!=COMIENDO){
estado[id]=COMIENDO;
  }
}

void soltar_tenedores(int id)
{
estado[id]=PENSANDO;
Comprobar(IZQUIERDA(id));
Comprobar(DERECHA(id));
}