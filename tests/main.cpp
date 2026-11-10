#include <DNG2D/Main.hpp>

DNG::Audio boom = DNG::Audio("assets/sounds/vine-boom.mp3");
DNG::Audio drop = DNG::Audio("assets/sounds/drop.wav");

class thing : public DNG::Dingus
{
private:
public:
    thing(DNG::Camera *camera, DNG::Texture *texture, DNG::Transform2<float> transform)
    {
        AssignCamera(camera);
        AssignTexture(texture);
        SetTransform(transform);
        AssignCollisionShape(DNG::CollisionShape::CreateBox(texture->GetSize()));
    }
    thing(DNG::Camera *camera, DNG::AnimatedTexture *texture, DNG::Transform2<float> transform)
    {
        AssignCamera(camera);
        AssignTexture(texture);
        SetTransform(transform);
        AssignCollisionShape(DNG::CollisionShape::CreateBox(texture->GetSize()));
    }

    void Update() override
    {
        if (GetPosition().y < -1000)
        {
            Destroy();
        }
    }

    void OnCollision(Dingus *other) override
    {
        float relVel = (GetVelocity() - other->GetVelocity()).Magnitude();

        // boom.Play(DNG::Math::Clamp(relVel/50.0f, 0.0f, 1.0f));
        if (relVel > 15.0f)
            DNG::Audio::Play(&drop, DNG::Math::Clamp(relVel / 100.0f, 0.0f, 1.0f), DNG::Random::GetNext(0.75f, 1.25f));
    }

    void MouseDown() override
    {
        if (DNG::Input::Mouse::Right)
        {
            Destroy();
        }
    }
};

DNG::Window window = DNG::Window("le wrapler rawr", DNG::Vector2(640, 480), SDL_WINDOW_RESIZABLE);

DNG::Camera camera = DNG::Camera(&window);

DNG::Texture test = DNG::Texture(&window, "assets/textures/billGates.bmp");
DNG::Texture cursor = DNG::Texture(&window, "assets/textures/cursor.png");
DNG::Texture cursor2 = DNG::Texture(&window, "assets/textures/cursorpressed.png");
DNG::Texture chud = DNG::Texture(&window, "assets/textures/chud.png");

DNG::Texture firetex = DNG::Texture(&window, "assets/textures/fire.png");
DNG::Atlas fireAtlas = DNG::Atlas(&firetex);
DNG::AnimatedTexture fire = DNG::AnimatedTexture(&fireAtlas, 8, 1);

DNG::File file = DNG::File("test.json");

DNG::Font font = DNG::Font("assets/fonts/Retro.Gaming.Extended.ttf", 32);

DNG::Dingus *nearest = nullptr;

void LayerAndCalcNearest()
{
    nearest->SetRenderLayer(0);
    window.SetCustomCursor(cursor2);
    nearest = DNG::Dingus::GetNearest(DNG::Input::Mouse::GetMouseWorldPos(&camera), "ball");
    nearest->SetRenderLayer(1);
}

void MouseLeft()
{
    window.SetCustomCursor(cursor2);
    thing *newDing = new thing(&camera, &fire, DNG::Transform2<float>(DNG::Input::Mouse::GetMouseWorldPos(&camera), chud.GetSize()));
    newDing->SetFramerate(24);
}
void MouseLeftUp()
{
    window.SetCustomCursor(cursor);
}

void MouseRight()
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

void ShakeWorld()
{
    for (auto d : DNG::Dingus::GetAllInstances())
    {
        d->ApplyForce(DNG::Vector2<float>(1000000.0f, 0.0f).Rotate(DNG::globalTimer.GetSecond() * 360.0));
    }
}

DNG::Action zoomIn = DNG::Action(ZoomIn, SDL_SCANCODE_UP);
DNG::Action zoomOut = DNG::Action(ZoomOut, SDL_SCANCODE_DOWN);
DNG::Action rotateC = DNG::Action(RotateC, SDL_SCANCODE_RIGHT);
DNG::Action rotateCC = DNG::Action(RotateCC, SDL_SCANCODE_LEFT);
DNG::Action moveRight = DNG::Action(MoveRight, SDL_SCANCODE_D);
DNG::Action moveLeft = DNG::Action(MoveLeft, SDL_SCANCODE_A);
DNG::Action moveUp = DNG::Action(MoveUp, SDL_SCANCODE_W, ACTION_WHILE_HELD);
DNG::Action moveDown = DNG::Action(MoveDown, SDL_SCANCODE_S);

DNG::Action mouseLeft = DNG::Action(MouseLeft, SDL_BUTTON_LEFT, ACTION_ON_PRESS);
DNG::Action mouseLeftUp = DNG::Action(MouseLeftUp, SDL_BUTTON_LEFT, ACTION_ON_RELEASE);
DNG::Action mouseRight = DNG::Action(MouseRight, SDL_BUTTON_RIGHT, ACTION_ON_PRESS);

DNG::Action whileSpace = DNG::Action(ShakeWorld, SDL_SCANCODE_SPACE, ACTION_WHILE_HELD);

int main()
{
    DNG::Init();

    window.SetCustomCursor(cursor);
    window.SetCursorScale(2.0f);

    DNG::Dingus *ground = new DNG::Dingus(&camera, &chud, DNG::Transform2<float>(0, -500, 5000, chud.GetSize().y));
    ground->AssignCollisionShape(DNG::CollisionShape::CreateBox(ground->GetTransform().size), 1, 0.25, 0.5)->SetStatic();
    DNG::Dingus *wall1 = new DNG::Dingus(&camera, &chud, DNG::Transform2<float>(2500, 2000, chud.GetSize().x, 5000));
    wall1->AssignCollisionShape(DNG::CollisionShape::CreateBox(wall1->GetTransform().size), 1, 0.25, 0.5)->SetStatic();
    DNG::Dingus *wall2 = new DNG::Dingus(&camera, &chud, DNG::Transform2<float>(-2500, 2000, chud.GetSize().x, 5000));
    wall2->AssignCollisionShape(DNG::CollisionShape::CreateBox(wall2->GetTransform().size), 1, 0.25, 0.5)->SetStatic();

    for (size_t i = 0; i < 2; ++i)
    {
        DNG::Dingus *ball = new DNG::Dingus(&camera, &fire, DNG::Transform2<float>(0, 1280 * i, chud.GetSize()));
        ball->AssignCollisionShape(DNG::CollisionShape::CreateBox(chud.GetSize() * DNG::Vector2<int>(1, 1)), 1, 0.25, 0.5);
        ball->SetName("ball");
        ball->SetFramerate(24);
    }

    //DNG::Dingus *animtest = new DNG::Dingus(&camera, &fire, DNG::Transform2<float>(0,0,100,100,0));
    //animtest->SetFramerate(12);

    double value;
    file.readProperty("value", &value, 10.0);

    DNG::Console::LogDebug(value);

    camera.SetZoom(0.1);

    while (DNG::Update())
    {
        DNG::Dingus::SetWorldTimeStep(DNG::globalTimer.GetDelta() * 10);

        if (DNG::Input::Mouse::Left)
        {
            // nearest->ApplyForce((DNG::Input::Mouse::GetMouseWorldPos(&camera) - nearest->GetPosition()).Scale(500, true));
        }
        window.SetCursorScale(DNG::Vector2<float>((cosf(DNG::globalTimer.GetSecond() * 10.0f) * 0.5f) + 1.0f, (sinf(DNG::globalTimer.GetSecond() * 10.0f) * 0.5f) + 1.0f));

        //animtest->SetPosition(DNG::Input::Mouse::GetMouseWorldPos(&camera));
    }

    file.writeProperty("value", DNG::globalTimer.GetFPS());
    DNG::Shutdown();
    return 0;
}