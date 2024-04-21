/*********************************************************************************************************
 * 
 * 
 * Autor: Abner Cordeiro - 2024
 *
 * Criei esse template para meu próprio uso, onde deixo algumas coisas prontas, para criar a janela
 * para centralizar a janela, criar um timer, pintar na janela, receber algumas teclas comuns.
 * Também crio Pens e Brush, Pens para definir a espessura e cor das linhas, e brush para definir a cor 
 * da área das formas.
 * 
 * 
*/
#include "framework.h"
#include "winapi_template_hdc.h"

#define MAX_LOADSTRING 100

// Variáveis Globais:
HINSTANCE hInst;                                // instância atual
WCHAR szTitle[MAX_LOADSTRING];                  // O texto da barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // o nome da classe da janela principal

// Declarações de encaminhamento de funções incluídas nesse módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

#define TIMER_ID        1 // ID do timer
#define TIMER_MS_UPDATE 2 // tempo de atualização ( 2ms )

HDC gScreenHDC          = NULL;
HBITMAP ghBitmapScreen  = NULL;

RECT gWinRect;      // tamanho da área de cliente
int giScreenX = 0;  // tamanho total do monitor
int giScreenY = 0;  // tamanho total do monitor

// na função CreateWindow uso as variaveis giScreenX, giScreenY e as definições abaixo para
// criar a janela com o tamanho necessário e centralizar no monitor.
#define WIN_WIDTH   800 // dimensões da janela
#define WIN_HEIGHT  700 // dimensões da janela

HBRUSH ghBrushRed;
HBRUSH ghBrushBlue;
HBRUSH ghBrushGreen;

HPEN ghPenBlue;
HPEN ghPenRed;
HPEN ghPenGreen;
HPEN ghPenNull;

HFONT ghFonte1;

void paint_func(HWND hwnd, HDC hDC_Screen)
{
    GetClientRect(hwnd, &gWinRect);

    HPEN hOriginalPen = (HPEN)SelectObject(hDC_Screen, ghPenBlue);
    
    Rectangle(hDC_Screen, 0, 0, gWinRect.right - 1, gWinRect.bottom - 1);
    

    SelectObject(hDC_Screen, ghPenGreen);
    Rectangle(hDC_Screen, 200, 200, 300, 300);

    SelectObject(hDC_Screen, ghPenRed);
    Rectangle(hDC_Screen, 320, 200, 500, 300);

    SelectObject(hDC_Screen, ghPenNull);
    HBRUSH hOriginalBrush = (HBRUSH)SelectObject(hDC_Screen, ghBrushBlue);
    Rectangle(hDC_Screen, 510, 200, 600, 300);

    char szTexto[] = "Ola Mundo";
    HFONT hOriginalFonte = (HFONT)SelectObject(hDC_Screen, ghFonte1);
    TextOutA(hDC_Screen, 50, 50, szTexto, lstrlenA(szTexto));

    SelectObject(hDC_Screen, hOriginalPen);
    SelectObject(hDC_Screen, hOriginalBrush);
    SelectObject(hDC_Screen, hOriginalFonte);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // Inicializar cadeias de caracteres globais
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPITEMPLATEHDC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Realize a inicialização do aplicativo:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPITEMPLATEHDC));

    MSG msg;

    // Loop de mensagem principal:
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
//  FUNÇÃO: MyRegisterClass()
//
//  FINALIDADE: Registra a classe de janela.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPITEMPLATEHDC));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPITEMPLATEHDC);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNÇÃO: InitInstance(HINSTANCE, int)
//
//   FINALIDADE: Salva o identificador de instância e cria a janela principal
//
//   COMENTÁRIOS:
//
//        Nesta função, o identificador de instâncias é salvo em uma variável global e
//        crie e exiba a janela do programa principal.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Armazenar o identificador de instância em nossa variável global


   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       (GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH) / 2,
       (GetSystemMetrics(SM_CYSCREEN) - WIN_HEIGHT) / 2,
       WIN_WIDTH, WIN_HEIGHT, HWND_DESKTOP, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNÇÃO: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  FINALIDADE: Processa as mensagens para a janela principal.
//
//  WM_COMMAND  - processar o menu do aplicativo
//  WM_PAINT    - Pintar a janela principal
//  WM_DESTROY  - postar uma mensagem de saída e retornar
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        giScreenX = GetSystemMetrics(SM_CXSCREEN);
        giScreenY = GetSystemMetrics(SM_CYSCREEN);

        ghBrushRed   = CreateSolidBrush(RGB(255, 100, 100));
        ghBrushBlue  = CreateSolidBrush(RGB(100, 100, 255));
        ghBrushGreen = CreateSolidBrush(RGB(100, 255, 100));

        ghPenRed   = CreatePen(PS_SOLID, 1, RGB(255, 100, 100));
        ghPenGreen = CreatePen(PS_SOLID, 1, RGB(100, 255, 100));
        ghPenBlue  = CreatePen(PS_SOLID, 1, RGB(100, 100, 255));
        ghPenNull = CreatePen(PS_NULL, 1, 0);

        ghFonte1 = CreateFontA(17, 7, 0, 0, FW_NORMAL,
            FALSE, FALSE, FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            PROOF_QUALITY,
            DEFAULT_PITCH,
            "Consolas");



        HDC hdc = GetDC(hWnd);

        gScreenHDC = CreateCompatibleDC(hdc);
        ghBitmapScreen = CreateCompatibleBitmap(hdc, giScreenX, giScreenY);
        SelectObject(gScreenHDC, ghBitmapScreen);

        ReleaseDC(hWnd, hdc);

        SetTimer(hWnd, TIMER_ID, TIMER_MS_UPDATE, NULL);
    }
    break;


    case WM_TIMER:
    {
        if (wParam == TIMER_ID)
        {
            HDC hdc = GetDC(hWnd);

            paint_func(hWnd, gScreenHDC);

            BitBlt(hdc, 0, 0, gWinRect.right, gWinRect.bottom, gScreenHDC, 0, 0, SRCCOPY);

            ReleaseDC(hWnd, hdc);
        }
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analise as seleções do menu:
            switch (wmId)
            {
            case IDM_ABOUT:
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
            case VK_UP:
            break;
            case VK_DOWN:
                break;
            case VK_RETURN:
                break;
            case VK_LEFT:
                break;

            case VK_RIGHT:
                break;

            case VK_F1:
                break;
        }
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            paint_func(hWnd, gScreenHDC);
            EndPaint(hWnd, &ps);
        }
        break;


    case WM_CLOSE:
    {
        KillTimer(hWnd, TIMER_ID);
        DeleteDC(gScreenHDC);
        DeleteObject(ghBitmapScreen);
        DeleteObject(ghBrushRed);
        DeleteObject(ghBrushGreen);
        DeleteObject(ghBrushBlue);

        DeleteObject(ghPenRed);
        DeleteObject(ghPenGreen);
        DeleteObject(ghPenBlue);
        DeleteObject(ghPenNull);

        DeleteObject(ghFonte1);

        DestroyWindow(hWnd);
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

