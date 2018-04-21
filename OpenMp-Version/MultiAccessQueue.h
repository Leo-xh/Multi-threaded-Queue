#include <queue>
#include <omp.h>
#include <cstdlib>

template<typename T>
class MultiAccessQueue
{
public:
  MultiAccessQueue();
  ~MultiAccessQueue();
  bool empty();
  size_t size();
  T front();
  T back();
  void push(T ele);
  void pop();

private:
  std::queue<T> oriQue;
  omp_lock_t headLock;
  omp_lock_t tailLock;
};

template <typename T>
MultiAccessQueue<T>::MultiAccessQueue()
{
  omp_init_lock(&headLock);
  omp_init_lock(&tailLock);
}

template <typename T>
MultiAccessQueue<T>::~MultiAccessQueue()
{
  omp_destroy_lock(&headLock);
  omp_destroy_lock(&tailLock);
}

template<typename T>
bool MultiAccessQueue<T>::empty()
{
  omp_set_lock(&headLock);
  omp_set_lock(&tailLock);
  bool ret = oriQue.empty();
  omp_unset_lock(&headLock);
  omp_unset_lock(&tailLock);
  return ret;
}

template<typename T>
size_t MultiAccessQueue<T>::size()
{
  omp_set_lock(&headLock);
  omp_set_lock(&tailLock);
  size_t ret = oriQue.size();
  omp_unset_lock(&headLock);
  omp_unset_lock(&tailLock);
  return ret;
}

//It seems that if we want to lock the variable when using front or back(returning the value), the responbility is due to the user.

//front() return a copy of the head element, back() return a copy of the tail element
//front() should synchronize with pop(), and back() should synchronize with push(), thus we need 2 locks.
template<typename T>
T MultiAccessQueue<T>::front()
{
  omp_set_lock(&headLock);
  T ret = oriQue.front();
  omp_unset_lock(&headLock);
  return ret;
}

template<typename T>
T MultiAccessQueue<T>::back()
{
  omp_set_lock(&tailLock);
  T ret = oriQue.back();
  omp_unset_lock(&tailLock);
  return ret;
}

template<typename T>
void MultiAccessQueue<T>::push(T ele)
{
  omp_set_lock(&tailLock);
  oriQue.push(ele);
  omp_unset_lock(&tailLock);
}

template<typename T>
void MultiAccessQueue<T>::pop()
{
  omp_set_lock(&headLock);
  oriQue.pop();
  omp_unset_lock(&headLock);
}
