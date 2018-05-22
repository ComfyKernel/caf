#pragma once

#include <vector>
#include "lumpitem.hpp"

namespace caf {
  class container {
  protected:

  public:
    container();
    container(const container&);

    // Functions
    // getItem (const std::string* item) : If the item exists it returns it
    //
    // findItemByName (const std::string&) : Returns the first item with name
    //
    // findItemsByName (const std::string&) : Finds all lumps by a name and it
    //                                        returns them in a vector. Errors
    //                                        return an empty vector.
    //
    // findItemsInPath (const std::string&) : Gets all lumps in a path and it
    //                                        returns them in a vector. Errors
    //                                        return an empty vector.
    //
    // addItem (const lumpitem& lump) : Adds an item to the container, if the
    //                                  item already exists it will not be added
    //
    // addItems (const std::vector<lumpitem>&) : Adds a list of items to the
    //                                           container, if an item already
    //                                           exists it will be ignored

    caf::lumpitem&              getItem        (const std::string& item = "");
    caf::lumpitem&              findItemByName (const std::string& name = "");
    std::vector<caf::lumpitem*> findItemsByName(const std::string& name = "");
    std::vector<caf::lumpitem*> findItemsInPath(const std::string& path = "");

    unsigned addItem (const caf::lumpitem& lump);
    unsigned addItems(const std::vector<caf::lumpitem>& vlumps);

    bool load (const std::string& file = "");
    bool write(const std::string& file = "");

    void dump_tree() const;
    void dump_item(const caf::lumpitem& l) const;
    void show_item(const caf::lumpitem& l) const;

    // Variables
    // Root : Used in figuring out where the lumps go relative to other
    //        containers. (Multiple containers that point to the same root)
    //
    // // Version
    // // Major & Minor : Used in determining what model of CAF to use
    // // Revision      : Used for validation

    std::string root;
    struct {
      uint16_t major;
      uint16_t minor;

      uint32_t revision;
    } version;
    std::vector<lumpitem> items;
  };
}
