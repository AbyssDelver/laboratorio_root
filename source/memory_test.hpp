#ifndef MEMORY_TEST_HPP
#define MEMORY_TEST_HPP
#include <iostream>

class memory_tracker {
 public:
  int allocated = 0;
  int freed = 0;

  int current_usage() { return (allocated - freed); }


  void reset() {
    allocated = 0;
    freed = 0;
  }

  void print(){
    std::cout << "memory usage" << this->current_usage() << '\n';
  }
};

inline memory_tracker tracker;

void* operator new(size_t size) {
  tracker.allocated += 1;
  return malloc(size);
}

// operator overload of delete
void operator delete(void* memory) noexcept {
  if (memory) {
    tracker.freed += 1;
    free(memory);
  }
}

#endif