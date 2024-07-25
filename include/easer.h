#pragma once
#include <bn_fixed.h>
#include <bn_math.h>

namespace aru {

class easer {
    private:
        const uint8_t _ease_time;
        uint8_t _timer;
        bn::fixed _start, _target;
        bn::fixed (*_ease_func)(const bn::fixed &);

    public:
        easer(const bn::fixed &start, const bn::fixed &target, 
            const uint8_t &ease_time, bn::fixed (*ease_func)(const bn::fixed &));
        void set_target(const bn::fixed &target);
        void update();
        bool done(){return _timer == _ease_time;}
        bn::fixed current();
        bn::fixed ease_pct()
            {return bn::fixed(_timer) / bn::fixed(_ease_time);}

        static bn::fixed quadratic_ease_in_out(const bn::fixed &p);
        static bn::fixed bezier_ease_in_out(const bn::fixed &p);
        static bn::fixed sine_ease_out(const bn::fixed &p)
                {return bn::degrees_sin(p * 90);}
        static bn::fixed sine_ease_in(const bn::fixed &p)
                {return bn::degrees_sin(p * 90);}
};

}