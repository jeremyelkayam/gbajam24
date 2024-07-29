#include "death_anim.h"
#include "bn_sprite_items_explosion.h"

namespace aru
{

death_anim::death_anim(player &player) : 
    _player(player),
    _fade_action(30, 1)
{
    for(uint8_t z  = 0; z < _explosions.max_size(); ++z)
    {
        _explosions.emplace_back(bn::sprite_items::explosion.create_sprite(z* 10, z* 10));
        _explosion_anims.emplace_back(
            bn::create_sprite_animate_action_once(_explosions.at(z), 4, 
            bn::sprite_items::explosion.tiles_item(), 0, 1, 2, 3, 4, 5, 6));
    }
}

void death_anim::update()
{
    if(!_fade_action.done()){
        _fade_action.update();
    }else if(!_explosion_anims.at(0).done()){
        for(auto &anim : _explosion_anims){
            anim.update();
        }
    }
}

bool death_anim::done() const
{
    return _explosion_anims.at(0).done();
}
}

