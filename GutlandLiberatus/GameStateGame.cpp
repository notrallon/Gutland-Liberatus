#include "GameStateGame.h"
#include "StateManager.h"

GameStateGame::GameStateGame(StateManager* stateManager) : Kengine::BaseState(stateManager)
{
}

GameStateGame::~GameStateGame()
{
    OnDestroy();
}

void GameStateGame::OnCreate()
{
    Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;

    evMgr->AddCallback(StateType::Game, "Key_Escape",
                       &GameStateGame::MainMenu, this);
    evMgr->AddCallback(StateType::Game, "Key_P",
                       &GameStateGame::Pause, this);
    evMgr->AddCallback(StateType::Game, "Key_O",
                       &GameStateGame::ToggleOverlay, this);

    sf::Vector2u size = m_stateMgr->GetContext()->window->GetWindowSize();
    m_view.setSize(size.x, size.y);
    m_view.setCenter(size.x / 2.0f, size.y / 2.0f);
    m_view.zoom(0.6f);
    m_stateMgr->GetContext()->window->GetRenderWindow()->setView(m_view);

	LoadGameSettings();

	m_gameMap = new Map(m_stateMgr->GetContext(), this);
    m_gameMap->LoadMap(std::string("media/Maps/" + m_mapName + ".tmx"));
}

void GameStateGame::OnDestroy()
{
    Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;

    evMgr->RemoveCallback(StateType::Game, "Key_Escape");
    evMgr->RemoveCallback(StateType::Game, "Key_P");
    evMgr->RemoveCallback(StateType::Game, "Key_O");

//	delete m_gameMap;
    m_gameMap = nullptr;
}

void GameStateGame::Activate()
{
	if (m_gameMap->music.Paused)
	{
		m_gameMap->music.play();
	}
}

void GameStateGame::Deactivate()
{
    m_gameMap->music.pause();
}

void GameStateGame::Update(const sf::Time& time)
{
    SharedContext* context = m_stateMgr->GetContext();
    EntityBase   * player  = context->entityManager->Find("Player");
    if (!player)
    {
        std::cout << "Respawning player..." << std::endl;
        context->entityManager->Add(EntityType::Player, "Player");
        player = context->entityManager->Find("Player");
        player->SetPosition(m_gameMap->GetPlayerStart());
        std::cout << "Player respawned..." << std::endl;
    }
    else
    {
        m_view.setCenter(player->GetPosition());
        context->window->GetRenderWindow()->setView(m_view);
    }

    sf::FloatRect viewSpace = context->window->GetViewSpace();

    if (viewSpace.left <= 0)
    {
        m_view.setCenter(viewSpace.width / 2.0f, m_view.getCenter().y);
        context->window->GetRenderWindow()->setView(m_view);
    }
    else if (viewSpace.left + viewSpace.width > (m_gameMap->GetMapSize().x) * TILE_SIZE)
    {
        m_view.setCenter(((m_gameMap->GetMapSize().x) * TILE_SIZE) - (viewSpace.width / 2.0f),
                         m_view.getCenter().y);
        context->window->GetRenderWindow()->setView(m_view);
    }

    if (viewSpace.top <= 0)
    {
        m_view.setCenter(m_view.getCenter().x, viewSpace.height / 2.0f);
        context->window->GetRenderWindow()->setView(m_view);
    }
    else if (viewSpace.top + viewSpace.height >
             (m_gameMap->GetMapSize().y) * TILE_SIZE)
    {
        m_view.setCenter(m_view.getCenter().x,
                         ((m_gameMap->GetMapSize().y) *
                          TILE_SIZE) - (viewSpace.height / 2.0f));
        context->window->GetRenderWindow()->setView(m_view);
    }

    m_gameMap->Update(time.asSeconds());
    m_stateMgr->GetContext()->entityManager->Update(time.asSeconds());
}

void GameStateGame::Draw()
{
    m_gameMap->Draw();
}

void GameStateGame::MainMenu(Kengine::EventDetails* details)
{
    m_stateMgr->SwitchTo(StateType::MainMenu);
}

void GameStateGame::Pause(Kengine::EventDetails* details)
{
    m_stateMgr->SwitchTo(StateType::Paused);
}

void GameStateGame::ToggleOverlay(Kengine::EventDetails* details)
{
    std::cout << "ToggleOverlay" << std::endl;
    m_stateMgr->GetContext()->debugOverlay.SetDebug(!m_stateMgr->GetContext()->debugOverlay.Debug());
}

void GameStateGame::LoadGameSettings()
{
	std::ifstream file;
	file.open(std::string("media/Game.cfg"));
	if (!file.is_open())
	{
		std::cout << "Failed loading Game.cfg" << std::endl;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line[0] == '|')
		{
			continue;
		}

		std::stringstream 	keystream(line);
		std::string 		type;
		keystream >> type;

		if (type == "Map")
		{
			keystream >> m_mapName;
		}
		else
		{
			std::cout << "Unknown type in game settings file: " << type << std::endl;
		}
	}
	file.close();
}
