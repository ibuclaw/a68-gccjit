B := build

ALL_FLAGS := -g -O0

CC := gcc-trunk
CFLAGS := $(ALL_FLAGS)

GA68 := ga68-trunk
A68FLAGS := -std=gnu68 $(ALL_FLAGS) -L$(B)

LDFLAGS := -lgccjit

OBJS := $(addprefix $(B)/, gccjit.o module-helpers.o)
DEMO_OBJS := $(addprefix $(B)/, demo.o)

all: $(OBJS) | $(B)

check: $(B)/gccjit-demo $(B)/demo.o

$(B)/gccjit-demo: $(OBJS) $(DEMO_OBJS)
	$(GA68) -o $@ $(OBJS) $(DEMO_OBJS) $(LDFLAGS)

$(B)/%.o: %.a68 | $(B)
	$(GA68) $(A68FLAGS) -c -o $@ $<

$(B)/%.o: %.c | $(B)
	$(CC) $(CFLAGS) -c -o $@ $<

$(B):
	[ -d $(B) ] || mkdir $(B)

.PHONY: clean
clean:
	rm -rf $(B)
