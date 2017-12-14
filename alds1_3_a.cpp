#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>

#include <sstream>
#include <iostream>
#include <string>
using namespace std;

//#define DEBUG (1)

#ifdef DEBUG
#define DPRINTF(...)  printf("D(L%d) %s: ", __LINE__, __func__); printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif

template <class T>
class myStack {
public:
  myStack(int capacity);
  ~myStack();
  void push(T i);
  T pop();
  bool isEmpty();
  bool isFull();
  int size();

private:
  T *p_;
  int current_;
  int capacity_;
};

template <class T>
myStack<T>::myStack(int capacity) :
  capacity_(capacity)
{
  p_ = new T[capacity_];
  current_ = 0;
}

template <class T>
myStack<T>::~myStack()
{
  delete [] p_;
}

template <class T>
void myStack<T>::push(T c)
{
  if (isFull()) {
    cout << "Full!!" << endl;
    return;
  }
  p_[current_++] = c;
}

template <class T>
int myStack<T>::size()
{
  return current_;
}

template <class T>
T myStack<T>::pop()
{
  if (isEmpty()) {
    cout << "Empty!!" << endl;
    return -1;
  }
  return p_[--current_];
}

template <class T>
bool myStack<T>::isEmpty()
{
  return (current_ == 0);
}

template <class T>
bool myStack<T>::isFull()
{
  return (current_ == capacity_ - 1);
}

template <class T>
void proc_add(myStack<T> &s)
{
  int a = s.pop();
  int b = s.pop();
  s.push(a + b);
}

template <class T>
void proc_sub(myStack<T> &s)
{
  int a = s.pop();
  int b = s.pop();
  s.push(b - a);
}

template <class T>
void proc_mul(myStack<T> &s)
{
  int a = s.pop();
  int b = s.pop();
  s.push(a * b);
}

int main() {
  const int capacity = 100 + 99;
  myStack<int> st(capacity);

  string s;
#if 0
  while (cin >> s) {
    DPRINTF("%s\n", s.c_str());
    bool isOperator = (s == "+" || s == "-" || s == "*");
    if (isOperator) {
      continue;
    }
    st.push(stoi(s));
  }

  while (!st.isEmpty()) {
    cout << st.pop() << endl;
  }
#else
  while (cin >> s) {
    DPRINTF("%s\n", s.c_str());

    bool isOperator = (s == "+" || s == "-" || s == "*");
    bool isNumber = !isOperator;
    if (isNumber) {
      st.push(stoi(s));
      continue;
    }

    if (s == "+")      proc_add(st);
    else if (s == "-") proc_sub(st);
    else if (s == "*") proc_mul(st);
    else cout << "FAILED to parse : " << s << endl;
  }
#endif

  assert(st.size() == 1);
  cout << st.pop() << endl;
  assert(st.size() == 0);

  return 0;
}

