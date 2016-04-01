#pragma once
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "EventManager.h"

namespace Kengine
{
class Window
{
public:
	Window();
	Window(const std::string& title, const sf::Vector2u& size);
	~Window();

	void BeginDraw(); // Clear the window.
	void EndDraw(); // Display the changes.

	void Update();

	bool IsDone();
	bool IsFullscreen();
	bool isFocused() { return m_isFocused; }
    
	EventManager* GetEventManager() { return &m_eventManager; }
	sf::Vector2u GetWindowSize();
	sf::RenderWindow* GetRenderWindow() { return &m_window; }
	sf::FloatRect GetViewSpace();

    void ToggleFullscreen(EventDetails* details);
    void Close(EventDetails* details = nullptr);

	void Draw(sf::Drawable& drawable);

private:
	sf::RenderWindow m_window;
	sf::Vector2u m_windowSize;
	std::string m_windowTitle;
    
    EventManager m_eventManager;
    
    bool m_isFocused;
	bool m_isDone = false;
	bool m_isFullscreen = false;

	void Setup(const std::string& title, const sf::Vector2u& size);
	void Destroy();
	void Create();
};
}