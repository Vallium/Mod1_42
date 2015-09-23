#define GLEW_STATIC
#include <GL/glew.h>

#include "Context.hpp"

int			main(int ac, char *av[]) {
	Context::init(ac, av);

	while(!Context::shouldClose())
	{
		Context::update();
		Context::draw();
	}

	Context::deinit();
	return 0;
}
