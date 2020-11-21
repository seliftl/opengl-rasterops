#pragma once

#include <glm/glm.hpp>
#include "glutils.h"

using glm::vec3;

#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"

class Scene
{
protected:
	glm::mat4 model, view, projection;

    float last_t = -1.0f;
    float camRotDown = 0.0f;
    float camRotLeft = 0.0f;

    vec3 transFront = vec3(0.0f, 0.0f, 0.0f);
    vec3 transRight = vec3(0.0f, 0.0f, 0.0f);

    bool enableOutlining;
    bool setTransparent;

public:
    int width;
    int height;

	Scene() : m_animate(true), width(800), height(600) { }
	virtual ~Scene() {}

	void setDimensions( int w, int h ) {
	    width = w;
	    height = h;
	}
	
    /**
      Load textures, initialize shaders, etc.
      */
    virtual void initScene() = 0;

    /**
      This is called prior to every frame.  Use this
      to update your animation.
      */
    virtual void update( float t ) = 0;

    virtual void update(float t, bool rotLeft, bool rotRight, bool rotUp, bool rotDown, bool moveLeft, bool moveRight, bool moveFront, bool moveBack, bool opaque, bool transparent, bool highlight)
    {
        // check if outlining is enabled
        if (transparent)
            setTransparent = true;
        if (opaque) {
            setTransparent = false;
        }

        enableOutlining = highlight; 

        // Time is in seconds. Rotate with rpm.
        const float rpm = 4.0f;
        if (last_t == -1.0f) last_t = t;
        float dt = t - last_t;
        last_t = t;
        float angleIncrement = 2 * glm::pi<float>() * rpm * dt / 60.0f;
        vec3 translationIncrementFront = vec3(0.0f, 0.0f, 0.001f);
        vec3 translationIncrementRight = vec3(0.001f, 0.0f, 0.0f);

        // camera movement
        camRotDown = camRotDown - (rotUp ? angleIncrement : 0) + (rotDown ? angleIncrement : 0);
        camRotLeft = camRotLeft + (rotLeft ? angleIncrement : 0) - (rotRight ? angleIncrement : 0);

        // model movement
        transFront = transFront + (moveFront ? translationIncrementFront : vec3(0.0f, 0.0f, 0.0f)) - (moveBack ? translationIncrementFront : vec3(0.0f, 0.0f, 0.0f));
        transRight = transRight + (moveRight ? translationIncrementRight : vec3(0.0f, 0.0f, 0.0f)) - (moveLeft ? translationIncrementRight : vec3(0.0f, 0.0f, 0.0f));
    }

    /**
      Draw your scene.
      */
    virtual void render() = 0;

    /**
      Called when screen is resized
      */
    virtual void resize(int , int , float) = 0;
    
    void animate( bool value ) { m_animate = value; }
    bool animating() { return m_animate; }
    
protected:
	bool m_animate;
};
