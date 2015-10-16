#ifndef KERNELMANAGER_HPP
#define KERNELMANAGER_HPP

# ifdef __APPLE__
    #include "OpenCL/opencl.h"
	#include "OpenCL/cl.hpp"
# else
    #include "CL/cl.h"
# endif

# include <string>

class KernelManager {
private:
	static KernelManager					*_instance;

	cl::Context								*_context;
	cl::Device								_defaultDevice;
	cl::Program::Sources					_sources;
	std::map<std::string, cl::Program *>	_programs;

	KernelManager();
public:
	static KernelManager	*getInstance();
	cl::Program				*getProgram(std::string name);

	void					addProgram(std::string name, std::string code);
};

#endif /* end of include guard: KERNELMANAGER_HPP */
