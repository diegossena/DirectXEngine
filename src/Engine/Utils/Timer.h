#include <chrono>
using namespace std::chrono;
class Timer
{

private:
  system_clock::time_point oTime = std::chrono::system_clock::now();
  system_clock::time_point fTime;

public:
  float elapsed()
  {
    fTime = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsedTime = fTime - oTime;
    oTime = fTime;
    return elapsedTime.count();
  }
};