#include "lab/fps_counter.hpp"

FpsCounter::FpsCounter(double currentTime):
  _previousTime{ 0.0 },
  _frameCount{ 0 },
  _last_fps{ 0 }
{}

void FpsCounter::update(double currentTime)
{
  ++_frameCount;

  if (currentTime - _previousTime >= 1.0)
  {
    _last_fps = _frameCount / (currentTime - _previousTime);

    _previousTime = currentTime;
    _frameCount = 0;
  }
}

double FpsCounter::get() const
{
  return _last_fps;
}
