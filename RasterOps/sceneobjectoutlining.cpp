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
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    view = glm::lookAt(vec3(1.25f, 1.25f, 1.25f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    angle = 0.0;

    prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Light.La", 0.7f, 0.7f, 0.2f);
    prog.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);

    // Load texture file
    GLint w, h;
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, texID);

#ifdef __APPLE__
    // Set the sampler uniform
    prog.setUniform("Tex1", 0);
#endif
}

void SceneObjectOutlining::update(float t) {
        
}

void SceneObjectOutlining::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    //view = glm::rotate(view, camRotDown, glm::vec3(-1.0f, 0.0f, 0.0f));
    //view = glm::rotate(view, camRotLeft, glm::vec3(0.0f, 1.0f, 0.0f));

    // set mask for each pixel of cube
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    prog.setUniform("Material.Kd", 0.0f, 0.21424f, 0.27568f);
    prog.setUniform("Material.Ka", 0.0f, 0.1745f, 0.215f);
    prog.setUniform("Material.Ks", 0.0f, 0.05f, 0.05f);
    prog.setUniform("Material.Shininess", 1.0f);

    model = mat4(1.0f);    

    glDisable(GL_DEPTH_TEST);
    setMatrices();
    cube.render();

    if (enableOutlining) {
        // write only to pixels where mask is not 1
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        prog.setUniform("Material.Kd", 0.8f, 0.21424f, 0.27568f);
        prog.setUniform("Material.Ka", 0.8f, 0.1745f, 0.215f);
        prog.setUniform("Material.Ks", 0.5f, 0.05f, 0.05f);
        prog.setUniform("Material.Shininess", 1.0f);

        model = mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
        //model = glm::translate(model, transFront);
        //model = glm::translate(model, transRight);

        setMatrices();
        cube.render();
        glEnable(GL_DEPTH_TEST);
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
        prog.compileShader("shader/texture_41.vs");
        prog.compileShader("shader/texture_41.fs");
#else
        prog.compileShader("shader/texture.vert.glsl");
        prog.compileShader("shader/texture.frag.glsl");
#endif
        prog.link();
        prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}