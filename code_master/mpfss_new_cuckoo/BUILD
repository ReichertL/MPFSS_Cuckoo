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
        "//includes/create_structs:create_structs",
        "@mpc_utils//mpc_utils:status",
        "@abseil//absl/types:span",

    ],

    copts = ["-fopenmp"], 
    linkopts=["-fopenmp"],
)
cc_library(
    name = "mpfss_new_cuckoo_lib",
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
        "//includes/create_structs:create_structs",
        "@mpc_utils//mpc_utils:status",
        "@abseil//absl/types:span",

    ],

    copts = ["-fopenmp"], 
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
        "//includes/create_structs:create_structs",
        "//code_master/mpfss_cuckoo:mpfss_cuckoo_obliv",
        "//includes:secret_share_beta",

    ],
)
