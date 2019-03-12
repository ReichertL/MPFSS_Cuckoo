load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")

oblivc_library(
    name = "idf_precomputation_obliv",
    srcs = [
        "idf_precomputation.oc",
    ],
    hdrs = [
        "idf_precomputation.h",
    ],
    deps = [
        ":obliv_logf",
        "@ack//:oram",
    ],
)

cc_library(
    name = "idf_precomputation",
    srcs = [
        "idf_precomputation.cpp",
    ],
    hdrs = [
        "idf_precomputation.hpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":idf_precomputation_obliv",
        "@abseil//absl/types:span",
        "@mpc_utils//mpc_utils:comm_channel_oblivc",
    ],
)

cc_test(
    name = "idf_precomputation_test",
    size = "large",
    srcs = [
        "idf_precomputation_test.cpp",
    ],
    deps = [
        ":idf_precomputation",
        "@mpc_utils//mpc_utils/testing:comm_channel_test_helper_oblivc",
        "@mpc_utils//mpc_utils/testing:test_deps",
    ],
)

oblivc_library(
    name = "obliv_logf",
    srcs = [
        "obliv_logf.oc",
    ],
    hdrs = [
        "obliv_logf.oh",
    ],
)

oblivc_library(
    name = "obliv_logf_test_obliv",
    srcs = [
        "obliv_logf_test.oc",
    ],
    hdrs = [
        "obliv_logf_test.h",
    ],
    deps = [
        ":obliv_logf",
    ],
)

cc_test(
    name = "obliv_logf_test",
    size = "small",
    srcs = [
        "obliv_logf_test.cpp",
    ],
    deps = [
        ":obliv_logf_test_obliv",
        "@mpc_utils//mpc_utils/testing:comm_channel_test_helper_oblivc",
        "@mpc_utils//mpc_utils/testing:test_deps",
    ],
)
