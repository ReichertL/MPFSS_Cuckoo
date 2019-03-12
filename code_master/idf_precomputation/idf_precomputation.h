#pragma once
#include <stdint.h>

typedef struct {
  // Number of input probabilities and values.
  int num_inputs;
  // Array of num_inputs multiplicative shares of (unnormalized) probabilities.
  const float *probabilities;
  // Array of num_inputs values.
  const float *values;
  // Whether to use FLORAM or not.
  bool allow_floram;
  // Noise parameter to use for each result value.
  float noise_parameter;
  // Number of desired values in the result.
  int num_results;
  // Indices in the original array of the chosen elements.
  int *result_indexes;
  // Noisy values corresponding to the chosen elements.
  float *result_values;
  // Number of gates used for the garbled circuit.
  int64_t num_gates;
} IDFPrecomputationOblivcArgs;

void IDFPrecomputationOblivc(void *args);
