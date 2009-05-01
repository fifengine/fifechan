/*
 * Code that sets up an DirectX application with Guichan using the 
 * Guichan DirectX back end.
 */

#include <guichan.hpp>
#include <guichan/directx3d.hpp>
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <sstream>

namespace directx3d
{
    bool running = true;
    LPDIRECT3D9 d3d;
    LPDIRECT3DDEVICE9 d3ddev;

    // All back ends contain objects to make Guichan work on a
    // specific target. They are a Graphics object to make Guichan 
    // able to draw itself using DirectX, an input objec to make 
    // Guichan able to get user input using DirectX and an ImageLoader 
    // object to make Guichan able to load images using DirectX.
    gcn::DirectX3DGraphics* graphics;
    gcn::DirectX3DInput* input = NULL;
    gcn::DirectX3DImageLoader* imageLoader;

    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
    {
        if (msg == WM_DESTROY)
        {
            PostQuitMessage(0);
            return FALSE;
        }
        else if (msg == WM_CLOSE)
        {
            PostQuitMessage(0);
            return FALSE;
        }

        if (input != NULL)
        {
            input->dispatchMessage(hWnd, msg, wParam, lParam);
       
            /*
            if (!input->isMouseQueueEmpty())
            {
                gcn::MouseInput mouseInput = input->dequeueMouseInput();
                std::string str;
                std::ostringstream os(str);
                if (mouseInput.getType() == gcn::MouseInput::Pressed)
                {
                    os << "Mouse pressed: ";
                }
                else if (mouseInput.getType() == gcn::MouseInput::Released)
                {
                    os << "Mouse released: ";
                }
                else if (mouseInput.getType() == gcn::MouseInput::Moved)
                {
                    os << "Mouse moved: ";
                }

                os << "x=" << mouseInput.getX() << " y=" << mouseInput.getY()
                    << " button=" << mouseInput.getButton() << std::endl;
                
                OutputDebugStringA(os.str().c_str());
            }*/

           /*
            if (!input->isKeyQueueEmpty())
            {
                gcn::KeyInput keyInput = input->dequeueKeyInput();
                std::string str;
                std::ostringstream os(str);
 
                if (keyInput.getType() == gcn::KeyInput::Pressed)
                {
                    os << "Key pressed: " << (char)keyInput.getKey().getValue() << " " << keyInput.getKey().isCharacter() << std::endl;
                }

                OutputDebugStringA(os.str().c_str());
            }*/
          
            
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    /**
     * Initialises the DirectX 3D application. This function creates the global
     * Gui object that can be populated by various examples.
     */
    bool init(HINSTANCE hInstance, int nCmdShow)
    {
        // We simply initialise DirectX as we would do with any DirectX
        // application.
        HWND hWnd;
        WNDCLASSEX wc;
        
        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        wc.cbSize        = sizeof(WNDCLASSEX);
        wc.style         = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc   = WndProc;
        wc.hInstance     = hInstance;
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
        wc.lpszClassName = "myWindowClass";

        if(!RegisterClassEx(&wc))
        {
            MessageBox(NULL, "Window Registration Failed!", "Error!",
                MB_ICONEXCLAMATION | MB_OK);
            return false;
        }

        hWnd = CreateWindowEx(NULL,
                              "myWindowClass",
                              "Guichan DirectX 3D Test",
                              WS_OVERLAPPEDWINDOW,
                              300, 
                              300, 
                              640, 
                              480,
                              NULL, 
                              NULL, 
                              hInstance, 
                              NULL);

        if (hWnd == NULL)
        {
            MessageBox(NULL, 
                       "Window Creation Failed!", 
                       "Error!",
                       MB_ICONEXCLAMATION | MB_OK);
            return false;
        }
        
        ShowWindow(hWnd, nCmdShow);

        d3d = Direct3DCreate9(D3D_SDK_VERSION);
        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory(&d3dpp, sizeof(d3dpp));
    
        d3dpp.Windowed = TRUE;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.hDeviceWindow = hWnd;

        d3d->CreateDevice(D3DADAPTER_DEFAULT,
                          D3DDEVTYPE_HAL,
                          hWnd,
                          D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                          &d3dpp,
                          &d3ddev);

        // Now it's time to initialise the Guichan DirectX 3D back end.

        imageLoader = new gcn::DirectX3DImageLoader(d3ddev);
        // The ImageLoader Guichan should use needs to be passed to the Image object
        // using a static function.
        gcn::Image::setImageLoader(imageLoader);
        graphics = new gcn::DirectX3DGraphics(d3ddev);
        // We need to tell the DirectX Graphics object how big the screen is.
        graphics->setTargetPlane(640, 480);
        graphics->setDevice(d3ddev);
        input = new gcn::DirectX3DInput();

        // Now we create the Gui object to be used with this DirectX
        // application.
        globals::gui = new gcn::Gui();
        // The Gui object needs a Graphics to be able to draw itself and an Input
        // object to be able to check for user input. In this case we provide the
        // Gui object with DirectX implementations of these objects hence 
        // making Guichan able to utilise DirectX.
        globals::gui->setGraphics(graphics);
        globals::gui->setInput(input);
    }

    /**
     * Halts the DirectX application.
     */
    void halt()
    {
        delete globals::gui;

        delete imageLoader;
        delete input;
        delete graphics;

        d3ddev->Release();
        d3d->Release();

        //UnregisterClass("MY_WINDOWS_CLASS", winClass.hInstance);
    }

    /**
     * Runs the DirectX application.
     */
    void run()
    {
        MSG msg;

        while(running)
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
	                break;
                }

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                // Now we let the Gui object perform its logic.
                globals::gui->logic();

                d3ddev->Clear(0, 
                              NULL, 
                              D3DCLEAR_TARGET, 
                              D3DCOLOR_XRGB(0, 0, 0), 
                              1.0f, 
                              0);
                d3ddev->BeginScene();
                // Now we let the Gui object draw itself.
                
                /*
                graphics->_beginDraw();
                graphics->pushClipArea(gcn::Rectangle(2,2,25,25));
                graphics->setColor(gcn::Color(0xff0000));
                graphics->fillRectangle(2,2,100,100);
                graphics->popClipArea();
                graphics->drawLine(32, 30, 28, 30);
                graphics->drawPoint(639, 479);
                graphics->drawPoint(637, 477);
                graphics->_endDraw();
                */
                globals::gui->draw();
                d3ddev->EndScene();
                 // Finally we update the screen.
                d3ddev->Present(NULL, NULL, NULL, NULL);
            }
        }
    }
}
