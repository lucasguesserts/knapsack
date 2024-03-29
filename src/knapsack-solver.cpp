#include <cmath>
#include <iostream>
#include <memory>
#include <chrono>

#include <boost/timer/timer.hpp>

#include <cxxopts.hpp>

#include "Instance.hpp"
#include "Knapsack.hpp"

#include "Brkga.hpp"
#include "Greedy.hpp"
#include "IntegerProgramming.hpp"
#include "Types.hpp"

using std::cout;
using std::endl;

void print_instance(const Instance & instance, const bool extra_debug = false) {
    if (extra_debug) {
        cout << endl
             << "--- items in instance of capacity " << instance.capacity() << " ---" << endl
             << endl;
        for (const auto & item : instance.items()) {
            cout << "id: " << item.id() << " profit: " << item.profit() << " weight: " << item.weight() << endl;
        }
    }
    return;
}

void print_knapsack(const Knapsack & knapsack, const Real elapsed_time, const bool debug, const bool extra_debug) {
    try {
        knapsack.check_validity();
    } catch (const std::exception & e) {
        cout << "Error: " << e.what() << endl;
        return;
    }
    if (extra_debug) {
        cout << endl;
        cout << "--- items in knapsack ---" << endl
             << endl;
        for (const auto & item : knapsack.items()) {
            cout << "id: " << item.id() << " profit: " << item.profit() << " weight: " << item.weight() << endl;
        }
        cout << "total weight: " << knapsack.weight() << " (< " << knapsack.capacity() << ")" << endl;
        cout << "total profit: " << knapsack.profit() << endl;
        cout << endl;
    } else if (debug) {
        cout << "profit: " << knapsack.profit() << endl;
        cout << "weight: " << knapsack.weight() << endl;
        cout << "capacity: " << knapsack.capacity() << endl;
        cout << "elapsed time: " << elapsed_time << endl;
    } else {
        const auto relative_profit = static_cast<Real>(knapsack.profit()) / knapsack.capacity();
        cout << "-" << relative_profit << " " << elapsed_time << endl;
    }
    return;
}

std::unique_ptr<Model> create_model(const cxxopts::ParseResult & cli_parameters) {
    const auto model_name = cli_parameters["model"].as<std::string>();
    if (model_name == "greedy") {
        return std::make_unique<Greedy>();
    } else if (model_name == "ip") {
        auto model = std::make_unique<IntegerProgramming>();
        model->extra_debug = cli_parameters["extra-debug"].as<bool>();
        model->maximum_running_time = cli_parameters["maximum_running_time"].as<Real>();
        model->seed = cli_parameters["seed"].as<Integer>();
        return model;
    } else if (model_name == "brkga") {
        auto model = std::make_unique<Brkga>();
        model->control_parameters.maximum_running_time = std::chrono::seconds(static_cast<Integer>(std::ceil(cli_parameters["maximum_running_time"].as<Real>())));
        model->seed = cli_parameters["seed"].as<Integer>();
        model->parameters.population_size = cli_parameters["population_size"].as<Integer>();
        model->parameters.elite_percentage = cli_parameters["elite_percentage"].as<Real>();
        model->parameters.mutants_percentage = cli_parameters["mutants_percentage"].as<Real>();
        model->parameters.num_elite_parents = cli_parameters["num_elite_parents"].as<Integer>();
        model->parameters.total_parents = cli_parameters["total_parents"].as<Integer>();
        return model;
    } else {
        throw std::runtime_error("unknown model: " + model_name);
    }
}

int main(const int argc, const char * const argv[]) {
    try {
        // CLI args
        cxxopts::Options options("knapsack-solver", "a solver with many models for the knapsack problem");

        options.add_options()("h,help", "Print this help message");
        options.add_options()("m,model", "model to be used to solve the problem, one of: greedy, ip, brkga", cxxopts::value<std::string>()->default_value("greedy"));
        options.add_options()("d,debug", "Enable debugging, print a little more data", cxxopts::value<bool>()->implicit_value("true")->default_value("false"));
        options.add_options()("extra-debug", "Enable debugging, print extra data", cxxopts::value<bool>()->implicit_value("true")->default_value("false"));
        options.add_options()("instance-file", "path to the instance file", cxxopts::value<std::string>());
        options.add_options()("s,seed", "Seed for the random number generator", cxxopts::value<Integer>()->default_value("1234"));
        options.add_options()("maximum_running_time", "Maximum running time, in seconds. It may be rounded up.", cxxopts::value<Real>()->default_value("1.0"));
        options.add_options()("population_size", "Number of elements in the population", cxxopts::value<Integer>()->default_value("1000"));
        options.add_options()("elite_percentage", "Percentage of individuals to become the elite set (0.0, 1.0]", cxxopts::value<Real>()->default_value("0.1"));
        options.add_options()("mutants_percentage", "Percentage of mutants to be inserted in the population (0.0, 1.0]", cxxopts::value<Real>()->default_value("0.1"));
        options.add_options()("num_elite_parents", "Number of elite parents for mating (> 0)", cxxopts::value<Integer>()->default_value("1"));
        options.add_options()("total_parents", "Number of total parents for mating (> 0)", cxxopts::value<Integer>()->default_value("2"));

        options.parse_positional({"instance-file"});
        options.positional_help("INSTANCE-FILE");

        const auto cli_parameters = options.parse(argc, argv);

        if (cli_parameters.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        const auto debug = cli_parameters["debug"].as<bool>();
        const auto extra_debug = cli_parameters["extra-debug"].as<bool>();
        const auto instance_file = cli_parameters["instance-file"].as<std::string>();

        // solver
        boost::timer::cpu_timer timer;

        std::unique_ptr<Model> model = create_model(cli_parameters);

        Instance instance(instance_file);
        print_instance(instance, extra_debug);

        timer.start();
        Knapsack knapsack = model->solve(instance);
        timer.stop();
        const Real running_time_seconds = timer.elapsed().user / 1.0e+9;
        print_knapsack(knapsack, running_time_seconds, debug, extra_debug);

    } catch (const std::exception & e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}
