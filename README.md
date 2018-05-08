RaspberryPi-GPIO - Basic GPIO and I2C control for the Raspberry Pi.

# Documentation 
There is more detailed Doxygen produced documentation available at:
http://alanbarr.github.com/RaspberryPi-GPIO

The documentation can be generated if you have Doxygen installed. 
Change directory into documentation and run `doxygen Doxyfile`.

# Building 
To build the library type `make` within the `src` directory.
The library will be produced in the root level library directory.

# Examples 
There are a few example's of using this library in the `examples` directory.
To build, type `make` in examples directory and the output binary files
will be available in `examples/output`.

# Usage
`rpiGpio.h` should be included in your source files. This header resides in the 
root level `include` directory.
The compiler being used should know where to look for `rpiGpio.h` as well as the
library file, `librpigpio.a` when linking.

NOTE: Building and execution should be done on the Raspberry Pi itself.
