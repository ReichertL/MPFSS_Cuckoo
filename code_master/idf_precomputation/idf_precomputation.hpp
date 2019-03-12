// Securely computes IDF values for a distributed set of text documents. The
// outputs guarantee differential privacy with respect to single documents.
#pragma once

#include <map>
#include "absl/types/span.h"
#include "mpc_utils/comm_channel.hpp"

namespace private_knn {
namespace idf_precomputation {

// Approximately selects the `num_samples` largest values among the sums of both
// parties' `counts` arrays. More concretely, selects `num_samples` indexes
// between 0 and counts.size() using the exponential mechanism. For each
// selected index i, sums up counts[i] on both parties and adds Laplace noise to
// ensure epsilon-differential privacy. Parties must ensure that the sum of
// counts[i] on both parties does not exceed `max_count`.
std::map<int, float> IDFPrecomputation(absl::Span<const float> counts,
                                       float max_count, int num_samples,
                                       double epsilon, bool allow_floram,
                                       comm_channel* channel,
                                       int64_t* gate_count = nullptr,
                                       int64_t* byte_count = nullptr);

}  // namespace idf_precomputation
}  // namespace private_knn