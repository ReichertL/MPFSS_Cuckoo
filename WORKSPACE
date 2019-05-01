load(
    "@bazel_tools//tools/build_defs/repo:http.bzl",
    "http_archive",
)


load("//code_master:preload.bzl", "code_master_preload")

code_master_preload()

load("//code_master:deps.bzl", "code_master_deps")

code_master_deps()

load(
    "@io_bazel_rules_docker//python:image.bzl",
    _py_image_repos = "repositories",
)

_py_image_repos()

#local_repository(
#    name = "semesterprojekt",
#    path = "../semesterprojekt",
#    )