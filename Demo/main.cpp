#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "demo.hpp"

int main() {
  xev::IApp* app = new xev::DemoApp();

  // // Problem: currently using unique ptr will causing the following error when shutting
  // // down. Process finished with exit code 139 (interrupted by signal 11:SIGSEGV) Error
  // // This problem may be caused by the XevBuffer destructor being called after Device is
  // // destroyed then accessing invalid memory.
  // // It's a minor problem, using raw pointer for now, but need to be fixed in the future.
  // std::unique_ptr<xev::IApp> app = std::make_unique<xev::DemoApp>();

  try {
    app->run();
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
