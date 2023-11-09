#ifndef TIMER_HPP
#define TIMER_HPP
#include <chrono>
#include <iostream>
#include <memory>
#include <string>

class Timer {
 public:
  Timer(std::string name) : m_name{name} {
    m_startTimepoint = std::chrono::high_resolution_clock::now();
  }

  ~Timer() { Stop(); }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
  void Stop() {
    auto endTimepoint = std::chrono::high_resolution_clock::now();

    auto start =
        std::chrono::time_point_cast<std::chrono::nanoseconds>(m_startTimepoint)
            .time_since_epoch()
            .count();
    auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimepoint)
                   .time_since_epoch()
                   .count();

    auto duration = end - start;

    std::cout << "timer name: " << m_name << " duration: " << duration << '\n';
  }

  std::string m_name;
};
#endif