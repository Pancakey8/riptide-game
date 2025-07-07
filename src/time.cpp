#include "time.hpp"

double measure(Time t1, Time t2) {
  std::chrono::duration<double> dur = t1 - t2;
  return dur.count();
}