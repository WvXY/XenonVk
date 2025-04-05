#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "demo.hpp"

int main() {
  std::unique_ptr<xev::IApp> app = std::make_unique<xev::DemoApp>();

  try {
    app->run();
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
