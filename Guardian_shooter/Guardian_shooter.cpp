// Guardian_shooter.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Guardian_shooter.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

using namespace YunutyEngine;
using namespace YunutyEngine::D2D;
using namespace UI;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GUARDIANSHOOTER, szWindowClass, MAX_LOADSTRING);
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUARDIANSHOOTER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GUARDIANSHOOTER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    // Perform application initialization:
    YunutyEngine::D2D::D2DCycle::GetInstance().Initialize(hInstance, wcex, nCmdShow, IDS_APP_TITLE, IDC_GUARDIANSHOOTER);

    Scene defaultScene;
    Scene::LoadScene(&defaultScene);
    auto mapTool = defaultScene.AddGameObject();
    auto uiWindow = defaultScene.AddGameObject();
    auto cam = defaultScene.AddGameObject();
    cam->AddComponent<GSCamera>()->Initialize();

    mapTool->AddComponent<gs_map::MapTool>();
    gs_map::MapTool::GetInstance()->Initialize();

    uiWindow->AddComponent<UIManager>();
    UIManager::GetInstance()->Initialize();
    UIManager::GetInstance()->ShowUI(UIManager::UIEnum::TitleUI, uiWindow);


    //Player::CreatePlayer(Vector3d(-100, 200, 0));
    ////ShooterOneShot::CreateShooterOneShot(Vector3d(500, 200, 0));
    //Bomber::CreateBomber(Vector3d(400, 200, 0));	

    //for (int i = 0; i < 10; ++i)
    //	Bullet::CreateBullet(Vector3d(500, 0, 0));
    //for (int i = 0; i < 10; ++i)
    //	Bomb::CreateBomb(Vector3d(0, 0, 0));

    //Item::CreateItem(Vector3d(200, 200, 0), ItemType::HealPack);

   //CreatePlayer(&defaultScene, Vector3d(-100, 200, 0));
   //Enemy::CreateEnemy(Vector3d(500, 200, 0));

    //auto snd = SoundSystem::PlaySoundfile("sounds/bloodtype.mp3");

    //gs_map::TerrainPoint* tempTP = gs_map::TerrainPoint::CreateTerrainPoint(cursorObj->GetTransform()->GetWorldPosition());
    //tempTP->GetGameObject()->GetComponent<D2DCircle>()->color = D2D1::ColorF::Green;

    YunutyEngine::D2D::D2DCycle::GetInstance().Play();
    //MapImage::Load(gs_map::MapTool::GetInstance()->GetLastEditedMapName());

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUARDIANSHOOTER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0) && !UIManager::GetInstance()->readyToQuit)
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    D2DCycle::GetInstance().Release();

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUARDIANSHOOTER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GUARDIANSHOOTER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    //HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    //    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

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
        case WM_DESTROY:
            //DestroyWindow(hWnd);
            UIManager::GetInstance()->readyToQuit = true;
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
