#ifndef SCENEOBJECTOUTLINING_H
#define SCENEOBJECTOUTLINING_H

#include "scene.h"
#include "glslprogram.h"
#include "cube.h"

#include <glm/glm.hpp>

class SceneObjectOutlining : public Scene
{
private:
    GLSLProgram prog;

    Cube cube;
    float angle;

    void setMatrices();
    void compileAndLinkShader();

public:
    SceneObjectOutlining();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int, float);
};

#endif // SCENEPERFRAGMENT_H
