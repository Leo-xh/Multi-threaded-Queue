#include "MultiAccessQueue.h"
#include <cstdio>
#include <omp.h>
#include <random>
#include <cstdlib>
#include <time.h>

MultiAccessQueue<int> que;

void insert();
void extract();

#define THREADS 128
#define TIMES 5000000

int main()
{
  omp_set_num_threads(THREADS);
  omp_set_nested(1);
  double start, stop;
  start = omp_get_wtime();
  #pragma omp parallel sections
  {
      #pragma omp section
  	{
      printf("Totally %d threads\n", omp_get_num_threads());
  	}
      #pragma omp section
  	{
  	  insert();
  	}
      #pragma omp section
  	{
  	  extract();
  	}
  }
  stop = omp_get_wtime();
  if (que.empty()) {
   printf("The queue is empty now.\n");
  }
  // printf("%f-%f\n", stop, start);
  printf("Parallel threads costs %lf seconds\n", stop-start);
  start = time(nullptr);
  for (int i = 1; i <= TIMES; i++)
  {
    que.push((i*18273645272/234971)%10000009);
  }
  for (int i = 1; i <= TIMES; i++)
    {
      while (que.empty()) {};
      que.pop();
    }
  stop = time(nullptr);
  if (que.empty()) {
     printf("The queue is empty now.\n");
  }
  // printf("%f-%f\n", stop, start);
  printf("Serial program costs %f seconds\n", stop-start);
  system("pause");
  return 0;
}

void insert()
{
  omp_set_num_threads(THREADS/2);
  #pragma omp parallel default(shared)
  {
    #pragma omp single 
  	{
        #pragma omp critical
      printf("There are %d threads executing inserting.\n", omp_get_num_threads());
    }
      #pragma omp for
    for (int i = 1; i <= TIMES; i++)
    {
      que.push((i*18273645272/234971)%10000009);
    }
  }
}

void extract()
{
  omp_set_num_threads(THREADS/2);
  #pragma omp parallel default(shared)
  {
    #pragma omp single
    {
        #pragma omp critical
      printf("There are %d threads executing extracting.\n", omp_get_num_threads());
  	}
  	int i;
    #pragma omp for
  	for (i = 1; i <= TIMES; i++)
  	{
  	  while (que.empty()) {};
        // #pragma omp critical
      // printf("The %d th number extracted is %d\n", i, que.front());
  	  que.pop();
  	}
  }
}
