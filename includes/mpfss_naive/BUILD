load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")

cc_binary(
    name = "mpfss_naive_main",
    srcs = [
        "mpfss_naive_main.c",
        "mpfss_naive.c",

    ],


    visibility = ["//visibility:public"],

    deps = [
        "//includes:util",
        "//includes:dbg",
        ":mpfss_naive_obliv",
    ],

    #copts = ["-DDEBUG -g3"], 
    #copts = ["-DINFO"], 
)

oblivc_library(
    name = "mpfss_naive_obliv",
    srcs = [
        
        "mpfss_naive.oc",
        "mpfss_naive.c",

    ],
    hdrs = [
        "mpfss_naive.oh",
        "mpfss_naive.h",
    ],
    deps = [
        "//includes:util",
        "//includes:dbg",
        #"@ack//:oram_fssl",
        "//includes/fss-altered:fss_cprg_altered",
        "//includes/fss-altered:fss_altered",
        "@ack//:obig",
        "@ack//:osort",
        ":parallel_naive",
        "//includes:uint128_c",

    ],
    visibility = ["//visibility:public"],
    #copts = ["-DDEBUG -g3"], 
    #copts = ["-DOriginal_FSS"], 

)


oblivc_library(
    name = "mpfss_naive_obliv_original",
    srcs = [
        
        "mpfss_naive.oc",
        "mpfss_naive.c",

    ],
    hdrs = [
        "mpfss_naive.oh",
        "mpfss_naive.h",
    ],
    deps = [
        "//includes:util",
        "//includes:dbg",
        #"@ack//:oram_fssl",
        "//includes/fss-altered:fss_cprg_altered",
        "//includes/fss-altered:fss_altered",
        "@ack//:obig",
        "@ack//:osort",
        ":parallel_naive",
        "//includes:uint128_c",

    ],
    visibility = ["//visibility:public"],
    #copts = ["-DDEBUG -g3"], 
    copts = ["-DOriginal_FSS"], 
)



cc_library(
    name="parallel_naive",
    srcs = [
        "parallel_dpf_naive.c",
    ],
    hdrs = [
        "parallel_dpf_naive.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//includes:dbg",
    ],
    copts = ["-fopenmp"], 
    linkopts=["-fopenmp"],
)



#cc_library(
#    name = "mpfss_naive",
#    srcs = [
#        "mpfss_naive.c",
#    ],
#
#    hdrs = [
#        "mpfss_naive.h",#
#
#    ],
#
#    visibility = ["//visibility:public"],
#
#    deps = [
#        "//includes:util",
#        "//includes:dbg",
#        ":mpfss_naive_obliv",
#    ],
#
#    #copts = ["-DDEBUG -g3"], 
#    #copts = ["-DINFO"], 
#)


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


