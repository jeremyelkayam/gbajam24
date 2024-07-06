#pragma once
#include <bn_sprite_animate_actions.h>
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
    virtual bn::deque<bn::unique_ptr<box>, 16> interact_boxes()=0;

protected:
    common_stuff &_cstuff;
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

}