#include "first_app.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>


int main() {
  lge::FirstApp app{};

  try {
    app.run();
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}