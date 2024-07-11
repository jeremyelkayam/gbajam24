#pragma once
#include <bn_sprite_animate_actions.h>
#include <bn_vector.h>
#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>
#include <bn_fixed_rect.h>
#include <bn_camera_ptr.h>
#include <bn_deque.h>
#include <bn_unique_ptr.h>
#include "entity.h"
#include "box.h"
#include "common_stuff.h"

namespace aru
{

class interactable_entity : public entity
{
public:
    interactable_entity(const bn::camera_ptr &cam, const bn::fixed &x, 
        const bn::fixed &y, const bn::fixed &width, const bn::fixed &height, 
        const bn::sprite_item &spritem, common_stuff &cstuff);
    
    virtual ~interactable_entity() {return;}
    virtual void update();
    void set_current_anim(uint8_t index);
    virtual bn::deque<bn::unique_ptr<box>, 16> interact_boxes()=0;

protected:
    uint8_t _current_anim;
    common_stuff &_cstuff;
    bn::vector<bn::sprite_animate_action<16>,4> _anims;
};

class slung : public interactable_entity
{
public:
    slung(const bn::camera_ptr &cam, const bn::fixed &x, 
        const bn::fixed &y, common_stuff &cstuff);
    virtual bn::deque<bn::unique_ptr<box>, 16> interact_boxes();
};

class vax_mchn: public interactable_entity
{
public:
    vax_mchn(const bn::camera_ptr &cam, const bn::fixed &x, 
        const bn::fixed &y, common_stuff &cstuff);
    virtual bn::deque<bn::unique_ptr<box>, 16> interact_boxes();
};

class hover_upgrader: public interactable_entity
{
public:
    hover_upgrader(const bn::camera_ptr &cam, const bn::fixed &x, 
        const bn::fixed &y, common_stuff &cstuff);
    virtual bn::deque<bn::unique_ptr<box>, 16> interact_boxes();
};

class shoot_upgrader: public interactable_entity
{
public:
    shoot_upgrader(const bn::camera_ptr &cam, const bn::fixed &x, 
        const bn::fixed &y, common_stuff &cstuff);
    virtual bn::deque<bn::unique_ptr<box>, 16> interact_boxes();
};

class slash_upgrader: public interactable_entity
{
public:
    slash_upgrader(const bn::camera_ptr &cam, const bn::fixed &x, 
        const bn::fixed &y, common_stuff &cstuff);
    virtual bn::deque<bn::unique_ptr<box>, 16> interact_boxes();
};

}