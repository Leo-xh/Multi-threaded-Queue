#include "MultiAccessQueue.h"
#include <cstdio>
#include <pthread.h>
#include <random>
#include <cstdlib>
#include <time.h>
#include <algorithm>

MultiAccessQueue<int> que;

void* insert(void * k);
void* extract(void * k);
void* dispatch_insert(void * arg);
void* dispatch_extract(void * arg);

#define THREADS 128
#define MAXTIMES 100000000
int TIMES;

pthread_attr_t attr;

int main()
{
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  for(TIMES = 1000; TIMES <= MAXTIMES; TIMES *= 10){
    double start, stop;
    start = clock();
    pthread_t dispatch_threads[2];
    int flag = pthread_create(&dispatch_threads[0], &attr, dispatch_insert, nullptr);
      if(!flag){
        printf("Begining inserting\n");
      } else {
        printf("Insert failed!\n");
      }
      flag = pthread_create(&dispatch_threads[1], nullptr, dispatch_extract, nullptr);
      if(!flag){
        printf("Begining extracting\n"); 
      } else {
        printf("Extract failed!\n");
      }
    pthread_join(dispatch_threads[0], nullptr);
    pthread_join(dispatch_threads[1], nullptr);
    stop = clock();
    if (que.empty()) {
      printf("%d random numbers inserted and extracted\n", TIMES);
      printf("The queue is empty now.\n");
    }
    // printf("%f-%f\n", stop, start);
    printf("Parallel threads costs %.10lf seconds\n", (stop-start)/CLOCKS_PER_SEC);
    start = clock();
    for (int i = 1; i <= TIMES; i++)
    {
      que.push((i*18273645272/234971)%10000009);
    }
    for (int i = 1; i <= TIMES; i++)
      {
        while (que.empty()) {};
        que.pop();
      }
    stop = clock();
    if (que.empty()) {
      printf("%d random numbers inserted and extracted\n", TIMES);
      printf("The queue is empty now.\n");
    }
    // printf("%f-%f\n", stop, start);
    printf("Serial program costs %.10lf seconds\n", (stop-start)/CLOCKS_PER_SEC);
  }
  system("pause");
  return 0;
}

void* dispatch_insert(void * arg)
{
  pthread_t insert_threads[THREADS/2];
  int count = 0, i;
  for (i = 0; i < THREADS/2; ++i)
  {
    int* steps = new int(i);
    int flag = pthread_create(&insert_threads[i], &attr, insert, (void*)(steps));
    if(!flag) count++;
  }
  printf("There are %d threads executing inserting.\n", count);
  for (int i = 0; i < THREADS/2; ++i)
  {
    pthread_join(insert_threads[i], nullptr);
  }
}

void* dispatch_extract(void * arg)
{
  pthread_t extract_threads[THREADS/2];
  int count = 0, i;
  for (i = 0; i < THREADS/2; ++i)
  {
    int* steps = new int(i);
    int flag = pthread_create(&extract_threads[i], &attr, extract, (void*)(steps));
    if(!flag) count++;
  }
  printf("There are %d threads executing extracting.\n", count);
  for (int i = 0; i < THREADS/2; ++i)
  {
    pthread_join(extract_threads[i], nullptr);
  }
}

void* insert(void *k)
{
  int steps = *((int*)k);  
  // printf("insert %d\n", steps);
  int limit = std::min((steps+1)*(TIMES/THREADS*2), TIMES);
  for (int i = steps*(TIMES/THREADS*2); i < limit; i++)
  {
    // printf("%dth number is inserted\n", i);
    que.push((i*18273645272/234971)%10000009);
  }
  // printf("%d done\n", steps);
  delete (int*)k;
}

void* extract(void *k)
{
  int steps = *((int*)k);  
  // printf("extract %d\n", steps);
  int limit = std::min((steps+1)*(TIMES/THREADS*2), TIMES);
  for (int i = steps*(TIMES/THREADS*2); i < limit ; i++)
  {
    while (que.empty()) {};
    // printf("%dth number is extracted\n", i);
    que.pop();
  }
  delete (int*)k;
}
