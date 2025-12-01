#pragma once
#include "core/Object.hpp"
#include <array>
#include <mutex>
namespace cube::core {
template <class T, size_t N = 1>
  requires requires() { N > 1; }
class Channel : public Object {
private:
  std::array<T, N> _buffer;
  std::mutex _mutex;
  std::mutex _mutex_send;
  std::mutex _mutex_recv;
  std::condition_variable _condition_send;
  std::condition_variable _condition_recv;
  size_t _index = 0;
  size_t _length = 0;

public:
  void send(T &&value) {
    while (_length == N) {
      std::unique_lock<std::mutex> lock(_mutex_send);
      _condition_send.wait(lock);
    }
    {
      std::unique_lock<std::mutex> lock(_mutex);
      size_t idx = _index + _length;
      if (idx >= N) {
        idx -= N;
      }
      _buffer[idx] = std::forward<T>(value);
      _length++;
    }
    _condition_recv.notify_one();
  }
  void send(const T &value) {
    while (_length == N) {
      std::unique_lock<std::mutex> lock(_mutex_send);
      _condition_send.wait(lock);
    }
    {
      std::unique_lock<std::mutex> lock(_mutex);
      size_t idx = _index + _length;
      if (idx >= N) {
        idx -= N;
      }
      _buffer[idx] = std::forward<T>(value);
      _length++;
    }
    _condition_recv.notify_one();
  }
  T recv() {
    while (_length == 0) {
      std::unique_lock<std::mutex> lock(_mutex_recv);
      _condition_recv.wait(lock);
    }
    T res;
    {
      std::unique_lock<std::mutex> lock(_mutex);
      res = std::move(_buffer[_index]);
      _index++;
      if (_index == N) {
        _index = 0;
      }
      _length--;
    }
    _condition_send.notify_one();
    return res;
  }
};
} // namespace cube::core