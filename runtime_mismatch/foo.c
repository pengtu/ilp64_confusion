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
    // First, open ILP64 MKL
    open_lib("libmkl_core.so");
    open_lib("libmkl_sequential.so");
    open_lib("libmkl_intel_ilp64_.so");

    // Next, load the given .so file and run its ddot test
    run_ddot("./libddot.so");
    return 0;
}

