//
// Created by Dmitry Khrykin on 2019-08-15.
//

#ifndef STRATEGR_OVERVIEW_H
#define STRATEGR_OVERVIEW_H

#include <vector>
#include <cmath>
#include <optional>

#include "color.h"
#include "geometry.h"

namespace stg {
    class strategy;

    class overview {
    public:
        struct overview_item {
            gfloat origin_x = 0;
            gfloat width = 0;

            stg::color color;
        };

        struct viewport_marker {
            gfloat origin_x = 0;
            gfloat width = 0;
        };

        explicit overview(const strategy &strategy, gfloat width);

        auto elements() -> std::vector<overview_item>;

        auto viewport_marker_for(const rect &slots_bounds,
                                 const rect &viewport_rect) const -> viewport_marker;
        auto scroll_offset_for(point mouse_position,
                               const rect &slots_bounds,
                               const rect &viewport_rect) const -> gfloat;

        auto current_time_position() -> gfloat;

    private:
        const strategy &strategy;
        stg::gfloat width = 0.0;
    };

}

#endif //STRATEGR_OVERVIEW_H
