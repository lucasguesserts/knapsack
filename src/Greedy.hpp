#ifndef GREEDY_HPP_
#define GREEDY_HPP_

#include <algorithm>
#include <vector>

#include "Instance.hpp"
#include "Item.hpp"
#include "Knapsack.hpp"
#include "Model.hpp"
#include "Types.hpp"

class Greedy : public Model {
public:
    Knapsack solve(const Instance & instance) {
        std::vector<Item> items = sort_items_by_relative_profit(instance.items());
        Knapsack knapsack(instance.capacity());
        for (const auto & item : items) {
            if (knapsack.weight() + item.weight() < knapsack.capacity()) {
                knapsack.add_item(item);
            }
        }
        return knapsack;
    }

private:
    static Real relative_profit(const Item & item) {
        return static_cast<Real>(item.profit()) / item.weight();
    }

    static std::vector<Item> sort_items_by_relative_profit(const std::vector<Item> & items) {
        std::vector<Item> sorted_items = items;
        std::sort(
            sorted_items.begin(),
            sorted_items.end(),
            [](const Item & a, const Item & b) {
                return relative_profit(a) < relative_profit(b);
            });
        std::reverse(sorted_items.begin(), sorted_items.end());
        return sorted_items;
    }
};

#endif
