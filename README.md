# ILP64/LP64 confusion testcases

This repository contains three test cases for looking at MKL's ILP64/LP64 confusion due to overloaded symbol names.  Each test case resides within its own directory:

* [`miscompile`](miscompile/):  This is the simplest testcase; it merely defines `-DMKL_ILP64=1` while simultaneously linking against `libmkl_rt`.  This is documented to not work within the MKL documentation, however it illustrates the fundamental problem; because symbols do not contain their type signature, having multiple, identically-named symbols with different type signatures opens the possibility for confusion within the compiler.

* [`runtime_mismatch`](runtime_mismatch/):  This shows that, even with "proper" compilation invocations, you can easily get into a situation where there is a mismatch, by having a "driver" program `foo` that loads the ILP64 MKL library, then loads a library that expects an LP64 MKL.

* [`testing_tool`](testing_tool/): This is the most complex testcase, containing a binary that can load multiple libraries at runtime, can be linked itself against MKL by the compiler, etc....  It was used to find the two smaller testcases, which are specific scenarios that the testing tool can create.
