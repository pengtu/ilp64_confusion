all: run

MKL_ROOT=/opt/intel/mkl
CPPFLAGS += -I$(MKL_ROOT)/include
LDFLAGS += -L$(MKL_ROOT)/lib/intel64 -Wl,-rpath,$(MKL_ROOT)/lib/intel64

foo: foo.c
	$(CC) $< $(CPPFLAGS) -DMKL_ILP64=1 $(LDFLAGS) -lmkl_rt -o $@

run: foo
	@./foo

clean:
	rm -f foo
