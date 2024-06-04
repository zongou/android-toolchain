# Doc

## Configure

alpine

```sh
apk add clang lld
```

debian(testing)

```sh
apt install clang-17 lld-17
ln -snf /usr/bin/clang-17 /usr/bin/clang
ln -snf /usr/bin/clang++-17 /usr/bin/clang++
ln -snf /usr/bin/ld.lld-17 /usr/bin/ld.lld
```

## Test

```sh
./bin/aarch64-linux-android21-clang tests/hello.c -o helloc
./bin/aarch64-linux-android21-clang++ tests/hello.cpp -o hellocpp
```
