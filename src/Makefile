# This make file creates a library for the gpio pins.
CC=gcc
CCFLAGS=-Wall -Werror -g -I../include -I. -Iinc -pedantic

AR=ar
ARFLAGS=-rcs

OUT_DIR=output
LIB_DIR=../library
LIB_NAME=librpigpio.a

VPATH= $(OUT_DIR) $(LIB_DIR)

OBJS= gpio.o i2c.o pwm.o clocks.o
OUT_OBJS=$(addprefix $(OUT_DIR)/,$(OBJS))

all: dirs $(LIB_NAME)

$(LIB_NAME): $(OUT_OBJS)
	$(AR) $(ARFLAGS) $(LIB_DIR)/$@ $^

$(OUT_DIR)/%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

dirs:
	test -d $(OUT_DIR) || mkdir $(OUT_DIR);
	test -d $(LIB_DIR) || mkdir $(LIB_DIR);

clean:
	-rm $(OUT_DIR)/*.o;
	-rmdir $(OUT_DIR);
	-rm $(LIB_DIR)/*.a;
	-rmdir $(LIB_DIR);
