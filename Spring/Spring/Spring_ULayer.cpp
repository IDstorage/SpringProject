#include "Spring_UFrameworks.h"

using namespace spring;


const std::string& ULayer::GetLayerName() const {
	return layerName;
}


void ULayer::Update() {
	for (auto childIter = childTree.begin(); childIter != childTree.end(); ++childIter) {
		childIter->second.Get()->Update();
	}
}

void ULayer::Render() {

}

void ULayer::Release() {

}


void ULayer::SetParent(IPointer<UScene> newParent) {
	if (parent.IsNull()) {
		parent->RemoveLayer(this, false);
	}
	parent = newParent;
}

IPointer<UScene> ULayer::GetParent() {
	return parent;
}


bool ULayer::AddObject(IPointer<UObject> object) {
	if (childTree.find(object->GetObjectName()) == childTree.end())
		return false;

	childTree[object->GetObjectName()] = IPointer<UObject>(object);
	childTree[object->GetObjectName()]->SetParent(this);
	return true;
}


bool ULayer::RemoveObject(IPointer<UObject> object, bool cleanUp) {
	auto removeTarget = childTree.find(object->GetObjectName());

	if (removeTarget == childTree.end())
		return false;

	if (cleanUp) {
		// TODO: ERASE
	}

	childTree.erase(object->GetObjectName());
	return true;
}

bool ULayer::RemoveObject(const std::string& name, bool cleanUp) {
	auto removeTarget = childTree.find(name);

	if (removeTarget == childTree.end())
		return false;

	if (cleanUp) {
		// TODO: ERASE
	}

	childTree.erase(name);
	return true;
}


IPointer<UObject> ULayer::GetChildObject(const std::string& name) {
	auto findTarget = childTree.find(name);

	if (findTarget == childTree.end())
		return nullptr;

	auto findUObject = findTarget->second;

	return findUObject.Get();
}

std::map<std::string, IPointer<UObject>> ULayer::GetChildTree() const {
	return childTree;
}