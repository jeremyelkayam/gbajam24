#include "easer.h"

namespace aru
{
    
easer::easer(const bn::fixed &start, const bn::fixed &target, 
    const uint8_t &ease_time, bn::fixed (*ease_func)(const bn::fixed &)) : 
        _ease_time(ease_time),
        _timer(0),
        _start(start),
        _target(target),
        _ease_func(ease_func)
{

}

void easer::set_target(const bn::fixed &target)
{
    if(target != _target){
        _start = current();
        _target = target;
        _timer = 0;
    }
}

void easer::update()
{
    if(_timer < _ease_time){
        ++_timer;
    }
}

bn::fixed easer::current(){
    return _start + (_target - _start) * _ease_func(ease_pct());
}

bn::fixed easer::quadratic_ease_in_out(const bn::fixed &p)
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

bn::fixed easer::bezier_ease_in_out(const bn::fixed &p)
{
	return p * p * (bn::fixed(3) - bn::fixed(2) * p);
}

} // namespace aru
