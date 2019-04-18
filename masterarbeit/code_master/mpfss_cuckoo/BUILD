load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")

cc_binary(
    name = "mpfss_cuckoo",
    srcs = [
        "mpfss_cuckoo.cpp",
    
    ],

    visibility = ["//visibility:public"],

    deps = [
        "//includes:util",
        "//includes:dbg",
        "//includes:cuckoo",
        ":buckets",
        ":mpfss_cuckoo_obliv",
        ":create_structs",

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
    ],
    deps = [
        "//includes:util",
        "//includes:dbg",
        "//semesterprojekt/code_projekt/mpfss_naive:mpfss_naive_obliv",
        ":create_structs"
        
    ],
    copts = ["-DDEBUG"], 
)

cc_library(
    name = "buckets",
    srcs = ["buckets.cpp"],
    hdrs = ["buckets.h",
             ],
    deps = [
        "//includes:dbg",
        "//includes:cuckoo",
        ":create_structs",
    ],
    copts = ["-DDEBUG"], 
)

oblivc_library(
    name = "create_structs",
    srcs = ["create_structs.c"],
    hdrs = ["create_structs.h",
           ],
    deps = [

    ],
    copts = ["-DDEBUG"], 
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
        ":create_structs",

    ],

    copts = ["-DDEBUG"], 
)

cc_test(
    name="mpfss_cuckoo_test",
    srcs = ["testing/mpfss_cuckoo_test.cpp"],
    deps = [
        "//includes:dbg",
        "@com_google_googletest//:gtest_main",
        ":mpfss_cuckoo_lib"
    ],
    copts = ["-DDEBUG"], 


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
        ":create_structs",
        "@com_google_googletest//:gtest_main",
    ],
   copts = ["-DDEBUG"], 
)
