load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")
#load("@io_bazel_rules_docker//cc:image.bzl", "cc_image")

#cc_image(
#    name = "cuckoo_image",
#    binary = ":mpfss_cuckoo",
#)


cc_binary(
    name = "interface",
    srcs = [
        "mpfss_cuckoo_interface.cpp",

    
    ],

    visibility = ["//visibility:public"],

    deps = [
        "//includes:util",
        "//includes:dbg",
        "//code_master/mpfss_cuckoo:mpfss_cuckoo_lib",
        "@mpc_utils//mpc_utils:status",
        "@abseil//absl/types:span",

    ],

    #copts = ["-DDEBUG"], 
)



