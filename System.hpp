#include <functional>
#include <vector>
#include <Windows.h>

class System {
public: 

	enum EventType {
		MouseDown,
		MouseUp,
		MousePressed,
	};

	class Wnd {
	public:
		static int OnWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch (msg)
			{
				case WM_LBUTTONDOWN:
					System::Mouse::get()->OnUpdate(System::EventType::OnButtonDown, System::Mouse::ButtonLeft);
					break;
				case WM_RBUTTONDOWN:
					System::Mouse::get()->OnUpdate(System::EventType::OnButtonDown, System::Mouse::ButtonRight);
					break;
				case WM_MBUTTONDOWN:
					System::Mouse::get()->OnUpdate(System::EventType::OnButtonDown, System::Mouse::ButtonMiddle);
					break;
				case WM_LBUTTONUP:
					System::Mouse::get()->OnUpdate(System::EventType::OnButtonUp, System::Mouse::ButtonLeft);
					break;
				case WM_RBUTTONUP:
					System::Mouse::get()->OnUpdate(System::EventType::OnButtonDown, System::Mouse::ButtonRight);
					break;
				case WM_MBUTTONUP:
					System::Mouse::get()->OnUpdate(System::EventType::OnButtonDown, System::Mouse::ButtonMiddle);
					break;
				default:
					return 0;
			}
			return 1;
		}
	};

	class Mouse {
	public:
		enum MouseButton {
			ButtonLeft,
			ButtonRight,
			ButtonMiddle,
		};

		std::vector<std::function<void(MouseButton)>> down;
		std::vector<std::function<void(MouseButton)>> up;
		std::vector<std::function<void(MouseButton)>> pressed;

		static Mouse* get() {
			static System::Mouse mouse;
			return &mouse;
		}

		void Process(EventType event_, MouseButton button) {
			switch (event_)
			{
				case System::OnButtonDown:
					
					for(auto f : down)
						f(button);
					break;
				case System::OnButtonUp:
					for (auto f : up)
						f(button);
					break;
			}
		}
		void Add(EventType event_, std::function<void(MouseButton)> func) {
			switch (event_)
				{
				case System::OnButtonDown:
					down.push_back(func);
					break;
				case System::OnButtonUp:
					up.push_back(func);
					break;
				case System::OnButtonPressed:
					pressed.push_back(func);
					break;
			}
		}
	};
};
