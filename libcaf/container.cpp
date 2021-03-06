#include <caf/container.hpp>

#include <iostream>
#include <fstream>
#include <cstring>
#include <initializer_list>

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

template<typename T>
void writeData(std::ofstream& stream, uint64_t& ptr, const std::initializer_list<T> il_data) {
  for(const auto& data : il_data) {
    T v = data;
    stream.write((const char*)&v, sizeof(T));
    ptr += sizeof(T);
  }
}

void writeString(std::ofstream& stream, uint64_t& ptr, const std::initializer_list<std::string> il_data) {
  for(const auto& data : il_data) {
    stream.write(data.c_str(), data.length());
    writeData<uint8_t>(stream, ptr, {0});
    ptr += data.length();
  }
}

bool caf::container::write(const std::string& file) {
  std::ofstream fi(file, std::ios::binary);

  if(!fi.is_open()) {
    std::cout<<"[CAF] Could not open file '"<<file<<"' for writing!\n";
    fi.close();
    return false;
  }

  uint64_t ptr = 0;

  writeData<char>(fi, ptr, {'C', 'A', 'F'});

  writeData<uint16_t>(fi, ptr, {version.major, version.minor});
  writeData<uint32_t>(fi, ptr, {version.revision});
  writeData<uint64_t>(fi, ptr, {ptr + root.length() + 9});
  writeString(fi, ptr, {root});

  for(unsigned i = 0; i < items.size(); ++i) {
    caf::lumpitem li = items[i];

    if(i < items.size() - 1) { // If the next item exists, set the flag, otherwise, remove it
      li.flags.a |= 0b10000000;
    } else {
      li.flags.a &= 0b01111111;
    }

    if(li.size <= 0) {
      li.flags.a |= 0b01000000;
    } else {
      li.flags.a &= 0b10111111;
    }

    writeData<uint8_t> (fi, ptr, {li.flags.a, li.flags.b});
    writeData<uint64_t>(fi, ptr, {li.size});
    writeData<uint32_t>(fi, ptr, {li.revision});

    uint64_t ptr_l = li.name.length() + li.path.length() + li.type.length() + 3;

    writeData<uint64_t>(fi, ptr, {ptr + ptr_l + 16,
                                  ptr + ptr_l + 8 + li.size});
    writeString(fi, ptr, {li.name, li.path, li.type});

    fi.write(li.data, li.size);
    ptr += li.size;
  }

  fi.close();
  return true;
}

void caf::container::dump_tree() const {
  std::cout<<"┌────────────────────────────────────────────────────\n"
           <<"│\n"
           <<"├╴ CAF Version    : "<<version.major<<"."<<version.minor<<"\n"
           <<"├╴ Asset Revision : "<<version.revision<<"\n"
           <<"├╴ Asset path     : '"<<root<<"'\n"
           <<"│\n";

  for(const auto& li : items) {
    std::cout<<"├╼ Lump ╾┬╴ '"<<li.path<<li.name<<"'\n"
             <<"│        ├╴ Name     : '"<<li.name<<"'\n"
             <<"│        ├╴ Path     : '"<<li.path<<"'\n"
             <<"│        ├╴ Type     : '"<<li.type<<"'\n"
             <<"│        ├╴ Revision : '"<<li.revision<<"'\n"
             <<"│        └╼ Data ╾┬╴ Pointer : "<<(void*)li.data<<"\n"
             <<"│                 └╴ Size    : "<<li.size<<"\n"
             <<"│\n";
  }

  std::cout<<"└────────────────────────────────────────────────────\n";
}

void caf::container::dump_item(const caf::lumpitem& li) const {
  std::cout<<"┌──────────────────────────────────────────────\n"
           <<"│\n"
           <<"├╴ Name     : '"<<li.name<<"'\n"
           <<"├╴ Path     : '"<<li.path<<"'\n"
           <<"├╴ Type     : '"<<li.type<<"'\n"
           <<"├╴ Revision : '"<<li.revision<<"'\n"
           <<"├╴ Data ╾┬╴ Pointer : '"<<(void*)li.data<<"'\n"
           <<"│        └╼ Size    : '"<<li.size<<"'\n"
           <<"│\n"
           <<"└──────────────────────────────────────────────\n";
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

  std::cout<<"[CAF] Cannot show lump, type '"<<li.type<<"' is unsupported\n";
}
