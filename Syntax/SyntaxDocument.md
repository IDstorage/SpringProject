# Spring Project
<br>

### Spring Syntax Document

<br>

---

<br>

#### Syntax 01 - Inheritate

```
// Inheritate class Scene to make new scene(empty).
class TestScene : public spring::Scene { };

// Inheritate class Object to make new object(empty).
class Player : public spring::Object { };
```

Scene, Layer, Object, Component 등의 클래스를 상속함으로서 해당 타입의 클래스를 정의할 수 있다.

Scene, Layer, Object, Component는 가장 큰 줄기인 상위 클래스이며 이들의 하위로는

>Scene : X
>Layer : UILayer ...
>Object : CharacterController, Sprite2D, Model3D ...
>Component : Transform, InputHandler, Renderer ...

가 있다. (Scene의 경우, 그 자체로도 최상위 클래스이다.) 물론, Renderer같이 하위 클래스가 또다른 하위 클래스를 가지는 경우도 존재한다.

<br>

#### Syntax 02 - Overriding

여타 엔진(대표적으로 Unity)들은 각 Scene이나 Layer마다의 프레임워크를 재정의(override)할 수 있고 객체(Object)마다의 라이프 사이클을 재정의하여 커스텀할 수 있다.

Scene과 Layer처럼 객체(Object)보다는 그 객체들을 지닐 수 있는 (후술 참고, Object도 자식 객체들을 지닐 수 있다.) 클래스의 경우 제공되는 프레임워크는 다음과 같다.

```
// Scene 클래스의 간소화 (override용 프레임워크만 포함)
class Scene {
protected:
    virtual bool Init() = 0;
    virtual void Update();
    virtual void Render() final;
    virtual void Release();
};
```

Scene이나 Layer를 상속받은 클래스(새로운 Scene 혹은 Layer)들은 Init()을 제외하고는 재정의하지 않아도 상관없다. **오직 Init()만 필수적으로 재정의 해야 한다.** 또한, **Render()는 재정의가 불가능하다.**

그와 반대로 Object의 경우는 더욱 세밀하다.

```
// Object 클래스의 간소화 (override용 프레임워크만 포함)
class Object {
protected:
    virtual void Start();
    
    virtual void BeginUpdate();
    virtual void Update();
    virtual void EndUpdate();
    
    virtual void Render() final;
    
    virtual void OnDestroy();
    
    virtual void OnEnable();
    virtual void OnDisable();
};
```

Start()는 해당 Object가 Layer등의 자식으로 포함되었지만 한 번도 실행하지 않았을 경우, 실행한다.
BeginUpdate()는 Update()이전에 호출되며 물리 등의 계산에 적합하다.
Update()는 BeginUpdate()와 Scene의 Input 처리가 끝났을 경우 실행된다.
EndUpdate()는 Update()와 Action이 실행된 이후, 처리된다.

<br>

#### Syntax 03 - Identity

모든 클래스는 고유 식별자(Identity)를 가진다. std::string을 통해 각 클래스의 고유 식별자를 선언하여 탐색등에 용이하게 한다.

식별자는 겹침(Overlap)이 없어야 하며, 겹침이 허용되는 범위는 부모가 다를 때만 허용된다.

>PlayScene의 자식들 중 "Block" 이라는 이름의 식별자는 하나만 존재해야 한다.
>만일 존재한다는 가정하에 BackgroundLayer의 자식들 중 "Block"이라는 이름이 존재하더라도
>부모가 다르기 때문에(PlayScene != BackgroundLayer) 관계없다.

<br>

#### Syntax 04 - Instance

다음은 2D 이미지 객체를 생성하여 현재 Scene에 할당된 디폴트 UILayer의 자식으로 설정한다.
Layer의 자식들(Object)은 렌더링될 수 있으며, 각 자식의 선택에 따라 렌더링 대상에서 제외될 수 있다.
즉, Layer의 자식이 아닌 Object들은 그저 데이터만 존재한다. (라이프 사이클을 실행하지 않는다.)

```
auto sprite = spring::Sprite2D::Create("Image.png");
sprite->SetAnchor(Anchor2D(0.5, 0.5));
sprite->SetPosition(Vec2(100, 200));
GetUILayers()[0]->AddChild(sprite, 10);
```

spring namespace 속 객체를 생성할 때는 **별다른 사유가 없다면 포인터를 사용하길 권장**하며 **가능한 Create 함수를 사용하여 생성**하길 바란다.

<br>

#### Syntax 05 - Auto-Release

자식을 부모에서 제외시키려면 RemoveXXX(...) 함수를 사용하면 된다.(XXX는 Layer나 Object등이 될 수 있다.) RemoveXXX(...) 함수의 선언부를 살펴보면
```
void RemoveXXX(XXX*, bool = true);
```
첫번째 인자는 삭제할 자식이다. 두번째 인자는 clean-up flag로 true일 경우 자동으로 메모리에서 할당 해제를 해준다. 디폴트로 true이며 false로 설정할 수는 있지만 **추천하지는 않는다(비표준이다).**

모든 객체들은 std::shared_ptr<>을 모방하였기에 자체적인 레퍼런스 카운팅이 존재한다.

<br>

#### Syntax 06 - Prefix

클래스의 접두사를 정리한 리스트이다.

> 수학 관련 클래스 -> F  (ex. class FVector2;)
> 프레임워크 관련 클래스 -> U  (ex. class UComponent;)
> UComponent를 상속한 컴포넌트 클래스 -> C  (ex. class CTransform;)
> 라이브러리 관련 클래스 -> I  (ex. class IProperty<...>;)
> 그래픽스 관련 클래스 -> G  (ex. class GRenderSystem;)

다음은 파일의 접두사를 정리한 리스트이다.

> 모든 Spring 관련 파일 -> Spring_  (ex. Spring_UFrameworks.h)
> 공용 헤더 파일 -> H  (ex. Spring_HPreDef.h)
> 클래스가 선언/정의된 파일 -> (해당 클래스의 접두사)  (ex. Spring_FVector2.cpp)
> 쉐이더 파일 -> SH  (ex. Spring_SHVertexShader.hlsl)
