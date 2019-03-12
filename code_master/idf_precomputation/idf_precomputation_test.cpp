#include "idf_precomputation.hpp"
#include "gtest/gtest.h"
#include "mpc_utils/testing/comm_channel_test_helper.hpp"

namespace private_knn {
namespace idf_precomputation {
namespace {

using mpc_utils::testing::CommChannelTestHelper;

void RunTest(int vocabulary_size, int num_samples, bool allow_floram,
             bool measure_communication) {
  CommChannelTestHelper helper(measure_communication);
  std::vector<float> counts(vocabulary_size);
  for (int i = 0; i < vocabulary_size; i++) {
    counts[i] = i;
  }
  int count_max = 2 * vocabulary_size - 2;
  double epsilon = 100;  // Make noise negligible for this test.
  boost::thread thread1(
      [&helper, &counts, count_max, num_samples, epsilon, allow_floram] {
        private_knn::idf_precomputation::IDFPrecomputation(
            counts, count_max, num_samples, epsilon, allow_floram,
            helper.GetChannel(1));
      });
  boost::thread_guard<> g(thread1);

  int64_t num_gates, byte_count;
  auto result = private_knn::idf_precomputation::IDFPrecomputation(
      counts, count_max, num_samples, epsilon, allow_floram,
      helper.GetChannel(0), &num_gates, &byte_count);

  EXPECT_EQ(result.size(), num_samples);
  EXPECT_GT(num_gates, 0);
  if (measure_communication) {
    EXPECT_GT(byte_count, 0);
  }
  for (auto pair : result) {
    EXPECT_LT(pair.first, vocabulary_size);
    EXPECT_GE(pair.first, 0);
    // Check that there is noise, but not too much.
    EXPECT_NE(pair.second, counts[pair.first]);
    EXPECT_LT(pair.second, 2 * pair.first + 1);
    EXPECT_GT(pair.second, 2 * pair.first - 1);
  }
}

TEST(IDFPrecomputation, TestSmallVocabulary) { RunTest(10, 5, true, false); }

TEST(IDFPrecomputation, TestSmallVocabularyMeasured) {
  RunTest(10, 5, true, true);
}

TEST(IDFPrecomputation, TestMediumVocabulary) { RunTest(100, 1, false, false); }

TEST(IDFPrecomputation, TestLargeVocabulary) {
  RunTest(1 << 14, 1, true, false);
}

}  // namespace
}  // namespace idf_precomputation
}  // namespace private_knn