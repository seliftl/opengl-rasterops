#include "scene.h"
#include "scenerunner.h"
#include "scenedepthtest.h"
#include "sceneobjectoutlining.h"

std::map<std::string, std::string> sceneInfo = {
	{ "outlining", "Object Outlining " },
	{ "depth", "Depth Test" },
};

int main(int argc, char *argv[])
{

	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneInfo);

	SceneRunner runner("Chapter 5 - " + recipe, 1024, 678, 4);
	std::unique_ptr<Scene> scene;
	if( recipe == "outlining" ) {
		scene = std::unique_ptr<Scene>( new SceneObjectOutlining() );
	} else if( recipe == "depth" ) {
		scene = std::unique_ptr<Scene>( new SceneDepthTest() );
	}
	else {
		printf("Unknown recipe: %s\n", recipe.c_str());
		exit(EXIT_FAILURE);
	}

    return runner.run(std::move(scene));
}
