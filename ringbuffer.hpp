#pragma once
#include <cstdint>
#include <exception>
#include <cmath>

struct BufferEmpty : public std::exception {
  const char * what() const throw() {
    return "Attempting to pop on empty buffer.";
  }
};

struct OutOfBounds : public std::exception {
  const char * what() const throw() {
    return "Requested Data is Out of Bounds";
  }
};

template <typename T>
class Ringbuffer {
private:
  T * buf;

  T * rptr;
  T * wptr;


public:
  Ringbuffer<T>(uint32_t size) : buf(new T[size]) { wptr = rptr = buf; };

  void push(T element);
  T pop();
  T goBackN(uint32_t n);
  uint32_t size();
  void clear();
  bool empty();
};

template <typename T>
void Ringbuffer<T>::push(T element) {
  *wptr = element;
  if(wptr == buf + sizeof(buf)) {
    wptr = buf;
  }
  else {
    wptr = (wptr + sizeof(T));
  }
}

template <typename T>
T Ringbuffer<T>::pop() {
  if (rptr == wptr) {
    throw BufferEmpty();
  }
  T element = *rptr;
  if(rptr == buf + sizeof(buf)) {
    rptr = buf;
  }
  else {
    rptr = (rptr + sizeof(T));
  }
  return element;
}

template <typename T>
T Ringbuffer<T>::goBackN(uint32_t N) {
  T * index = rptr - (sizeof(T) * (N));
  if(index < buf) {
    index = (buf + sizeof(buf)) - (buf - index);
  }
  if(index >= wptr || N >= (sizeof(buf) / sizeof(T)) - 1) {
    throw OutOfBounds();
  }
  return *index;
}

template <typename T>
uint32_t Ringbuffer<T>::size() {
  std::abs(rptr - wptr) / sizeof(T);
}

template <typename T>
void Ringbuffer<T>::clear() {
  rptr = wptr;
}

template <typename T>
bool Ringbuffer<T>::empty() {
  return size() > 0;
}
