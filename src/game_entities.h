#ifndef GAME_H
#define GAME_H

struct game_entity
{
    unsigned int left_score;
    unsigned int right_score;
};

struct paddle_movement
{
    float movement;
    float movement_dir;
};

struct ball_movement
{
    float movementX;
    float movementY;

    bool signX;
    bool signY;

    bool left_center;
    bool right_center;

    float velocityX;
    float velocityY;
};

#endif
