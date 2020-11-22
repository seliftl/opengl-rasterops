#include "scenedepthtest.h"
#include "texture.h"
#include "stb/stb_image.h"

#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4;

SceneDepthTest::SceneDepthTest() { }

void SceneDepthTest::initScene()
{
    compileAndLinkShader();

    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_ONE, GL_ONE);

    view = glm::lookAt(vec3(0.0f, 1.25f, 1.25f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    prog.setUniform("Light.Position", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    prog.setUniform("Light.Ld", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Light.La", 0.7f, 0.7f, 0.2f);
    prog.setUniform("Light.Ls", 1.0f, 1.0f, 1.0f);

#ifdef __APPLE__
    // Set the sampler uniform
    prog.setUniform("Tex1", 0);
#endif
}

void SceneDepthTest::update(float t) {}

void SceneDepthTest::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (enableDepthTest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if(enableBlending)
        glEnable(GL_BLEND);
    else 
        glDisable(GL_BLEND);

    // camera movement
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    view = glm::rotate(view, camRotDown, glm::vec3(-1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, camRotLeft, glm::vec3(0.0f, 1.0f, 0.0f));

    // model 1
    prog.setUniform("Material.Kd", 0.0f, 0.21424f, 0.27568f);
    prog.setUniform("Material.Ka", 0.0f, 0.1745f, 0.215f);
    prog.setUniform("Material.Ks", 0.0f, 0.05f, 0.05f);
    prog.setUniform("Material.Shininess", 1.0f);

    model = mat4(1.0f);    
    model = glm::translate(model, glm::vec3(-0.3f, 0.2f, -2.0f));
    
    setMatrices();
    cube.render();

    // model 2
    prog.setUniform("Material.Kd", 0.8f, 0.21424f, 0.27568f);
    prog.setUniform("Material.Ka", 0.8f, 0.1745f, 0.215f);
    prog.setUniform("Material.Ks", 0.5f, 0.05f, 0.05f);
    prog.setUniform("Material.Shininess", 1.0f);

    model = mat4(1.0f);
    // model movement
    model = glm::translate(model, transFront);
    model = glm::translate(model, transRight);

    setMatrices();
    cube.render();
}

void SceneDepthTest::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
        glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneDepthTest::resize(int w, int h, float fov)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(fov), (float)w / h, 0.3f, 100.0f);
}

void SceneDepthTest::compileAndLinkShader()
{
    try {
#ifdef __APPLE__
        prog.compileShader("shader/texture_41.vs");
        prog.compileShader("shader/texture_41.fs");
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