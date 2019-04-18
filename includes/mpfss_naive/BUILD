load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")


oblivc_library(
    name = "mpfss_naive_obliv",
    srcs = [
        
        "mpfss_naive.oc",

    ],
    hdrs = [
        "mpfss_naive.oh",
        "mpfss_naive.h",

    ],
    deps = [
        "//includes:util",
        "//includes:dbg",
        "@ack//:oram_fssl",
    ],
    visibility = ["//visibility:public"],
    copts = ["-DDEBUG -g3"], 
)



