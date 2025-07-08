#pragma once

#include "Drawable.h"
#include "raylib.h"
#include "ResourceManager.h"
#include <map>


class Screen : public Drawable {
    protected:
    std::map<std::string, Texture2D>& textures;

    public:
    Screen();
    virtual ~Screen() override = default;
    virtual void draw() = 0;
};