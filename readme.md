#### やりたかったこと

- 60秒のタイマーをもつ
```c++
static int iCount1 = 60; // 60秒タイマー
    switch (msg) {
    case WM_CREATE:
        SetTimer(hwnd, TM_COUNT1, 1000, NULL); // 1000ミリ(1second)毎に変化するタイマー
        return 0;
```

- タイマーの文字をwindowに表示

```c++
case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        wsprintf(strCount, L"TM_COUNT1 = %d\n",iCount1);
        GetClientRect(hwnd, &rctSize);
        DrawText(hdc, strCount, -1, &rctSize, DT_LEFT);
        EndPaint(hwnd, &ps);
```

- windowはctrl+alt でイベント発生
- clickイベントでwindow消滅(自滅できればok)
```c++
case WM_KEYDOWN:
        switch (wp) {
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
        // 一旦画面を無効→WM_PAINTを再び呼んで再描画
        InvalidateRect(hwnd, NULL, TRUE);
```


// VK = Virtual Keyboard