# Knapsack

This repository contains:

1. a small set of instances of the knapsack problem;
   1. they are in [this directory](./instances/);
2. the implementation in C++ of some models to solve the knapsack problem;
   1. they are in [this directory](./src/);

## Running

To run, simply execute:

```sh
make
```

It will compile and run all the models for all the instances. The results will be saved in the [results file](./results.log) (it is currently empty).

## Analysis

One can run an analysis of the results by executing:

```sh
$ python analysis.py
Average IP: 0.9990563802881991
Average Greedy: 1.0050669905697236
```

The `Average IP` is computed dividing all results of the BRKGA model by the results of the IP model, and taking the average. The `Average Greedy` is the same, but using the results of the Greedy model instead of the IP.

## Activating the virtual environment:

```sh
# create venv
python3 -m venv venv
# activate venv
. venv/bin/activate
# install requirements
pip install --upgrade pip
pip install -r requirements.txt
```

## Tuning

To tune the parameters of the BRKGA model using [`irace`](https://github.com/MLopez-Ibanez/irace), run:

```sh
cd tuning
irace --forbidden-file=./forbidden.txt | tee irace.log
```

The results of the tuning will be saved in the [irace log file](./tuning/irace.log), and they will be displayed in the standard output as well.
