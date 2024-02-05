#include <iostream>
#include <memory>

#include "Instance.hpp"
#include "Knapsack.hpp"

#include "Greedy.hpp"
#include "IntegerProgramming.hpp"
#include "Brkga.hpp"

using std::cout;
using std::endl;

void print_usage(const std::string & program_name) {
    cout << "Usage: " << program_name << " "
         << "<model>"
         << " "
         << "<instance_file>" << endl;
    cout << "  model: 'greedy', 'ip'" << endl;
}

void print_instance(const Instance & instance) {
#if DEBUG
    cout << endl
         << "--- items in instance of capacity " << instance.capacity() << " ---" << endl
         << endl;
    for (const auto & item : instance.items()) {
        cout << "id: " << item.id() << " profit: " << item.profit() << " weight: " << item.weight() << endl;
    }
#endif
    return;
}

void print_knapsack(const Knapsack & knapsack) {
    cout << endl;
#if DEBUG
    cout << "--- items in knapsack ---" << endl
         << endl;
    for (const auto & item : knapsack.items()) {
        cout << "id: " << item.id() << " profit: " << item.profit() << " weight: " << item.weight() << endl;
    }
#endif
    cout << "total weight: " << knapsack.weight() << " (< " << knapsack.capacity() << ")" << endl;
    cout << "total profit: " << knapsack.profit() << endl;
    cout << endl;
    return;
}

std::unique_ptr<Model> create_model(const std::string & model_name) {
    if (model_name == "greedy") {
        return std::make_unique<Greedy>();
    } else if (model_name == "ip") {
        return std::make_unique<IntegerProgramming>();
    } else if (model_name == "brkga") {
        return std::make_unique<Brkga>();
    } else {
        throw std::runtime_error("unknown model: " + model_name);
    }
}

int main(const int argc, const char * const argv[]) {
    try {
        if (argc != 3) {
            print_usage(argv[0]);
            return 1;
        }

        std::unique_ptr<Model> model = create_model(argv[1]);

        Instance instance(argv[2]);

#if DEBUG
        print_instance(instance);
#endif

        Knapsack knapsack = model->solve(instance);
        print_knapsack(knapsack);

    } catch (const std::exception & e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}
