#pragma once

#include <vector>
#include <string>

struct lump {
public:
  uint64_t s_lump_size;
  char*    c_lump_data;
  uint32_t l_revision;
  std::string s_name;
  std::string s_path;
  std::string s_type;
  uint8_t  b_flag_a;
  uint8_t  b_flag_b;
};

class caf {
public:
  struct {
  public:
    uint16_t s_caf_major;
    uint16_t s_caf_minor;
    uint32_t l_revision;
    std::string s_root;
  } rootinfo;

  std::vector<lump> lumps;

  bool load(std::string file);
  void dumpTree();
};
