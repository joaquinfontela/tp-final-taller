#ifndef DECORATION_TILE_FACTORY_H
#define DECORATION_TILE_FACTORY_H

#include "tile.h"
#include "tile_factory.h"
#include <vector>

class decoration_tile_factory : public tile_factory
{
public:
    decoration_tile_factory();
    tile* get_tile(std::vector<int>& coordinates) override;
};

#endif // DECORATION_TILE_FACTORY_H