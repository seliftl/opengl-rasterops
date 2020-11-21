#ifndef SCENEALPHATEST_H
#define SCENEALPHATEST_H

#include "scene.h"
#include "glslprogram.h"
#include "cube.h"

#include <glm/glm.hpp>

class SceneAlphaTest : public Scene
{
private:
    GLSLProgram prog;

    Cube cube;

    float angle, tPrev, rotSpeed;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneAlphaTest();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int, float);
};

#endif // SCENEALPHATEST_H
