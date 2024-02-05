#ifndef ITEM_HPP_
#define ITEM_HPP_

#include "Types.hpp"

class Item {
public:
    Item(Integer id, Integer profit, Integer weight)
        : _id(id)
        , _profit(profit)
        , _weight(weight) {}

    inline Integer id() const { return _id; }
    inline Integer profit() const { return _profit; }
    inline Integer weight() const { return _weight; }

private:
    Integer _id;
    Integer _profit;
    Integer _weight;
};

#endif
