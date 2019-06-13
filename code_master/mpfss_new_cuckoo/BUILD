load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")


cc_binary(
    name = "mpfss_new_cuckoo",
    srcs = [
        "mpfss_new_cuckoo_main.cpp",
        "mpfss_new_cuckoo.cpp",
        "mpfss_new_cuckoo.h",
        "create_structs_new.h"
    
    ],

    visibility = ["//visibility:public"],

    deps = [
        "//includes:util",
        "//includes:dbg",
        "//includes:cuckoo",
        "//includes:benchmark-cpp",
        "//code_master/mpfss_cuckoo:buckets",
        ":mpfss_new_cuckoo_obliv",
        "//code_master/mpfss_cuckoo:create_structs",
        "@mpc_utils//mpc_utils:status",
        "@abseil//absl/types:span",

    ],

    copts = ["-fopenmp -DDEBUG"], 
    linkopts=["-fopenmp"],
)


oblivc_library(
    name = "mpfss_new_cuckoo_obliv",
    srcs = [
        "mpfss_new_cuckoo.oc",
    ],
    hdrs = [
        "mpfss_new_cuckoo.oh",
        "create_structs_new.h"

    ],
    visibility = ["//visibility:public"],

    deps = [
        "//includes:util",
        "//includes:dbg",
        "//includes/mpfss_naive:mpfss_naive_obliv",
        "//code_master/mpfss_cuckoo:create_structs",
        "//code_master/mpfss_cuckoo:mpfss_cuckoo_obliv",

    ],
)
