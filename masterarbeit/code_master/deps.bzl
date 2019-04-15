load(
    "@bazel_tools//tools/build_defs/repo:http.bzl",
    "http_archive",
)
load("@mpc_utils//mpc_utils:deps.bzl", "mpc_utils_deps")

# Sanitize a dependency so that it works correctly from code that includes
# this workspace as a git submodule.
def clean_dep(dep):
    return str(Label(dep))

def code_master_deps():
    mpc_utils_deps()

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


    # if "cryptopp" not in native.existing_rules(): 
    #     http_archive(
    #         name = "cryptopp",
    #         url = "https://github.com/weidai11/cryptopp/archive/master.zip",
    #         strip_prefix= "cryptopp-master",
    #         build_file = clean_dep("//third_party:cryptopp.BUILD"),
    #     )