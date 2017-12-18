#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

#define RINGBUF (1)
//#define DEBUG (1)

#ifdef DEBUG
#define DPRINTF(...)  printf("D(L%d) %s: ", __LINE__, __func__); printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif

class Process {
public:
  Process() {};
  Process(string name, int workload);
  ~Process() {};
  int run(int time);
  bool isDone();
  inline string name() { return name_; }
  inline int workload() { return workload_; }
private:
  string name_;
  int workload_;
};

Process::Process(string name, int workload) :
  name_(name),
  workload_(workload) {}

int Process::run(int time)
{
  if (workload_ > time) {
    workload_ -= time;
    return time;
  } else {
    int consume = workload_;
    workload_ -= consume;
    return consume;
  }
}

bool Process::isDone()
{
  return (workload_ <= 0);
}


#ifdef RINGBUF
// Ring buffer
// One of issues is that it needs to determine variable data when construting Queue
// Since not appropriate to design queue with ring buffer...
template <class T>
class Queue {
public:
  Queue(int capacity);
  ~Queue();
  void push(T d);
  void pop();
  T front();
  bool empty();
  bool full();
private:
  int capacity_;
#ifdef DEBUG
public:
#else //DEBUG
private:
#endif //DEBUG
  T* d_;
  int put_, get_;
};


template <class T>
Queue<T>::Queue(int capacity) :
  capacity_(capacity),
  put_(0),
  get_(0)
{
  d_ = new T[capacity_];
}

template <class T>
Queue<T>::~Queue()
{
  delete [] d_;
}

template <class T>
void Queue<T>::push(T d)
{
  if (full()) {
    cout << "FULL!!" << endl;
    return ;
  }
  d_[put_] = d;
  put_ = (put_ + 1 ) % capacity_;
}

template <class T>
void Queue<T>::pop()
{
  if (empty()) {
    cout << "EMPTY!!" << endl;
    return ;
  }
  get_ = (get_ + 1 ) % capacity_;
}

template <class T>
T Queue<T>::front()
{
  return d_[get_];
}

template <class T>
bool Queue<T>::full()
{
  return get_ == ((put_ + 1) % capacity_);
}

template <class T>
bool Queue<T>::empty()
{
  return (put_ == get_);
}
#endif //RINGBUF


#ifdef LINKED_LIST
template <class T>
struct Node {
  T d_;
  Node<T>* next_;
};

template <class T>
class Queue {
public:
  Queue();
  ~Queue();
  void push(T d);
  void pop();
  T front();
  bool empty();
  void print();
#ifdef DEBUG
public:
#else
private:
#endif
  Node<T>* head_;
};

template <class T>
Queue<T>::Queue()
{
  head_ = nullptr;
}

template <class T>
Queue<T>::~Queue()
{
  Node<T>* current = head_;
  while (current != nullptr) {
    Node<T>* next = current->next_;
    delete current;
    current = next;
  }
}

template <class T>
void Queue<T>::push(T d)
{
  Node<T>* tail = new Node<T>;
  tail->d_ = d;
  tail->next_ = nullptr;

  if (head_ == nullptr) {
    head_ = tail;
    return ;
  }

  Node<T>* node = head_;
  while (node->next_ != nullptr)
    node = node->next_;
  node->next_ = tail;
}

template <class T>
void Queue<T>::pop()
{
  Node<T>* t = head_;
  head_ = head_->next_;
  delete t;
}

template <class T>
T Queue<T>::front()
{
  return head_->d_;
}

template <class T>
bool Queue<T>::empty()
{
  return (head_ == nullptr);
}
#endif //LINKED_LIST


int main() {
  int numOfProc, quantum;
  cin >> numOfProc >> quantum;
  DPRINTF("numOfProc : %d\n", numOfProc);
  DPRINTF("quantum: %d\n", quantum);

#ifdef RINGBUF
  const int MAX_NUM_OF_PROC = 100000 + 1;
  Queue<Process> queue(MAX_NUM_OF_PROC);
#endif //RINGBUF
#ifdef LINKED_LIST
  Queue<Process> queue;
#endif // LINKED_LIST

  for (int i = 0; i < numOfProc; i++) {
    string proc;
    int workload;
    cin >> proc >> workload;
    DPRINTF("process %s, workload %d\n", proc.c_str(), workload);
    queue.push(Process(proc, workload));
  }

#ifdef DEBUG
#ifdef RINGBUF
  {
    if (queue.put_ > queue.get_) {
      for (int i = queue.get_; i < queue.put_; i++) {
        Process *proc = &queue.d_[i];
        DPRINTF("process %s, workload %d\n",
            proc->name().c_str(), proc->workload());
      }
    }
  }
#endif //RINGBUF
#ifdef LINKED_LIST
  {
    DPRINTF("");
    Node<Process> *node = queue.head_;
    while (node != nullptr) {
      if (node != queue.head_) cout << " -> ";
      printf("(%s, %d)", node->d_.name().c_str(), node->d_.workload());
      node = node->next_;
    }
    cout << endl;
  }
#endif //LINKED_LIST
#endif //DEBUG

  int time = 0;
  while (!queue.empty()) {
    Process proc = queue.front();
    queue.pop();
    time += proc.run(quantum);
    DPRINTF("TIME: %d, PROC(%s %d)\n", time, proc.name().c_str(), proc.workload());
    if (proc.isDone()) {
      printf("%s %d\n", proc.name().c_str(), time);
    } else {
      queue.push(proc);
    }
  }

  return 0;
}

