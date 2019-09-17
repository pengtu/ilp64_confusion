all: run

MKL_ROOT=/opt/intel/mkl
CPPFLAGS += -I$(MKL_ROOT)/include
LDFLAGS += -ldl -L$(MKL_ROOT)/lib/intel64 -Wl,-rpath,$(MKL_ROOT)/lib/intel64

# We will build libddot against both ilp64 and lp64 MKL
libddot.so: ddot.c
	$(CC) $< -shared $(CPPFLAGS) $(LDFLAGS) -lmkl_rt -o $@

foo: foo.c libddot.so
	$(CC) $< $(CPPFLAGS) $(LDFLAGS) -o $@


run: foo
	@./foo

clean:
	rm -f foo libddot.so
