# Simplified minimal working example

This showcases a simplified version of a single scenario achievable with the full "testing tool" placed within [the other directory](../testing_tool).  This showcases a single dynamic library, linked against `libmkl_rt`, and thereby implicitly assuming an LP64 interface, being loaded into aprocess that has already loaded the ILP64 interface.  Because the symbol names are the same, the dynamic linker does not replace the symbols already loaded, and so the LP64-expecting library calls ILP64 function names, resulting in a segfault.
