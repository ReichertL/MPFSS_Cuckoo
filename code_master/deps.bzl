load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive",)
load(
    "@rules_foreign_cc//:workspace_definitions.bzl",
    "rules_foreign_cc_dependencies",
)
load("@mpc_utils//mpc_utils:deps.bzl", "mpc_utils_deps")
load(
    "@io_bazel_rules_docker//repositories:repositories.bzl",
    container_repositories = "repositories",
)
load("@io_bazel_rules_docker//repositories:deps.bzl", container_deps = "deps")
load( "@io_bazel_rules_docker//cc:image.bzl", _cc_image_repos = "repositories",)
load(
    "@io_bazel_rules_docker//container:container.bzl",
    "container_pull",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

# Sanitize a dependency so that it works correctly from code that includes
# this workspace as a git submodule.
def clean_dep(dep):
    return str(Label(dep))

def code_master_deps():
    rules_foreign_cc_dependencies()
    mpc_utils_deps()
    container_repositories()
    container_deps()
    _cc_image_repos()
    boost_deps()

    if "ack" not in native.existing_rules():
        http_archive(
            name = "ack",
            url = "https://bitbucket.org/jackdoerner/absentminded-crypto-kit/get/5230cef0887e.zip",
            sha256 = "133bfb1d5fdc2def0c32047e9f9288f71354ec56e003199b8bc516fb7a927fca",
            strip_prefix = "jackdoerner-absentminded-crypto-kit-5230cef0887e/src",
            build_file = clean_dep("//third_party:ack.BUILD"),
        )

    if "com_google_absl" not in native.existing_rules(): 
        # abseil-cpp
        http_archive(
          name = "com_google_absl",
          urls = ["https://github.com/abseil/abseil-cpp/archive/7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72.zip"],  # 2019-03-14
          strip_prefix = "abseil-cpp-7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72",
          sha256 = "71d00d15fe6370220b6685552fb66e5814f4dd2e130f3836fc084c894943753f",
        )


    if "com_google_googletest" not in native.existing_rules(): 
        # Google Test
        http_archive(
          name = "com_google_googletest",
          urls = ["https://github.com/google/googletest/archive/8b6d3f9c4a774bef3081195d422993323b6bb2e0.zip"],  # 2019-03-05
          strip_prefix = "googletest-8b6d3f9c4a774bef3081195d422993323b6bb2e0",
          sha256 = "d21ba93d7f193a9a0ab80b96e8890d520b25704a6fac976fe9da81fffb3392e3",
		    )




    # Something needs a recent GlibC, which is not included in standard distroless images.
    container_pull(
        name = "distroless_base",
        digest = "sha256:c6f196cb2cd479d2ed2986dcb8365f52ebf6f22fc05d63aa77a51fc6725d082f",
        registry = "index.docker.io",
        repository = "schoppmp/distroless-arch",
    )
