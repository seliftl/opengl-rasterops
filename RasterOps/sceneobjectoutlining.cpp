#include "sceneobjectoutlining.h"
#include "texture.h"
#include "stb/stb_image.h"

#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

SceneObjectOutlining::SceneObjectOutlining() { }

void SceneObjectOutlining::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    view = glm::lookAt(vec3(1.25f, 1.25f, 1.25f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    prog.setUniform("Light.Position", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Light.La", 0.7f, 0.7f, 0.2f);
    prog.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);

    GLint w, h;
#ifdef __APPLE__
#endif
}

void SceneObjectOutlining::update(float t) {}

void SceneObjectOutlining::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // all pixels should pass test
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    // enable writing to stencil buffer
    glStencilMask(0xFF);

    prog.setUniform("Material.Kd", 0.0f, 0.21424f, 0.27568f);
    prog.setUniform("Material.Ka", 0.0f, 0.1745f, 0.215f);
    prog.setUniform("Material.Ks", 0.0f, 0.05f, 0.05f);
    prog.setUniform("Material.Shininess", 1.0f);

    model = mat4(1.0f);    

    setMatrices();
    cube.render();

    if (enableOutlining) {
        // write only to pixels where mask is not 1 -> spare cube
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

        // disable writing to stencil buffer
        glStencilMask(0x00);

        prog.setUniform("Material.Kd", 0.8f, 0.21424f, 0.27568f);
        prog.setUniform("Material.Ka", 0.8f, 0.1745f, 0.215f);
        prog.setUniform("Material.Ks", 0.5f, 0.05f, 0.05f);
        prog.setUniform("Material.Shininess", 1.0f);

        model = mat4(1.0f);

        // scale up cube
        model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));

        setMatrices();
        cube.render();
    }
}

void SceneObjectOutlining::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneObjectOutlining::resize(int w, int h, float fov)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(fov), (float)w / h, 0.3f, 100.0f);
}

void SceneObjectOutlining::compileAndLinkShader()
{
    try {
#ifdef __APPLE__
#else
        prog.compileShader("shader/basic.vert.glsl");
        prog.compileShader("shader/basic.frag.glsl");
#endif
        prog.link();
        prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}