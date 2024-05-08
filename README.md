# memops-miyoo-benchmark
 Benchmarking for memory ops for Miyoo/Sigmastar

# Instructions:

```
git clone https://github.com/shauninman/union-miyoomini-toolchain.git
cd union-miyoomini-toolchain
make shell
git clone https://github.com/XK9274/memops-miyoo-benchmark.git
git config --global --add safe.directory /root/workspace/memops-miyoo-benchmark
git config --global --add safe.directory /root/workspace/memops-miyoo-benchmark/neon-arm-library-miyoo
cd memops-miyoo-benchmark
./build.sh
```

# Example:

```
Standard memcpy for 1 MB took: 0.005127 seconds
NEON memcpy for 1 MB took: 0.001919 seconds
Standard memset for 1 MB took: 0.000787 seconds

MI_SYS_MemcpyPa for 1 MB took: 0.000037 seconds
MI_SYS_MemsetPa for 1 MB took: 0.000048 seconds

Standard memcpy for 2 MB took: 0.006533 seconds
NEON memcpy for 2 MB took: 0.003267 seconds
Standard memset for 2 MB took: 0.000926 seconds

MI_SYS_MemcpyPa for 2 MB took: 0.000063 seconds
MI_SYS_MemsetPa for 2 MB took: 0.000054 seconds

Standard memcpy for 3 MB took: 0.008923 seconds
NEON memcpy for 3 MB took: 0.006058 seconds
Standard memset for 3 MB took: 0.001716 seconds

MI_SYS_MemcpyPa for 3 MB took: 0.000035 seconds
MI_SYS_MemsetPa for 3 MB took: 0.000031 seconds

Standard memcpy for 4 MB took: 0.010935 seconds
NEON memcpy for 4 MB took: 0.006039 seconds
Standard memset for 4 MB took: 0.001349 seconds

MI_SYS_MemcpyPa for 4 MB took: 0.000041 seconds
MI_SYS_MemsetPa for 4 MB took: 0.000030 seconds

Standard memcpy for 5 MB took: 0.013172 seconds
NEON memcpy for 5 MB took: 0.007661 seconds
Standard memset for 5 MB took: 0.001686 seconds

MI_SYS_MemcpyPa for 5 MB took: 0.000062 seconds
MI_SYS_MemsetPa for 5 MB took: 0.000029 seconds
```
