#pragma once


#include "Spring_HPreHeader.h"
#include "Spring_HPreDef.h"

namespace spring {

	template <class _Typ>
	class IPointer;

	class UObject;
	class ULayer;


	class URef {

	protected:
		int refCount = 0;

	public:
		void PlusRefCount(int n) {
			refCount += n;
			if (refCount < 0)
				refCount = 0;
		}

		bool IsNoRef() const {
			return refCount == 0;
		}

	};


	class UScene : public URef {

	protected:
		UScene() = default;
		~UScene() = default;

	public:
		STATIC_CREATE_FUNC(UScene);

	protected:
		const std::string& sceneName = "";

	public:
		const std::string& GetSceneName() const;

	public:
		virtual void Init() = 0;
		virtual void Update();
		virtual void Render() final;
		virtual void Release();

	protected:
		//std::map<const std::string, IPointer<ULayer>> childTree;
		std::map<std::string, IPointer<ULayer>> childTree;

	public:
		bool AddLayer(IPointer<ULayer>);

		bool RemoveLayer(IPointer<ULayer>, bool = true);
		bool RemoveLayer(const std::string&, bool = true);

		IPointer<ULayer> GetChildLayer(const std::string&);
		std::map<std::string, IPointer<ULayer>> GetChildTree() const;

	};


	class ULayer : public URef {

	protected:
		ULayer() = default;
		~ULayer() = default;

	public:
		STATIC_CREATE_FUNC(ULayer);

	protected:
		const std::string& layerName = "";

	public:
		const std::string& GetLayerName() const;

	public:
		virtual void Init() = 0;
		virtual void Update();
		virtual void Render() final;
		virtual void Release();

	protected:
		IPointer<UScene> parent;

	public:
		// TODO:
		/*
		SetParent가 호출될때 기존에 부모가 있었다면 RemoveUObject를 사용하여
		원래 부모의 자식으로서의 관계를 지우고 부모를 새로 설정한다.
		해당 함수는 AddXXX를 통해 자동으로 호출된다.
		*/
		void SetParent(IPointer<UScene>);
		IPointer<UScene> GetParent();

	protected:
		std::map<std::string, IPointer<UObject>> childTree;

	public:
		bool AddObject(IPointer<UObject>);

		bool RemoveObject(IPointer<UObject>, bool = true);
		bool RemoveObject(const std::string&, bool = true);

		IPointer<UObject> GetChildObject(const std::string&);
		std::map<std::string, IPointer<UObject>> GetChildTree() const;

	};


	class UObject : public URef {

	protected:
		UObject() = default;
		~UObject() = default;

	public:
		STATIC_CREATE_FUNC(UObject);

	protected:
		std::string objectName = "";

	public:
		const std::string& GetObjectName() const;

	public:
		virtual void Init();
		virtual void Update();
		virtual void Render() final;
		virtual void Release();

	protected:
		IPointer<ULayer> parentLayer;
		IPointer<UObject> parentObject;

	public:
		void SetParent(IPointer<ULayer>);
		void SetParent(IPointer<UObject>);
		IPointer<ULayer> GetParentLayer();
		IPointer<UObject> GetParentObject();

	protected:
		bool shouldRender;

	public:
		void SetShouldRender(bool);
		bool ShouldRender() const;

	protected:
		std::map<std::string, IPointer<UObject>> childTree;

	public:
		bool AddObject(IPointer<UObject>);

		bool RemoveObject(IPointer<UObject>, bool = true);
		bool RemoveObject(const std::string&, bool = true);

		IPointer<UObject> GetChildObject(const std::string&);
		std::map<std::string, IPointer<UObject>> GetChildTree() const;

	};


	class UComponent : public URef {

	protected:
		UComponent() = default;
		~UComponent() = default;

	public:
		STATIC_CREATE_FUNC(UComponent);

	protected:
		std::string componentName = "";

	public:
		const std::string& GetComponentName() const;

	public:
		virtual void Init();

		virtual void BeginUpdate();
		virtual void Update();
		virtual void EndUpdate();

		virtual void Render() final;

		virtual void Release();

		virtual void OnEnable();
		virtual void OnDisable();

	protected:
		IPointer<UObject> parentObject;

	public:
		void SetParent(IPointer<UObject>);
		IPointer<UObject> GetParentObject();
	};

}