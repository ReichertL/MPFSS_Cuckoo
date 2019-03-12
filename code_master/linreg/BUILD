load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")

cc_library(
    name = "dbg",
    hdrs = ["dbg.h"],
)

cc_library(
    name = "pIO",
    hdrs = ["protocolIO.h"],
)


oblivc_library(
    name = "linreg_obliv",
    srcs = [
        "linreg.oc",
    ],
    hdrs = [
        "linreg.oh",
    ],
    deps = [
        ":dbg",
        ":pIO",

    ],
)

cc_library(
    name = "linreg",
    srcs = [
        "linreg.c",
    ],
    hdrs = [
        "linreg.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":linreg_obliv",
        ":dbg",
        ":pIO",
    ],
)