#pragma once


#pragma region Framework Helper

#define DEFAULT_CONSTRUCTOR(CLASS_NAME) \
protected: \
CLASS_NAME() {} \
~CLASS_NAME() {}

#define STATIC_CREATE_FUNC(CLASS_NAME) \
public: \
static CLASS_NAME* Create(const std::string&);

#define STATIC_CREATE_FUNC_VA(CLASS_NAME, CHILD_CLASS) \
public: \
static CLASS_NAME* Create(const std::string&, CHILD_CLASS*...);

#pragma endregion


#pragma region Game Instance (Singleton)

#define SINGLETON(CLASS_NAME) \
private: \
static CLASS_NAME* instance; \
CLASS_NAME(); \
public: \
static CLASS_NAME* GetInstance() { \
if(instance == nullptr) \
instance = new CLASS_NAME(); \
return instance;\
}

#define SINGLETON_NOFUNC(CLASS_NAME) \
private: \
CLASS_NAME(); \
public: \
static CLASS_NAME* MakeInst() { \
return new CLASS_NAME(); \
}

#pragma endregion 
