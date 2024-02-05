#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "Instance.hpp"
#include "Knapsack.hpp"

class Model {
public:
    Model() = default;
    virtual ~Model() = default;

    virtual Knapsack solve(const Instance & instance) = 0;
};

#endif
