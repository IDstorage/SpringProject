#include "Spring_UFrameworks.h"

using namespace spring;


const std::string& ULayer::GetULayerName() const {
	return layerName;
}


void ULayer::Update() {
	for (auto childIter = childTree.begin(); childIter != childTree.end(); ++childIter) {
		childIter->second.Get()->BeginUpdate();
	}

	for (auto childIter = childTree.begin(); childIter != childTree.end(); ++childIter) {
		childIter->second.Get()->Update();
	}

	for (auto childIter = childTree.begin(); childIter != childTree.end(); ++childIter) {
		childIter->second.Get()->EndUpdate();
	}
}

void ULayer::Render() {

}

void ULayer::Release() {

}


void ULayer::SetParent(IPointer<UScene> newParent) {
	if (parent.IsNull()) {
		parent->RemoveULayer(this, false);
	}
	parent = newParent;
}

IPointer<UScene> ULayer::GetParent() {
	return parent;
}


bool ULayer::AddUObject(IPointer<UObject> object) {
	if (childTree.find(object->GetUObjectName()) == childTree.end())
		return false;

	childTree[object->GetUObjectName()] = IPointer<UObject>(object);
	childTree[object->GetUObjectName()]->SetParent(this);
	return true;
}


bool ULayer::RemoveUObject(IPointer<UObject> object, bool cleanUp) {
	auto removeTarget = childTree.find(object->GetUObjectName());

	if (removeTarget == childTree.end())
		return false;

	if (cleanUp) {
		// TODO: ERASE
	}

	childTree.erase(object->GetUObjectName());
	return true;
}

bool ULayer::RemoveUObject(const std::string& name, bool cleanUp) {
	auto removeTarget = childTree.find(name);

	if (removeTarget == childTree.end())
		return false;

	if (cleanUp) {
		// TODO: ERASE
	}

	childTree.erase(name);
	return true;
}


IPointer<UObject> ULayer::GetChildUObject(const std::string& name) {
	auto findTarget = childTree.find(name);

	if (findTarget == childTree.end())
		return nullptr;

	auto findUObject = findTarget->second;

	return findUObject.Get();
}

std::map<std::string, IPointer<UObject>> ULayer::GetChildTree() const {
	return childTree;
}