#include "KernelManager.hpp"

KernelManager::_instance = nullptr;

KernelManager::KernelManager() {}

KernelManager	*KernelManager::getInstance() {
	if (_instance == nullptr)
		_instance = new KernelManager();
	return _instance;
}
