#include <queue>
#include <pthread.h>
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
  pthread_mutex_t headLock;
  pthread_mutex_t tailLock;
};


template <typename T>
MultiAccessQueue<T>::MultiAccessQueue()
{
  pthread_mutex_init(&headLock, nullptr);
  pthread_mutex_init(&tailLock, nullptr);
}

template <typename T>
MultiAccessQueue<T>::~MultiAccessQueue()
{
  pthread_mutex_destroy(&headLock);
  pthread_mutex_destroy(&tailLock);
}

template<typename T>
bool MultiAccessQueue<T>::empty()
{
  pthread_mutex_lock(&headLock);
  pthread_mutex_lock(&tailLock);
  bool ret = oriQue.empty();
  pthread_mutex_unlock(&headLock);
  pthread_mutex_unlock(&tailLock);
  return ret;
}

template<typename T>
size_t MultiAccessQueue<T>::size()
{
  pthread_mutex_lock(&headLock);
  pthread_mutex_lock(&tailLock);
  size_t ret = oriQue.size();
  pthread_mutex_unlock(&headLock);
  pthread_mutex_unlock(&tailLock);
  return ret;
}

//It seems that if we want to lock the variable when using front or back(returning the value), the responbility is due to the user.

//front() return a copy of the head element, back() return a copy of the tail element
//front() should synchronize with pop(), and back() should synchronize with push(), thus we need 2 locks.
template<typename T>
T MultiAccessQueue<T>::front()
{
  pthread_mutex_lock(&headLock);
  T ret = oriQue.front();
  pthread_mutex_unlock(&headLock);
  return ret;
}

template<typename T>
T MultiAccessQueue<T>::back()
{
  pthread_mutex_lock(&tailLock);
  T ret = oriQue.back();
  pthread_mutex_unlock(&tailLock);
  return ret;
}

template<typename T>
void MultiAccessQueue<T>::push(T ele)
{
  pthread_mutex_lock(&tailLock);
  oriQue.push(ele);
  pthread_mutex_unlock(&tailLock);
}

template<typename T>
void MultiAccessQueue<T>::pop()
{
  pthread_mutex_lock(&headLock);
  oriQue.pop();
  pthread_mutex_unlock(&headLock);
}
