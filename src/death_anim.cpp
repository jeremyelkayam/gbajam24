#include "death_anim.h"
#include "bn_sprite_items_explosion.h"

namespace aru
{

death_anim::death_anim(player &player) : 
    _player(player)
{
    _fade_action.emplace(30, 1);
    _player.clear_rising_text();
    _player.set_visible(true);
    bn::blending::set_black_fade_color();

}

void death_anim::update()
{
    if(_fade_action){
        _fade_action->update();
        if(_fade_action->done()){
            // _explosions.emplace_back(bn::sprite_items::explosion, -6, -10);
            // _explosions.emplace_back(bn::sprite_items::explosion, 15, 15);
            _fade_action.reset();
        }
    }
    for(auto &splosion : _explosions) {
        splosion.update();
    }
}

bool death_anim::done() const
{
    return false;
}

animated_sprite::animated_sprite(bn::camera_ptr &cam, const bn::sprite_item &spritem, const bn::fixed &x, const bn::fixed &y) : 
    _sprite(spritem.create_sprite(x, y)),
    _anim(bn::create_sprite_animate_action_once(
        _sprite, 4, spritem.tiles_item(), 0, 1, 2, 3, 4, 5, 6)
    )
{
    _sprite.set_camera(cam);
}

void animated_sprite::reset(const bn::fixed &x, const bn::fixed &y)
{
    _sprite.set_position(x, y);
    _anim.reset();
}

}

