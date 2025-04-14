#include "Code/engine.h"

int main(int argc, char **argv) {
  // instantiate the engine, with window name w and h.
  Engine *engine = new Engine("PA3", 800, 600);

  // if initialization fails, print error message and clean up.
  if (!engine->Initialize()) {
    printf("The engine failed to start.\n");
    delete engine;
    engine = nullptr;
    return 1;
  }

  // run the engine.
  engine->Run();

  // delete the engine and set to null.
  delete engine;
  engine = nullptr;

  return 0;
}