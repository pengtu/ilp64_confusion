all: run

MKL_ROOT=/home/peng/Downloads/mkl
CPPFLAGS += -I$(MKL_ROOT)/include -O0 -std=c99 -fPIC
LDFLAGS += -ldl -Wl,-rpath,$(MKL_ROOT)/lib/intel64

ifneq ($(LINK_MKL),)
# If we're using the compiler to do our linking (disable GCC's tendency to drop mkl_sequential and mkl_core)
LDFLAGS += -L$(MKL_ROOT)/lib/intel64 -Wl,--no-as-needed
LINK_MKL_LP64  = -lmkl_sequential -lmkl_intel_lp64 -lmkl_core
LINK_MKL_ILP64 = -lmkl_sequential -lmkl_intel_ilp64_ -lmkl_core
endif

# We will build libddot against both ilp64 and lp64 MKL
libddot-ilp64.so: ddot_ilp64.c
	$(CC) $< -g -shared -DMKL_ILP64=1 $(CPPFLAGS) $(LDFLAGS) $(LINK_MKL_ILP64) -o $@
libddot-lp64.so: ddot.c
	$(CC) $< -g -shared $(CPPFLAGS) $(LDFLAGS) $(LINK_MKL_LP64) -o $@

foo: foo.c libddot-ilp64.so libddot-lp64.so
	$(CC) $< -g $(CPPFLAGS) $(LDFLAGS) -o $@


# Colorization helpers
RED = \033[0;31m
NC = \033[0m


run: foo
	@echo "$(RED)Running code expecting LP64 on LP64 mkl$(NC) (should work)"
	./foo LOAD_MKL_LP64 CALL_LP64
	@echo
	@echo "$(RED)Running code expecting ILP64 on ILP64 mkl$(NC) (should work)"
	./foo LOAD_MKL_ILP64 CALL_ILP64
	@echo
	@echo "$(RED)Running code expecting LP64 on ILP64 mkl$(NC) (BROKEN)"
	-./foo LOAD_MKL_ILP64 CALL_LP64
	@echo
	@echo "$(RED)Running code expecting ILP64 on LP64 mkl$(NC) (BROKEN)"
	-./foo LOAD_MKL_LP64 CALL_ILP64
	@echo
	@echo "$(RED)Running LP64 code after running ILP64 code$(NC) (BROKEN)"
	-./foo LOAD_MKL_ILP64 CALL_ILP64 CALL_LP64
	@echo
	@echo "$(RED)Running LP64 code after running ILP64 code (with no explicit loading)$(NC) (INCOMPATIBLE WITH JULIA)"
	-./foo CALL_ILP64 CALL_LP64
	@echo

clean:
	rm -f foo libddot-ilp64.so libddot-lp64.so
