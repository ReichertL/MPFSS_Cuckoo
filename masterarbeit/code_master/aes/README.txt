Run executable for usage info. Example usage from root of WORKSPACE:
$  bazel build //code_master/aes
$ ./bazel-bin/code_master/aes 1235    --     80000000000000000000000000000000 &
$ ./bazel-bin/code_master/aes 1235 localhost 00000000000000000000000000000000
Result: 0edd33d3c621e546455bd8ba1418bec8

More AES test vectors can be found at: http://csrc.nist.gov/groups/STM/cavp/documents/aes/KAT_AES.zip, archive file ECBVarKey128.rsp
