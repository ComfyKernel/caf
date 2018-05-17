#include <caf/caf.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>

bool caf::load(const std::string& file) {
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

    if(l.s_path.back() != '/')
      l.s_path += '/';

    if((l.b_flag_a>>7) != 1) {
      itemExists = false;
    }

    fi.seekg(ptr_lump, fi.beg);

    char* buf = (char*)std::malloc(l.s_lump_size);
    fi.read(buf, l.s_lump_size);
    l.c_lump_data = buf;

    fi.seekg(ptr_item, fi.beg);

    lumps.push_back(l);
  }

  fi.close();
  return true;
}

void caf::dump_tree() const {
  std::cout<<"_________________________________________\n";
  std::cout<<"| Dumping lump tree for CAF Root '"<<rootinfo.s_root<<"'\n";
  std::cout<<"|- CAF Version    : ["<<rootinfo.s_caf_major<<", "<<rootinfo.s_caf_minor<<"]\n";
  std::cout<<"|- Asset Revision : "<<rootinfo.l_revision<<"\n";
  std::cout<<"|- Asset path     : '"<<rootinfo.s_root<<"'\n";

  for(const auto& l : lumps) {
    std::cout<<"|______________________________________\n";
    std::cout<<"|- Lump -|- '"<<l.s_path<<l.s_name<<"'";
    std::cout<<"|        |- Name     : '"<<l.s_name<<"'\n";
    std::cout<<"|        |- Path     : '"<<l.s_path<<"'\n";
    std::cout<<"|        |- Type     : '"<<l.s_type<<"'\n";
    std::cout<<"|        |- Revision : "<<l.l_revision<<"'\n";
    std::cout<<"|        |- Data -|- Pointer : "<<(void*)l.c_lump_data<<"\n";
    std::cout<<"|                 |- Size    : "<<l.s_lump_size<<"\n";
/*    std::string prefix="text/";
    if(!l.s_type.compare(0, prefix.size(), prefix) && l.c_lump_data != nullptr) {
      unsigned int i = 0;
      char* c = l.c_lump_data;
      while(i < l.s_lump_size) {
        std::cout<<(*c);
        i++;
        c++;
      }
      std::cout<<"\n";
    }*/
  }

  std::cout<<"|________________________________________\n";
}

void caf::dump_lump(const lump& l) const {
  std::cout<<"| Dumping info for lump : '"<<l.s_path<<l.s_name<<"'\n";

  std::cout<<"|- Name     : '"<<l.s_name<<"'\n";
  std::cout<<"|- Path     : '"<<l.s_path<<"'\n";
  std::cout<<"|- Type     : '"<<l.s_type<<"'\n";
  std::cout<<"|- Revision : '"<<l.l_revision<<"'\n";
  std::cout<<"|- Data -|- Pointer : '"<<(void*)l.c_lump_data<<"'\n";
  std::cout<<"         |- Size    : '"<<l.s_lump_size<<"'\n";
}

lump _l_not_found;

lump& caf::find_lump(const std::string& l) {
  for(auto& li : lumps) {
    if((li.s_path + li.s_name) == l) {
      return li;
    }
  }

  return _l_not_found;
}

void caf::show_lump(const lump& l) const {
  if(l.s_type.find("/text") != std::string::npos) {
    unsigned int i = 0;
    char* c = l.c_lump_data;
    while(i < l.s_lump_size) {
      std::cout<<(*c);
      i++;
      c++;
    }
    std::cout<<"\n";
    return;
  }

  std::cout<<"Cannot show lump, type '"<<l.s_type<<"' Unsupported\n";
}
