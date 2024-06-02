#include "effect.h"
#include <bn_log.h>

namespace aru {

effect::effect(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, const bn::sprite_item &spritem, const uint8_t &indices) : 
    _sprite(spritem.create_sprite(x,y)),
    _anim(bn::sprite_animate_action<10>::once(_sprite,1,spritem.tiles_item(), indices_span(indices)))
{
    _sprite.set_camera(cam);
    _sprite.set_visible(false);
}

bn::span<uint16_t> effect::indices_span(const uint8_t &indices){
    uint16_t gfxindices_arr[10];
    for(uint16_t z = 0; z < indices ; ++z){
        gfxindices_arr[z] = z;
    }

    bn::span<uint16_t> result(gfxindices_arr, &gfxindices_arr[indices]);

    //todo: why does it only work if we log the result ? 
    for(uint16_t z : result){
        BN_LOG("index: ", z);
    }

    return result;
}

void effect::update(){
    if(_sprite.visible() && !_anim.done()){
        _anim.update();
    }else{
        _sprite.set_visible(false);
    }
}

void effect::start(const bn::fixed &x, const bn::fixed &y){
    _sprite.set_position(x,y);
    _sprite.set_visible(true);
    _anim.reset();
}


}