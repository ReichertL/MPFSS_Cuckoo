load(
    "@bazel_tools//tools/build_defs/repo:http.bzl",
    "http_archive",
)

# Dependencies that need to be defined before :deps.bzl can be loaded.
# Copy those in a similar preload.bzl file in any workspace that depends on
# this one.
def code_master_preload():
    # Transitive dependencies of mpc_utils.
    if "com_github_nelhage_rules_boost" not in native.existing_rules():
        http_archive(
            name = "com_github_nelhage_rules_boost",
            strip_prefix = "rules_boost-ccc90b00c2fae7267cc25b77199fb992acd5e799",
            url = "https://github.com/nelhage/rules_boost/archive/ccc90b00c2fae7267cc25b77199fb992acd5e799.zip",
            sha256 = "68c67476f8e6257cb858a2378a4663a54690ed64562db2c3be4702c537e1edd8",
        )
    if "com_github_schoppmp_rules_oblivc" not in native.existing_rules():
        http_archive(
            name = "com_github_schoppmp_rules_oblivc",
            sha256 = "10f53f0fab3e374fdef83dc4279d902a357993a87c91d6de3c30e64db59f87ee",
            url = "https://github.com/schoppmp/rules_oblivc/archive/d33ab3a707b9d6e9a79a683e660e572ab8e92f16.zip",
            strip_prefix = "rules_oblivc-d33ab3a707b9d6e9a79a683e660e572ab8e92f16",
        )
    if "rules_foreign_cc" not in native.existing_rules():
        http_archive(
            name = "rules_foreign_cc",
            url = "https://github.com/bazelbuild/rules_foreign_cc/archive/9eb30f8c5a214799b73707666ca49e7b7a35978f.zip",
            strip_prefix = "rules_foreign_cc-9eb30f8c5a214799b73707666ca49e7b7a35978f",
            sha256 = "0d82c6e7fcf623885a47ce52c2e30321aa0b868ff3b9fedfd6670541fc5874ad",
        )

    # Transitive dependency of io_bazel_rules_docker
    if "bazel_skylib" not in native.existing_rules():
        http_archive(
            name = "bazel_skylib",
            strip_prefix = "bazel-skylib-8f3151fb4a91d5f2ae4cad5901ea72fe30a2aba0",
            urls = ["https://github.com/bazelbuild/bazel-skylib/archive/8f3151fb4a91d5f2ae4cad5901ea72fe30a2aba0.zip"],
            sha256 = "34c84c459569d2618694e3c48e5fa21d326cc1333e51eed25fe8aa4be6fbbbd5",
        )
    if "bazel_gazelle" not in native.existing_rules():
        http_archive(
            name = "bazel_gazelle",
            strip_prefix = "bazel-gazelle-6eaa2eac3d92c17e1607c280c91251774471ab8f",
            urls = ["https://github.com/bazelbuild/bazel-gazelle/archive/6eaa2eac3d92c17e1607c280c91251774471ab8f.zip"],
            sha256 = "1bce24b1fc3239200fbf1c158a1603bce1675c1233927cc37e76f2258e8b8d47",
        )
    if "io_bazel_rules_go" not in native.existing_rules():
        http_archive(
            name = "io_bazel_rules_go",
            strip_prefix = "rules_go-03e0fda7076cbae926fe7699c5236e57165e7a1c",
            urls = ["https://github.com/bazelbuild/rules_go/archive/03e0fda7076cbae926fe7699c5236e57165e7a1c.zip"],
            sha256 = "4f29f7788ec95689779d09ab51a4d8125e52747f9fa2df68c866b0a58ff0d983",
        )
    if "rules_python" not in native.existing_rules():
        http_archive(
            name = "rules_python",
            url = "https://github.com/bazelbuild/rules_python/releases/download/0.1.0/rules_python-0.1.0.tar.gz",
            sha256 = "b6d46438523a3ec0f3cead544190ee13223a52f6a6765a29eae7b7cc24cc83a0",
        )

    # New dependencies.
    if "mpc_utils" not in native.existing_rules():
        http_archive(
            name = "mpc_utils",
            sha256 = "e73613d2e81a9de3471b82f821cd6d8752b5c56aa766c995ad8c00fd7d145144",
            strip_prefix = "mpc-utils-f2b8c1723db6cc149602652dd1c47bfe1e7dea21",
            url = "https://github.com/schoppmp/mpc-utils/archive/f2b8c1723db6cc149602652dd1c47bfe1e7dea21.zip",
        )
    if "io_bazel_rules_docker" not in native.existing_rules():
        http_archive(
            name = "io_bazel_rules_docker",
            strip_prefix = "rules_docker-cc45596d140b3b8651eb7b51b561f1bf72d1eea9",
            urls = ["https://github.com/bazelbuild/rules_docker/archive/cc45596d140b3b8651eb7b51b561f1bf72d1eea9.zip"],
        )