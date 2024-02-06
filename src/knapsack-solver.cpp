#include <iostream>
#include <memory>

#include <cxxopts.hpp>

#include "Instance.hpp"
#include "Knapsack.hpp"

#include "Brkga.hpp"
#include "Greedy.hpp"
#include "IntegerProgramming.hpp"

using std::cout;
using std::endl;

void print_instance(const Instance & instance, const bool debug = false) {
    if (debug) {
        cout << endl
             << "--- items in instance of capacity " << instance.capacity() << " ---" << endl
             << endl;
        for (const auto & item : instance.items()) {
            cout << "id: " << item.id() << " profit: " << item.profit() << " weight: " << item.weight() << endl;
        }
    }
    return;
}

void print_knapsack(const Knapsack & knapsack, const bool debug = false) {
    if (debug) {
        cout << endl;
        cout << "--- items in knapsack ---" << endl
             << endl;
        for (const auto & item : knapsack.items()) {
            cout << "id: " << item.id() << " profit: " << item.profit() << " weight: " << item.weight() << endl;
        }
        cout << "total weight: " << knapsack.weight() << " (< " << knapsack.capacity() << ")" << endl;
        cout << "total profit: " << knapsack.profit() << endl;
        cout << endl;
    } else {
        cout << knapsack.profit() << endl;
    }
    return;
}

std::unique_ptr<Model> create_model(const std::string & model_name, const cxxopts::ParseResult & cli_parameters) {
    if (model_name == "greedy") {
        return std::make_unique<Greedy>();
    } else if (model_name == "ip") {
        return std::make_unique<IntegerProgramming>();
    } else if (model_name == "brkga") {
        const auto population_size = cli_parameters["population-size"].as<Integer>();
        return std::make_unique<Brkga>(population_size);
    } else {
        throw std::runtime_error("unknown model: " + model_name);
    }
}

int main(const int argc, const char * const argv[]) {
    try {
        // CLI args
        cxxopts::Options options("knapsack-solver", "a solver with many models for the knapsack problem");

        options.add_options()("m,model", "model to be used to solve the problem, one of: greedy, ip, brkga", cxxopts::value<std::string>()->default_value("greedy"));
        options.add_options()("d,debug", "Enable debugging", cxxopts::value<bool>()->implicit_value("true")->default_value("false"));
        options.add_options()("population-size", "size of the population for BRKGA (only works if model is brkga)", cxxopts::value<Integer>()->default_value("100"));
        options.add_options()("instance-file", "path to the instance file", cxxopts::value<std::string>());
        options.add_options()("h,help", "Print this help message");

        options.parse_positional({"instance-file"});
        options.positional_help("INSTANCE-FILE");

        const auto cli_parameters = options.parse(argc, argv);

        if (cli_parameters.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        const auto debug = cli_parameters["debug"].as<bool>();
        const auto model_name = cli_parameters["model"].as<std::string>();
        const auto instance_file = cli_parameters["instance-file"].as<std::string>();

        // solver
        std::unique_ptr<Model> model = create_model(model_name, cli_parameters);

        Instance instance(instance_file);
        print_instance(instance, debug);

        Knapsack knapsack = model->solve(instance);
        print_knapsack(knapsack, debug);

    } catch (const std::exception & e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}
