# Doc

Android toolchain wrapper that make uses of

- clang toolchain
- ndk sysroot
- ndk clang resource dir

to compile android native code without using ndk.

## Configure

alpine

```sh
apk add clang lld
```

debian clang++ version >= 16 works

```sh
apt install clang-16 lld-16
ln -snf /usr/bin/clang-16 /usr/local/bin/clang
ln -snf /usr/bin/clang++-16 /usr/local/bin/clang++
ln -snf /usr/bin/ld.lld-16 /usr/local/bin/ld.lld
```

## Test

```sh
./bin/aarch64-linux-android21-clang tests/hello.c -o helloc
./bin/aarch64-linux-android21-clang++ tests/hello.cpp -o hellocpp
```

Test with different clang/clang++

```sh
CLANG=clang-17 ./bin/aarch64-linux-android21-clang tests/hello.c -o helloc
CLANGXX=clangxx-17 ./bin/aarch64-linux-android21-clang++ tests/hello.cpp -o hellocpp
```
