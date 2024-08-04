#pragma once
#include <bn_fixed.h>
#include <bn_string.h>
#include "bn_sprite_palette_items_aru_palette_green.h"
#include "bn_sprite_palette_items_aru_palette_purple.h"
#include "bn_sprite_palette_items_aru_palette_orange.h"
#include "bn_sprite_items_slung_portrait_neutral.h"
#include "bn_sprite_items_slung_portrait_surprised.h"
#include "bn_sprite_items_slung_portrait_sad.h"

namespace aru 
{
    constexpr uint32_t MAX_UINT32 = 4294967295;
    constexpr uint16_t MAX_UINT16 = 65535;
    constexpr uint8_t MAX_UINT8 = 255;

    constexpr bn::fixed INVSQRT2 = bn::fixed(0.707107);
    constexpr bn::fixed INVSQRT54THS = bn::fixed(0.894421);

    constexpr bn::fixed GRAVITY_ACCELERATION = bn::fixed(0.5);
    
    constexpr bn::fixed PLAYER_WIDTH = 16;
    constexpr bn::fixed PLAYER_HEIGHT = 36;
    constexpr bn::fixed PLAYER_MAX_XSPEED = 2;
    constexpr bn::fixed PLAYER_FALL_YSPEED = 5;
    constexpr bn::fixed PLAYER_JUMP_YSPEED = -5;

    //the cap on the amount of time you can extend your upward momentum by holding a
    //basically you know how you can hold a for a higher jump like in mario? 
    //This is how long you can hold a 
    constexpr uint8_t PLAYER_JUMP_HOLD_A_CAP = 6;
    constexpr uint8_t PLAYER_HP = 50;

    //how much damage when someone comes into contact with the player
    constexpr uint8_t PLAYER_CONTACT_DAMAGE = 2;

    //how many invincibility frames you get after being hit
    constexpr uint8_t PLAYER_IFRAMES = 120;
    //amount of time you can hover
    constexpr uint8_t PLAYER_HOVER_TIME[3] = {30, 60, 90};
    constexpr const bn::sprite_palette_item PLAYER_PALETTE[3] = {
        bn::sprite_palette_items::aru_palette_orange,
        bn::sprite_palette_items::aru_palette_green,
        bn::sprite_palette_items::aru_palette_purple,
    };

    
    constexpr bn::fixed BULLET_WIDTH = 6;
    constexpr bn::fixed BULLET_HEIGHT = 4;
    constexpr bn::fixed BULLET_MAX_XSPEED = 6;
    constexpr bn::fixed BULLET_MAX_YSPEED = 0;
    constexpr bn::fixed BULLET_FIRE_DELAY = 10;
    constexpr uint8_t BULLET_HP = 1;
    constexpr uint8_t BULLET_CONTACT_DAMAGE = 5;
    constexpr uint8_t BULLET_IFRAMES = 0;

    constexpr bn::fixed TB_ARROW_X = 110;
    constexpr bn::fixed TB_ARROW_Y = 64;
    constexpr bn::fixed TB_ARROW_INC_PER_FRAME = 0.25;
    constexpr uint8_t TB_ARROW_OTIME = 25;

    constexpr uint16_t UM_MAX = 65535;

    constexpr bn::fixed DB_USEL_Y = -58;
    constexpr bn::fixed DB_DSEL_Y = -49;

    struct line_info {
        const char *text;
        const uint8_t anim_index;
        const bn::sprite_item &portrait;
    };

    constexpr const line_info LV1_CUTSCENE_DIALOGUE[18] = {
        {"...", 2, bn::sprite_items::slung_portrait_neutral},
        {"Diagnostics seem fine... Maybe a new module is needed?", 2, bn::sprite_items::slung_portrait_neutral},
        {"...", 2, bn::sprite_items::slung_portrait_neutral},
        {"...", 2, bn::sprite_items::slung_portrait_neutral},
        {"Oh-! ARU-l33n, I didn't notice you warp in. How long have you been standing there?", 1, bn::sprite_items::slung_portrait_surprised},
        {"Right, well, anyways... How did everything go?", 1, bn::sprite_items::slung_portrait_neutral},
        {"Ah, that's it! That's the part!!! Wonderful, wonderful! I knew I could trust you. Now let me just...", 1, bn::sprite_items::slung_portrait_neutral},
        {"...", 1, bn::sprite_items::slung_portrait_neutral},
        {"...No dice. Well, it was only the first of many. Thank you, ARU-l33n. You did well, very well. I'm already picking up a signal from the next piece, and I'm updating the teleporter coordinates now. So when you're ready...", 1, bn::sprite_items::slung_portrait_neutral},
        {"Ah- wait! I almost forgot to mention. During your adventure you likely picked up clusters of Ultra-Matter. We can put those to use.", 1, bn::sprite_items::slung_portrait_neutral},        
        {"Since you're the one putting yourself in harm's way, I'll let you choose what to modify and when. For simplicity's sake I will categorize them: SHOOT, SLASH and HOVER.", 1, bn::sprite_items::slung_portrait_neutral},
        {"Hehe... Quite like a video game, actually", 1, bn::sprite_items::slung_portrait_neutral},
        {"Um... sorry, don't mind me.", 1, bn::sprite_items::slung_portrait_sad},
        {"Anywho... How does that sound? Would you like to upgrade your systems now? Just step over to the terminals before heading out in the warp and press A to see what each will require.", 1, bn::sprite_items::slung_portrait_neutral},
        {"Oh, and... completely up to you, but if you have any resources to spare, I've also added the option to donate Energy Clusters to help with the completion of the machine. You can deposit energy into the machine by pressing A next to it.", 1, bn::sprite_items::slung_portrait_neutral},
        {"Though please, don't feel bad if you use them for upgrades instead. I'm sure I can still complete it without them, it will just help the process along...", 1, bn::sprite_items::slung_portrait_neutral},
        {"Probably.", 1, bn::sprite_items::slung_portrait_neutral},
        {"Enough of my rambling- I know you're very busy! Thank you for your work, ARU-l33n, and be safe!", 1, bn::sprite_items::slung_portrait_neutral}
    };

}