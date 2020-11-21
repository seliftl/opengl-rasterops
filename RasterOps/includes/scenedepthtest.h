#ifndef SCENEDEPTHTEST_H
#define SCENEDEPTHTEST_H

#include "scene.h"
#include "glslprogram.h"
#include "cube.h"

#include <glm/glm.hpp>

class SceneDepthTest : public Scene
{
private:
    GLSLProgram prog;

    Cube cube;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneDepthTest();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int, float);
};

#endif // SCENEPERFRAGMENT_H
