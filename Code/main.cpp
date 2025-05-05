#include "engine.h"

int main(int argc, char **argv) {
  Engine engine("Tutorial Window Name", 800, 600);

  if (!engine.Initialize()) {
    return 1;
  }

  engine.Run();

  return 0;
}
