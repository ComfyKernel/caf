#include <iostream>
#include <cstring>
#include <utility>

#include <caf/container.hpp>
#include <tinyxml.h>

bool cmd_tree(int, char**);
bool cmd_lump(int, char**);
bool cmd_show(int, char**);

typedef bool (*f_command)(int, char**);
typedef std::pair<f_command, const char*> p_command;

p_command commands[] = {
  p_command(cmd_tree, "tree"),
  p_command(cmd_lump, "lump"),
  p_command(cmd_show, "show")
};

int main(int argc, char *argv[]) {
  if(argc < 3) {
    std::cout<<"Please provide a command / filename for the tool\n";
    return -1;
  }

  for(const auto& c : commands) {
    if(strcmp(c.second, argv[2]) == 0) {
      return !c.first(argc, argv);
    }
  }

  std::cout<<"Command not found '"<<argv[2]<<"'\n";

  return 0;
}

bool cmd_tree(int argc, char *argv[]) {
  caf::container c;
  c.load(argv[1]);
  c.dump_tree();
  return true;
}

bool cmd_lump(int argc, char *argv[]) {
  if(argc < 4) {
    std::cout<<"Command 'lump' needs more arguments!\n";
    return false;
  }

  caf::container c;
  c.load(argv[1]);
  c.dump_item(c.getItem(argv[3]));
  return true;
}

bool cmd_show(int argc, char *argv[]) {
  if(argc < 4) {
    std::cout<<"Command 'show' needs more arguments!\n";
    return false;
  }

  caf::container c;
  c.load(argv[1]);
  c.show_item(c.getItem(argv[3]));
  return true;
}
