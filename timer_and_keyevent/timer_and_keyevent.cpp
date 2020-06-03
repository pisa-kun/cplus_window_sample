// timer_and_keyevent.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "timer_and_keyevent.h"
#include <iostream>

#define MAX_LOADSTRING 100
#define TM_COUNT1 1
#define Seconds 1000
#define BUTTON_ID1 99

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

LPCTSTR strItem[] = {
    TEXT("60  (1min)") ,
    TEXT("180 (3min)") ,
    TEXT("300 (5min)") ,
};

int lastTime[] = { 60,180,300 }; // strItemのstrをint変換する配列

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TIMERANDKEYEVENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TIMERANDKEYEVENT));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TIMERANDKEYEVENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TIMERANDKEYEVENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   // ボタンはクリックされると親ウインドウにWM_COMMANDを送る
   CreateWindow(
       TEXT("BUTTON"), TEXT("Hoge"),
       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
       0, 200, 100, 50,
       hWnd, (HMENU)BUTTON_ID1, hInstance, NULL
   );

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rctSize;
    static int iCount1 = 60; // 60秒タイマー
    static TCHAR strCount[64];
    static TCHAR strText[64];

    static HWND combo, label;
    int i;

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CREATE:
        SetTimer(hwnd, TM_COUNT1, Seconds, NULL); // 1000ミリ(1second)毎に変化するタイマー

        // コンボボックスの作成
        // CBS_DROPDOWNLIST で入力不可のリスト表示
        combo = CreateWindow(
            TEXT("COMBOBOX"), NULL,
            WS_CHILD | WS_VISIBLE |  CBS_DROPDOWNLIST,
            400, 0, 200, 200, hwnd, (HMENU)1,
            ((LPCREATESTRUCT)(lp))->hInstance, NULL);
        // コンボボックスに要素を追加
        for (i = 0; i < 3; i++)
            SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)strItem[i]);

        // コンボボックスの内容を表示するラベルを作成する
        label = CreateWindow(
            TEXT("STATIC"), NULL,
            WS_CHILD | WS_VISIBLE,
            0, 100, 200, 100, hwnd, (HMENU)2,
            ((LPCREATESTRUCT)(lp))->hInstance, NULL);

        return 0;
    case WM_COMMAND:
        if (HIWORD(wp) == CBN_SELCHANGE) {
            auto time = lastTime[SendMessage(combo, CB_GETCURSEL, 0, 0)];
            wsprintf(strText, TEXT("アイテム数 = %d\n選択項目 = %d\n待機時間 = %d"),
                SendMessage(combo, CB_GETCOUNT, 0, 0),
                SendMessage(combo, CB_GETCURSEL, 0, 0),
                time);
            SetWindowText(label, strText);
            // iCountをSetされた時間に更新する
            // 忘れずに再描画
            iCount1 = time;
            InvalidateRect(hwnd, NULL, TRUE);
        }

        switch (LOWORD(wp)) {
        case BUTTON_ID1:
            iCount1 = 444;
            break;
        }
    case WM_KEYDOWN:
        switch (wp) {
        case VK_CONTROL:
            iCount1 = 222;
            break;
        case VK_RIGHT:
            iCount1 = 333;
            break;
        case VK_MENU: //ctrl+alt = alt扱い？
            
                // ctrl + alt、でウインドウ表示。yesだとカウンターの数値を2倍にする
                auto result = MessageBox(hwnd, L"push", L"title", MB_OKCANCEL);
                if (result == 1 /*MB_OK = yes*/) {
                    iCount1 = 2 * Seconds;
                }
        }
    case WM_TIMER:
        switch (wp) {
        case TM_COUNT1:
            iCount1--;
            if (iCount1 < 0) { // 60秒経過したらアプリ終了
                PostQuitMessage(0);
                return 1;
            }
            break;
        }
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        wsprintf(strCount, L"TM_COUNT1 = %d\n",iCount1);
        GetClientRect(hwnd, &rctSize);
        DrawText(hdc, strCount, -1, &rctSize, DT_LEFT);
        EndPaint(hwnd, &ps);
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    MessageBox(hDlg, L"hogehogehoge", L"Hoge", 0);
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
