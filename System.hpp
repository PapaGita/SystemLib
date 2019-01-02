#include <functional>
#include <vector>
#include <Windows.h>
#include <windowsx.h>

class System {
public: 

	enum EventType {
		OnButtonDown,
		OnButtonUp,
		OnButtonPressed,
	};

	template<typename T>
	class Vec2 {
	public:
		T x;
		T y;
		Vec2(T x, T y) : x(x), y(y) {}
		Vec2() {}
	};

	template<typename T>
	class Vec4 {
	public:
		T x;
		T y;
		T x1;
		T y1;
		Vec4(T x, T y, T x1, T y1) : x(x), y(y), x1(x1), y1(y1) {}
		Vec4() {}
	};
	typedef Vec2<int> PosInt;
	typedef Vec4<int> RectInt;

	class Window {
	public:
		static PosInt getWindowSize(HWND hwnd)
		{
			RECT rect;
			PosInt size;
			if (!hwnd) return size;
			GetWindowRect(hwnd, &rect);
			size.x = (int)(rect.right - rect.left);
			size.y = (int)(rect.bottom - rect.top);
			return size;
		}
		static PosInt getWindowPos(HWND hwnd) {
			RECT rect = { NULL };
			PosInt pos;
			if (!hwnd) return pos;
			GetClientRect(hwnd, &rect);
			MapWindowPoints(hwnd, nullptr, (POINT*)&rect, 2);
			pos.x = (int)rect.left;
			pos.y = (int)rect.top;
			return pos;
		}
	};

	class Wnd {
	public:
		static int OnWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch (msg)
			{
				case WM_LBUTTONDOWN:
					System::Mouse::get()->onUpdate(System::EventType::OnButtonDown, System::Mouse::ButtonLeft);
					break;
				case WM_RBUTTONDOWN:
					System::Mouse::get()->onUpdate(System::EventType::OnButtonDown, System::Mouse::ButtonRight);
					break;
				case WM_MBUTTONDOWN:
					System::Mouse::get()->onUpdate(System::EventType::OnButtonDown, System::Mouse::ButtonMiddle);
					break;
				case WM_LBUTTONUP:
					System::Mouse::get()->onUpdate(System::EventType::OnButtonUp, System::Mouse::ButtonLeft);
					break;
				case WM_RBUTTONUP:
					System::Mouse::get()->onUpdate(System::EventType::OnButtonDown, System::Mouse::ButtonRight);
					break;
				case WM_MBUTTONUP:
					System::Mouse::get()->onUpdate(System::EventType::OnButtonDown, System::Mouse::ButtonMiddle);
					break;
				case WM_MOUSEMOVE:
					{
						int x = GET_X_LPARAM(lParam);
						int y = GET_Y_LPARAM(lParam);
						Mouse::get()->handleMouseMove(x, y);
					}
					break;
				default:
					return 0;
			}
			return 1;
		}
	};

	class MouseAcceleration {
	public:
		int x, y;
		MouseAcceleration(){}
		MouseAcceleration(int x, int y) : x(x), y(y) {}
	};

	class Mouse {
	public:
		enum MouseButton {
			ButtonLeft,
			ButtonRight,
			ButtonMiddle,
		};

	private:
		int sdx;
		int sdy;
		int lx;
		int ly;
		
		HWND hwnd = nullptr;

		std::vector<std::function<void(MouseButton)>> down;
		std::vector<std::function<void(MouseButton)>> up;
		std::vector<std::function<void(MouseButton)>> pressed;

	public:
		void handleMouseMove(int x, int y) {
			PosInt ps = System::Window::getWindowPos(hwnd);
			x += ps.x;
			y += ps.y;
			int dx = x - lx;
			int dy = ly - y;
			if (dx != 0 || dy != 0) {
				sdx += dx;
				sdy += dy;
				lx = x;
				ly = y;
			}
		}

		/*nullptr for remove lock*/
		void setLockWindow(HWND hwnd) {
			this->hwnd = hwnd;
		}

		void centerMouse() {
			if (!hwnd) return;
			PosInt wsize = System::Window::getWindowSize(hwnd);
			PosInt pos = System::Window::getWindowPos(hwnd);
			PosInt center;
			center.x = pos.x + wsize.x / 2;
			center.y = pos.y + wsize.y / 2;
			lx = center.x;
			ly = center.y;
			SetCursorPos(center.x, center.y);
		}

		MouseAcceleration getAcceleration() {
			int x = sdx;
			int y = sdy;
			if ((sdx != 0 || sdy != 0) && GetForegroundWindow() == hwnd)
				centerMouse();
			sdx = sdy = 0;
			return MouseAcceleration(x, y);
		}

		static Mouse* get() {
			static System::Mouse mouse;
			return &mouse;
		}

		void onUpdate(EventType event_, MouseButton button) {
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
		void add(EventType event_, std::function<void(MouseButton)> func) {
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
