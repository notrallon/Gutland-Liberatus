#include "GameStateCombat.h"
#include "StateManager.h"

GameStateCombat::GameStateCombat(StateManager* stateManager) : Kengine::BaseState(stateManager)
{}

GameStateCombat::~GameStateCombat()
{
	OnDestroy();
}

void GameStateCombat::OnCreate()
{
	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;

	evMgr->AddCallback(StateType::Combat, "Mouse_Left",
		&GameStateCombat::MouseClick, this);
	evMgr->AddCallback(StateType::Combat, "Key_Escape",
		&GameStateCombat::MainMenu, this);
	evMgr->AddCallback(StateType::Combat, "Key_P",
		&GameStateCombat::Pause, this);
	evMgr->AddCallback(StateType::Combat, "Key_O",
		&GameStateCombat::ToggleOverlay, this);

	sf::Vector2u size = m_stateMgr->GetContext()->window->GetWindowSize();
	
	//Selection and creation of fighting background
	//For now a static background
	m_backgroundTexture.loadFromFile("media/Textures/cityBackground.png");
	m_background.setTexture(m_backgroundTexture);
	m_background.setPosition(sf::Vector2f(0.f, 0.f));

	//Portrait is static for now
	m_portraitTexture.loadFromFile("media/Textures/placeholderport.png");
	m_portrait.setTexture(m_portraitTexture);
	m_portrait.setPosition(sf::Vector2f(510.f, 540.f));
	m_portrait.setScale(sf::Vector2f(4.6f, 4.6f));   
	
	//Portraitframe is static dont move
	m_portraitFrameTexture.loadFromFile("media/Textures/portraitFrame.png");
	m_portraitFrame.setTexture(m_portraitFrameTexture);
	m_portraitFrame.setPosition(sf::Vector2f(510.f, 540.f));
	m_portraitFrame.setScale(sf::Vector2f(4.6f, 4.6f));

	//The bottom frame is static dont move
	m_bottomFrameTexture.loadFromFile("media/Textures/BottomFrame.png");
	m_bottomFrame.setTexture(m_bottomFrameTexture);
	m_bottomFrame.setPosition(sf::Vector2f(0.f, 520.f));
	m_bottomFrame.setScale(sf::Vector2f(18.3f, 3.0f));

	std::string buttonString[3];
	buttonString[0] = "FIGHT";
	buttonString[1] = "ACTION";
	buttonString[2] = "ESCAPE";
	for (int i = 0; i < 3; i++)
	{
		m_buttons.addButton(buttonString[i], sf::Vector2f(700.f, 542.f + (58.f * i)), sf::Vector2f(200.f, 55.f), sf::Color(255, 255, 255, 255), sf::Color(50, 50, 50, 255), sf::Color(20, 20, 20, 255));
	}
}

void GameStateCombat::OnDestroy()
{
	Kengine::EventManager* evMgr = m_stateMgr->GetContext()->eventManager;

	evMgr->RemoveCallback(StateType::Game, "Key_Escape");
	evMgr->RemoveCallback(StateType::Game, "Key_P");
	evMgr->RemoveCallback(StateType::Game, "Key_O");
}

void GameStateCombat::Activate()
{
}

void GameStateCombat::Deactivate()
{
}

void GameStateCombat::Update(const sf::Time& time)
{
	SharedContext* context = m_stateMgr->GetContext();
	
	m_buttons.update(m_stateMgr->GetContext()->eventManager->GetMousePos(m_stateMgr->GetContext()->window->GetRenderWindow()), m_stateMgr->GetContext()->window->GetRenderWindow());
	//m_frame.Update(time.asSeconds());
	m_stateMgr->GetContext()->entityManager->Update(time.asSeconds());
}

void GameStateCombat::Draw()
{
	sf::RenderWindow* window = m_stateMgr->GetContext()->window->GetRenderWindow();

	window->draw(m_background);

	m_stateMgr->GetContext()->entityManager->Draw();
	//m_frame.Draw(window);
	window->draw(m_bottomFrame);

	m_buttons.draw();
	window->draw(m_portraitFrame);
	window->draw(m_portrait);
	
}

void GameStateCombat::MainMenu(Kengine::EventDetails* details)
{
	m_stateMgr->SwitchTo(StateType::MainMenu);
}

void GameStateCombat::Pause(Kengine::EventDetails* details)
{
	m_stateMgr->SwitchTo(StateType::Paused);
}

void GameStateCombat::ToggleOverlay(Kengine::EventDetails* details)
{
	std::cout << "ToggleOverlay" << std::endl;
	m_stateMgr->GetContext()->debugOverlay.SetDebug(!m_stateMgr->GetContext()->debugOverlay.Debug());
}

void GameStateCombat::MouseClick(Kengine::EventDetails * details)
{
	std::string active = m_buttons.getActiveButton();
	if (active == "FIGHT")
	{
		std::cout << "FIGHT" << std::endl;
	}else if (active == "ACTION")
	{
		std::cout << "ACTION" << std::endl;
	}
	else if (active == "ESCAPE")
	{
		std::cout << "ESCAPE" << std::endl;
	}

}