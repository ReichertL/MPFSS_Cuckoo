load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")


cc_binary(
    name = "srand_test",
    srcs = ["srand_test.cpp"],
    visibility = ["//visibility:public"],
    copts = ["-fopenmp"], 
    linkopts=["-fopenmp"],

)

