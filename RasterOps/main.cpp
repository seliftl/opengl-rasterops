#include "scene.h"
#include "scenerunner.h"
#include "scenedepthtest.h"
#include "scenealphatest.h"
#include "sceneobjectoutlining.h"
#include "sceneoit.h"

std::map<std::string, std::string> sceneInfo = {
	{ "outlining", "Object Outlining " },
	{ "depth", "Depth Test" },
	{ "transparent-depth", "Transparent Depth Test" },
	{ "oit", "Order Independent Transparency" }
};

int main(int argc, char *argv[])
{

	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneInfo);

	SceneRunner runner("Chapter 5 - " + recipe, 1024, 678, 4);
	std::unique_ptr<Scene> scene;
	if( recipe == "outlining" ) {
		scene = std::unique_ptr<Scene>( new SceneObjectOutlining() );
	}
	else if (recipe == "depth") {
		scene = std::unique_ptr<Scene>(new SceneDepthTest());
	}
	else if (recipe == "transparent-depth") {
		scene = std::unique_ptr<Scene>(new SceneAlphaTest());
	}
	else if (recipe == "oit") {
		scene = std::unique_ptr<Scene>(new SceneOit());
	}
	else {
		printf("Unknown recipe: %s\n", recipe.c_str());
		exit(EXIT_FAILURE);
	}

    return runner.run(std::move(scene));
}
