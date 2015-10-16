#ifndef KERNELMANAGER_HPP
#define KERNELMANAGER_HPP

class KernelManager {
private:
	KernelManager	*_instance;

	KernelManager();
public:
	KernelManager	*getInstance();
};

#endif /* end of include guard: KERNELMANAGER_HPP */
