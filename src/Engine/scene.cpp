#include "Engine/scene.h"

#include "Engine/app.h"
#include "Input/controller.h"

Scene::Scene()
: m_spController(std::make_unique<Controller>())
{
}

Scene::~Scene()
{
}