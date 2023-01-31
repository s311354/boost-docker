## Boost CMake Examples  ##

### Build Examples ###

```bash
$ cmake -B build
$ cmake --build build --config --parallel
```

### Run Examples ###

```bash
$ echo 1 2 3 4 |  ./build/arraynexting/ArrayNexting
The longest length of a set s[k] = {nums[k], nums[nums[k]], nums[nums[nums[k]]], ... } is : 1

$ ./build/extractemail/ExtractEmail  < extractemail/data/tangled.txt
German fairy tale
```
