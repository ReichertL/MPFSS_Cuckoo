#pragma once

typedef struct {
  int len;
  const float* inputs;
  float* outputs;
} OblivLogfTestOblivcArgs;

void OblivLogfTestOblivc(void* vargs);