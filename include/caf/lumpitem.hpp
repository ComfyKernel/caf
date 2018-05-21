#pragma once

#include <string>

namespace caf {
  class lumpitem {
  private:
  public:
    lumpitem();
    lumpitem(const lumpitem& l);
    ~lumpitem();

    // Variables
    // name : Used in identification
    // type : Used to tell what the item is, similar to mime types
    // path : Used with the name to produce a valid id
    //
    // revision : Keeps track of asset changes
    //
    // size : keeps the size of the stored data
    // data : a lump of data stored in an unsigned char*
    //
    // flags : a big mess that is used to signal a reader / writer what to do

    std::string name, type, path;
    uint32_t       revision = 0;
    uint64_t       size     = 0;
    char* data     = nullptr;

    struct {
      uint8_t a;
      uint8_t b;
    } flags;
  };
}