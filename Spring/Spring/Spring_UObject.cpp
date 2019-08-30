#include "Spring_UFrameworks.h"

using namespace spring;


UObject* UObject::Create(const std::string& name) {
	UObject* newObj = new UObject();
	newObj->objectName = name;
	return newObj;
}


const std::string& UObject::GetObjectName() const {
	return objectName;
}


void UObject::Init() {

}

void UObject::Update() {
	for (auto childIter = childTree.begin(); childIter != childTree.end(); ++childIter) {
		childIter->second.Get()->Update();
	}
}

void UObject::Render() {
	for (auto childIter = childTree.begin(); childIter != childTree.end(); ++childIter) {
		childIter->second.Get()->Render();
	}
}

void UObject::Release() {
	for (auto childIter = childTree.begin(); childIter != childTree.end(); ++childIter) {
		childIter->second.Get()->Release();
	}
}


void UObject::SetParent(IPointer<ULayer> newParent) {
	if (parentLayer.IsNull()) {
		parentLayer->RemoveObject(this, false);
	}
	parentLayer = newParent;
}

void UObject::SetParent(IPointer<UObject> newParent) {
	if (parentObject.IsNull()) {
		parentObject->RemoveObject(this, false);
	}
	parentObject = newParent;
}

IPointer<ULayer> UObject::GetParentLayer() {
	return parentLayer;
}

IPointer<UObject> UObject::GetParentObject() {
	return parentObject;
}


void UObject::SetShouldRender(bool b) {
	shouldRender = b;
}

bool UObject::ShouldRender() const {
	return shouldRender;
}


bool UObject::AddObject(IPointer<UObject> object) {
	if (childTree.find(object->GetObjectName()) == childTree.end())
		return false;

	childTree[object->GetObjectName()] = IPointer<UObject>(object);
	childTree[object->GetObjectName()]->SetParent(this);
	return true;
}


bool UObject::RemoveObject(IPointer<UObject> object, bool cleanUp) {
	auto removeTarget = childTree.find(object->GetObjectName());

	if (removeTarget == childTree.end())
		return false;

	if (cleanUp) {
		// TODO: ERASE
	}

	childTree.erase(object->GetObjectName());
	return true;
}

bool UObject::RemoveObject(const std::string& name, bool cleanUp) {
	auto removeTarget = childTree.find(name);

	if (removeTarget == childTree.end())
		return false;

	if (cleanUp) {
		// TODO: ERASE
	}

	childTree.erase(name);
	return true;
}


IPointer<UObject> UObject::GetChildObject(const std::string& name) {
	auto findTarget = childTree.find(name);

	if (findTarget == childTree.end())
		return nullptr;

	auto findUObject = findTarget->second;

	return findUObject.Get();
}

std::map<std::string, IPointer<UObject>> UObject::GetChildTree() const {
	return childTree;
}