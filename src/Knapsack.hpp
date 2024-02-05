#ifndef KNAPSACK_HPP_
#define KNAPSACK_HPP_

#include <stdexcept>
#include <vector>

#include "Item.hpp"
#include "Types.hpp"

class Knapsack {
public:
    Knapsack(Integer capacity)
        : _capacity(capacity)
        , _items{}
        , _current_weight(0)
        , _current_profit(0) {}

    void add_item(const Item & item) {
        if (this->_current_weight + item.weight() > this->_capacity) {
            throw std::invalid_argument("The item does not fit in the knapsack");
        }
        this->_items.push_back(item);
        this->_current_profit += item.profit();
        this->_current_weight += item.weight();
        return;
    }

    inline Integer capacity() const { return _capacity; }
    inline const std::vector<Item> & items() const { return _items; }
    inline Integer weight() const { return _current_weight; }
    inline Integer profit() const { return _current_profit; }

private:
    Integer _capacity;
    std::vector<Item> _items;
    Integer _current_weight;
    Integer _current_profit;
};

#endif
