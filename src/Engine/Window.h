#pragma once
#include <windows.h>
#include <tchar.h>
#include <string>
#include "Engine/Utils/Size.h"

#include <iostream>
// (const char *windowName, Size screen, HINSTANCE hInstance, int nCmdShow)
class Window
{
private:
  // Props
  bool oIsOpen = true;
  bool oHasFocus = true;
  Size oScreen;
  HWND oHWnd;
  // Window Events
  void onResize(unsigned short width, unsigned short height)
  {
    Window::oScreen.width = width;
    Window::oScreen.height = height;
  }
  void onFocus(bool hasFocus)
  {
    Window::oHasFocus = hasFocus;
  }
  // Static
  static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
    if (message == WM_CREATE)
    {
      LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
      Window *pWindow = (Window *)pcs->lpCreateParams;

      ::SetWindowLongPtrW(
          hWnd,
          GWLP_USERDATA,
          PtrToUlong(pWindow));

      return 1;
    }

    Window *pWindow = reinterpret_cast<Window *>(
        ::GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (pWindow)
    {
      switch (message)
      {
      case WM_SETFOCUS:
      {
        pWindow->onFocus(true);
        return 0;
      }
      case WM_KILLFOCUS:
      {
        pWindow->onFocus(false);
        return 0;
      }
      case WM_SIZE:
      {
        unsigned short width = LOWORD(lParam);
        unsigned short height = HIWORD(lParam);
        pWindow->onResize(width, height);
        return 0;
      }
      case WM_DESTROY:
      {
        PostQuitMessage(0);
        return 1;
      }
      }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
  }

public:
  // Getters
  const Size &screen = oScreen;
  const HWND &hWnd = oHWnd;
  const bool &hasFocus = oHasFocus;
  // Constructor
  Window(const char *windowName, Size screen) : oScreen(screen)
  {
    Window::oScreen.width = 800;
    WNDCLASSEX wc = {};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = Window::WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.lpszClassName = _T(windowName);

    RegisterClassEx(&wc);

    RECT wr = {0, 0, screen.width, screen.height};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    Window::oHWnd = CreateWindowEx(
        0,                   // extended window style
        wc.lpszClassName,    // pointer to registered class name
        wc.lpszClassName,    // pointer to window name
        WS_OVERLAPPEDWINDOW, // window style
        CW_USEDEFAULT,       // horizontal position of window
        CW_USEDEFAULT,       // vertical position of window
        wr.right - wr.left,  // window width
        wr.bottom - wr.top,  // window height
        0,                   // handle to parent or owner window
        0,                   // handle to menu, or child-window identifier
        wc.hInstance,        // handle to application instance
        this                 // pointer to window-creation data
    );
    ShowWindow(hWnd, SW_SHOWDEFAULT);
  }
  bool isOpen()
  {
    MSG msg;
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);

      switch (msg.message)
      {
      case WM_SIZE:
        std::cout << true << '\n';
      }

      if (msg.message == WM_QUIT)
        return false;
    }
    return Window::oIsOpen;
  }
  void close()
  {
    Window::oIsOpen = false;
  }
};