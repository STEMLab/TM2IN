# TM2IN
Generate appropriate Solid geometry for Indoor Map from Triangle Mesh data such as TVR, COLLADA, 3DS. 

## Author
Dongmin Kim, Pusan National University

## Dependencies
CGAL
SFCGAL
Eigen3
Boost
rapidxml

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

[More information about options.](https://github.com)