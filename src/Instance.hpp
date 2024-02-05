#ifndef INSTANCE_HPP_
#define INSTANCE_HPP_

#include <algorithm>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>

#include "Item.hpp"
#include "Types.hpp"

class Instance {
public:
    Instance(const std::string & file_path) {
        std::ifstream file(file_path, std::ios::in);
        if (!file.is_open()) {
            throw std::invalid_argument("The file '" + file_path + "' could not be opened");
        }
        Integer number_of_items;
        file >> number_of_items;
        this->_items.reserve(number_of_items);
        for (Integer i = 0; i < number_of_items; ++i) {
            Integer id, profit, weight;
            file >> id >> profit >> weight;
            if (profit < 0 || weight < 0) {
                throw std::invalid_argument("The profit and weight of an item must be non-negative");
            }
            this->_items.emplace_back(id, profit, weight);
        }
        file >> this->_capacity;
        this->check_if_is_valid();
        return;
    }

    inline Integer number_of_items() const { return _items.size(); }
    inline Integer capacity() const { return _capacity; }
    inline const std::vector<Item> & items() const { return _items; }

private:
    std::vector<Item> _items;
    Integer _capacity;

    void check_if_is_valid() const {
        // there is no item
        if (this->_items.empty()) {
            throw std::invalid_argument("The instance must have at least one item");
        }
        if (!this->each_item_fits_in_the_knapsack()) {
            throw std::invalid_argument("Each item must fit in the knapsack");
        }
        if (!this->total_weight_exceeds_capacity()) {
            throw std::invalid_argument("The total weight of the items must exceed the capacity of the knapsack");
        }
        if (!this->are_identifiers_unique()) {
            throw std::invalid_argument("The identifiers of the items must be unique");
        }
        return;
    }

    bool each_item_fits_in_the_knapsack() const {
        return std::all_of(
            _items.begin(),
            _items.end(),
            [this](const Item & item) {
                return item.weight() <= this->_capacity;
            });
    }

    bool total_weight_exceeds_capacity() const {
        const Integer total_weight = std::accumulate(
            _items.begin(),
            _items.end(),
            Integer(0),
            [](Integer sum, const Item & item) {
                return sum + item.weight();
            });
        return total_weight > this->_capacity;
    }

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
