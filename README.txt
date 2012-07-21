RaspberryPi-GPIO - Basic GPIO and I2C control for the Raspberry Pi.
#################### DOCUMENTATION #####################
There is more detailed Doxygen produced documentation available in:
http://alanbarr.github.com/RaspberryPi-GPIO

The documentation can be generated if you have Doxygen installed. 
Change directory into documentation and run "doxygen Doxyfile".

#################### BUILDING ####################
To build the library type make within the src directory.
The library will be produced in the root level library directory.

#################### EXAMPLES #################### 
There are a few example's of using this library in examples.
To make the source, type make in examples directory and the output binary files
will be available in examples/output.

#################### USAGE #####################                               
rpiGpio.h should be included in your source files. This header resides in the 
root level include directory.
The compiler being used should know where to look for rpiGpio.h as well as the 
library file, librpigpio.a when linking.

NOTE: Building and execution should be done on the Raspberry Pi itself.
