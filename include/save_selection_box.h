#pragma once
#include "selection_box.h"

namespace aru { 
    class save_selection_box : public selection_box {
        private:
            common_stuff &_cstuff;

        public:
            save_selection_box(common_stuff &cstuff);
            void update();
            virtual bn::unique_ptr<box> next_box();
    };
}