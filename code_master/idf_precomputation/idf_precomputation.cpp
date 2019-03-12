#include "idf_precomputation.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
extern "C" {
#include "idf_precomputation.h"
#include "obliv.h"
}

namespace private_knn {
namespace idf_precomputation {

std::map<int, float> IDFPrecomputation(absl::Span<const float> counts,
                                       float max_count, int num_samples,
                                       double epsilon, bool allow_floram,
                                       comm_channel* channel,
                                       int64_t* gate_count,
                                       int64_t* byte_count) {
  int vocabulary_size = counts.size();
  std::vector<float> probabilities(vocabulary_size);

  // We need to make sure that the un-normalized probabilities passed to the
  // exponential mechanism fit in a float. Since we can't normalize the
  // probabilities (we don't know the other party's shares), we adjust the
  // privacy budget spent on the exponential mechanism to do the scaling.
  double epsilon_exponential = std::min(
      epsilon / 2,
      static_cast<double>(std::log(std::numeric_limits<float>::max())) *
          num_samples / max_count);
  double epsilon_laplace = epsilon - epsilon_exponential;
  for (int i = 0; i < vocabulary_size; i++) {
    probabilities[i] = std::exp(counts[i] * epsilon_exponential / num_samples);
  }
  std::vector<int> result_indexes(num_samples);
  std::vector<float> result_values(num_samples);

  // Run Yao's protocol using Obliv-C.
  IDFPrecomputationOblivcArgs args = {
    num_inputs : vocabulary_size,
    probabilities : probabilities.data(),
    values : counts.data(),
    allow_floram : allow_floram,
    noise_parameter : static_cast<float>(num_samples / epsilon_laplace),
    num_results : num_samples,
    result_indexes : result_indexes.data(),
    result_values : result_values.data()
  };
  ProtocolDesc pd = {};
  setCurrentParty(&pd, channel->get_id() < channel->get_peer_id() ? 1 : 2);
  if (channel->connect_to_oblivc(pd) == -1) {
    BOOST_THROW_EXCEPTION(std::runtime_error("Obliv-C connection failed"));
  }
  execYaoProtocol(&pd, IDFPrecomputationOblivc, &args);

  // Merge result into a map.
  std::map<int, float> result;
  for (int i = 0; i < num_samples; i++) {
    result[result_indexes[i]] = result_values[i];
  }
  if (gate_count != nullptr) {
    *gate_count = args.num_gates;
  }
  if (channel->is_measured() && byte_count != nullptr) {
    *byte_count = tcp2PBytesSent(&pd);
  }
  cleanupProtocol(&pd);
  return result;
}

}  // namespace idf_precomputation
}  // namespace private_knn