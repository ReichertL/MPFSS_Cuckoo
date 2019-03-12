#include <boost/thread.hpp>
#include <cmath>
#include "gtest/gtest.h"
#include "mpc_utils/testing/comm_channel_test_helper.hpp"
extern "C" {
#include "obliv_logf_test.h"
}

namespace private_knn {
namespace idf_precomputation {
namespace {

using mpc_utils::testing::CommChannelTestHelper;

TEST(OblivLogf, TestUnitInterval) {
  CommChannelTestHelper helper(false);

  std::vector<float> inputs;
  for (float f = 0.0f; f <= 1.0f; f += 0.01f) {
    inputs.push_back(f);
  }
  std::vector<float> outputs(inputs.size(), 1);
  auto inputs2 = inputs, outputs2 = outputs;
  OblivLogfTestOblivcArgs args = {
    len : static_cast<int>(inputs.size()),
    inputs : inputs.data(),
    outputs : outputs.data(),
  };
  boost::thread thread1([&helper, &inputs2, &outputs2] {
    OblivLogfTestOblivcArgs args = {
      len : static_cast<int>(inputs2.size()),
      inputs : inputs2.data(),
      outputs : outputs2.data(),
    };
    ProtocolDesc pd1;
    setCurrentParty(&pd1, 2);
    helper.GetChannel(1)->connect_to_oblivc(pd1);
    execYaoProtocol(&pd1, &OblivLogfTestOblivc, &args);
    cleanupProtocol(&pd1);
  });
  boost::thread_guard<> g(thread1);
  ProtocolDesc pd0;
  setCurrentParty(&pd0, 1);
  helper.GetChannel(0)->connect_to_oblivc(pd0);
  execYaoProtocol(&pd0, &OblivLogfTestOblivc, &args);
  cleanupProtocol(&pd0);

  for (int i = 0; i < static_cast<int>(outputs.size()); i++) {
    EXPECT_FLOAT_EQ(outputs[i], logf(inputs[i]));
  }
}

}  // namespace
}  // namespace idf_precomputation
}  // namespace private_knn