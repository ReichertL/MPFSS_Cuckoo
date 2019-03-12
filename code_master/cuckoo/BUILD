load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")


oblivc_library(
    name = "mpfss_cuckoo_obliv",
    srcs = [
        
        "mpfss_cuckoo.oc",

    ],
    hdrs = [
        "mpfss_cuckoo.oh",
        "mpfss_cuckoo.h",

    ],
    deps = [
        "//code_master/includes:util",
        "//code_master/includes:dbg",
        "@ack//:oram_fssl",
    ],
    copts = ["-DDEBUG"], 
)


cc_binary(
    name = "mpfss_cuckoo",
    srcs = [
        "mpfss_cuckoo.c",
    ],

    visibility = ["//visibility:public"],

    deps = [
        "//code_master/includes:util",
        "//code_master/includes:dbg",
        ":mpfss_cuckoo_obliv",
    ],

    copts = ["-DDEBUG"], 
)