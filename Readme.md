Bayesian Network Modelling as part of Artificial Intelligence course at IIT Delhi (2023-24 Semester I)
# Medical Diagnosis using Bayesian Networks

## Overview

This project implements a solution to learn missing probability values in a Bayesian Network used for medical diagnosis. The network structure is provided in the form of a `.bif` file (`alarm.bif`), and the task is to estimate missing Conditional Probability Table (CPT) entries using patient records from `records.dat`.

The Bayesian network models various diagnoses and symptoms, and our goal is to compute these missing parameters using the data. Once learned, the results are written back to `solved_alarm.bif`.

## Problem Statement

We are provided with a Bayesian Network where certain probabilities are missing (denoted by `-1`). Using the dataset of patient records, we need to learn the missing values. Each record contains the observed values for the variables in the network, with some records having missing values denoted by `"?"`.

The goal is to:
1. Learn the missing probabilities using an iterative algorithm.
2. Output the solved Bayesian network with complete probability tables in `.bif` format.

## Approach

### 1. Input Parsing

The algorithm starts by reading the network structure from the `.bif` file. This includes:
- The variables (nodes) in the network.
- The possible values for each variable (e.g., True/False).
- The parent-child relationships between the variables.
- The CPTs associated with each variable, where some probabilities are missing (`-1`).

### 2. Data Preprocessing

The dataset (`records.dat`) is read, where each row corresponds to a patient record. The data contains the observed values for the nodes in the network, with missing values represented as `"?"`. 

For records with missing values, the algorithm generates all possible completions by filling in the missing value with each possible category (e.g., True or False).

### 3. CPT Initialization

Initially, the algorithm assigns uniform probabilities to all missing entries in the CPTs. This neutral guess provides a starting point for learning the true probabilities from the data.

### 4. Expectation Step (E-step)

In the E-step:
- For each record in the dataset, the algorithm calculates the probability of each possible completion (for records with missing values).
- The joint probability of the observed data and each possible value for the missing data is computed using the current state of the CPTs.
- The algorithm uses these probabilities to update a weighted count of how often each value occurs for the missing variable, based on the observed data.

### 5. Maximization Step (M-step)

In the M-step:
- The CPTs are updated based on the weighted counts computed in the E-step.
- For each possible combination of parent values, the CPT entries are normalized to ensure that they sum to 1, reflecting the learned probabilities.
  
This step adjusts the CPTs to better fit the observed data and the estimated values for missing data.

### 6. Iterative Refinement

The algorithm iteratively alternates between the E-step and M-step, refining the probability estimates at each iteration. This process continues until:
- The CPT values converge (i.e., they stop changing significantly), or
- The time limit of 2 minutes is reached.

### 7. Handling Missing Data

For records containing missing values (`"?"`):
- The algorithm considers all possible completions of the record.
- It computes the likelihood of each completion and adjusts the CPTs accordingly. 
- The missing data is treated probabilistically, ensuring that the uncertainty is factored into the final learned CPTs.

### 8. Output

Once the learning process is complete, the final CPTs are written back to the `.bif` file (`solved_alarm.bif`). The `-1` entries in the original CPTs are replaced with the learned probabilities, formatted to four decimal places.

