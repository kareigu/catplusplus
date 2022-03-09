#include <Application/core.h>
#include <utility>


int main(int argc, char** argv) {
  int result = Application::run(std::move(argc), std::move(argv));

  return result;
}
