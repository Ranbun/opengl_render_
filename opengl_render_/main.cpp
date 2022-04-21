#include "ofapp.h"
#include "deferredshading.h"

int main()
{
    RenderApp::initialize(gl_version_major::major_3,
        gl_version_minor::minor_3);

    DeferredShading renderWidget;
    renderWidget.setTitle(std::string("OpenGL"));

    renderWidget.run();
    return 0;
}
