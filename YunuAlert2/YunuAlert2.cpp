// YunuAlert2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "YunuAlert2.h"
#include "YunutyEngine.h"

using namespace YunutyEngine;
using namespace YunutyEngine::D2D;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

GameObject* SpawnGI(const Vector2d& position, Scene& scene, int ownerIndex = 0);
GameObject* SpawnConscript(const Vector2d& position, Scene& scene, int ownerIndex = 1);

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YUNUALERT2));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_YUNUALERT2);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_YUNUALERT2, szWindowClass, MAX_LOADSTRING);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_YUNUALERT2));

    MSG msg;

    YunutyEngine::D2D::D2DCycle::GetInstance().Initialize(hInstance, wcex, nCmdShow, IDS_APP_TITLE, IDC_YUNUALERT2);

    Scene defaultScene;
    Scene::LoadScene(&defaultScene);

    auto map_terrain = defaultScene.AddGameObject();
    map_terrain->AddComponent<D2DSprite>()->SetSpriteFilePath(L"map/map.png");
    map_terrain->GetComponent<D2DSprite>()->SetDrawRect(Rect(1920 * 4, 1080 * 4));
    map_terrain->AddComponent<RA2NavigationField2D>()->SetField(Rect(1920 * 4, 1080 * 4), 30);
    map_terrain->GetComponent<RA2NavigationField2D>()->LoadTerrainData(L"map/terrainfo.png");
    Vector2d(0, 1).GetAngleDegree();
    Vector2d(-0.777, 0.777).GetAngleDegree();
    Vector2d(-1, 0).GetAngleDegree();
    //map_terrain->GetComponent<RA2NavigationField2D>()->LoadTerrainData(L"map/map.png");

    auto RTSInterfaceManager = defaultScene.AddGameObject();
    RTSInterfaceManager->AddComponent<RTSControlInterface>();

    auto rightClickEffect = defaultScene.AddGameObject();
    rightClickEffect->AddComponent<D2DAnimatedSprite>();
    rightClickEffect->GetComponent<D2DAnimatedSprite>()->SetIsRepeating(false);
    rightClickEffect->GetComponent<D2DAnimatedSprite>()->LoadAnimationFromFile(L"UI/MoveClickAnim", 0.05);

    RTSInterfaceManager->GetComponent<RTSControlInterface>()->rightClickAnimation =
        rightClickEffect->GetComponent<D2DAnimatedSprite>();

    auto cam = defaultScene.AddGameObject();
    cam->GetTransform()->position.y = 100;
    cam->AddComponent<RTSCamera>();


    auto UI_HUD = defaultScene.AddGameObject(cam);
    UI_HUD->AddComponent<CircleCollider2D>()->SetRadius(100);
    UI_HUD->AddComponent<D2DText>();
    UI_HUD->AddComponent<D2DSprite>();
    UI_HUD->GetComponent<D2DSprite>()->SetSpriteFilePath(L"UI/HUD.png");
    UI_HUD->GetComponent<D2DSprite>()->SetDrawRect(Rect(1920, 1080));

    auto GI0 = SpawnGI(Vector2d(100, 500), defaultScene, 1);
    auto GI1 = SpawnGI(Vector2d(150, 500), defaultScene);
    auto GI2 = SpawnGI(Vector2d(200, 500), defaultScene);
    auto GI3 = SpawnGI(Vector2d(250, 500), defaultScene);
    GI3->GetTransform()->rotation = Quaternion(Vector3d(0,0,130));
    auto csrpt0 = SpawnConscript(Vector2d(50, 100), defaultScene);

    auto UI_SelectRect = defaultScene.AddGameObject();
    UI_SelectRect->AddComponent<D2DRectangle>();
    UI_SelectRect->GetComponent<D2DRectangle>()->filled = true;
    UI_SelectRect->GetComponent<D2DRectangle>()->color = D2D1::ColorF(0, 1, 0, 0.5);
    UI_SelectRect->AddComponent<BoxCollider2D>();
    RTSInterfaceManager->GetComponent<RTSControlInterface>()->dragRect = UI_SelectRect->GetComponent<D2DRectangle>();
    RTSInterfaceManager->GetComponent<RTSControlInterface>()->dragRectCollider = UI_SelectRect->GetComponent<BoxCollider2D>();

    RTSInterfaceManager->GetComponent<RTSControlInterface>()->selectedUnits.insert(GI0->GetComponent<Unit>());

    auto debugTilePool = defaultScene.AddGameObject();
    debugTilePool->AddComponent<DebugTilePool>();
    debugTilePool->GetComponent<DebugTilePool>()->Borrow(Vector2d::zero, 50, D2D1::ColorF::Red);

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

    YunutyEngine::D2D::D2DCycle::GetInstance().Release();
    return (int)msg.wParam;
}

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
GameObject* SpawnGI(const Vector2d& position, Scene& scene, int ownerIndex)
{
    auto GI = scene.AddGameObject();
    GI->GetTransform()->position = position;
    GI->AddComponent<RA2UnitGraphic>()->SetUnitSprites(L"GI");
    GI->GetComponent<RA2UnitGraphic>()->PlayMove();
    GI->AddComponent<NavigationUnit2D>();
    GI->AddComponent<CircleCollider2D>()->SetRadius(200);
#if _DEBUG
    GI->AddComponent<D2DCircle>()->radius = 200;
    GI->GetComponent<D2DCircle>()->filled = false;
    GI->GetComponent<D2DCircle>()->border = 5;
#endif
    GI->AddComponent<Unit>();
    GI->GetComponent<Unit>()->ownerIndex = ownerIndex;
    GI->GetComponent<Unit>()->SetUnitCollider(GI->AddComponent<CircleCollider2D>());
    GI->GetComponent<Unit>()->unitGraphic = GI->GetComponent<RA2UnitGraphic>();
    GI->GetComponent<Unit>()->navUnitComponent = GI->GetComponent<NavigationUnit2D>();
    return GI;
}
GameObject* SpawnConscript(const Vector2d& position, Scene& scene, int ownerIndex)
{
    auto conscript = scene.AddGameObject();
    conscript->GetTransform()->position = position;
    conscript->AddComponent<RA2UnitGraphic>()->SetUnitSprites(L"Conscript");
    conscript->GetComponent<RA2UnitGraphic>()->PlayMove();
    conscript->AddComponent<NavigationUnit2D>();
    conscript->AddComponent<CircleCollider2D>()->SetRadius(130);
#if _DEBUG
    conscript->AddComponent<D2DCircle>()->radius = 130;
    conscript->GetComponent<D2DCircle>()->filled = false;
    conscript->GetComponent<D2DCircle>()->border = 1;
#endif
    conscript->AddComponent<Unit>();
    conscript->GetComponent<Unit>()->ownerIndex = ownerIndex;
    conscript->GetComponent<Unit>()->SetUnitCollider(conscript->AddComponent<CircleCollider2D>());
    conscript->GetComponent<Unit>()->unitGraphic = conscript->GetComponent<RA2UnitGraphic>();
    conscript->GetComponent<Unit>()->navUnitComponent = conscript->GetComponent<NavigationUnit2D>();
    return conscript;
}
