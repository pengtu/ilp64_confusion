# ILP64/LP64 confusion example

This repository contains an example showcasing the trouble linkers run into when faced with multiple, incompatible definitions of the same symbol.  We define a simple function `do_ddot()` that calls `clas_ddoti()`.  We then compile two versions of the library, one with the expectation of an ILP64 BLAS, and one with the expectation of an LP64 BLAS.  These are called `libddot-ilp64.so` and `libddopt-lp64.so`, respectively.

Next, we define a driver application, called `foo`, that will `dlopen()` libraries and call `do_ddot()` as we direct.  We can tell this application to load various pieces of MKL at will, to showcase what happens when a user program, such as Julia, already has ILP64 MKL loaded, and a client application that was compiled against LP64 MKL is loaded into Julia's address space.

## Demos

To run the demo, simply type `make`.  Note that you may need to modify some paths within `Makefile` to successfully compile.

## Example invocations:

* `./foo LOAD_MKL_LP64 CALL_LP64` - This instructs `foo` to first load the basic MKL libraries, then load the LP64 MKL interface library (the default; if we were not doing everything manually, this is the interface that would be loaded by `libmk_rt`).  We finally instruct `foo` to load `libddot-lp64.so` and call `do_ddot()`.  This succeeds, garnering the correct result value of `285`.

* `./foo LOAD_MKL_ILP64 CALL_ILP64` - This does the same above, but with `ILP64` in all cases.  It also calculates the correct result value of `285`.

* `./foo LOAD_MKL_ILP64 CALL_LP64` - This is the first example of a mismatch; in this case, MKL attempts to interpret the provided array of `MKL_INT` values incorrectly, causing a segfault, because the provided 4-byte indices are far too short, thus causing MKL to read off the end of the array, interpreting random data as the input indices to the `ddoti()` function.

* `./foo LOAD_MKL_LP64 CALL_ILP64` - This is the other mismatch; we do not segfault in this case, the array of 64-bit indices is instead interpreted as an array of 32-bit indices, causing (in this case) every other index to be zero, and resulting in the incorrect result value of `60`.

* `./foo LOAD_MKL_ILP64 CALL_ILP64 CALL_LP64` - This is the most realistic scenario, where Julia has already loaded the ILP64 runtime libraries, then loads another dynamic library that expects to be able to call LP64 BLAS symbols.  This segfaults, of course, however note that attempting to place a `LOAD_MKL_ILP64` before the `CALL_LP64` command does not improve the situation, because the relevant MKL symbols are already loaded into the process.  This is due to the fact that we are loading libraries with `RTLD_GLOBAL` passed to `dlopen()`.

* `./foo CALL_ILP64 CALL_LP64` - This is the scenario most pieces of software will operate within; they rely on the system linker completely, which allows the operating system to namespace all BLAS operations within each sub-module.  This is unfeasible for any system that does not have complete control over the entire BLAS ecosystem.

## Using the system dynamic linker

So far, our example has made use of explicit `dlopen()` and `dlsym()` calls by the "driver" program `foo`, and the loaded libraries have not told the dynamic linker to load `libmkl_rt` or `libmkl_intel_ilp64.so`.  While this is very true-to-life to how Julia operates, other pieces of software more likely pass `-lmkl_intel_ilp64.so` to the compiler, baking in a dependence on the correct library, and ensuring that the correct symbol is bound at link-time.  Except, this only works when the driving executable has not already loaded a BLAS executable.

You can observe this situation by running `make clean` followed by the same `make` incantation above but defining the `LINK_MKL` make variable:
```
make clean; make LINK_MKL=1
```

This will run the above tests, and result in the same problems, except for the last invocation which will work, due to the driver program not already having `libmkl_intel_*` bound in with `RTLD_GLOBAL`.


# Example output:
```
$ make
Running code expecting LP64 on LP64 mkl (should work)
./foo LOAD_MKL_LP64 CALL_LP64
Loading libmkl_core.so...
Loading libmkl_sequential.so...
Loading libmkl_intel_lp64.so...
Loading ./libddot-lp64.so...
 - ddot(): sizeof(MKL_INT) == 4
 - result: 285

Running code expecting ILP64 on ILP64 mkl (should work)
./foo LOAD_MKL_ILP64 CALL_ILP64
Loading libmkl_core.so...
Loading libmkl_sequential.so...
Loading libmkl_intel_ilp64.so...
Loading ./libddot-ilp64.so...
 - ddot(): sizeof(MKL_INT) == 8
 - result: 285

Running code expecting LP64 on ILP64 mkl (BROKEN)
./foo LOAD_MKL_ILP64 CALL_LP64
Makefile:30: recipe for target 'run' failed
make: [run] Segmentation fault (ignored)

Running code expecting ILP64 on LP64 mkl (BROKEN)
./foo LOAD_MKL_LP64 CALL_ILP64
Loading libmkl_core.so...
Loading libmkl_sequential.so...
Loading libmkl_intel_lp64.so...
Loading ./libddot-ilp64.so...
 - ddot(): sizeof(MKL_INT) == 8
 - result: 60

Running LP64 code after running ILP64 code (BROKEN)
./foo LOAD_MKL_ILP64 CALL_ILP64 CALL_LP64
Makefile:30: recipe for target 'run' failed
make: [run] Segmentation fault (ignored)

Running LP64 code after running ILP64 code (with no explicit loading) (INCOMPATIBLE WITH JULIA)
./foo CALL_ILP64 CALL_LP64
./foo: symbol lookup error: ./libddot-ilp64.so: undefined symbol: cblas_ddoti
Makefile:30: recipe for target 'run' failed
make: [run] Error 127 (ignored)
```
