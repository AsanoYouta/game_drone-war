// main.cpp : アプリケーションのエントリ ポイントを定義

#include "framework.h"
#include "main.h"
#include "Source/Game/Game.h"
#include "Source/DirectX/DirectX.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE m_instanceHandle;                       // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

//関数の宣言:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                CreateConsole();
void                CloseConsole();

//エントリーポイント
int APIENTRY wWinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPWSTR    lpCmdLine,
    _In_     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // グローバル文字列を初期化
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);         //ウィンドウ名の登録
    LoadStringW(hInstance, IDC_GAME, szWindowClass, MAX_LOADSTRING);  //ウィンドウクラス名の登録
    MyRegisterClass(hInstance);

    // アプリケーション初期化
    if (!InitInstance (hInstance, nCmdShow))
    {
        DEBUG_LOG("Failed to InitApplication\n");
        return FALSE;
    }
    
    // メインループ:   
    GAME.RunLoop();

    auto msg = GAME.Shutdown();
    GAME.DeleteInstance();

    return (int) msg.wParam;
}

//ウィンドウ情報の登録
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//インスタンスハンドルを保存してメインウィンドウを作成
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   m_instanceHandle = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND windowHandle = CreateWindowW(
       szWindowClass, //ウィンドウクラス名でウィンドウ情報取得
       szTitle, 
       WS_OVERLAPPEDWINDOW, //スタイル
       CW_USEDEFAULT, 0,    //表示位置
       CW_USEDEFAULT, 0,    //サイズ
       nullptr, nullptr, hInstance, nullptr);

   if (!windowHandle)
   {
      return FALSE;
   }

   //ビューポート情報設定
   int width = 1280;
   int height = 720;

   RECT window_rect;
   if (GetWindowRect(windowHandle, &window_rect) == false) return FALSE;
   RECT client_rect;
   if (GetClientRect(windowHandle, &client_rect) == false) return FALSE;
   // フレームサイズ算出
   int frameSize_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
   int frameSize_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);
   //リサイズ用サイズの算出
   int resizeWidth = frameSize_x + width;
   int resizeHeight = frameSize_y + height;
   //ウィンドウのリサイズ
   SetWindowPos(
       windowHandle,
       NULL,
       CW_USEDEFAULT, CW_USEDEFAULT,
       resizeWidth, resizeHeight,
       SWP_NOMOVE);

   //ゲームシステム生成:
   Game::CreateInstance();
   if (!GAME.Initialize(windowHandle, width, height))
   {
       DEBUG_LOG("Failed to GameInit\n");
       return FALSE;
   }

   ShowWindow(windowHandle, nCmdShow);
   UpdateWindow(windowHandle);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: 
        {
#ifdef _DEBUG
            //コンソール表示
            CreateConsole();
#endif // DEBUG
            break;
        }

    case WM_COMMAND: //アプリケーションメニューの処理
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(m_instanceHandle, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        }

    case WM_PAINT: //メイン ウィンドウを描画する
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        {
            PostQuitMessage(0);
#ifdef _DEBUG
            CloseConsole();
#endif // DEBUG
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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

void CreateConsole() {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);
}

void CloseConsole() {
    FreeConsole();
}
