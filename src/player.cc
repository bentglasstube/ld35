#include "player.h"

#include "accelerators.h"

namespace {
  const float kFriction         = 0.0005f;

  const float kWalkingAccel     = 0.0005f;
  const float kAirAccel         = 0.0003f;
  const float kMaxSpeedX        = 0.15f;
  const float kJumpSpeed        = 0.35f;

  const float kGravity          = 0.0012f;
  const float kTerminalVelocity = 0.5f;
}

Player::Player() : accel_x(0), velo_x(0), velo_y(0), pos_x(0), pos_y(0), sprite("sprites", 0, 0, 16, 32, 4, 14) {}

void Player::update(unsigned int elapsed, Map map) {
  if (elapsed > 50) elapsed = 50;
  update_x(elapsed, map);
  update_y(elapsed, map);
}

void Player::draw(Graphics& graphics) {
  sprite.draw(graphics, pos_x - 8, pos_y - 32, facing == LEFT ? Graphics::FlipDirection::HORIZONTAL : Graphics::FlipDirection::NONE);
}

void Player::start_moving_left() {
  accel_x = -1;
  facing = LEFT;
}

void Player::start_moving_right() {
  accel_x = 1;
  facing = RIGHT;
}

void Player::stop_moving() {
  accel_x = 0;
}

void Player::start_jumping() {
  if (velo_y == 0) {
    jump = true;
    velo_y = -kJumpSpeed;
  }
}

void Player::stop_jumping() {
  jump = false;
}

void Player::set_position(float x, float y) {
  pos_x = x;
  pos_y = y;
}

bool Player::on_ground() const {
  return velo_y == 0;
}

void Player::update_x(unsigned int elapsed, Map map) {
  velo_x += accel_x * (on_ground() ? kWalkingAccel : kAirAccel) * elapsed;

  if (on_ground()) {
    // TODO get friction from ground tile
    velo_x = FrictionAccelerator(kFriction).update_velocity(velo_x, elapsed);
    if (accel_x != 0) velo_x = ConstAccelerator(accel_x * kWalkingAccel, accel_x * kMaxSpeedX).update_velocity(velo_x, elapsed);
  } else {
    if (accel_x != 0) velo_x = ConstAccelerator(accel_x * kAirAccel, accel_x * kMaxSpeedX).update_velocity(velo_x, elapsed);
  }

  if (map.collision(box_col_h(), velo_x * elapsed, 0)) {
    velo_x = 0;
  } else {
    pos_x += velo_x * elapsed;
  }
}

void Player::update_y(unsigned int elapsed, Map map) {
  velo_y = ConstAccelerator(kGravity, kTerminalVelocity).update_velocity(velo_y, elapsed);

  if (map.collision(box_col_v(), 0, velo_y * elapsed)) {
    velo_y = 0;
  } else {
    pos_y += velo_y * elapsed;
  }
}

Rect Player::box_col_h() { return Rect(pos_x - 8, pos_y - 32, pos_x + 8, pos_y); }
Rect Player::box_col_v() { return Rect(pos_x - 8, pos_y - 32, pos_x + 8, pos_y); }
