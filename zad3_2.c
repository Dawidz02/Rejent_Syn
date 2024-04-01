#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/* N*S == X*Y */
#define N 8  /* Liczba watkow */
#define S 12  /* Liczba szans na zdobycie terytorium */
#define X 10 /* Wymiary terytorium */
#define Y 10 /* Wymiary terytorium */

int syg_gotowy = 0;
int dzialka[X][Y];
int signal_sender = 0;
pthread_mutex_t mutex_przydzielenie;
pthread_cond_t cond_koniec;

void *los_terytorium(void *arg)
{
    srand48(time(NULL)+(long)arg);
    int nr_potomka = (long)arg;
    int x = 0, y = 0, l_losowan = 0;
    // printf("Uruchomiono potomka %d\n", nr_potomka);
    while (l_losowan < S)
    {
        // printf("liczba losowan %d, watku %d\n", l_losowan, nr_potomka);
        x = ((int)(drand48()*10) % X);
        y = ((int)(drand48()*10) % Y);
        //printf("watek %d wylosowal %d %d\n",nr_potomka,x,y);
        pthread_mutex_lock(&mutex_przydzielenie);
        if (dzialka[x][y] == 0)
        {
            //printf("Potomek %d bierze dzialke %d %d\n", nr_potomka, x, y);
            dzialka[x][y] = nr_potomka;
        }
        pthread_mutex_unlock(&mutex_przydzielenie);
        l_losowan++;
    }
    while(syg_gotowy == 1){
        sleep(1);
    }
    syg_gotowy = 1;
    signal_sender = nr_potomka;
    //printf("Potomek %d wysyla sygnal\n", signal_sender);
    pthread_cond_signal(&cond_koniec);

    pthread_exit(NULL);
}

void *Rejent_funkcja(void *arg)
{
    /* Wyswietlenie liczby dzialek zdobytych przez syna */
    for (int z = 0; z < N; z++)
    {
        pthread_mutex_lock(&mutex_przydzielenie);
        int l_dzialek = 0;
        //printf("%d\n", z);
        pthread_cond_wait(&cond_koniec, &mutex_przydzielenie);
        printf("Syn %d posiada dzialki o wspolrzednych:\n",signal_sender);
        for (int x = 0; x < X; x++)
        {
            for (int y = 0; y < Y; y++)
            {
                if (dzialka[x][y] == signal_sender)
                {
                    printf("(%d, %d)  ", x,y);
                    l_dzialek++;
                }
            }
        }
        printf("\nZdobyl on %d dzialek.\n\n", l_dzialek);
        pthread_mutex_unlock(&mutex_przydzielenie);
        syg_gotowy = 0;
    }

    /* Wypisanie przynaleznosci dzialek */
    for(int x=0; x<X; x++)
        {
            for(int y=0; y<Y; y++)
            {
                printf("%d ",dzialka[x][y]);
            }
            printf("\n");
        }
    
    /* Liczba nie zajetych pol */
    int l_dzialek = 0;
    for (int x = 0; x < X; x++)
        {
            for (int y = 0; y < Y; y++)
            {
                if (dzialka[x][y] == 0)
                {
                    l_dzialek++;
                }
            }
        }
    printf("\n Liczba nie zajetych dzialek wynosi %d\n", l_dzialek);
}

int main()
{
    int l_dzialek = 0;
    /* Wyzerowanie tablicy dzialek */
    for (int x = 0; x < X; x++)
    {
        for (int y = 0; y < Y; y++)
        {
            dzialka[x][y] = 0;
        }
    }

    pthread_mutex_init(&mutex_przydzielenie, NULL);
    pthread_cond_init(&cond_koniec, NULL);

    pthread_t th[N];
    pthread_t rejent;

    /* Tworzenie watkow */
    if (pthread_create(&rejent, NULL, &Rejent_funkcja, NULL) != 0)
    {
        printf("Blad: pthread_create\n");
    }

    for (long i = 0; i < N; i++)
    {
        // printf("Utworzenie watku %ld.\n", i);
        if (pthread_create(&th[i], NULL, &los_terytorium, (void *)(i + 1)) != 0)
        {
            printf("Blad: pthread_create\n");
        }
    }


    for (int i = 0; i < N; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            printf("Blad: pthread_join\n");
        }
    }
    if (pthread_join(rejent, NULL) != 0)
    {
            printf("Blad: pthread_join\n");
    }

    pthread_mutex_destroy(&mutex_przydzielenie);
    pthread_cond_destroy(&cond_koniec);
}
