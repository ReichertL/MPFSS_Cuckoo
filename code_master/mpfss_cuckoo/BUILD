load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")
#load("@io_bazel_rules_docker//cc:image.bzl", "cc_image")

#cc_image(
#    name = "cuckoo_image",
#    binary = ":mpfss_cuckoo",
#)


cc_binary(
    name = "mpfss_cuckoo",
    srcs = [
        "mpfss_cuckoo_main.cpp",
        "mpfss_cuckoo.cpp",
        "mpfss_cuckoo.h"
    
    ],

    visibility = ["//visibility:public"],

    deps = [
        "//includes:util",
        "//includes:dbg",
        "//includes:cuckoo",
        "//includes:benchmark-cpp",
        ":buckets",
        ":mpfss_cuckoo_obliv",
        "//includes/create_structs:create_structs",
        "@mpc_utils//mpc_utils:status",
        "@abseil//absl/types:span",
        "@boost//:random",


    ],

    #copts = ["-DDEBUG"], 
    copts = ["-fopenmp"], 
    linkopts=["-fopenmp"],
)

cc_binary(
    name = "mpfss_vole_cuckoo",
    srcs = [
        "mpfss_vole_cuckoo_main.cpp",
        "mpfss_cuckoo.cpp",
        "mpfss_cuckoo.h"
    
    ],

    visibility = ["//visibility:public"],

    deps = [
        "//includes:util",
        "//includes:dbg",
        "//includes:cuckoo",
        "//includes:benchmark-cpp",
        ":buckets",
        ":mpfss_cuckoo_obliv",
        "//includes/create_structs:create_structs",
        "@mpc_utils//mpc_utils:status",
        "@abseil//absl/types:span",

    ],

    #copts = ["-DDEBUG"], 
)


oblivc_library(
    name = "mpfss_cuckoo_obliv",
    srcs = [
        "mpfss_cuckoo.oc",
    ],
    hdrs = [
        "mpfss_cuckoo.oh",
        "parallel.h",
    ],
    visibility = ["//visibility:public"],

    deps = [
        "//includes:util",
        "//includes:dbg",
        "//includes:uint128_c",
        #"@semesterprojekt//code_projekt/mpfss_naive:mpfss_naive_obliv",
        "//includes/mpfss_naive:mpfss_naive_obliv",
        "//includes/create_structs:create_structs",
        ":parallel",
        ":parallel_dpf",
        "//includes:secret_share_beta",

    ],
    #copts = ["-DDEBUG"], 
)

cc_library(
    name="parallel",
    srcs = [
        "parallel.c",
    ],
    hdrs = [
        "parallel.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//includes:dbg",
    ],
    copts = ["-fopenmp"], 
    linkopts=["-fopenmp"],
)

oblivc_library(
    name="parallel_dpf",
    srcs = [
        "parallel_dpf.oc",
    ],
    hdrs = [
        "parallel_dpf.h"
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//includes:dbg",
        "//includes:uint128_c",
        "//includes/mpfss_naive:mpfss_naive_obliv",
        "//includes/create_structs:create_structs",
        ":parallel",
    ],
)

cc_library(
    name = "buckets",
    srcs = ["buckets.cpp"],
    hdrs = ["buckets.h",
             ],
    visibility = ["//visibility:public"],
     
    deps = [
        "//includes:dbg",
        "//includes:benchmark-cpp",
        "//includes:cuckoo",
        "//includes/create_structs:create_structs",
    ],
    #copts = ["-DDEBUG"], 
)



cc_library(
    name = "mpfss_cuckoo_lib",
    srcs = [
        "mpfss_cuckoo.cpp",
    ],
    hdrs =[
        "mpfss_cuckoo.h"
    ],

    visibility = ["//visibility:public"],

    deps = [
        "//includes:util",
        "//includes:dbg",
        "//includes:cuckoo",
        ":buckets",
        ":mpfss_cuckoo_obliv",
        "//includes/create_structs:create_structs",
        "@mpc_utils//mpc_utils:status",
        "@abseil//absl/types:span",

    ],

    #copts = ["-DDEBUG"], 
)

cc_test(
    name="mpfss_cuckoo_test",
    srcs = ["testing/mpfss_cuckoo_test.cpp"],
    deps = [
        "//includes:dbg",
        "@com_google_googletest//:gtest_main",
        ":mpfss_cuckoo_lib"
    ],
    #copts = ["-DDEBUG"], 
)

cc_test(
    name = "buckets_test",
    srcs = ["testing/buckets_test.cpp"],
    visibility = ["//visibility:public"],
    deps = [
        "//includes:dbg",
        "//includes:util",
        "//includes:cuckoo",
        ":buckets",
        "//includes/create_structs:create_structs",
        "@com_google_googletest//:gtest_main",
    ],
   #copts = ["-DDEBUG"], 
)
