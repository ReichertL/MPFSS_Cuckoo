load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")

cc_binary(
    name = "mpfss_cuckoo",
    srcs = [
        "mpfss_cuckoo.cpp",
        "mpfss_cuckoo.h"
    
    ],

    visibility = ["//visibility:public"],

    deps = [
        "//includes:util",
        "//includes:dbg",
        "//includes:cuckoo",
        ":buckets",
        ":mpfss_cuckoo_obliv",

    ],

    copts = ["-DDEBUG"], 
)

oblivc_library(
    name = "mpfss_cuckoo_obliv",
    srcs = [
        
        "mpfss_cuckoo.oc",

    ],
    hdrs = [
        "mpfss_cuckoo.oh",
        "mpfss_cuckoo.h"

    ],
    deps = [
        "//includes:util",
        "//includes:dbg",
        #"@ack//:oram_fssl",
        "//semesterprojekt/code_projekt/mpfss_naive:mpfss_naive_obliv",

        
    ],
    copts = ["-DDEBUG"], 
)

cc_library(
    name = "buckets",
    srcs = ["buckets.cpp"],
    hdrs = ["buckets.h",
            "mpfss_cuckoo.h"
             ],
    deps = [
        "//includes:dbg",
        "//includes:cuckoo",
    ],
    copts = ["-DDEBUG"], 
    )





cc_test(
    name = "bucket_test",
    srcs = [    "buckets.cpp",
                "mpfss_cuckoo.h",
                "buckets.h"
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//includes:dbg",
        "//includes:cuckoo",
    ],
    copts = ["-DDEBUG"], 
    )



oblivc_library(
    name = "mpfss_naive_wrapper_obliv",
    srcs = [
        
        "mpfss_naive_wrapper.oc",

    ],
    hdrs = [
        "mpfss_naive_wrapper.h"

    ],
    deps = [
        
        "//includes:dbg",
        "//includes:list_utils",
        "//semesterprojekt/code_projekt/mpfss_naive:mpfss_naive_obliv",        
    ],
    copts = ["-DDEBUG"], 
)

