#include <caf/lumpitem.hpp>
#include <cstring>

// Constructors / Destructors
//         ()        : Does nothing.
// (const lumpitem&) : If data exists it will delete it and replace it.
//         ~         : Deletes the data.

caf::lumpitem::lumpitem() { }
caf::lumpitem::lumpitem(const lumpitem& l)
  : size(l.size), revision(l.revision), flags(l.flags),
    name(l.name), type(l.type), path(l.path) {
  if(data != nullptr)
    delete data;

  data = static_cast<char*>(malloc(l.size));
  std::memcpy(data, l.data, l.size);
}
caf::lumpitem::~lumpitem() {
  delete data;
}
