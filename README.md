# TM2IN
Generate appropriate Solid geometry for Indoor Map from Triangle Mesh data such as TVR, COLLADA, 3DS. 

## Geometric Models


## Author
Dongmin Kim, Pusan National University

## Dependencies


## Building

Dependencies should be pre-installed in your environment.

```
git clone https://github.com/STEMLab/TM2IN
mkdir build
cd build
cmake ..
make
```

## Usage

```
TM2IN [options] [input_file_name]

```

For example,

```
TM2IN --input-dir=../../input/ --output-dir=../../result --version=0.4.2 inputfile.tvr
```


There are more informations about options. (http://github.com/STEMLab/TM2IN/tree/master/doc/usage-manual.md) 


