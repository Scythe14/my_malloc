# my_malloc

## Details

I realized this project with another student (Heru Abollo) during my second year at EPITECH (2014-2015).

## Usage

Run the Makefile to create the shared library : libmy_malloc_[MACHINE].so
A link to this generated shared library will be created : libmy_malloc.so

```
make
```
To run a program with the malloc you just generate, LD_PRELOAD is required.

```
LD_PRELOAD=./libmy_malloc.so ls -R / --color
```