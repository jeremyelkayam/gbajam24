#pragma once
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>
#include <bn_fixed_rect.h>
#include <bn_camera_ptr.h>

namespace aru
{

class entity
{
public:
    entity(const bn::camera_ptr &cam, const bn::fixed &x, const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, const bn::sprite_item &spritem);
    virtual ~entity() {return;}
    virtual void update()=0;
    bn::fixed_point pos() const {return _hitbox.position();}
    bn::fixed x() const {return _hitbox.x();}
    bn::fixed y() const {return _hitbox.y();}
    bn::fixed_rect hitbox() const {return _hitbox;}

    bool facing_right() const {return _sprite.horizontal_flip();}

    void set_visible(const bool &visible) {_sprite.set_visible(visible);}

    virtual void set_blending_enabled(bool enabled)
        { _sprite.set_blending_enabled(enabled); }

    virtual void set_mosaic_enabled(bool enabled)
        { _sprite.set_mosaic_enabled(enabled); }

    void set_palette(const bn::sprite_palette_item &pal)
        { _sprite.set_palette(pal); }

    bn::sprite_ptr &sprite(){return _sprite;}

protected:
    bn::fixed_rect _hitbox;
    bn::sprite_ptr _sprite;

};

}