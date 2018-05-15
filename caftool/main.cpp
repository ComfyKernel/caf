#include <iostream>
#include <caf/caf.h>

int main(int argc, char *argv[]) {
  if(argc < 2) {
    std::cout<<"Please provide a filename for the reader\n";
    return -1;
  }

  caf c;

  c.load(argv[1]);

  c.dumpTree();

  return 0;
}
