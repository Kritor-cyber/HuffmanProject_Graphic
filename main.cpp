#include <Windows.h>
#include <windowsx.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT CreateWindowComponents(HWND hwnd);

HWND CreatePushButton(HWND parent, const wchar_t* text, int posX, int posY, int sizeX, int sizeY, HMENU buttonId);

void ButtonClicked(WORD buttonId, HWND parent);

bool CALLBACK SetFont(HWND child, LPARAM font)
{
    SendMessage(child, WM_SETFONT, font, true);
    return true;
}

HWND pathEdit;

#define BUTTON_COMPRESS 1
#define BUTTON_UNCOMPRESS 2
#define BUTTON_CONVERT 3
#define BUTTON_CHOOSEFILE 4

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Learn to Program Windows",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,

        CW_USEDEFAULT, CW_USEDEFAULT, 452, 125,

        NULL, 
        NULL,
        hInstance,
        NULL);

    if (hwnd == NULL)
    {
        return 0;
    }
    EnumChildWindows(hwnd, (WNDENUMPROC)SetFont, (LPARAM)GetStockObject(DEFAULT_GUI_FONT));
    
    ShowWindow(hwnd, nCmdShow);

    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        
        return CreateWindowComponents(hwnd);
    }
    case WM_CTLCOLORSTATIC:
    {
        return (INT_PTR)GetSysColorBrush(COLOR_WINDOW);
    }
    case WM_COMMAND:
    {
        if (HIWORD(wParam) == BN_CLICKED)
        {
            ButtonClicked(LOWORD(wParam), hwnd);

            return 0;
        }
        return 0;
    }
    case WM_CLOSE:
        if (MessageBox(hwnd, L"Really quit?", L"My app", MB_OKCANCEL) == IDOK)
        {
            DestroyWindow(hwnd);
        }

        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CreateWindowComponents(HWND hwnd)
{
    RECT rect;
    GetWindowRect(hwnd, &rect);

    LPTEXTMETRIC lptm = (LPTEXTMETRIC)malloc(sizeof(TEXTMETRIC));
    int buttonHeight = 0;
    int textHeight;
    if (lptm)
    {
        GetTextMetrics(GetWindowDC(hwnd), lptm);

        textHeight = lptm->tmHeight;
        buttonHeight = textHeight + textHeight / 2;
        textHeight = textHeight + textHeight / 10;
        free(lptm);
    }
    else
    {
        MessageBeep(0);
        MessageBox(hwnd, L"Impossible d'allouer de la mémoire pour obtenir la taille du texte", L"ERROR : My app", MB_OK);
        PostQuitMessage(0);
        return 0;
    }

    int xGroup = 4;
    int yGroup = 0;

    HWND groupBox = CreateWindow(L"BUTTON", 
        L"Compresseur de fichiers",
        WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
        xGroup,
        yGroup,
        xGroup + 0 + 325+100,
        yGroup + 12 + (buttonHeight) * 3,
        hwnd,
        NULL,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL);

    

    pathEdit = CreateWindow(L"EDIT",
        L"Choisissez votre fichier",
        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL,
        xGroup + 20, yGroup + 8 + (buttonHeight * 1.5) - (textHeight / 2),
        200, textHeight,
        hwnd, NULL,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
    Edit_Enable(pathEdit, TRUE);


    HWND hwndButton = CreatePushButton(hwnd, L"Choisir...", xGroup + 0 + 220, yGroup + 8 + (buttonHeight*1.5) - (textHeight/2), 100, textHeight, (HMENU)BUTTON_CHOOSEFILE);
    if (hwndButton == NULL)
    {
        MessageBeep(0);
        MessageBox(hwnd, L"Impossible de créer le bouton pour choisir le fichier", L"ERROR : My app", MB_OK);
        PostQuitMessage(0);
    }

    hwndButton = CreatePushButton(hwnd, L"Compresser", xGroup + 0 + 325, yGroup + 8, 100, buttonHeight, (HMENU)BUTTON_COMPRESS);
    if (hwndButton == NULL)
    {
        MessageBeep(0);
        MessageBox(hwnd, L"Impossible de créer le bouton pour compresser", L"ERROR : My app", MB_OK);
        PostQuitMessage(0);
    }


    hwndButton = CreatePushButton(hwnd, L"Décompresser", xGroup + 0 + 325, yGroup + 8 + buttonHeight, 100, buttonHeight, (HMENU)BUTTON_UNCOMPRESS);
    if (hwndButton == NULL)
    {
        MessageBeep(0);
        MessageBox(hwnd, L"Impossible de créer le bouton pour décompresser", L"ERROR : My app", MB_OK);
        PostQuitMessage(0);
    }

    hwndButton = CreatePushButton(hwnd, L"Convertir en binaire", xGroup + 0 + 325, yGroup + 8 + (buttonHeight) * 2, 100, buttonHeight, (HMENU)BUTTON_CONVERT);
    if (hwndButton == NULL)
    {
        MessageBeep(0);
        MessageBox(hwnd, L"Impossible de créer le bouton pour convertir en binaire", L"ERROR : My app", MB_OK);
        PostQuitMessage(0);
    }

    return 0;
}

HWND CreatePushButton(HWND parent, const wchar_t* text, int posX, int posY, int sizeX, int sizeY, HMENU buttonId)
{
    HWND button = CreateWindow(
        L"BUTTON",
        text,
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        posX,
        posY,
        sizeX,
        sizeY,
        parent,
        buttonId,
        (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
        NULL);

    return button;
}

void ButtonClicked(WORD buttonId, HWND parent)
{
    switch (buttonId)
    {
    case BUTTON_COMPRESS:
        MessageBox(parent, L"Tu veux compresser", L"My app", MB_OK);
        break;
    case BUTTON_UNCOMPRESS:
        MessageBox(parent, L"Tu veux décompresser", L"My app", MB_OK);
        break;
    case BUTTON_CONVERT:
        MessageBox(parent, L"Tu veux convertir", L"My app", MB_OK);
        break;
    case BUTTON_CHOOSEFILE:
    {
        MessageBox(parent, L"Vous devriez avoir à choisir votre fichier", L"My app", MB_OK);
        OPENFILENAME ofn;       // common dialog box structure
        wchar_t szFile[260];       // buffer for file name
        Edit_GetText(pathEdit, szFile, 260);

        //HWND hwnd;              // owner window
        HANDLE hf;              // file handle

        // Initialize OPENFILENAME
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = parent;
        ofn.lpstrFile = szFile;
        // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
        // use the contents of szFile to initialize itself.
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"All\0*.*\0Text (.txt)\0*.txt\0My File Type (.mytype)\0*.mytype\0";
        ofn.nFilterIndex = 2;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Display the Open dialog box. 

        if (GetOpenFileName(&ofn) == TRUE)
            hf = CreateFile(ofn.lpstrFile,
                GENERIC_READ,
                0,
                (LPSECURITY_ATTRIBUTES)NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                (HANDLE)NULL);
        if (szFile[0] != '\0')
            Edit_SetText(pathEdit, szFile);
        //MessageBox(hwnd, szFile, L"My app", MB_OK);
        break;
    }
    default:
        MessageBox(parent, L"Bouton inconnu", L"My app", MB_OK);
    }
}