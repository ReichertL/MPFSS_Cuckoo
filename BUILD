load("@io_bazel_rules_docker//cc:image.bzl", "cc_image")
load(   "@io_bazel_rules_docker//container:container.bzl", "container_image",)
load("@io_bazel_rules_docker//container:bundle.bzl", "container_bundle",)
load( "@io_bazel_rules_docker//container:push.bzl", "container_push",)

#-------standalone--------------------------------------------------
container_push(
   name = "push_standalone",
   image = ":run_standalone_image",
   format = "Docker",
   registry = "gitlab.informatik.hu-berlin.de:4567",
   repository = "ti/theses/student-content/reichert-leonie-ma/mpfss_cuckoo",
   tag= "run_standalone_image"
)

cc_image(
    name = "run_standalone_image",
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
   repository = "ti/theses/student-content/reichert-leonie-ma/mpfss_cuckoo",
   tag= "test_standalone_image"
)

cc_image(
    name = "test_standalone_image",
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

#---------------------single party--------------------------------------------------
container_push(
   name = "push_single_party",
   image = ":run_cuckoo_single_party_image",
   format = "Docker",
   registry = "gitlab.informatik.hu-berlin.de:4567",
   repository = "ti/theses/student-content/reichert-leonie-ma/mpfss_cuckoo",
   tag= "run_cuckoo_single_party_image"
)

cc_image(
    name = "run_cuckoo_single_party_image",
    binary = ":run_cuckoo_single_party",
)

cc_binary(
    name = "run_cuckoo_single_party",
    srcs = [ "run_cuckoo_single_party.cpp" ],
    visibility = ["//visibility:public"],
    deps = [
        "//code_master/mpfss_cuckoo:mpfss_cuckoo_lib"

    ],
    #copts = ["-DTESTING"], 
)



#--------------------single party test---------------------------------------------
container_push(
   name = "push_test_single_party",
   image = ":test_single_party_image",
   format = "Docker",
   registry = "gitlab.informatik.hu-berlin.de:4567",
   repository = "ti/theses/student-content/reichert-leonie-ma/mpfss_cuckoo",
   tag= "test_single_party_image"
)

cc_image(
    name = "test_single_party_image",
    binary = ":run_cuckoo_single_party_test",
)

cc_binary(
    name = "run_cuckoo_single_party_test",
    srcs = [ "run_cuckoo_single_party.cpp" ],
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
   repository = "ti/theses/student-content/reichert-leonie-ma/mpfss_cuckoo",
   tag= "threads"
)

cc_image(
    name = "threads_single_party",
    binary = ":threads",
)

cc_binary(
    name = "threads",
    srcs = [ "run_cuckoo_single_party_threads.cpp" ],
    visibility = ["//visibility:public"],
    deps = [
        "//code_master/mpfss_cuckoo:mpfss_cuckoo_lib"

    ],
)



#--------------------dpf---------------------------------------------
container_push(
   name = "push_dpf",
   image = ":dpf_image",
   format = "Docker",
   registry = "gitlab.informatik.hu-berlin.de:4567",
   repository = "ti/theses/student-content/reichert-leonie-ma/mpfss_cuckoo",
   tag= "dpf"
)

cc_image(
    name = "dpf_image",
    binary = ":dpf",
)

cc_binary(
    name = "dpf",
    srcs = [ "run_dpf.cpp" ],
    visibility = ["//visibility:public"],
    deps = [
        "//includes/mpfss_naive:mpfss_naive"

    ],
)





#cc_image(
#    name = "run_cuckoo_p1",
#    binary = ":run_cuckoo_single_party",
#   args = [
#        "localhost:88888",
#        "1"
#   
#    ],
#)

#cc_image(
#    name = "run_cuckoo_p2",
#    binary = ":run_cuckoo_single_party",
#    args = [
#        "localhost:88888",
#        "2"   
#    ],
#)








