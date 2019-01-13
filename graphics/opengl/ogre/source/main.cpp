// Include the most common headers from the C standard library
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Include the main libnx system header, for Switch development
#include <switch.h>

// Needed for chdir
#include <unistd.h>

// Application demo class.
#include "Application.h"

static void setMesaConfig()
{
    // Uncomment below to disable error checking and save CPU time (useful for production):
    //setenv("MESA_NO_ERROR", "1", 1);

    // Uncomment below to enable Mesa logging:
    //setenv("EGL_LOG_LEVEL", "debug", 1);
    //setenv("MESA_VERBOSE", "all", 1);
    //setenv("NOUVEAU_MESA_DEBUG", "1", 1);

    // Uncomment below to enable shader debugging in Nouveau:
    //setenv("NV50_PROG_OPTIMIZE", "0", 1);
    //setenv("NV50_PROG_DEBUG", "1", 1);
    //setenv("NV50_PROG_CHIPSET", "0x120", 1);
}

// Main program entrypoint
int main(int argc, char* argv[])
{
    // Set mesa configuration (useful for debugging)
    setMesaConfig();

    // Start in the right directory for logging (if enabled).
    chdir("/ogre");

    // Init the romfs
    romfsInit();

    // Create Application object and start the demo
    Application app;
    app.go();

    // Cleanup romfs
    romfsExit();

    return 0;
}
