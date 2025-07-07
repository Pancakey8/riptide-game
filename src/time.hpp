#include <chrono>

using Time = std::chrono::time_point<std::chrono::steady_clock>;

double measure(Time t1, Time t2);