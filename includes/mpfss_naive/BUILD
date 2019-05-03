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
    #copts = ["-DDEBUG -g3"], 
)


cc_binary(
    name = "mpfss_naive",
    srcs = [
        "mpfss_naive.c",
    ],

    deps = [
        "//includes:util",
        "//includes:dbg",
        ":mpfss_naive_obliv",
    ],

    #copts = ["-DDEBUG -g3"], 
    copts = ["-DINFO"], 
)


oblivc_library(
    name = "mpfss_naive_unittest_obliv",
    srcs = [
        
        "test/mpfss_naive_unittest.oc",

    ],
    hdrs = [

        "test/mpfss_naive_unittest.h",

    ],
    deps = [
        "//includes:util",
        "//includes:dbg",
        "@ack//:oram_fssl",
        ":mpfss_naive_obliv",
        "@com_google_googletest//:gtest_main",

    ],
    #copts = ["-DDEBUG -g3"], 
)

cc_test(
    name = "mpfss_naive_unittest",
    size = "large",
    srcs = [
        "test/mpfss_naive_unittest.c",
    ],
    deps = [
        ":mpfss_naive_unittest_obliv",
        "//includes:util",
        "//includes:dbg",
    ],
    visibility = ["//visibility:public"],
    #copts = ["-DDEBUG -g3"], 


)


