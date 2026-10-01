B := build

ALL_FLAGS := -g -O0

CC := gcc-trunk
CFLAGS := $(ALL_FLAGS)

GA68 := ga68-trunk
A68FLAGS := -std=gnu68 $(ALL_FLAGS) -L$(B)

LDFLAGS := -lgccjit

all: $(B)/gccjit.o | $(B)

check: $(B)/gccjit-demo $(B)/demo.o

OBJS := $(addprefix $(B)/, gccjit.o demo.o)

$(B)/gccjit-demo: $(OBJS)
	$(GA68) -o $@ $(OBJS) $(LDFLAGS)

$(B)/%.o: %.a68 | $(B)
	$(GA68) $(A68FLAGS) -c -o $@ $<

$(B):
	[ -d $(B) ] || mkdir $(B)

.PHONY: clean
clean:
	rm -rf $(B)
