all: run

MKL_ROOT=/export/users/ptu1/workspace/mkl/src/__release_lnx/mkl
CPPFLAGS += -I$(MKL_ROOT)/include -std=c99
LDFLAGS += -L$(MKL_ROOT)/lib/intel64 -Wl,-rpath,$(MKL_ROOT)/lib/intel64

foo: foo.c
	$(CC) $< $(CPPFLAGS) -DMKL_ILP64=1 $(LDFLAGS) -lmkl_intel_ilp64_ -lmkl_sequential -lmkl_core -lm -o $@

run: foo
	@./foo

clean:
	rm -f foo
