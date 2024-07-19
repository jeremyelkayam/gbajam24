#include "camera_manager.h"

#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_math.h>

namespace aru {

camera_manager::camera_manager(bn::camera_ptr cam, const level &lv, const player &pl) :
    _cam(cam),
    _level(lv),
    _player(pl),
    _x_ease(_cam.x() - _player.x(), 50 * (_player.facing_right() ? 1 : -1), 60),
    _y_ease(_cam.x() - _player.x(), _cam.x() - _player.x(), 120) {

}

void camera_manager::update(){

    _x_ease.set_target(50 * (_player.facing_right() ? 1 : -1));


    switch(_player.state()){
        case PSTATE::HOVER:
            _y_ease.set_target(-50);
        break;
        case PSTATE::FALL:
            if(_player.time_since_state_change() > 30){
                _y_ease.set_target(50);
            }
        break;
        case PSTATE::JUMP:
        break;
        default:
            _y_ease.set_target(-30);
    }


    _x_ease.update();
    _y_ease.update();

    bn::fixed new_x = _player.x() + _x_ease.current();
    bn::fixed new_y = _player.y() + _y_ease.current();
    if(new_x < 120) new_x = 120;
    if(new_x > (_level.width() - 120)) new_x = _level.width() - 120;

    //don't go past the top row of tiles
    if(new_y < 88) new_y = 88;
    if(new_y > (_level.height() - 80)) new_y = _level.height() - 80;
    _cam.set_position(new_x, new_y);
}

camera_manager::easer::easer(const bn::fixed &start, const bn::fixed &target, 
    const uint8_t &ease_time) : 
        _ease_time(ease_time),
        _timer(0),
        _start(start),
        _target(target)
{

}

void camera_manager::easer::set_target(const bn::fixed &target)
{
    if(target != _target){
        _start = current();
        _target = target;
        _timer = 0;
    }
}

void camera_manager::easer::update()
{
    if(_timer < _ease_time){
        ++_timer;
        BN_LOG(current());
    }
}

bn::fixed camera_manager::easer::current(){
    return _start + (_target - _start) * sine_ease_out(bn::fixed(_timer) / bn::fixed(_ease_time));
}

bn::fixed camera_manager::easer::quadratic_ease_in_out(const bn::fixed &p)
{
	if(p < 0.5)
	{
		return 2 * p * p;
	}
	else
	{
		return (-2 * p * p) + (4 * p) - 1;
	}
}

bn::fixed camera_manager::easer::bezier_ease_in_out(const bn::fixed &p)
{
	return p * p * (bn::fixed(3) - bn::fixed(2) * p);
}

bn::fixed camera_manager::easer::sine_ease_out(const bn::fixed &p)
{
	return bn::degrees_sin(p * 90);
}

}
