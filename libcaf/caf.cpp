#include <caf/caf.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>

bool caf::load(std::string file) {
  std::ifstream fi(file, std::ios::binary);

  if(!fi.is_open()) {
    std::cout<<"Unable to open file '"<<file<<"'!\n";
    fi.close();
    return false;
  }

  char data[4];
  fi.read(data, 3);
  data[3] = 0;

  if(strcmp(data, "CAF") != 0) {
    std::cout<<"Invalid CAF Header!\n";
    fi.close();
    return false;
  }

  auto readByte  = [&]() -> uint8_t  {
    uint8_t  v = 0;
    fi.read((char*)&v, 1);
    return v;
  };
  auto readShort = [&]() -> uint16_t {
    uint16_t v = 0;
    fi.read((char*)&v, 2);
    return v;
  };
  auto readLong = [&]() -> uint32_t {
    uint32_t v = 0;
    fi.read((char*)&v, 4);
    return v;
  };
  auto readSuper = [&]() -> uint64_t {
    uint64_t v = 0;
    fi.read((char*)&v, 8);
    return v;
  };
  auto readString = [&]() -> std::string {
    std::string v;
    char c;
    fi.get(c);
    while(c) {
      v += c;
      fi.get(c);
    }
    return v;
  };

  rootinfo.s_caf_major = readShort();
  rootinfo.s_caf_minor = readShort();
  rootinfo.l_revision  = readLong ();
  uint64_t ptr_item    = readSuper();
  rootinfo.s_root      = readString();

  if(ptr_item == 0) {
    std::cout<<"CAF File empty (No Lumps)\n";
    fi.close();
    return true;
  }

  bool itemExists = true;

  fi.seekg(ptr_item, fi.beg);

  while(itemExists) {
    lump l;

    l.b_flag_a = readByte();
    l.b_flag_b = readByte();
    l.s_lump_size     = readSuper();
    l.l_revision      = readLong ();
    uint64_t ptr_lump = readSuper();
    ptr_item          = readSuper();

    l.s_name = readString();
    l.s_path = readString();
    l.s_type = readString();

    lumps.push_back(l);
    if((l.b_flag_a>>7) != 1) {
      itemExists = false;
    }

    fi.seekg(ptr_item, fi.beg);
  }

  fi.close();
  return true;
}

void caf::dumpTree() {
  std::cout<<"_________________________________________\n";
  std::cout<<"| Dumping lump tree for CAF Root '"<<rootinfo.s_root<<"'\n";
  std::cout<<"|- CAF Version    : ["<<rootinfo.s_caf_major<<", "<<rootinfo.s_caf_minor<<"]\n";
  std::cout<<"|- Asset Revision : "<<rootinfo.l_revision<<"\n";
  std::cout<<"|- Asset path     : '"<<rootinfo.s_root<<"'\n";

  for(const auto& l : lumps) {
    std::cout<<"|______________________________________\n";
    std::cout<<"|- Lump -|- name     : '"<<l.s_name<<"'\n";
    std::cout<<"|        |- Path     : '"<<l.s_path<<"'\n";
    std::cout<<"|        |- Type     : '"<<l.s_type<<"'\n";
    std::cout<<"|        |- Revision : "<<l.l_revision<<"'\n";
    std::cout<<"|        |- Data -|- Pointer : "<<(void*)l.c_lump_data<<"\n";
    std::cout<<"|                 |- Size    : "<<l.s_lump_size<<"\n";
  }

  std::cout<<"|________________________________________\n";
}
