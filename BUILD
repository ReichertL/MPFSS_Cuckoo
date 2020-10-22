load("@io_bazel_rules_docker//cc:image.bzl", "cc_image")
load("@io_bazel_rules_docker//container:container.bzl", "container_bundle")
load("@io_bazel_rules_docker//container:push.bzl", "container_push")

#-------standalone--------------------------------------------------
container_push(
   name = "push_standalone",
   image = ":run_standalone_image",
   format = "Docker",
   registry = "gitlab.informatik.hu-berlin.de:4567",
   repository = "schopmaf/mpfss_cuckoo",
   tag= "run_standalone_image"
)

cc_image(
    name = "run_standalone_image",
    base = "@distroless_base//image",
    binary = ":run_cuckoo",
)

cc_binary(
    name = "run_cuckoo",
    srcs = [ "run_cuckoo.cpp" ],
    visibility = ["//visibility:public"],
    deps = [
        "//code_master/mpfss_cuckoo:mpfss_cuckoo_lib"

    ],
)

#-------------------test standalone--------------------------------------------------

container_push(
   name = "push_test_standalone",
   image = ":test_standalone_image",
   format = "Docker",
   registry = "gitlab.informatik.hu-berlin.de:4567",
   repository = "schopmaf/mpfss_cuckoo",
   tag= "test_standalone_image"
)

cc_image(
    name = "test_standalone_image",
    base = "@distroless_base//image",
    binary = ":run_cuckoo_test",
)

cc_binary(
    name = "run_cuckoo_test",
    srcs = [ "run_cuckoo.cpp" ],
    visibility = ["//visibility:public"],
    deps = [
        "//code_master/mpfss_cuckoo:mpfss_cuckoo_lib"

    ],
    copts = ["-DTESTING"], 
)


#--------------------single party threads---------------------------------------------
container_push(
   name = "push_threads",
   image = ":threads_single_party",
   format = "Docker",
   registry = "gitlab.informatik.hu-berlin.de:4567",
   repository = "schopmaf/mpfss_cuckoo",
   tag= "threads"
)

cc_image(
    name = "threads_single_party",
    base = "@distroless_base//image",
    binary = ":threads",
)

cc_binary(
    name = "threads",
    srcs = [ "run_cuckoo_single_party_threads.cpp" ],
    visibility = ["//visibility:public"],
    deps = [
        "//code_master/mpfss_cuckoo:mpfss_cuckoo_lib"

    ],
    #copts=["-DDEBUG"],
)

#--------------------new single party threads---------------------------------------------
container_push(
    name = "push_threads_new",
    image = ":threads_new_single_party",
    format = "Docker",
    registry = "gitlab.informatik.hu-berlin.de:4567",
    repository = "schopmaf/mpfss_cuckoo",
    tag= "threads_new"
)

cc_image(
    name = "threads_new_single_party",
    base = "@distroless_base//image",
    binary = ":threads_new",
)

cc_binary(
    name = "threads_new",
    srcs = [ "run_cuckoo_new_single_party_threads.cpp" ],
    visibility = ["//visibility:public"],
    deps = [
        "//code_master/mpfss_new_cuckoo:mpfss_new_cuckoo_lib",
        "@com_google_absl//absl/types:span",

    ],

    copts = ["-fopenmp"], 
    linkopts=["-fopenmp"],
)

#--------------------naive single party threads---------------------------------------------
container_push(
    name = "push_naive",
    image = ":image_naive",
    format = "Docker",
    registry = "gitlab.informatik.hu-berlin.de:4567",
    repository = "schopmaf/mpfss_cuckoo",
    tag= "threads_naive"
)

cc_image(
    name = "image_naive",
    base = "@distroless_base//image",
    binary = ":naive",
)

cc_binary(
    name = "naive",
    srcs = [ "run_naive_threads.cpp" ],
    visibility = ["//visibility:public"],
    deps = [
        "//includes/mpfss_naive:mpfss_naive_obliv",
    ],

    copts = ["-fopenmp"], 
    linkopts=["-fopenmp"],
)

#--------------------dpf---------------------------------------------
container_push(
   name = "push_dpf",
   image = ":dpf_image",
   format = "Docker",
   registry = "gitlab.informatik.hu-berlin.de:4567",
   repository = "schopmaf/mpfss_cuckoo",
   tag= "dpf"
)

cc_image(
    base = "@distroless_base//image",
    name = "dpf_image",
    binary = ":dpf",
)

cc_binary(
    name = "dpf",
    srcs = [ "run_dpf.cpp" ],
    visibility = ["//visibility:public"],
    deps = [
        "//includes/mpfss_naive:mpfss_naive_obliv"

    ],
)










