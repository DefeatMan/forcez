#include "forcez.h"

int main(int argc, char *argv[]) {
  forcez::Argp::Init(argc, argv);
  forcez::ExtractArchive();
  return 0;
}
