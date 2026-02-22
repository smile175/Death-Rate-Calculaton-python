#include <windows.h>
#include <tchar.h>

static const TCHAR *g_title = _T("Taiwan Counties - Simple Schematic (Win32 GDI)");

typedef struct {
    int x, y, w, h;
    const TCHAR *name;
} RectRegion;

// A very rough schematic layout for counties/cities.
// This is NOT geographically accurate; it's a simple visual map.
static const RectRegion g_regions[] = {
    {260,  20, 120,  40, _T("基隆市")},
    {140,  20, 120,  40, _T("臺北市")},
    { 20,  20, 120,  40, _T("新北市")},
    {140,  70, 120,  40, _T("桃園市")},
    { 20,  70, 120,  40, _T("新竹縣")},
    {260,  70, 120,  40, _T("新竹市")},
    { 20, 120, 120,  40, _T("苗栗縣")},
    {140, 120, 120,  40, _T("臺中市")},
    {260, 120, 120,  40, _T("彰化縣")},
    { 20, 170, 120,  40, _T("南投縣")},
    {140, 170, 120,  40, _T("雲林縣")},
    {260, 170, 120,  40, _T("嘉義縣")},
    {380, 170, 120,  40, _T("嘉義市")},
    {140, 220, 120,  40, _T("臺南市")},
    {260, 220, 120,  40, _T("高雄市")},
    {380, 220, 120,  40, _T("屏東縣")},
    {260, 270, 120,  40, _T("臺東縣")},
    {140, 270, 120,  40, _T("花蓮縣")},
    { 20, 270, 120,  40, _T("宜蘭縣")},
    {520, 120, 120,  40, _T("澎湖縣")},
    {520,  20, 120,  40, _T("金門縣")},
    {520,  70, 120,  40, _T("連江縣")},
};

static void DrawMap(HDC hdc) {
    SetBkMode(hdc, TRANSPARENT);

    HFONT hFont = CreateFont(
        16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Microsoft JhengHei")
    );
    HFONT hOld = (HFONT)SelectObject(hdc, hFont);

    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(30, 30, 30));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hBrush = CreateSolidBrush(RGB(230, 240, 255));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    for (int i = 0; i < (int)(sizeof(g_regions) / sizeof(g_regions[0])); i++) {
        const RectRegion *r = &g_regions[i];
        Rectangle(hdc, r->x, r->y, r->x + r->w, r->y + r->h);
        RECT rc = { r->x, r->y, r->x + r->w, r->y + r->h };
        DrawText(hdc, r->name, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    SelectObject(hdc, hOld);
    DeleteObject(hFont);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        DrawMap(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrev; (void)lpCmdLine;

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = _T("SimpleMapWnd");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, _T("RegisterClass failed."), g_title, MB_ICONERROR | MB_OK);
        return 1;
    }

    HWND hwnd = CreateWindow(
        wc.lpszClassName, g_title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 380,
        NULL, NULL, hInst, NULL
    );

    if (!hwnd) {
        MessageBox(NULL, _T("CreateWindow failed."), g_title, MB_ICONERROR | MB_OK);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
