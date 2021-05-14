/*
Cena de los filosofos con semáforos.
Durante el tiempo que un filósofo está comiendo o pensando permanecerá
bloqueado un número aleatorio de segundos entre 0 y 7.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
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
sem_t mutex,s[N]; //Mutex: Solo un filosofo puede coger o soltar tenedores; s[N]: Controlamos si he podido
//coger tenedores o no, si no he podido, me bloqueo hasta que nos despierte el filosofo de la izqda o dcha

void pensar(int id);
void comer(int id);
void coger_tenedores(int id);
void Comprobar(int id);
void soltar_tenedores(int id);

int main(){

int v[N]/*para decirle a cada hilo quien es*/,value,i;
sem_init(&mutex,0,1);
pthread_t filosofo[N];
void *Filosofo(void* );

for(i=0;i<N;i++) //NO SE PUEDEN LANZAR LOS HILOS HASTA QUE LOS SEMAFOROS NO ESTEN INICIALIZADOS
{
sem_init(&s[i],0,0);
estado[i]=PENSANDO;
}

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
sem_wait(&mutex); //Si un filosofo esta cogiendo tenedores, no podre coger y me 
//quedare bloqueado. Si no hay nadie cogiendo tenedores, cogere yo
estado[id]=HAMBRIENTO;
Comprobar(id);
sem_post(&mutex); //Permito que otro filosofo coja tenedores
sem_wait(&s[id]); //Si no hace el post de la funcion Comprobar(), se quedaria pillado aqui
}

void Comprobar(int id){
if(estado[id]==HAMBRIENTO&&estado[IZQUIERDA(id)]!=COMIENDO&&estado[DERECHA(id)]!=COMIENDO){
estado[id]=COMIENDO;
sem_post(&s[id]); //Sirve para liberar al filosofo para que no se quede pillado mas adelante, 
//puesto que ha podido coger tenedores
  }
}

void soltar_tenedores(int id)
{
sem_wait(&mutex); //Si hay alguien cogiendo tenedores, nadie mas puede coger. 
//Si estoy cogiendo tenedores, bloqueo para que nadie mas pueda hacerlo
estado[id]=PENSANDO;
Comprobar(IZQUIERDA(id));
Comprobar(DERECHA(id));
sem_post(&mutex);
}

