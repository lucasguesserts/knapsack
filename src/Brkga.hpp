#ifndef BRKGA_HPP_
#define BRKGA_HPP_

#include <algorithm>
#include <chrono>
#include <memory>
#include <utility>
#include <vector>

#include <brkga_mp_ipr.hpp>

#include "Instance.hpp"
#include "Knapsack.hpp"
#include "Model.hpp"
#include "Types.hpp"

class Decoder {
public:
    Decoder(const Instance & instance)
        : instance(instance) {}

    Knapsack solve(const std::vector<Real> & chromosome) {
        const auto items_order_indices = allocation_order(chromosome);
        Knapsack knapsack(this->instance.capacity());
        const auto & items = this->instance.items();
        for (const auto & index : items_order_indices) {
            const auto & item = items[index];
            if (knapsack.weight() + item.weight() < knapsack.capacity()) {
                knapsack.add_item(item);
            }
        }
        return knapsack;
    }

    Real decode(const std::vector<Real> & chromosome, bool rewrite) {
        auto knapsack = solve(chromosome);
        auto fitness = knapsack.profit();
        return fitness;
    }

private:
    const Instance & instance;

    static std::vector<Integer> allocation_order(const std::vector<Real> & chromosome) {
        // create a vector of pairs (index, gene) and sort it by gene value (decreasing order)
        std::vector<std::pair<Integer, Real>> index_gene_pairs;
        for (Integer i = 0; i < chromosome.size(); i++) {
            index_gene_pairs.push_back(std::make_pair(i, chromosome[i]));
        }
        sort(
            index_gene_pairs.begin(),
            index_gene_pairs.end(),
            [](const std::pair<Integer, Real> & a, const std::pair<Integer, Real> & b) {
                return a.second < b.second;
            });
        reverse(index_gene_pairs.begin(), index_gene_pairs.end());
        // extract the indices
        std::vector<Integer> indices;
        indices.reserve(index_gene_pairs.size());
        std::transform(
            index_gene_pairs.begin(),
            index_gene_pairs.end(),
            std::back_inserter(indices),
            [](const std::pair<Integer, Real> & pair) {
                return pair.first;
            });
        return indices;
    }
};

class Brkga : public Model {
public:
    Knapsack solve(const Instance & instance) override {
        const auto parameters = make_brkga_parameters();
        const auto control_parameters = make_brkga_control_parameters();
        auto decoder = Decoder(instance);

        BRKGA::BRKGA_MP_IPR<Decoder> solver(
            decoder,
            BRKGA::Sense::MAXIMIZE,
            1234,
            instance.number_of_items(),
            parameters,
            1);
        // run BRKGA
        const auto result_status = solver.run(control_parameters, nullptr);
        auto knapsack = decoder.solve(result_status.best_chromosome);
        return knapsack;
    }

private:
    BRKGA::BrkgaParams make_brkga_parameters() {
        auto parameters = BRKGA::BrkgaParams{};
        parameters.population_size = 100;
        parameters.elite_percentage = 0.3;
        parameters.mutants_percentage = 0.2;
        parameters.num_elite_parents = 2;
        parameters.total_parents = 3;
        parameters.bias_type = BRKGA::BiasFunctionType::LOGINVERSE;
        parameters.num_independent_populations = 3;
        parameters.pr_number_pairs = 0;
        parameters.pr_minimum_distance = 0.15;
        parameters.pr_type = BRKGA::PathRelinking::Type::DIRECT;
        parameters.pr_selection = BRKGA::PathRelinking::Selection::BESTSOLUTION;
        parameters.pr_distance_function_type = BRKGA::PathRelinking::DistanceFunctionType::KENDALLTAU;
        switch (parameters.pr_distance_function_type) {
        case BRKGA::PathRelinking::DistanceFunctionType::KENDALLTAU:
            parameters.pr_distance_function = std::make_shared<BRKGA::KendallTauDistance>();
            break;
        case BRKGA::PathRelinking::DistanceFunctionType::HAMMING:
            parameters.pr_distance_function = std::make_shared<BRKGA::HammingDistance>();
            break;
        default:
            throw std::runtime_error("Unknown distance function type.");
        }
        parameters.alpha_block_size = 1.0;
        parameters.pr_percentage = 1.0;
        parameters.num_exchange_individuals = 1;
        parameters.shaking_type = BRKGA::ShakingType::SWAP;
        parameters.shaking_intensity_lower_bound = 0.25;
        parameters.shaking_intensity_upper_bound = 0.75;
        return parameters;
    }

    BRKGA::ControlParams make_brkga_control_parameters() {
        auto parameters = BRKGA::ControlParams{};
        parameters.maximum_running_time = std::chrono::seconds{1};
        parameters.exchange_interval = 100;
        parameters.ipr_interval = 200;
        parameters.shake_interval = 300;
        parameters.reset_interval = 500;
        parameters.stall_offset = 1000;
        return parameters;
    }
};

#endif
