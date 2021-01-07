# Multi-Point Function Secret Sharing using Cuckoo Hashing

## Tools and Dependecies
* Obliv-C as Multi-Party Computation framework
* Absentminded Crypto Library for Distribute Point Functions (DPF) 
* Bazel for building the project
* Abseil Library for hashing

    
## Functionality 
### Function Secret Sharing
Function secret sharing (FSS) is a useful primitive for secure multi-party computation.
It creates secret shares of functions over a domain with size n. In this two party
protocol, each party receives a vector as final result. Combining these vectors gives
the input function evaluated over [0, n]. So far, FSS implementations have focused on
distributed point functions (DPF), which is the secret sharing of point functions. A
point function is unequal to zero at exactly one location.

### Multipoint Function Secret Sharing
More complex is the creation of secret shares of multi-point functions, also called
multi-point function secret sharing (MPFSS). MPFSS shares for example can be
used as source of correlated randomness in vector oblivious linear-function evaluation
(VOLE). A straightforward method to accomplish MPFSS is to run a fixed amount of
DPFs over the same domain and then combine the results.
Boyle et al. proposed a technique optimizing the creation of MPFSS through
batching. Instead of the simple approach, the domain is split into buckets with lengths
significantly shorter than n. Indices of the multi-point function are assigned to buckets.
Then one DPF per bucket is called. To solve the assignment problem, the authors
leverage the primitive of combinatorial batch codes. The basic idea of this optimization
is saving costs by having more DPF runs on shorter domains. While the authors give
estimates, they did not implement their protocol and evaluate its real world trade off.

### Performance Improvments using Cuckoo Hashing
This work focuses on a similar optimization with the main difference that instead of
applying combinatorial batch codes, cuckoo hashing is used. Cuckoo hashing allows
constant look-up time and sublinear insertion time even in the worst-case. A lot
of research has been done applying cuckoo hashing to MPC and many works have
employed it for their purposes. Here, it is used in the configuration with three hash
functions, random-walk selection and no stash.
The MPFSS protocol proposed on the basis cuckoo hashing was implemented and
evaluated. The analysis shows that it performs significantly better than the naive
approach for almost all parameters. It is faster especially for large values of domain
size and number of indices, which are relevant in practice.



Check my [master thesis](https://github.com/ReichertL/Masterthesis) or our [paper](https://eprint.iacr.org/2019/1084) for a more detailed explanation. 


## How to execute

* Install [Bazel](https://docs.bazel.build/versions/3.7.0/install.html) 

* To test MPFSS with Cuckoo hashing and multi-threading
```
    bazel build //:threads_new
    bazel run //:threads_new localhost:5555 1
    bazel run //:threads_new localhost:5555 2   #In a second shell
```
In this example, the protocol is run over `localhost` and the two shells use port `5555` to communicate. 
To take measurements using different host, simply change the `localhost` to the corresponding IP of the other party and change `5555` to the designated port of the other party. 
The last argument allows the program to know if it needs to establish a new connection or wait for incoming connections. Always start party `1` before party `2`.
To alter parameters used during execution(such as number of runs, size n of the multi-point function or number of threads)  check  `run_cuckoo_new_single_party_threads.cpp`. 


* To test a naive version of MPFSS without Cuckoo Hashing.
```
    bazel build //:naive
    bazel run //:naive localhost:5555 1
    bazel run //:naive localhost:5555 2   #In a second shell
    
``` 

To alter parameters (such as number of runs, size n of the multi-point function or number of threads)  check  `run_naive_threads.cpp`. 
    

* To test the DPF of the Absentminded Crypto Library. 
```    
    bazel build //:dpf
    bazel run //:dpf localhost:5555 1
    bazel run //:dpf localhost:5555 2   #In a second shell
``` 



To alter parameters (such as number of runs, size n of the multi-point function or number of threads)  check  `run_dpf.cpp`. 



## Using the Container Registry
To create Docker files, edit the `cc_image` statements in the top level `BUILD` file.
To automatically push these images to a GitHub/GitLab Container Registry, alter the corresponding `container_push` statements. 
Using the registry simplifies moving containers to servers for measurements. 


* Pull image from gitlab registry:

```
sudo docker pull gitlab.informatik.hu-berlin.de:4567/path/to/repository/mpfss_cuckoo:test_cuckoo_single_party
```

* Execute docker and log using `standalone` as name for the container:

```
sudo docker run -it -d --name standalone gitlab.informatik.hu-berlin.de:4567/path/to/repository/mpfss_cuckoo:test_cuckoo_single_party 52.169.2.97:5555
```

* Expose docker ports on host to take measurements between two hosts:
```
sudo docker run -it -d --net host --name standalone gitlab.informatik.hu-berlin.de:4567/path/to/repository/mpfss_cuckoo:test_cuckoo_single_party 52.169.2.97:5555
```

* Show logs of container with name `standalone`:
```
sudo docker logs standalone
```

* Terminate docker container with name `standalone`:
```
sudo docker stop standalone
```

