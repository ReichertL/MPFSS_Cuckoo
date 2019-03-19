load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")


oblivc_library(
    name = "aes_obliv",
    srcs = [
        "aes.oc",
    ],
    hdrs = [
        "aes.h",
    ],
    deps = [
        "//masterarbeit/code_master/includes:util",

    ],
)

cc_binary(
    name = "aes",
    srcs = [
        "aes.c",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//masterarbeit/code_master/includes:util",
        ":aes_obliv",
    ],
)