#include "combat_entity.h"
#include "bn_sprite_items_explosion.h"
#include <bn_log.h>

namespace aru 
{
combat_entity::combat_entity(const bn::camera_ptr &cam, const bn::fixed &x, 
        const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, 
        const bn::fixed &max_xspeed, const bn::fixed &max_up_speed, 
        const bn::fixed &max_down_speed, const uint8_t &hp, 
        const uint8_t &contact_damage, const uint8_t &iframes, level &level, 
        const bn::sprite_item &spritem, 
        bn::sprite_text_generator &rising_text_generator): 
    physics_entity(cam, x, y, width, height, max_xspeed, 
        max_up_speed, max_down_speed, level, spritem),
    _rising_text_generator(rising_text_generator),
    _HIT_IFRAMES(iframes),
    _max_hp(hp),
    _hp(hp),
    _contact_damage(contact_damage),
    _iframes(0)
{

}

void combat_entity::update(){
    physics_entity::update();

    if(_iframes){
        _sprite.set_visible(_iframes % 2);
        --_iframes;
    }

    if(_explosion_anim && !_explosion_anim->done()) _explosion_anim->update();

    for(rising_text &r : _rising_text){
        r.update();
    }
    
    bn::erase_if(_rising_text, rising_text_deletable);

}


void combat_entity::hit(uint8_t damage, bn::fixed x_push, bn::fixed y_push){
    if(!_iframes && !_explosion_anim){

        if(_hp > damage){
            _hp -= damage;

            bn::fixed new_angle = _sprite.rotation_angle() - 2*x_push;
            if(new_angle < 0){
                new_angle += 360;
            }else if(360 <= new_angle){
                new_angle -= 360;
            }
            _sprite.set_rotation_angle(new_angle);

            _xspeed += x_push;

            _yspeed += y_push;

        }else{
            _hp = 0;
            die();
        }

        if(_rising_text.full()){
            _rising_text.pop_back();
        }
        _rising_text.emplace_front(*_sprite.camera().get(), _rising_text_generator, 
            -damage, x(), y());        

        _iframes = _HIT_IFRAMES;
    }

    //TODO ALSO: maybe add a bit of punchiness to the attack like freeze frames or shake.
}

void combat_entity::die(){
    _sprite.set_item(bn::sprite_items::explosion);
    _sprite.set_scale(2);
    _explosion_anim.emplace(bn::create_sprite_animate_action_once(_sprite, 4, bn::sprite_items::explosion.tiles_item(), 0, 1, 2, 3, 4, 5, 6));
}

void combat_entity::set_blending_enabled(bool enabled)
{
    entity::set_blending_enabled(enabled);
    for(auto &text : _rising_text)
    {
        text.set_blending_enabled(enabled);
    }
}

void combat_entity::set_mosaic_enabled(bool enabled)
{
    entity::set_mosaic_enabled(enabled);
    for(auto &text : _rising_text)
    {
        text.set_mosaic_enabled(enabled);
    }
}

void combat_entity::set_visible(const bool &visible)
{
    entity::set_visible(visible);
    for(auto &text : _rising_text)
    {
        text.set_visible(visible);
    }
}

}