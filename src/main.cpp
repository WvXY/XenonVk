#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "first_app.hpp"

int main() {
  xev::FirstApp app{};

  try {
    app.run();
  }
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
