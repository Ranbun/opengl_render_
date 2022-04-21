#include "deferredshading.h"
int main()
{
    DeferredShading::initialize(gl_version_major::major_3, gl_version_minor::minor_3);
    DeferredShading app;
    app.run();

    return 0;
}
