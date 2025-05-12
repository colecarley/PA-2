#include "engine.h"

int main(int argc, char **argv) {
  Engine engine("Solar System", 800, 600);

  if (!engine.Initialize()) {
    return 1;
  }

  engine.Run();

  return 0;
}
