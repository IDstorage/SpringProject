#include "Spring_UFrameworks.h"

using namespace spring;


UObject* UObject::Create(const std::string& name) {
	UObject* newObj = new UObject();
	newObj->objectName = name;
	return newObj;
}


const std::string& UObject::GetUObjectName() const {
	return objectName;
}


void UObject::Init() {

}

void UObject::BeginUpdate() {
	for (auto childIter = childTree.begin(); childIter != childTree.end(); ++childIter) {
		childIter->second.Get()->BeginUpdate();
	}
}

void UObject::Update() {
	for (auto childIter = childTree.begin(); childIter != childTree.end(); ++childIter) {
		childIter->second.Get()->Update();
	}
}

void UObject::EndUpdate() {
	for (auto childIter = childTree.begin(); childIter != childTree.end(); ++childIter) {
		childIter->second.Get()->EndUpdate();
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

void UObject::OnEnable() {

}

void UObject::OnDisable() {

}


void UObject::SetParent(IPointer<ULayer> newParent) {
	if (parentULayer.IsNull()) {
		parentULayer->RemoveUObject(this, false);
	}
	parentULayer = newParent;
}

void UObject::SetParent(IPointer<UObject> newParent) {
	if (parentUObject.IsNull()) {
		parentUObject->RemoveUObject(this, false);
	}
	parentUObject = newParent;
}

IPointer<ULayer> UObject::GetParentULayer() {
	return parentULayer;
}

IPointer<UObject> UObject::GetParentUObject() {
	return parentUObject;
}


void UObject::SetShouldRender(bool b) {
	shouldRender = b;
}

bool UObject::ShouldRender() const {
	return shouldRender;
}


bool UObject::AddUObject(IPointer<UObject> object) {
	if (childTree.find(object->GetUObjectName()) == childTree.end())
		return false;

	childTree[object->GetUObjectName()] = IPointer<UObject>(object);
	childTree[object->GetUObjectName()]->SetParent(this);
	return true;
}


bool UObject::RemoveUObject(IPointer<UObject> object, bool cleanUp) {
	auto removeTarget = childTree.find(object->GetUObjectName());

	if (removeTarget == childTree.end())
		return false;

	if (cleanUp) {
		// TODO: ERASE
	}

	childTree.erase(object->GetUObjectName());
	return true;
}

bool UObject::RemoveUObject(const std::string& name, bool cleanUp) {
	auto removeTarget = childTree.find(name);

	if (removeTarget == childTree.end())
		return false;

	if (cleanUp) {
		// TODO: ERASE
	}

	childTree.erase(name);
	return true;
}


IPointer<UObject> UObject::GetChildUObject(const std::string& name) {
	auto findTarget = childTree.find(name);

	if (findTarget == childTree.end())
		return nullptr;

	auto findUObject = findTarget->second;

	return findUObject.Get();
}

std::map<std::string, IPointer<UObject>> UObject::GetChildTree() const {
	return childTree;
}