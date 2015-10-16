#include "KernelManager.hpp"

KernelManager::_instance = nullptr;

KernelManager::KernelManager() {
	//get all platforms (drivers)
	std::vector<cl::Platform> allPlatforms;
	cl::Platform::get(&allPlatforms);
	if(allPlatforms.size()==0){
		std::cout<<" No platforms found. Check OpenCL installation!\n";
		exit(1);
	}
	cl::Platform defaultPlatform=allPlatforms[0];
	std::cout << "Using platform: "<<defaultPlatform.getInfo<CL_PLATFORM_NAME>()<<"\n";

	//get default device of the default platform
	std::vector<cl::Device> allDevices;
	defaultPlatform.getDevices(CL_DEVICE_TYPE_ALL, &allDevices);
	if(allDevices.size()==0){
		std::cout<<" No devices found. Check OpenCL installation!\n";
		exit(1);
	}
	_defaultDevice=allDevices[0];
	std::cout<< "Using device: "<<_defaultDevice.getInfo<CL_DEVICE_NAME>()<<"\n";

	_context = new cl::Context({_defaultDevice});
}

KernelManager	*KernelManager::getInstance() {
	if (_instance == nullptr)
		_instance = new KernelManager();
	return _instance;
}

cl::Program		*KernelManager::getProgram(std::string name) {
	auto	program = _programs.find(name);

	if (program == programs.end())
		throw std::exception();
	return *program;
}

void			KernelManager::addProgram(std::string name, std::string kernelCode) {
	// kernel calculates for each element
	_sources.push_back({kernelCode.c_str(),kernelCode.length()});

	cl::Program		*program = new cl::Program(*_context,_sources);
	if(program->build({_defaultDevice})!=CL_SUCCESS){
		std::cout<<" Error building: "<<program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(_defaultDevice)<<"\n";
		exit(1);
	}
	_programs[name] = program;
}
