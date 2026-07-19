#pragma once
#include "ParticleSystem.h"
#include "config.hpp"
#include <SDL3_image/SDL_image.h>

class ParticleExample : public ParticleSystem
{
public:
  ParticleExample() {}
  virtual ~ParticleExample() {}

  enum PatticleStyle
  {
    NONE,
    FIRE,
    FIRE_WORK,
    SUN,
    GALAXY,
    FLOWER,
    METEOR,
    SPIRAL,
    EXPLOSION,
    SMOKE,
    SNOW,
    RAIN,
  };

  PatticleStyle style_ = NONE;
  void setStyle(PatticleStyle style);

  SDL_Texture* getDefaultTexture()
  {
    if (!_renderer) {
      return nullptr;
    }
    const std::string path =
      Config::get_project_root() + "/assets/images/particles/snow.png";
    static SDL_Texture* t = IMG_LoadTexture(_renderer, path.c_str());
    return t;
  }

  void setTest(int o) {}
};