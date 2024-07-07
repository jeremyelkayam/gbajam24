#pragma once
#include <bn_fixed.h>
#include <bn_string.h>

namespace aru 
{
    constexpr bn::fixed INVSQRT2 = bn::fixed(0.707107);
    constexpr bn::fixed INVSQRT54THS = bn::fixed(0.894421);
    
    constexpr bn::fixed PLAYER_WIDTH = 16;
    constexpr bn::fixed PLAYER_HEIGHT = 36;
    constexpr bn::fixed PLAYER_MAX_XSPEED = 3;
    constexpr bn::fixed PLAYER_MAX_YSPEED = 8;
    constexpr uint8_t PLAYER_HP = 50;
    constexpr uint8_t PLAYER_CONTACT_DAMAGE = 2;
    constexpr uint8_t PLAYER_IFRAMES = 120;
    constexpr uint8_t PLAYER_HOVER_TIME = 60;
    

    
    constexpr bn::fixed BULLET_WIDTH = 6;
    constexpr bn::fixed BULLET_HEIGHT = 4;
    constexpr bn::fixed BULLET_MAX_XSPEED = 6;
    constexpr bn::fixed BULLET_MAX_YSPEED = 0;
    constexpr uint8_t BULLET_HP = 1;
    constexpr uint8_t BULLET_CONTACT_DAMAGE = 5;
    constexpr uint8_t BULLET_IFRAMES = 0;

    constexpr bn::fixed TB_ARROW_X = 110;
    constexpr bn::fixed TB_ARROW_Y = 68;
    constexpr bn::fixed TB_ARROW_INC_PER_FRAME = 0.25;
    constexpr uint8_t TB_ARROW_OTIME = 25;

    constexpr uint16_t UM_MAX = 65535;

    constexpr bn::fixed DB_USEL_Y = -58;
    constexpr bn::fixed DB_DSEL_Y = -49;
}