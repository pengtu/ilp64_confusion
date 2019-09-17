#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>

void * open_lib(const char * libname)
{
    printf("Loading %s...\n", libname);
    void *handle = dlopen(libname, RTLD_GLOBAL | RTLD_LAZY);
    if (handle == NULL) {
        printf("Could not load %s: %s\n", libname, dlerror());
        exit(1);
    }
    return handle;
}

void run_ddot(const char * libname)
{
    // Open library (and all dependent libraries)
    void *handle = open_lib(libname);

    // Look up our testing function
    double (*fptr)() = dlsym(handle, "do_ddot");
    if (fptr == NULL) {
        printf("Could not locate do_ddot() within %s: %s\n", libname, dlerror());
    }

    // Run it, printing the result out
    printf(" - result: %g\n", fptr());

    // Clean up
    dlclose(handle);
}

int main(int argc, char const *argv[])
{
    if (argc == 1) {
        printf("Example usages:\n");
        printf("  %s CALL_LP64\n", argv[0]);
        printf("  %s CALL_LP64 CALL_ILP64\n", argv[0]);
        printf("  %s LOAD_MKL_ILP64 CALL_ILP64\n", argv[0]);
        printf("  %s LOAD_MKL_ILP64 CALL_LP64\n", argv[0]);
        exit(1);
    }
    for (int idx=1; idx<argc; ++idx ) {
        // This force-loads the default MKL into our current process
        if(strcmp(argv[idx], "LOAD_MKL_RT") == 0) {
            open_lib("libmkl_rt.so");
        }
        // This force-loads ILP64 MKL into our current process
        if(strcmp(argv[idx], "LOAD_MKL_ILP64") == 0) {
            open_lib("libmkl_core.so");
            open_lib("libmkl_sequential.so");
            open_lib("libmkl_intel_ilp64.so");
        }
        // This force-loads LP64 MKL into our current process
        if(strcmp(argv[idx], "LOAD_MKL_LP64") == 0) {
            open_lib("libmkl_core.so");
            open_lib("libmkl_sequential.so");
            open_lib("libmkl_intel_lp64.so");
        }
        if(strcmp(argv[idx], "CALL_ILP64") == 0) {
            run_ddot("./libddot-ilp64.so");
        }
        if(strcmp(argv[idx], "CALL_LP64") == 0) {
            run_ddot("./libddot-lp64.so");
        }
    }
    return 0;
}

