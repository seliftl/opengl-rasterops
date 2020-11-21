#ifndef SCENETRANSPARENTDEPTHTEST_H
#define SCENETRANSPARENTDEPTHTEST_H

#include "scene.h"
#include "glslprogram.h"
#include "cube.h"

#include <glm/glm.hpp>

class SceneTransparentDepthTest : public Scene
{
private:
    GLSLProgram prog;

    Cube cube;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneTransparentDepthTest();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int, float);
};

#endif // SCENEALPHATEST_H
