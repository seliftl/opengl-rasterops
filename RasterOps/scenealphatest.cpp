#include "scenealphatest.h"
#include "texture.h"

#include <iostream>
using std::endl;
using std::cerr;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
using glm::vec3;
using glm::mat4;

SceneAlphaTest::SceneAlphaTest() : angle(0.0f), tPrev(0.0f), rotSpeed(glm::pi<float>() / 2.0f), cube(1.0f)
{ }

void SceneAlphaTest::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    projection = mat4(1.0f);

	prog.setUniform("Light.L", vec3(1.0f));
	prog.setUniform("Light.La", vec3(0.2f) );

    // Load texture files
	GLuint cement = Texture::loadTexture("../media/texture/cement.jpg");
	GLuint moss = Texture::loadTexture("../media/texture/window.png");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cement);

    glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, moss);

#ifdef __APPLE__
  prog.setUniform("BaseTex", 0);
  prog.setUniform("AlphaTex", 1);
#endif

}

void SceneAlphaTest::update( float t )
{
    float deltaT = t - tPrev;
    if(tPrev == 0.0f) deltaT = 0.0f;
    tPrev = t;

    if( this->m_animate ) {
	angle += rotSpeed * deltaT;
	if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
    }
}

void SceneAlphaTest::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
    view = glm::rotate(view, camRotDown, glm::vec3(-1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, camRotLeft, glm::vec3(0.0f, 1.0f, 0.0f));

    prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Ka", 1.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Shininess", 1.0f);
    prog.setUniform("textureRendering", 1);

    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    cube.render();

    prog.setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    prog.setUniform("Material.Kd", 0.0f, 0.21424f, 0.27568f);
    prog.setUniform("Material.Ka", 0.0f, 0.1745f, 0.215f);
    prog.setUniform("Material.Ks", 0.0f, 0.05f, 0.05f);
    prog.setUniform("Material.Shininess", 1.0f);
    prog.setUniform("textureRendering", 0);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(-0.3f, 0.2f, -2.0f));
    model = glm::translate(model, transFront);
    model = glm::translate(model, transRight);
    model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
    setMatrices();
    cube.render();
}

void SceneAlphaTest::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    glm::mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneAlphaTest::resize(int w, int h, float fov)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(60.0f), (float)w/h, 0.3f, 100.0f);
}

void SceneAlphaTest::compileAndLinkShader()
{
  try {
#ifdef __APPLE__
    prog.compileShader("shader/alphatest_41.vs");
    prog.compileShader("shader/alphatest_41.fs");
#else
    prog.compileShader("shader/alphatest.vert.glsl");
    prog.compileShader("shader/alphatest.frag.glsl");
#endif
    prog.link();
    prog.use();
  } catch(GLSLProgramException & e) {
    cerr << e.what() << endl;
    exit( EXIT_FAILURE );
  }
}
