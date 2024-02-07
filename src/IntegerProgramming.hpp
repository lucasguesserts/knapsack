#ifndef INTEGER_PROGRAMMING_HPP_
#define INTEGER_PROGRAMMING_HPP_

#include "gurobi_c++.h"

#include "Instance.hpp"
#include "Knapsack.hpp"
#include "Model.hpp"

class IntegerProgramming : public Model {
public:
    IntegerProgramming() = default;

    Real maximum_running_time = 2.0;
    bool extra_debug = false;
    Integer seed = 1234;

    Knapsack solve(const Instance & instance) override {
        try {
            const auto & items = instance.items();
            // model
            GRBEnv env = GRBEnv(true);
            env.set(GRB_IntParam_OutputFlag, extra_debug);
            env.set(GRB_IntParam_Threads, 1);
            env.set(GRB_DoubleParam_TimeLimit, this->maximum_running_time);
            env.set(GRB_DoubleParam_MIPGap, 0.0);
            env.set(GRB_DoubleParam_MIPGapAbs, 0.0);
            env.set(GRB_DoubleParam_FeasibilityTol, 1e-9);
            env.set(GRB_DoubleParam_IntFeasTol, 1e-9);
            env.start();
            GRBModel model = GRBModel(env);
            // variables
            GRBVar * x = model.addVars(instance.number_of_items(), GRB_BINARY);
            // constraints
            GRBLinExpr total_weight_of_selected_items = 0;
            for (std::size_t i = 0; i < instance.number_of_items(); ++i) {
                total_weight_of_selected_items += items[i].weight() * x[i];
            }
            model.addConstr(total_weight_of_selected_items <= instance.capacity());
            // objective
            GRBLinExpr total_profit_of_selected_items = 0;
            for (std::size_t i = 0; i < instance.number_of_items(); ++i) {
                total_profit_of_selected_items += items[i].profit() * x[i];
            }
            model.setObjective(total_profit_of_selected_items, GRB_MAXIMIZE);
            model.optimize();
            // solution
            Knapsack knapsack(instance.capacity());
            for (std::size_t i = 0; i < instance.number_of_items(); ++i) {
                if (x[i].get(GRB_DoubleAttr_X) >= 0.99) {
                    knapsack.add_item(items[i]);
                }
            }
            delete x;
            return knapsack;
        } catch (GRBException & e) {
            throw std::runtime_error(e.getMessage());
        }
    }
};

#endif
