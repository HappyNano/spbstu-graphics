#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP

class FpsCounter final
{
 public:
  using this_t = FpsCounter;

  FpsCounter(double currentTime);
  FpsCounter(const this_t &) = delete;
  FpsCounter(this_t &&) = delete;
  ~FpsCounter() = default;

  void update(double currentTime);
  double get() const;

 private:
  double _previousTime;
  int _frameCount;

  double _last_fps;
};

#endif
