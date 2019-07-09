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
            url = "https://github.com/nelhage/rules_boost/archive/691a53dd7dc4fb8ab70f61acad9b750a1bf10dc6.zip",
            sha256 = "5837d6bcf96c60dc1407126e828287098f91a8c69d8c2ccf8ebb0282ed35b401",
            strip_prefix = "rules_boost-691a53dd7dc4fb8ab70f61acad9b750a1bf10dc6",
        )
    if "com_github_schoppmp_rules_oblivc" not in native.existing_rules():
        http_archive(
            name = "com_github_schoppmp_rules_oblivc",
            sha256 = "7727be81abceab5e31c30319c14a414a6392bf6527bb62b1a53252b299de3b21",
            url = "https://github.com/schoppmp/rules_oblivc/archive/master.zip",
            strip_prefix = "rules_oblivc-master",
        )
    if "rules_foreign_cc" not in native.existing_rules():
        http_archive(
            name = "rules_foreign_cc",
            url = "https://github.com/bazelbuild/rules_foreign_cc/archive/8648b0446092ef2a34d45b02c8dc4c35c3a8df79.zip",
            strip_prefix = "rules_foreign_cc-8648b0446092ef2a34d45b02c8dc4c35c3a8df79",
            sha256 = "20b47a5828d742715e1a03a99036efc5acca927700840a893abc74480e91d4f9",
        )

    # New dependencies.
    if "mpc_utils" not in native.existing_rules():
        http_archive(
            name = "mpc_utils",
            url = "https://github.com/schoppmp/mpc-utils/archive/737cbea41bfd40ea48c29266b27c12ff888b6d47.zip",
            sha256 = "e4deedc4193e2abd53290f04460ca7c4665e6c556f5132631ef572dc1a59effa",
            strip_prefix = "mpc-utils-737cbea41bfd40ea48c29266b27c12ff888b6d47",
        )


    # Transitive dependency of io_bazel_rules_docker
    if "bazel_skylib" not in native.existing_rules():
        http_archive(
            name = "bazel_skylib",
            sha256 = "c33a54ef16961e48df7d306a67ccb92c0c4627d0549df519e07533a6f3d270aa",
            strip_prefix = "bazel-skylib-9b85311ab47548ec051171213a1b3b4b3b3c9dc8",
            url = "https://github.com/bazelbuild/bazel-skylib/archive/9b85311ab47548ec051171213a1b3b4b3b3c9dc8.zip",
        )



    if "io_bazel_rules_docker" not in native.existing_rules(): 
        http_archive(
        name = "io_bazel_rules_docker",
        sha256 = "aed1c249d4ec8f703edddf35cbe9dfaca0b5f5ea6e4cd9e83e99f3b0d1136c3d",
        strip_prefix = "rules_docker-0.7.0",
        urls = ["https://github.com/bazelbuild/rules_docker/archive/v0.7.0.tar.gz"],
        )
    
    
    
    if "com_github_nelhage_rules_boost" not in native.existing_rules(): 
        http_archive(
        name = "boost_nelhage",
        remote = "https://github.com/nelhage/rules_boost/archive/master.tar.gz",
    )
