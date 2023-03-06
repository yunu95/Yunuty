// YunutyClient.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "YunutyClient.h"
#include "YunuD2DGraphicCore.h"
#include "YunutyEngine.h"
#include "SpeedMeter.h"
#include "ShakyFella.h"
#include "QuadTreeDebugger.h"
#include "FPSBenchMark.h"
#include "RotatingFella.h"

#define MAX_LOADSTRING 100

using namespace YunutyEngine;
using namespace YunutyEngine::D2D;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, HWND*);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YUNUTYCLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_YUNUTYCLIENT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_YUNUTYCLIENT));
    MSG msg;

    Scene defaultScene;
    Scene::LoadScene(&defaultScene);
    auto cam = defaultScene.AddGameObject()->AddComponent<ZoomableCamera>();
    cam->GetGameObject()->AddComponent<D2DText>();
    cam->GetGameObject()->AddComponent<FPSBenchMark>();
    auto rotObjParent = defaultScene.AddGameObject();
    auto rotObj = defaultScene.AddGameObject(rotObjParent)->AddComponent<D2DRectangle>();
    rotObj->height = 100;
    rotObj->width = 100;
    rotObj->border = 10;
    rotObj->color = D2D1::ColorF::White;
    rotObj->filled = false;
    rotObj->GetTransform()->SetWorldPosition(Vector2d(100, 200));
    rotObj->GetGameObject()->AddComponent<RotatingFella>();
    rotObj->GetGameObject()->AddComponent<ShakyFella>();

    Scene::getCurrentScene()->DestroyGameObject(rotObj->GetGameObject());

    auto rotObj2= defaultScene.AddGameObject()->AddComponent<D2DSprite>();
    rotObj2->SetSpriteFilePath(L"Sprites/protagonist.jpg");
    rotObj2->SetDrawRect(Rect(1000, 1000));
    rotObj2->GetGameObject()->AddComponent<RotatingFella>();
    //for (int i = 0; i < 10000; i++)
        //defaultScene.AddGameObject()->AddComponent<D2DRectangle>();

    YunutyEngine::D2D::D2DCycle::GetInstance().Initialize(hInstance, wcex, nCmdShow, IDS_APP_TITLE, IDC_YUNUTYCLIENT);
    YunutyEngine::D2D::D2DCycle::GetInstance().Play();

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND* hWnd)
{
    hInst = hInstance; // Store instance handle in our global variable

    *hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(*hWnd, nCmdShow);
    UpdateWindow(*hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
