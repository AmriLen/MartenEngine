#include "../Application.hpp"
#include "../Window.hpp"
#include "../../MartenUtils/MartenLogger.hpp"

#include <iostream>

namespace MartenEngine {
    Application::Application() {
        MartenLogger::Initialize();
        MartenLogger::SECS("Starting MartenEngine...");
    }

    Application::~Application() {
        MartenLogger::SECS("Closing MartenEngine...");
        MartenLogger::Shutdown();
    }

    int Application::start(unsigned int window_width, unsigned int window_height, const char* window_title) {
        m_pWindow = std::make_unique<Window>(window_title, window_width, window_height);

        if (!m_pWindow->window_state()) {
            MartenLogger::CRIT("[Application::start] Failed to create window");
            return -1;
        }

        m_event_dispatcher.add_event_listener<EventMouseMoved>([](EventMouseMoved& event) {
			//MartenLogger::INFO("[Application::start] [MouseMoved] Mouse moved to " + std::to_string(event.x) + " x " + std::to_string(event.y));
		});

        m_event_dispatcher.add_event_listener<EventWindowResize>([](EventWindowResize& event) {
			MartenLogger::INFO("[Application::start] [Resized] Changed size to " + std::to_string(event.width) + " x " + std::to_string(event.height));
		});

        m_event_dispatcher.add_event_listener<EventWindowClose>([&](EventWindowClose& event) {
			MartenLogger::INFO("[Application::start] [WindowClose]");
			m_bCloseWindow = true;
		});

		m_pWindow->set_event_callback([&](BaseEvent& event) {
			m_event_dispatcher.dispatch(event);
		});

        while (!m_bCloseWindow) {
            m_pWindow->on_update();
            on_update();
        }

        m_pWindow = nullptr;
        return 0;
    }
}