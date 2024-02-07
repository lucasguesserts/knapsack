#ifndef KNAPSACK_HPP_
#define KNAPSACK_HPP_

#include <algorithm>
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

    void check_validity() const {
        if (this->_items.empty()) {
            throw std::runtime_error("The knapsack does not have any item");
        }
        if (this->_current_weight > this->_capacity) {
            throw std::runtime_error("The total weight of the items exceeds the capacity of the knapsack");
        }
        if (!this->are_identifiers_unique()) {
            throw std::runtime_error("The identifiers of the items are not unique");
        }
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

    bool are_identifiers_unique() const {
        std::vector<Integer> identifiers;
        identifiers.reserve(this->_items.size());
        for (const auto & item : this->_items) {
            identifiers.push_back(item.id());
        }
        std::sort(identifiers.begin(), identifiers.end());
        for (Integer i = 1; i < identifiers.size(); ++i) {
            if (identifiers[i] == identifiers[i - 1]) {
                return false;
            }
        }
        return true;
    }
};

#endif
