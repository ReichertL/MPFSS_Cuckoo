#load("@io_bazel_rules_docker//python:image.bzl", "py_image")
load("@io_bazel_rules_docker//cc:image.bzl", "cc_image")

cc_image(
    name = "run_cuckoo_image",
    binary = ":run_cuckoo_cc",
)

#py_binary(
#   name = "run_cuckoo",
#    srcs = ["run_cuckoo.py"],
#    data = ["//code_master/mpfss_cuckoo:mpfss_cuckoo"], 
#)

#py_image(
#    name = "run_cuckoo_image",
#    srcs = ["run_cuckoo_docker.py"],
#    main = "run_cuckoo_docker.py",
#    data = ["//code_master/mpfss_cuckoo:mpfss_cuckoo"], 
#)


cc_binary(
    name = "run_cuckoo_cc",
    srcs = [
        "run_cuckoo.cpp",    
    ],

    visibility = ["//visibility:public"],

    deps = [
        "//code_master/mpfss_cuckoo:mpfss_cuckoo_lib"

    ],

    #copts = ["-DDEBUG"], 
)