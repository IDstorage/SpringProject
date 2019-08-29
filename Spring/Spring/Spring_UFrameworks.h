#pragma once


#include "Spring_HPreHeader.h"
#include "Spring_HPreDef.h"


namespace spring {

	template <class _Typ>
	class IPointer;

	class UObject;
	class ULayer;

	class UScene {

	protected:
		UScene() = default;
		~UScene() = default;

	public:
		STATIC_CREATE_FUNC(UScene);

	protected:
		friend class IPointer<UScene>;
		int refCount = 0;

	protected:
		const std::string& sceneName = "";

	public:
		const std::string& GetUSceneName() const;

	public:
		virtual void Init() = 0;
		virtual void Update();
		virtual void Render() final;
		virtual void Release();

	protected:
		//std::map<const std::string, IPointer<ULayer>> childTree;
		std::map<std::string, IPointer<ULayer>> childTree;

	public:
		bool AddULayer(IPointer<ULayer>);

		bool RemoveULayer(IPointer<ULayer>, bool = true);
		bool RemoveULayer(const std::string&, bool = true);

		IPointer<ULayer> GetChildULayer(const std::string&);
		std::map<std::string, IPointer<ULayer>> GetChildTree() const;

	};


	class ULayer {

	protected:
		ULayer() = default;
		~ULayer() = default;

	public:
		STATIC_CREATE_FUNC(ULayer);

	protected:
		friend class IPointer<ULayer>;
		int refCount = 0;

	protected:
		const std::string& layerName = "";

	public:
		const std::string& GetULayerName() const;

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
		bool AddUObject(IPointer<UObject>);

		bool RemoveUObject(IPointer<UObject>, bool = true);
		bool RemoveUObject(const std::string&, bool = true);

		IPointer<UObject> GetChildUObject(const std::string&);
		std::map<std::string, IPointer<UObject>> GetChildTree() const;

	};


	class UObject {

	protected:
		UObject() = default;
		~UObject() = default;

	public:
		STATIC_CREATE_FUNC(UObject);

	protected:
		friend class IPointer<UObject>;
		int refCount = 0;

	protected:
		std::string objectName = "";

	public:
		const std::string& GetUObjectName() const;

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
		IPointer<ULayer> parentULayer;
		IPointer<UObject> parentUObject;

	public:
		void SetParent(IPointer<ULayer>);
		void SetParent(IPointer<UObject>);
		IPointer<ULayer> GetParentULayer();
		IPointer<UObject> GetParentUObject();

	protected:
		bool shouldRender;

	public:
		void SetShouldRender(bool);
		bool ShouldRender() const;

	protected:
		std::map<std::string, IPointer<UObject>> childTree;

	public:
		bool AddUObject(IPointer<UObject>);

		bool RemoveUObject(IPointer<UObject>, bool = true);
		bool RemoveUObject(const std::string&, bool = true);

		IPointer<UObject> GetChildUObject(const std::string&);
		std::map<std::string, IPointer<UObject>> GetChildTree() const;

	};

}