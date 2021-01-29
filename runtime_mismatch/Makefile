all: run

MKL_ROOT=/export/users/ptu1/workspace/mkl/src/__release_lnx/mkl
CPPFLAGS += -I$(MKL_ROOT)/include -std=c99
LDFLAGS += -ldl -L$(MKL_ROOT)/lib/intel64 -Wl,-rpath,$(MKL_ROOT)/lib/intel64

# We will build libddot against both ilp64 and lp64 MKL
libddot.so: ddot.c
	$(CC) $< -fPIC -shared $(CPPFLAGS) $(LDFLAGS) -lmkl_rt -o $@

foo: foo.c libddot.so
	$(CC) $< $(CPPFLAGS) $(LDFLAGS) -o $@


run: foo
	@./foo

clean:
	rm -f foo libddot.so
