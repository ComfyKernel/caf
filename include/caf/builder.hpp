#ifndef CAF_BUILDER_HPP
#define CAF_BUILDER_HPP

#include "types.hpp"

namespace caf {
  class builder {
  protected:
    caf _caf;
    
  public:
    const caf& getCaf();

    builder();
    
  };
}

#endif
