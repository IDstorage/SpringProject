#include "Spring_UFrameworks.h"

using namespace spring;


const std::string& UScene::GetSceneName() const {
	return sceneName;
}


void UScene::Update() {

}

void UScene::Render() {

}

void UScene::Release() {

}


bool UScene::AddLayer(IPointer<ULayer> layer) {
	if (childTree.find(layer->GetLayerName()) == childTree.end())
		return false;

	childTree[layer->GetLayerName()] = IPointer<ULayer>(layer);
	return true;
}


bool UScene::RemoveLayer(IPointer<ULayer> layer, bool cleanUp) {
	auto removeTarget = childTree.find(layer->GetLayerName());

	if (removeTarget == childTree.end())
		return false;

	if (cleanUp) {
		// TODO: ERASE
	}

	childTree.erase(layer->GetLayerName());
	return true;
}

bool UScene::RemoveLayer(const std::string& name, bool cleanUp) {
	auto removeTarget = childTree.find(name);

	if (removeTarget == childTree.end())
		return false;

	if (cleanUp) {
		// TODO: ERASE
	}

	childTree.erase(name);
	return true;
}


IPointer<ULayer> UScene::GetChildLayer(const std::string& name) {
	auto findTarget = childTree.find(name);

	if (findTarget == childTree.end())
		return nullptr;

	auto findULayer = findTarget->second;

	return findULayer.Get();
}

std::map<std::string, IPointer<ULayer>> UScene::GetChildTree() const {
	return childTree;
}