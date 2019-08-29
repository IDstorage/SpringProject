#include "Spring_UFrameworks.h"

using namespace spring;


const std::string& UScene::GetUSceneName() const {
	return sceneName;
}


void UScene::Update() {

}

void UScene::Render() {

}

void UScene::Release() {

}


bool UScene::AddULayer(IPointer<ULayer> layer) {
	if (childTree.find(layer->GetULayerName()) == childTree.end())
		return false;

	childTree[layer->GetULayerName()] = IPointer<ULayer>(layer);
	return true;
}


bool UScene::RemoveULayer(IPointer<ULayer> layer, bool cleanUp) {
	auto removeTarget = childTree.find(layer->GetULayerName());

	if (removeTarget == childTree.end())
		return false;

	if (cleanUp) {
		// TODO: ERASE
	}

	childTree.erase(layer->GetULayerName());
	return true;
}

bool UScene::RemoveULayer(const std::string& name, bool cleanUp) {
	auto removeTarget = childTree.find(name);

	if (removeTarget == childTree.end())
		return false;

	if (cleanUp) {
		// TODO: ERASE
	}

	childTree.erase(name);
	return true;
}


IPointer<ULayer> UScene::GetChildULayer(const std::string& name) {
	auto findTarget = childTree.find(name);

	if (findTarget == childTree.end())
		return nullptr;

	auto findULayer = findTarget->second;

	return findULayer.Get();
}

std::map<std::string, IPointer<ULayer>> UScene::GetChildTree() const {
	return childTree;
}