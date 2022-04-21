#ifndef SSAO_H_
#define SSAO_H_

#include "glad/glad.h"
#include <map>
#include "model.h"
#include "ofapp.h"


class SSAO final :public RenderApp
{
public:
    SSAO();
    ~SSAO() override;

    void init()
    {

    }

    void render()
    {
        __super::render();
    }




};



#endif // !SSAO_H_

