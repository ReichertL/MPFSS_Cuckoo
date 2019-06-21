load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")

oblivc_library(
    name = "fss_cprg_altered",
    srcs = [
        
        "fss_cprg_altered.oc",
    ],
    hdrs = [
        "fss_cprg_altered.oh",

    ],
    deps = [
        "@ack//:oram_fssl",
        "@ack//:ackutil",
        ":fss_cprg_altered_c",


    ],
    visibility = ["//visibility:public"],
    #copts = ["-DDEBUG -g3"], 
    copts = ["-fopenmp"], 
    linkopts=["-fopenmp"],
)

cc_library(
    name = "fss_cprg_altered_c",
    srcs = [
        
        "fss_cprg_altered.c",
    ],
    hdrs = [
        "fss_cprg_altered.h",

    ],
    deps = [
        "@ack//:oram_fssl",
        "@ack//:ackutil",

    ],
    visibility = ["//visibility:private"],
    #copts = ["-DDEBUG -g3"], 
    copts = ["-fopenmp"], 
    linkopts=["-fopenmp"],

    )


oblivc_library(
    name = "fss_altered",
    srcs = [
        
        "fss_altered.oc",
    ],
    hdrs = [
        "fss_altered.oh",

    ],
    deps = [
        "@ack//:oram_fssl",
        "@ack//:ackutil",
        ":fss_cprg_altered_c",


    ],
    visibility = ["//visibility:public"],
    #copts = ["-DDEBUG -g3"], 
    copts = ["-fopenmp"], 
    linkopts=["-fopenmp"],
)

cc_library(
    name = "fss_altered_c",
    srcs = [
        
        "fss_altered.c",
    ],
    hdrs = [
        "fss_altered.h",

    ],
    deps = [
        "@ack//:oram_fssl",
        "@ack//:ackutil",

    ],
    visibility = ["//visibility:private"],
    #copts = ["-DDEBUG -g3"], 
    copts = ["-fopenmp"], 
    linkopts=["-fopenmp"],

    )