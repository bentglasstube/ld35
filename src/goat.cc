#include "goat.h"

Goat::Goat() : Player(),
  pushing(false),
  walking("sprites", 0, 32, 16, 16, 4, 14),
  ramming("sprites", 0, 48, 16, 16, 4, 14),
  standing("sprites", 16, 32, 16, 16) {}

// TODO allow goats to push crates

Sprite* Goat::get_sprite() {
  if (jump || !on_ground()) return &standing;
  else if (pushing) return &ramming;
  else if (velo_x != 0) return &walking;
  else return &standing;
}
