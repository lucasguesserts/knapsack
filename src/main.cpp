#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include "Instance.hpp"
#include "Item.hpp"
#include "Knapsack.hpp"
#include "Types.hpp"

#include "Greedy.hpp"

using std::cout;
using std::endl;

void print_usage(const std::string & program_name) {
    cout << "Usage: " << program_name << " <instance_file>" << endl;
}

void print_instance(const Instance & instance) {
    cout << endl
         << "=== items in instance of capacity " << instance.capacity() << " ===" << endl
         << endl;
    for (const auto & item : instance.items()) {
        cout << "id: " << item.id() << " profit: " << item.profit() << " weight: " << item.weight() << endl;
    }
}

void print_knapsack(const Knapsack & knapsack) {
    cout << endl
         << "=== items in knapsack ===" << endl
         << endl;
    for (const auto & item : knapsack.items()) {
        cout << "id: " << item.id() << " profit: " << item.profit() << " weight: " << item.weight() << endl;
    }
    cout << "total weight: " << knapsack.weight() << " (< " << knapsack.capacity() << ")" << endl;
    cout << "total profit: " << knapsack.profit() << endl;
    cout << endl;
}

int main(const int argc, const char * const argv[]) {
    try {
        if (argc != 2) {
            print_usage(argv[0]);
            return 1;
        }

        Instance instance(argv[1]);
        print_instance(instance);

        std::unique_ptr<Model> model = std::make_unique<Greedy>();
        Knapsack knapsack = model->solve(instance);
        print_knapsack(knapsack);

    } catch (const std::exception & e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}
