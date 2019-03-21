load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")


cc_library(
    name = "dbg",
    hdrs = ["dbg.h"],
    visibility = ["//visibility:public"],
)

oblivc_library(
    name = "util",
    srcs = ["util.c"],
    hdrs = ["util.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":dbg",
    ],
    copts = ["-DDEBUG"], 
)
cc_library(
    name = "list_utils",
    srcs = ["list_utils.c"],
    hdrs = ["list_utils.h"],
    visibility = ["//visibility:public"],
)
cc_library(
    name = "linked_list",
    srcs = ["linked_list.c"],
    hdrs = ["linked_list.h"],
    visibility = ["//visibility:public"],
)
cc_library(
    name = "binary_search_tree",
    srcs = ["binary_search_tree.c"],
    hdrs = ["binary_search_tree.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":dbg",
    ],
    copts = ["-DDEBUG "], 
)

cc_library(
    name = "cuckoo",
    srcs = ["cuckoo.cpp"],
    hdrs = ["cuckoo.h" ],
    visibility = ["//visibility:public"],
    deps = [
        ":dbg",
        "@absl//absl/hash",
    ],
    copts = ["-DDEBUG"], 
    )