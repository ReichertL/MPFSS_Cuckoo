load(
    "@bazel_tools//tools/build_defs/repo:http.bzl",
    "http_archive",
)


load("//masterarbeit/code_master:preload.bzl", "code_master_preload")

code_master_preload()

load("//masterarbeit/code_master:deps.bzl", "code_master_deps")

code_master_deps()

load("//semesterprojekt/code_projekt:preload.bzl", "code_projekt_preload")

code_projekt_preload()

load("//semesterprojekt/code_projekt:deps.bzl", "code_projekt_deps")

code_projekt_deps()


#local_repository(
#  # Name of the Abseil repository. This name is defined within Abseil's
#  # WORKSPACE file, in its `workspace()` metadata
#  name = "absl",
#
#  # NOTE: Bazel paths must be absolute paths. E.g., you can't use ~/Source
#  path = "/home/turing/abseil-cpp",
#)

# new_local_repository(
#     name = "cryptopp",
#     path = "/home/turing/TI/includes/cryptopp-master",
#     build_file = "cryptopp.BUILD",
# )
