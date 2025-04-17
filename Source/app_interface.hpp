#pragma once

namespace xev {

class IApp {
public:
  virtual ~IApp() = default;
  virtual void run() = 0;

private:
  virtual void load() = 0;
  virtual void tick(float) {}
  virtual void fixedTick(float) = 0;
};

} // namespace xev
