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
    name = "cuckoo",
    srcs = ["cuckoo/cuckoo.cpp"],
    hdrs = ["cuckoo/cuckoo.h" ],
    visibility = ["//visibility:public"],
    deps = [
        ":dbg",
        "@com_google_absl//absl/hash"
    ],
    #copts = ["-DDEBUG"], 
    )

cc_test(
    name = "cuckoo_test",
    srcs = ["cuckoo/cuckoo_test.cpp"],
    visibility = ["//visibility:public"],
    deps = [
        ":dbg",
        "@com_google_absl//absl/hash:hash_testing",
        ":cuckoo",
        "@com_google_googletest//:gtest_main",


    ],
   # copts = ["-DDEBUG"], 
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
