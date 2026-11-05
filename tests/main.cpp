#include <DNG2D/Main.hpp>

DNG::Window window = DNG::Window("le wrapler rawr", DNG::Vector2(640, 480), SDL_WINDOW_RESIZABLE);

DNG::Camera camera = DNG::Camera(&window);

DNG::Texture test = DNG::Texture(&window, "assets/textures/billGates.bmp");
DNG::Texture cursor = DNG::Texture(&window, "assets/textures/cursor.png");
DNG::Texture cursor2 = DNG::Texture(&window, "assets/textures/cursorpressed.png");
DNG::Texture chud = DNG::Texture(&window, "assets/textures/chud.png");

DNG::Atlas fireAtlas = DNG::Atlas::Create(&window, "assets/textures/fire.png");

DNG::AnimatedTexture fire = DNG::AnimatedTexture(&fireAtlas, 8, 1);

DNG::File file = DNG::File("test.json");

DNG::Font font = DNG::Font("assets/fonts/Retro.Gaming.Extended.ttf", 32);

void WhenBallClicked(DNG::Dingus *me)
{
    if (me->GetName() == "ball")
    {
        me->Destroy();
    }
}

DNG::Dingus *nearest = nullptr;

void ResetCursor()
{
    window.SetCustomCursor(cursor);
}
DNG::RunOnce resetCursor(ResetCursor);
void LayerAndCalcNearest()
{
    nearest->SetRenderLayer(0);
    window.SetCustomCursor(cursor2);
    nearest = DNG::Dingus::GetNearest(DNG::Input::Mouse::GetMouseWorldPos(&camera), "ball");
    nearest->SetRenderLayer(1);
}
DNG::RunOnce layerAndCalcNearest(LayerAndCalcNearest);

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
    camera.ChangePositionBy(0, 200.0 / camera.GetZoom(), DNG::globalTimer.GetDelta());
}
void MoveDown()
{
    camera.ChangePositionBy(0, -200.0 / camera.GetZoom(), DNG::globalTimer.GetDelta());
}
void MoveLeft()
{
    camera.ChangePositionBy(-200.0 / camera.GetZoom(), 0, DNG::globalTimer.GetDelta());
}
void MoveRight()
{
    camera.ChangePositionBy(200.0 / camera.GetZoom(), 0, DNG::globalTimer.GetDelta());
}
void ZoomIn()
{
    camera.ChangeZoomBy(1.0 * camera.GetZoom(), DNG::globalTimer.GetDelta());
}
void ZoomOut()
{
    camera.ChangeZoomBy(-1.0 * camera.GetZoom(), DNG::globalTimer.GetDelta());
}
void RotateC()
{
    camera.ChangeAngleBy(-30.0, DNG::globalTimer.GetDelta());
}
void RotateCC()
{
    camera.ChangeAngleBy(30.0, DNG::globalTimer.GetDelta());
}

DNG::Action zoomIn = DNG::Action(ZoomIn, SDL_SCANCODE_UP);
DNG::Action zoomOut = DNG::Action(ZoomOut, SDL_SCANCODE_DOWN);
DNG::Action rotateC = DNG::Action(RotateC, SDL_SCANCODE_RIGHT);
DNG::Action rotateCC = DNG::Action(RotateCC, SDL_SCANCODE_LEFT);
DNG::Action moveRight = DNG::Action(MoveRight, SDL_SCANCODE_D);
DNG::Action moveLeft = DNG::Action(MoveLeft, SDL_SCANCODE_A);
DNG::Action moveUp = DNG::Action(MoveUp, SDL_SCANCODE_W, ACTION_WHILE_HELD);
DNG::Action moveDown = DNG::Action(MoveDown, SDL_SCANCODE_S);

DNG::Action mouseLeft = DNG::Action(MouseLeft, SDL_BUTTON_LEFT);
DNG::Action mouseRight = DNG::Action(MouseRight, SDL_BUTTON_RIGHT);

int main()
{
    DNG::Init();

    window.SetCustomCursor(cursor);
    window.SetCursorScale(2.0f);

    DNG::Dingus *ground = DNG::Dingus::Create(&camera, &chud, DNG::Transform2<float>(0, -500, 5000, chud.GetSize().y));
    ground->AssignCollisionShape(DNG::CollisionShape::CreateBox(ground->GetTransform().size), 1, 0.25, 0.5)->SetStatic();
    DNG::Dingus *wall1 = DNG::Dingus::Create(&camera, &chud, DNG::Transform2<float>(2500, 2000, chud.GetSize().x, 5000));
    wall1->AssignCollisionShape(DNG::CollisionShape::CreateBox(wall1->GetTransform().size), 1, 0.25, 0.5)->SetStatic();
    DNG::Dingus *wall2 = DNG::Dingus::Create(&camera, &chud, DNG::Transform2<float>(-2500, 2000, chud.GetSize().x, 5000));
    wall2->AssignCollisionShape(DNG::CollisionShape::CreateBox(wall2->GetTransform().size), 1, 0.25, 0.5)->SetStatic();

    for (size_t i = 0; i < 100; ++i)
    {
        DNG::Dingus *ball = DNG::Dingus::Create(&camera, &chud, DNG::Transform2<float>(0, 1280 * i, chud.GetSize() * DNG::Vector2<int>(1, 1)));
        ball->AssignCollisionShape(DNG::CollisionShape::CreateBox(chud.GetSize() * DNG::Vector2<int>(1, 1)), 1, 0.25, 0.5);
        ball->SetName("ball");
        ball->AssignAction(WhenBallClicked, SDL_BUTTON_RIGHT, ACTION_ON_PRESS);
    }

    DNG::Dingus *animtest = DNG::Dingus::Create(&camera, &fire, DNG::Transform2<float>(0,0,100,100,0));

    double value;
    file.readProperty("value", &value, 10.0);

    DNG::Console::LogDebug(value);

    camera.SetZoom(0.1);

    while (DNG::Update())
    {
        DNG::Dingus::SetWorldTimeStep(DNG::globalTimer.GetDelta() * 10);

        if (DNG::Input::Mouse::Left)
        {
            nearest->ApplyForce((DNG::Input::Mouse::GetMouseWorldPos(&camera) - nearest->GetPosition()).Scale(500, true));
        }
        window.SetCursorScale(DNG::Vector2<float>((cosf(DNG::globalTimer.GetSecond() * 10.0f) * 0.5f) + 1.0f, (sinf(DNG::globalTimer.GetSecond() * 10.0f) * 0.5f) + 1.0f));
        animtest->SetPosition(DNG::Input::Mouse::GetMouseWorldPos(&camera));
    }

    file.writeProperty("value", DNG::globalTimer.GetFPS());
    DNG::Shutdown();
    return 0;
}