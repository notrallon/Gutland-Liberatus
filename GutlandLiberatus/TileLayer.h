#pragma once

#include "Layer.h"
#include "Map.h"

/*
   ===========================================================
   Create a Layer of tiles that is rendered on our screen.
   ===========================================================
 */
class TileLayer: public Layer
{
public:
                            TileLayer(SharedContext * context, TileSet * tileset, std::string layerName);
                            ~TileLayer();

    virtual void            Update();
    virtual void            Draw();

    virtual Tile*           GetTile(unsigned int x, unsigned int y);
    virtual std::string     GetLayerName() const;

    void                    CreateLayer(TiXmlElement* tileData, int mapWidth);

private:
    SharedContext *m_context;
    TileSet       *m_tileSet;
    Map           *m_gameMap;
    TileMap       m_tileMap;
    std::string   m_layerName;
};
