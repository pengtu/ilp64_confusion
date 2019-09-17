# Compiler confusion

This showcases a simplified version of a single scenario achievable with the full "testing tool" placed within [the other directory](../testing_tool).  This showcases a single executable, compiled against `libmkl_rt`, calculating the wrong value because the user code expects ILP64 (`-DMKL_ILP64=1`) but the backing library provides an LP64 implementation (`-lmkl_rt`).  This would not be an issue if the ILP64 code used different symbol names instead of overloading the already-existing symbol names.
