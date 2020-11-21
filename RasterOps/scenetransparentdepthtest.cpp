#include "scenetransparentdepthtest.h"
#include "texture.h"

#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::mat4;

SceneTransparentDepthTest::SceneTransparentDepthTest() : cube(1.0f)
{ }

void SceneTransparentDepthTest::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    projection = mat4(1.0f);

	prog.setUniform("Light.L", vec3(1.0f));
	prog.setUniform("Light.La", vec3(0.2f) );

    // Load transparent texture file
	GLuint window = Texture::loadTexture("../media/texture/window.png");

    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, window);

#ifdef __APPLE__
  prog.setUniform("BaseTex", 0);
  prog.setUniform("AlphaTex", 1);
#endif
}

void SceneTransparentDepthTest::update( float t ) {}

void SceneTransparentDepthTest::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // camera movement
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
    view = glm::rotate(view, camRotDown, glm::vec3(-1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, camRotLeft, glm::vec3(0.0f, 1.0f, 0.0f));

    // model 1
    prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("Material.Kd", 0.8f, 0.21424f, 0.27568f);
    prog.setUniform("Material.Ka", 0.8f, 0.1745f, 0.215f);
    prog.setUniform("Material.Ks", 0.5f, 0.05f, 0.05f);
    prog.setUniform("Material.Shininess", 1.0f);

    // check transparency
    if (setTransparent)
        prog.setUniform("textureRendering", 1);
    else
        prog.setUniform("textureRendering", 0);

    model = mat4(1.0f);

    // model movement
    model = glm::translate(model, transFront);
    model = glm::translate(model, transRight);
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    cube.render();

    // model 2
    prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("Material.Kd", 0.0f, 0.21424f, 0.27568f);
    prog.setUniform("Material.Ka", 0.0f, 0.1745f, 0.215f);
    prog.setUniform("Material.Ks", 0.0f, 0.05f, 0.05f);
    prog.setUniform("Material.Shininess", 1.0f);

    // set opaque 
    prog.setUniform("textureRendering", 0);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(-0.3f, 0.2f, -2.0f));
    setMatrices();
    cube.render();
}

void SceneTransparentDepthTest::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    glm::mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneTransparentDepthTest::resize(int w, int h, float fov)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneTransparentDepthTest::compileAndLinkShader()
{
  try {
#ifdef __APPLE__

#else
    prog.compileShader("shader/transparentdepth.vert.glsl");
    prog.compileShader("shader/transparentdepth.frag.glsl");
#endif
    prog.link();
    prog.use();
  } catch(GLSLProgramException & e) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
