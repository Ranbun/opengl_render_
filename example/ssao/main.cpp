#include "ssao.h"

int  main()
{
    SSAO::initialize(gl_version_major::major_3, gl_version_minor::minor_3);
    SSAO app;
    app.run();

    return 0;

}
