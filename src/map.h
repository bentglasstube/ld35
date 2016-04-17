#pragma once

#include "graphics.h"
#include "rect.h"
#include "spritemap.h"

class Map {
  public:

    struct Tile {
      bool obstruction;
      float friction;
      float top, left, right, bottom;
    };

    Map();

    void load(std::string file);
    void draw(Graphics& graphics, int x_offset, int y_offset);

    Tile tile_at(float x, float y);

    Tile collision(Rect box, float dx, float dy);

    int pixel_width() { return width * 16; }
    int pixel_height() { return height * 16; }

  private:
    SpriteMap tileset;
    int height, width;
    char tiles[128][1024];

    Tile tile(int x, int y);
    Tile check_tile_range(int x1, int x2, int y1, int y2);
};
