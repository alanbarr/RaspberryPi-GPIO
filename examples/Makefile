CC=gcc
AR=ar
CCFLAGS=-Wall -Werror -g -I../include
LD_FLAGS=--static -L$(LIB_PATH) 

LIB_BASE_NAME=rpigpio
LIB_NAME=librpigpio.a
LIB_PATH=../library
LIB_MAKE_PATH=../src

VPATH= $(LIB_PATH) $(OUTDIR)

OUTDIR= output

all: dirs gpio_example_output.exe     \
		  gpio_example_input.exe      \
		  i2c_example_bitexpander.exe \
		  i2c_example_eeprom.exe      \
		  i2c_example_temp_sensor.exe \

%.exe: %.c $(LIB_NAME)
	$(CC) $(CCFLAGS) $(LD_FLAGS) -o $(OUTDIR)/$@ \
									$<			 \
									-l$(LIB_BASE_NAME)

$(LIB_NAME):
	cd $(LIB_MAKE_PATH); make;

dirs:
	test -d $(OUTDIR) || mkdir $(OUTDIR);

clean:
	-rm $(OUTDIR)/*;
	-rmdir $(OUTDIR);
