#ifndef CAF_TYPES_HPP
#define CAF_TYPES_HPP

#include <cstdint>
#include <string>

namespace caf {
  struct flags {
    bool unused7 : 1; // BYTE 0
    bool unused6 : 1;
    bool unused5 : 1;
    bool unused4 : 1;
    bool file    : 1;
    bool empty   : 1;
    bool ignore  : 1;
    bool item    : 1;
    
    bool type7 : 1; // BYTE 1
    bool type6 : 1;
    bool type5 : 1;
    bool type4 : 1;
    bool type3 : 1;
    bool type2 : 1;
    bool type1 : 1;
  };
  
  struct lumpitem {
    union {
      uint16_t all;
      flags bits;
    } flags;
    
    uint32_t ptr_lump;    
    uint16_t ver_major;
    uint16_t ver_minor;
    uint32_t ptr_lumpitem;
    std::string name;
    std::string path;
    std::string type;
  };

  class caf {
    uint16_t ver_major;
    uint16_t ver_minor;
    uint32_t revision;
    uint32_t ptr_lumpitem;
    std::string root;
  };
}

#endif
