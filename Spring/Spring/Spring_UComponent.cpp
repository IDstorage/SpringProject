#include "Spring_UFrameworks.h"

using namespace spring;


UComponent* UComponent::Create(const std::string& name) {
	UComponent* newObj = new UComponent();
	newObj->componentName = name;
	return newObj;
}


const std::string& UComponent::GetComponentName() const {
	return componentName;
}


void UComponent::Init() {

}

void UComponent::BeginUpdate() {

}

void UComponent::Update() {

}

void UComponent::EndUpdate() {

}

void UComponent::Render() {

}

void UComponent::Release() {

}

void UComponent::OnEnable() {

}

void UComponent::OnDisable() {

}


void UComponent::SetParent(IPointer<UObject> newParent) {
	parentObject = newParent;
}

IPointer<UObject> UComponent::GetParentObject() {
	return parentObject;
}