#include "death_anim.h"
#include "bn_sprite_items_explosion.h"

namespace aru
{

death_anim::death_anim(player &player, bn::camera_ptr &cam) : 
    _player(player),
    _cam(cam),
    _initial_fade_done(false)
{
    _fade_action.emplace(30, 1);
    _player.clear_rising_text();
    _player.set_visible(true);
    bn::blending::set_black_fade_color();

    for(const bn::fixed_point &coord : EXPLOSION_COORDS) 
    {
        _explosion_coords.push_back(coord);
    }

    //todo: turn off all the backgrounds once fading is done 
    // and set the transparent color to pure black

}

void death_anim::update()
{
    if(!_initial_fade_done){
        _fade_action->update();
        if(_fade_action->done()){
            _explosions.emplace_front(_cam, bn::sprite_items::explosion, _player.pos() + _explosion_coords.front());
            _explosion_coords.pop_front();
            _fade_action.reset();
            _initial_fade_done = true;
            bn::blending::set_white_fade_color();
            bn::blending::set_fade_alpha(0);
            _player.set_blending_enabled(true);
            _fade_action.emplace(90, 1);
        }
    }else{
        if(_fade_action && !_fade_action->done()){
            _fade_action->update();
        }
        if(!_explosion_coords.empty() && !_explosions.empty() && 
            !_explosions.full() && _explosions.front().current_index() > 1){
            _explosions.emplace_front(_cam, bn::sprite_items::explosion, _player.pos() + _explosion_coords.front());
            _explosion_coords.pop_front();
        }
        for(auto &splosion : _explosions) {
            splosion.update();
            if(splosion.done() && !_explosion_coords.empty()){
                splosion.reset(_player.pos() + _explosion_coords.front());
                _explosion_coords.pop_front();
            }
        }
        bn::erase_if(_explosions, anim_spr_done);
        if(_explosion_coords.empty() && !_trans_alpha_action){
            _player.set_palette(
                common_stuff::monochrome_sprite_palette(bn::color(31,31,31)));
            bn::blending::set_fade_alpha(0);
            _trans_alpha_action.emplace(40, 0);
            _hscale_action.emplace(_player.sprite(), 40, 2);
            _vscale_action.emplace(_player.sprite(), 40, 2);
            _mosaic_action.emplace(40, 1);
        }

        if(_trans_alpha_action){
            if(!_trans_alpha_action->done()){
                _trans_alpha_action->update();
                _hscale_action->update();
                _vscale_action->update();
                _mosaic_action->update();
            }
            if(_trans_alpha_action->done()){
                bn::blending::set_transparency_alpha(1);
                _player.set_palette(
                    common_stuff::monochrome_sprite_palette(bn::color(0,0,0)));
            }
        }
    }
}

bool death_anim::done() const
{
    return _trans_alpha_action && _trans_alpha_action->done();
}

animated_sprite::animated_sprite(bn::camera_ptr &cam, const bn::sprite_item &spritem, 
    const bn::fixed_point &pos) : 
    _sprite(spritem.create_sprite(pos)),
    _anim(bn::create_sprite_animate_action_once(
        _sprite, 4, spritem.tiles_item(), 0, 1, 2, 3, 4, 5, 6)
    )
{
    _sprite.set_camera(cam);
}

void animated_sprite::reset(const bn::fixed_point &pos)
{
    _sprite.set_position(pos);
    _anim.reset();
}

}

