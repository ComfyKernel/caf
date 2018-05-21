#include <caf/container.hpp>
#include <iostream>
#include <fstream>
#include <cstring>

caf::lumpitem _container_emptyitem;

caf::container::container() { }
caf::container::container(const caf::container& cnt)
  : version(cnt.version), root(cnt.root), items(cnt.items) { }

caf::lumpitem& caf::container::getItem(const std::string& item) {
  for(auto& li : items) {
    if((li.path + li.name) == item)
      return li;
  }

  return _container_emptyitem;
}

caf::lumpitem& caf::container::findItemByName(const std::string& name) {
  for(auto& li : items) {
    if(li.name == name)
      return li;
  }

  return _container_emptyitem;
}

std::vector<caf::lumpitem*> caf::container::findItemsByName(const std::string& name) {
  std::vector<caf::lumpitem*> _lumps;
  for(auto& li : items) {
    if(li.name == name)
      _lumps.push_back(&li);
  }

  return _lumps;
}

std::vector<caf::lumpitem*> caf::container::findItemsInPath(const std::string& path) {
  std::string pname;
  if(path.back() != '/') {
    pname += '/';
  }

  std::vector<caf::lumpitem*> _lumps;

  for(auto& li : items) {
    if(li.path.compare(0, pname.length(), pname) == 0)
      _lumps.push_back(&li);
  }

  return _lumps;
}

unsigned caf::container::addItem(const caf::lumpitem& lump) {
  for(auto& li : items) {
    if((li.path + li.name) == (lump.path + lump.name)) {
      return 0;
    }
  }

  items.push_back(lump);
  return 1;
}

unsigned caf::container::addItems(const std::vector<caf::lumpitem>& vlumps) {
  for(auto& li : vlumps) {
    addItem(li);
  }

  return 1;
}

template<typename T>
T readData(std::ifstream& stream) {
  T v = 0;
  stream.read((char*)&v, sizeof(T));
  return v;
}

std::string readString(std::ifstream& stream) {
  std::string v;
  char c;
  stream.get(c);
  while(c) {
    v += c;
    stream.get(c);
  }
  return v;
}

bool caf::container::load(const std::string& fname) {
  std::ifstream fi(fname, std::ios::binary);

  if(!fi.is_open()) {
    std::cout<<"[CAF] Unable to open file '"<<fname<<"'\n";
    fi.close();
    return false;
  }

  char data[4];
  fi.read(data, 3);
  data[3] = 0;

  if(strcmp(data, "CAF") != 0) {
    std::cout<<"[CAF] Invalid header in file '"<<fname<<"'\n";
    fi.close();
    return false;
  }

  version.major    = readData<uint16_t>(fi);
  version.minor    = readData<uint16_t>(fi);
  version.revision = readData<uint32_t>(fi);
  uint16_t p_item  = readData<uint64_t>(fi);
  root             = readString(fi);

  if(p_item == 0) {
    std::cout<<"[CAF] lumpitem pointer empty, assuming caf is empty\n";
    fi.close();
    return true;
  }

  fi.seekg(p_item, fi.beg);
  bool item = true;

  while(item) {
    caf::lumpitem _vli;
    items.push_back(_vli);
    caf::lumpitem& li = items.back();

    li.flags.a      = readData<uint8_t>(fi);
    li.flags.b      = readData<uint8_t>(fi);
    li.size         = readData<uint64_t>(fi);
    li.revision     = readData<uint32_t>(fi);
    uint64_t p_lump = readData<uint64_t>(fi);
    p_item          = readData<uint64_t>(fi);

    li.name = readString(fi);
    li.path = readString(fi);
    li.type = readString(fi);

    if(li.path.back() != '/')
      li.path += '/';

    if(((li.flags.a >> 7) & 1) != 1 || p_lump == 0)
      item = false;

    fi.seekg(p_lump, fi.beg);

    char* buf = static_cast<char*>(std::malloc(li.size));
    fi.read(buf, li.size);
    li.data = buf;

    fi.seekg(p_item, fi.beg);
  }

  fi.close();
  return true;
}

void caf::container::dump_tree() const {
  std::cout<<"┌────────────────────────────────────────────────────\n";
  std::cout<<"│\n";
  std::cout<<"├╴ CAF Version    : ["<<version.major<<", "<<version.minor<<"]\n";
  std::cout<<"├╴ Asset Revision : "<<version.revision<<"\n";
  std::cout<<"├╴ Asset path     : '"<<root<<"'\n";
  std::cout<<"│\n";

  for(const auto& li : items) {
    std::cout<<"├╼ Lump ╾┬╴ '"<<li.path<<li.name<<"'\n";
    std::cout<<"│        ├╴ Name     : '"<<li.name<<"'\n";
    std::cout<<"│        ├╴ Path     : '"<<li.path<<"'\n";
    std::cout<<"│        ├╴ Type     : '"<<li.type<<"'\n";
    std::cout<<"│        ├╴ Revision : '"<<li.revision<<"'\n";
    std::cout<<"│        └╼ Data ╾┬╴ Pointer : "<<(void*)li.data<<"\n";
    std::cout<<"│                 └╴ Size    : "<<li.size<<"\n";
    std::cout<<"│\n";
  }

  std::cout<<"└────────────────────────────────────────────────────\n";
}

void caf::container::dump_item(const caf::lumpitem& li) const {
  std::cout<<"┌──────────────────────────────────────────────\n";
  std::cout<<"│\n";
  std::cout<<"├╴ Name     : '"<<li.name<<"'\n";
  std::cout<<"├╴ Path     : '"<<li.path<<"'\n";
  std::cout<<"├╴ Type     : '"<<li.type<<"'\n";
  std::cout<<"├╴ Revision : '"<<li.revision<<"'\n";
  std::cout<<"├╴ Data ╾┬╴ Pointer : '"<<(void*)li.data<<"'\n";
  std::cout<<"│        └╼ Size    : '"<<li.size<<"'\n";
  std::cout<<"│\n";
  std::cout<<"└──────────────────────────────────────────────\n";
}

void caf::container::show_item(const caf::lumpitem& li) const {
  std::string typestr = "/text/";
  if(li.type.compare(0, typestr.length(), typestr) != 0) {
    unsigned int i = 0;
    char* c = li.data;
    while(i < li.size) {
      std::cout<<(*c);
      i++;
      c++;
    }
    std::cout<<"\n";
    return;
  }

  std::cout<<"[CAF] Cannot show lump, type '"<<li.type<<"' Unsupported\n";
}
