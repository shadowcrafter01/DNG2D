#include <ENG2D/Main.hpp>
#include <memory>
#include <stdexcept>

ENG::Window window = ENG::Window("le wrapler rawr", ENG::Vector2(640, 480), SDL_WINDOW_RESIZABLE);

ENG::Camera camera = ENG::Camera(&window);

ENG::Texture test = ENG::Texture(&window, "assets/textures/billGates.bmp");
ENG::Texture cursor = ENG::Texture(&window, "assets/textures/cursor.png");
ENG::Texture cursor2 = ENG::Texture(&window, "assets/textures/cursorpressed.png");
ENG::Texture chud = ENG::Texture(&window, "assets/textures/chud.png");

// ENG::Atlas fireAtlas = ENG::Atlas::Create(&window, "assets/textures/fire.png");

// ENG::AnimatedTexture fire = ENG::AnimatedTexture(&fireAtlas, 8, 1);

ENG::File file = ENG::File("test.json");

ENG::Font font = ENG::Font("assets/fonts/Retro.Gaming.Extended.ttf", 32);

void WhenBallClicked(ENG::Dingus *me)
{
    if (me->GetName() == "ball")
    {
        me->Destroy();
    }
}

ENG::Dingus *nearest = nullptr;

void ResetCursor()
{
    window.SetCustomCursor(cursor);
}
ENG::RunOnce resetCursor(ResetCursor);
void LayerAndCalcNearest()
{
    nearest->SetRenderLayer(0);
    window.SetCustomCursor(cursor2);
    nearest = ENG::Dingus::GetNearest(ENG::Input::Mouse::GetMouseWorldPos(&camera), "ball");
    nearest->SetRenderLayer(1);
}
ENG::RunOnce layerAndCalcNearest(LayerAndCalcNearest);

void MouseLeft(bool buttonState)
{
    resetCursor.OnFalse(buttonState);
    layerAndCalcNearest.OnTrue(buttonState);
}

void MouseRight(bool buttonState)
{
}

void MoveUp()
{
    camera.ChangePositionBy(0, 200.0 / camera.GetZoom(), ENG::globalTimer.GetDelta());
}
void MoveDown()
{
    camera.ChangePositionBy(0, -200.0 / camera.GetZoom(), ENG::globalTimer.GetDelta());
}
void MoveLeft()
{
    camera.ChangePositionBy(-200.0 / camera.GetZoom(), ENG::globalTimer.GetDelta(), 0);
}
void MoveRight()
{
    camera.ChangePositionBy(200.0 / camera.GetZoom(), ENG::globalTimer.GetDelta(), 0);
}
void ZoomIn()
{
    camera.ChangeZoomBy(1.0 * camera.GetZoom(), ENG::globalTimer.GetDelta());
}
void ZoomOut()
{
    camera.ChangeZoomBy(-1.0 * camera.GetZoom(), ENG::globalTimer.GetDelta());
}
void RotateC()
{
    camera.ChangeAngleBy(-30.0, ENG::globalTimer.GetDelta());
}
void RotateCC()
{
    camera.ChangeAngleBy(30.0, ENG::globalTimer.GetDelta());
}

ENG::Action zoomIn = ENG::Action(ZoomIn, SDL_SCANCODE_UP);
ENG::Action zoomOut = ENG::Action(ZoomOut, SDL_SCANCODE_DOWN);
ENG::Action rotateC = ENG::Action(RotateC, SDL_SCANCODE_RIGHT);
ENG::Action rotateCC = ENG::Action(RotateCC, SDL_SCANCODE_LEFT);
ENG::Action moveRight = ENG::Action(MoveRight, SDL_SCANCODE_D);
ENG::Action moveLeft = ENG::Action(MoveLeft, SDL_SCANCODE_A);
ENG::Action moveUp = ENG::Action(MoveUp, SDL_SCANCODE_W, ACTION_WHILE_HELD);
ENG::Action moveDown = ENG::Action(MoveDown, SDL_SCANCODE_S);

ENG::Action mouseLeft = ENG::Action(MouseLeft, SDL_BUTTON_LEFT);
ENG::Action mouseRight = ENG::Action(MouseRight, SDL_BUTTON_RIGHT);

int main()
{
    ENG::Init();

    window.SetCustomCursor(cursor);
    window.SetCursorScale(2.0f);

    ENG::Dingus *ground = ENG::Dingus::Create(&camera, &chud, ENG::Transform2<float>(0, -500, 5000, chud.GetSize().y));
    ground->AssignCollisionShape(ENG::CollisionShape::CreateBox(ground->GetTransform().size), 1, 0.25, 0.5)->SetStatic();
    // ENG::Dingus *wall1 = ENG::Dingus::Create(&camera, &chud, ENG::Transform2<float>(2500, 2000, chud.size.x, 5000));
    // wall1->AssignCollisionShape(ENG::CollisionShape::Box(wall1->GetTransform().size), 1, 0.25, 0.5)->SetStatic();
    // ENG::Dingus *wall2 = ENG::Dingus::Create(&camera, &chud, ENG::Transform2<float>(-2500, 2000, chud.size.x, 5000));
    // wall2->AssignCollisionShape(ENG::CollisionShape::Box(wall2->GetTransform().size), 1, 0.25, 0.5)->SetStatic();

    for (size_t i = 0; i < 100; ++i)
    {
        ENG::Dingus *ball = ENG::Dingus::Create(&camera, &chud, ENG::Transform2<float>(0, 1280 * i, chud.GetSize() * ENG::Vector2<int>(1, 1)));
        ball->AssignCollisionShape(ENG::CollisionShape::CreateBox(chud.GetSize() * ENG::Vector2<int>(1, 1)), 1, 0.25, 0.5);
        ball->SetName("ball");
        ball->AssignAction(WhenBallClicked, SDL_BUTTON_RIGHT, ACTION_ON_PRESS);
    }

    double value;
    file.readProperty("value", &value, 10.0);

    ENG::Console::LogDebug(value);

    camera.SetZoom(0.1);

    while (ENG::Update())
    {
        ENG::Dingus::SetWorldTimeStep(ENG::globalTimer.GetDelta() * 10);

        if (ENG::Input::Mouse::Left)
        {
            nearest->ApplyForce((ENG::Input::Mouse::GetMouseWorldPos(&camera) - nearest->GetPosition()).Scale(500, true));
        }
        window.SetCursorScale(ENG::Vector2<float>((cosf(ENG::globalTimer.GetSecond() * 10.0f) * 0.5f) + 1.0f, (sinf(ENG::globalTimer.GetSecond() * 10.0f) * 0.5f) + 1.0f));
    }

    file.writeProperty("value", ENG::globalTimer.GetFPS());
    ENG::Shutdown();
    return 0;
}