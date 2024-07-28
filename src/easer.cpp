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

bn::fixed easer::current() const
{
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

bn::fixed easer::bounce_ease_out(const bn::fixed &p)
{

	if(p < bn::fixed(0.36363636363))
	{
		return (121 * p * p)/bn::fixed(16.0);
	}
	else if(p < bn::fixed(0.72727272727))
	{
		return (bn::fixed(9.075) * p * p) - (bn::fixed(9.9) * p) + bn::fixed(3.4);
	}
	else if(p < bn::fixed(0.9))
	{
		return (bn::fixed(12.0664819945) * p * p) - (bn::fixed(19.6354570637) * p) + 
            bn::fixed(8.89806094183);
	}
	else
	{
		return (bn::fixed(10.8) * p * p) - (bn::fixed(20.52) * p) + bn::fixed(10.72);
	}
}

bn::fixed easer::back_ease_out(const bn::fixed &p)
{
	bn::fixed f = (1 - p);
	return 1 - (f * f * f - f * degrees_sin(f * 180));
}


} // namespace aru
