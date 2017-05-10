#include "Application.h"

#include "ShapesArray.h"

Application::Application() {
}


Application::~Application() {
}


const bool Application::Update() {
	if (m_window->Update()) return true;
	

	m_player->Update(*m_board);
	m_board->Update();
	

	return false;
}

void Application::Draw() {
	m_window->StartDraw();

	m_board->Draw(*m_window->Renderer());

	m_window->EndDraw();
}

void Application::OnStartUp() {
	m_window = new GameWindow(WINDOW_TITLE, WINDOW_X, WINDOW_Y);

	// load textures
	ImageFactory::Init();
	ImageFactory::LoadInTextures();

	m_board = new Board(16, 21);
	m_player = new Input();
}

void Application::OnShutDown() {
	// TODO delete pointers
}