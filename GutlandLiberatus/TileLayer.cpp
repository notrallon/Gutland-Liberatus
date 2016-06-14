#include "TileLayer.h"

TileLayer::TileLayer(SharedContext* context, TileSet* tileset,
                     std::string layerName) :
    m_context(context), m_tileSet(tileset), m_layerName(layerName)
{
    m_gameMap = context->gameMap;
}

TileLayer::~TileLayer()
{
    for (TileMap::iterator itr = m_tileMap.begin(); itr != m_tileMap.end(); ++itr)
    {
        delete itr->second;
        itr->second = nullptr;
    }
    m_tileMap.clear();
    m_tileSet = nullptr;
    m_context->entityManager->Purge();
    m_context = nullptr;
}

void TileLayer::Update()
{
}

Tile* TileLayer::GetTile(unsigned int x, unsigned int y)
{
    TileMap::iterator itr = m_tileMap.find(m_gameMap->ConvertCoords(x, y));
    return(itr != m_tileMap.end() ? itr->second : nullptr);
}

std::string TileLayer::GetLayerName() const
{
    return m_layerName;
}

void TileLayer::Draw()
{
    sf::RenderWindow* window = m_context->window->GetRenderWindow();

    // These 3 lines of code makes sure that anything
    // that is not currently within the viewspace of
    // will not be drawn. Culling.
    sf::FloatRect viewSpace = m_context->window->GetViewSpace();
    sf::Vector2i  tileBegin(floor(viewSpace.left / TILE_SIZE),
                            floor(viewSpace.top / TILE_SIZE));
    sf::Vector2i  tileEnd(ceil((viewSpace.left + viewSpace.width) / TILE_SIZE),
                          ceil((viewSpace.top + viewSpace.height) / TILE_SIZE));

//    unsigned int count = 0;
    for (int y = tileBegin.y; y <= tileEnd.y; ++y)
    {
		for (int x = tileBegin.x; x <= tileEnd.x; ++x)
		{
            if (x < 0 || y < 0)
            {
                continue;
            }

            Tile* tile = GetTile(x, y);
            if (!tile)
            {
                continue;
            }

            sf::Sprite& sprite = tile->properties->m_sprite;
            sprite.setPosition(x * tile->properties->tileWidth, y * tile->properties->tileHeight);
            window->draw(sprite);
//            ++count;

            // Debug.
            if (m_context->debugOverlay.Debug())
            {
                if (tile->properties->m_deadly || tile->warp)
                {
                    sf::RectangleShape* tileMarker = new sf::RectangleShape(
                        sf::Vector2f(TILE_SIZE, TILE_SIZE));
                    tileMarker->setPosition(x * TILE_SIZE, y * TILE_SIZE);
                    if (tile->properties->m_deadly)
                    {
                        tileMarker->setFillColor(sf::Color(255, 0, 0, 100));
                    }
                    else if (tile->warp)
                    {
                        tileMarker->setFillColor(sf::Color(0, 255, 0, 150));
                    }
                    m_context->debugOverlay.Add(tileMarker);
                }
            }
            // End debug.
        }
    }
}

void TileLayer::CreateLayer(TiXmlElement* tileData, int mapWidth)
{
    int x = 0, y = 0;

    for (TiXmlElement* e = tileData->FirstChildElement(); e != NULL; e = e->NextSiblingElement())
    {
        if (x >= mapWidth)
        {
            x = 0;
            ++y;
        }

        if (e->Value() == std::string("tile"))
        {
            int tileID = 0;
            e->Attribute("gid", &tileID);

            tileID--;

            if (tileID < 0)
            {
                ++x; continue;
            }

            TileSet::iterator itr = m_tileSet->find(tileID);

            Tile * tile = new Tile();

            tile->properties = itr->second;

            if (!m_tileMap.emplace(m_gameMap->ConvertCoords(x, y), tile).second)
            {
                // Duplicate tile detected!
                std::cout << "! Duplicate tile! : " << x
                          << "" << y << std::endl;
                delete tile;
                tile = nullptr;
                continue;
            }

            if (tile->properties->warpName == "warp")
            {
                tile->warp = true;
            }

            ++x;
        }
    }
}
