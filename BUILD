#load("@io_bazel_rules_docker//python:image.bzl", "py_image")
load("@io_bazel_rules_docker//cc:image.bzl", "cc_image")
load(
    "@io_bazel_rules_docker//container:container.bzl",
    "container_image",
)

cc_image(
    name = "run_cuckoo_cc_image",
   # base= ":base_with_volume",
    binary = ":run_cuckoo_cc",
)

container_image(
    name = "base_with_volume",
    volumes=["/mnt"],

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