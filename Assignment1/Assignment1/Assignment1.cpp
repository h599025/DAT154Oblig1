// Assignment1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Assignment1.h"

#define MAX_LOADSTRING 100
#define LONG_LIGHT 1;
#define SHORT_LIGHT 2;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int trafficLight = 1;
BOOL LButtonPressed = false;
BOOL RButtonPressed = false;
float pw = 0.1f; // Probability of cars arriving from west per second
float pn = 0.1f; // Probability of cars arriving from north per second

std::vector<RECT> carsTopBottom;
std::vector<RECT> carsLeftRight;
std::vector<COLORREF> carColors = {
    RGB(255, 0, 0),    // Red
    RGB(0, 255, 0),    // Green
    RGB(0, 0, 255),    // Blue
    RGB(255, 255, 0),  // Yellow
    RGB(255, 165, 0)   // Orange
};
std::vector<COLORREF> carColorsLeftRight;
std::vector<COLORREF> carColorsTopBottom;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    MyDlg(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_ASSIGNMENT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASSIGNMENT1));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASSIGNMENT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ASSIGNMENT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// Helper function to draw the traffic light
void DrawTrafficLightTopBottom(HDC hdc, COLORREF red, COLORREF yellow, COLORREF green)
{
    HBRUSH hBrush;

    // Draw Red light
    hBrush = CreateSolidBrush(red);
    SelectObject(hdc, hBrush);
    Ellipse(hdc, 573, 236, 586, 249);
    DeleteObject(hBrush);

    // Draw Yellow light
    hBrush = CreateSolidBrush(yellow);
    SelectObject(hdc, hBrush);
    Ellipse(hdc, 573, 255, 586, 268);
    DeleteObject(hBrush);

    // Draw Green light
    hBrush = CreateSolidBrush(green);
    SelectObject(hdc, hBrush);
    Ellipse(hdc, 573, 273, 586, 286);
    DeleteObject(hBrush);
}

// Helper function to draw the traffic light
void DrawTrafficLightLeftRight(HDC hdc, COLORREF red, COLORREF yellow, COLORREF green)
{
    HBRUSH hBrush;

    // Draw Red light
    hBrush = CreateSolidBrush(red);
    SelectObject(hdc, hBrush);
    Ellipse(hdc, 573, 413, 586, 426);
    DeleteObject(hBrush);

    // Draw Yellow light
    hBrush = CreateSolidBrush(yellow);
    SelectObject(hdc, hBrush);
    Ellipse(hdc, 555, 413, 568, 426);
    DeleteObject(hBrush);

    // Draw Green light
    hBrush = CreateSolidBrush(green);
    SelectObject(hdc, hBrush);
    Ellipse(hdc, 537, 413, 550, 426);
    DeleteObject(hBrush);
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
    static POINT positionTopBottom{ 650, 0 };
    static POINT positionLeftRight{ 0, 350 };
    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 1, 8000, 0);
        SetTimer(hWnd, 2, 1000, 0);
        break;
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
    case WM_TIMER:
    {
        UINT_PTR nIDEvent = wParam;

        switch (nIDEvent)
        {
        case 1:
            // Update the traffic light state
            trafficLight = (trafficLight % 4) + 1;

            if (trafficLight == 2 || trafficLight == 4)
                SetTimer(hWnd, 1, 2000, 0);
            else
                SetTimer(hWnd, 1, 8000, 0);

            // Trigger a repaint of the window
            InvalidateRect(hWnd, NULL, true);
            break;
        case 2:
        {
            RECT rect;
            GetClientRect(hWnd, &rect);

            bool carAtLightTopBottom = false;
            // Move cars in the top-bottom lane before light
            for (size_t i = 0; i < carsTopBottom.size(); i++) {
                // Before light
                if (carsTopBottom[i].bottom < 300 && !carAtLightTopBottom && RButtonPressed) {
                    carsTopBottom[i].bottom += 50;
                    carsTopBottom[i].top += 50;
                }

                // Past light
                else if (carsTopBottom[i].bottom >= 340 && RButtonPressed) {
                    carsTopBottom[i].bottom += 50;
                    carsTopBottom[i].top += 50;
                }

                // Red Light
                else if ((trafficLight == 1 || trafficLight == 2 || trafficLight == 4) && RButtonPressed) {
                    // If car at light
                    if (carAtLightTopBottom) {
                        // Stop the current car
                        if (carsTopBottom[i - 1].top - carsTopBottom[i].bottom <= 50) {
                            carsTopBottom[i].bottom += 0;
                            carsTopBottom[i].top += 0;
                        }
                        else {
                            carsTopBottom[i].bottom += 50;
                            carsTopBottom[i].top += 50;
                        }
                    }
                    // Check if a car is at the light
                    else if (300 <= carsTopBottom[i].bottom && carsTopBottom[i].bottom <= 340) {
                        carAtLightTopBottom = true;
                    }
                }

                // Green light
                else if ((trafficLight == 3) && RButtonPressed) {
                    carsTopBottom[i].bottom += 50;
                    carsTopBottom[i].top += 50;
                }
            }

            bool carAtLight = false;
            // Move cars in the left-right lane before light
            for (size_t i = 0; i < carsLeftRight.size(); i++) {
                // Before light
                if (carsLeftRight[i].right < 600 && !carAtLight && LButtonPressed) {
                    carsLeftRight[i].right += 50;
                    carsLeftRight[i].left += 50;
                }

                // Past light
                else if (carsLeftRight[i].right >= 640 && LButtonPressed) {
                    carsLeftRight[i].right += 50;
                    carsLeftRight[i].left += 50;
                    }
                
                // Red Light
                else if ((trafficLight == 2 || trafficLight == 3 || trafficLight == 4) && LButtonPressed) {
                    // If car at light
                    if (carAtLight) {
                        // Stop the current car
                        if (carsLeftRight[i - 1].left - carsLeftRight[i].right <= 50) {
                            carsLeftRight[i].right += 0;
                            carsLeftRight[i].left += 0;
                        }
                        else {
                            carsLeftRight[i].right += 50;
                            carsLeftRight[i].left += 50;
                        }
                    }
                    // Check if a car is at the light
                    else if (600 <= carsLeftRight[i].right && carsLeftRight[i].right <= 640) {
                        carAtLight = true;
                    }
                }

                // Green light
                else if ((trafficLight == 1) && LButtonPressed) {
                    carsLeftRight[i].right += 50;
                    carsLeftRight[i].left += 50;
                }
            }


            if (LButtonPressed && ((float)rand() / RAND_MAX) < pw) {
                // Check if the vector is empty or if the last car's left coordinate is different
                if (carsLeftRight.empty() || carsLeftRight.back().left != positionLeftRight.x - 15) {
                    RECT newCarRect = { positionLeftRight.x - 15, positionLeftRight.y - 8, positionLeftRight.x + 15, positionLeftRight.y + 8 };
                    carsLeftRight.push_back(newCarRect);

                    // Assign a color to the new car
                    COLORREF carColor = carColors[carsLeftRight.size() % carColors.size()];
                    carColorsLeftRight.push_back(carColor);
                }
            }

            if (RButtonPressed && ((float)rand() / RAND_MAX) < pn) {
                // Check if the vector is empty or if the last car's top coordinate is different
                if (carsTopBottom.empty() || carsTopBottom.back().top != positionTopBottom.y - 15) {
                    RECT newCarRect = { positionTopBottom.x - 8, positionTopBottom.y - 15, positionTopBottom.x + 8, positionTopBottom.y + 15 };
                    carsTopBottom.push_back(newCarRect);

                    // Assign a color to the new car
                    COLORREF carColor = carColors[carsTopBottom.size() % carColors.size()];
                    carColorsTopBottom.push_back(carColor);
                }
            }

            // Trigger a repaint of the window
            InvalidateRect(hWnd, NULL, true);
        }
        break;
        }
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;

            // Retrieving the coordinates of the window
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);

            HDC hdc = BeginPaint(hWnd, &ps);


            // Drawing two gray rectangles (road)
            HBRUSH hGrayBrush = CreateSolidBrush(RGB(198, 198, 198));
            HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(198, 198, 198));
            HGDIOBJ hOrg = SelectObject(hdc, hGrayBrush);
            HGDIOBJ hOrgPen = SelectObject(hdc, whitePen);

            Rectangle(hdc, 600, clientRect.top, 700, clientRect.bottom);
            Rectangle(hdc, clientRect.left, 300, clientRect.right, 400);
            SelectObject(hdc, hOrg);
            SelectObject(hdc, hOrgPen);

            DeleteObject(hGrayBrush);
            DeleteObject(whitePen);


            // Drawing a black trafficlight (top-bottom)
            HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
            hOrg = SelectObject(hdc, hBlackBrush);
            Rectangle(hdc, 570, 233, 590, 290);
            SelectObject(hdc, hOrg);
            DeleteObject(hBlackBrush);

            // Drawing a black trafficlight (left-right)
            hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
            hOrg = SelectObject(hdc, hBlackBrush);
            Rectangle(hdc, 533, 410, 590, 430);
            SelectObject(hdc, hOrg);
            DeleteObject(hBlackBrush);


            // Draw the traffic light based on the current state
            switch (trafficLight)
            {
            case 1:
                DrawTrafficLightTopBottom(hdc, RGB(255, 0, 0), RGB(128, 128, 128), RGB(128, 128, 128));     // red
                DrawTrafficLightLeftRight(hdc, RGB(128, 128, 128), RGB(128, 128, 128), RGB(0, 255, 0));     // green
                break;
            case 2:
                DrawTrafficLightTopBottom(hdc, RGB(255, 0, 0), RGB(255, 255, 0), RGB(128, 128, 128));       // red yellow
                DrawTrafficLightLeftRight(hdc, RGB(128, 128, 128), RGB(255, 255, 0), RGB(128, 128, 128));   // yellow
                break;
            case 3:
                DrawTrafficLightTopBottom(hdc, RGB(128, 128, 128), RGB(128, 128, 128), RGB(0, 255, 0));     // green
                DrawTrafficLightLeftRight(hdc, RGB(255, 0, 0), RGB(128, 128, 128), RGB(128, 128, 128));     // red
                break;  
            case 4:
                DrawTrafficLightTopBottom(hdc, RGB(128, 128, 128), RGB(255, 255, 0), RGB(128, 128, 128));   // yellow
                DrawTrafficLightLeftRight(hdc, RGB(255, 0, 0), RGB(255, 255, 0), RGB(128, 128, 128));       // red yellow
                break;
            }

            for (size_t i = 0; i < carsLeftRight.size(); ++i) {
                HBRUSH bCarBrush = CreateSolidBrush(carColorsLeftRight[i]);
                hOrg = SelectObject(hdc, bCarBrush);
                Rectangle(hdc, carsLeftRight[i].left, carsLeftRight[i].top, carsLeftRight[i].right, carsLeftRight[i].bottom);
                SelectObject(hdc, hOrg);
                DeleteObject(bCarBrush);
            }

            for (size_t i = 0; i < carsTopBottom.size(); ++i) {
                HBRUSH bCarBrush = CreateSolidBrush(carColorsTopBottom[i]);
                hOrg = SelectObject(hdc, bCarBrush);
                Rectangle(hdc, carsTopBottom[i].left, carsTopBottom[i].top, carsTopBottom[i].right, carsTopBottom[i].bottom);
                SelectObject(hdc, hOrg);
                DeleteObject(bCarBrush);
            }

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
        {
            LButtonPressed = true;

            InvalidateRect(hWnd, 0, true);
        }
        break;
    case WM_RBUTTONDOWN:
        {
            RButtonPressed = true;

            InvalidateRect(hWnd, 0, true);
        }
        break;
    case WM_MBUTTONDOWN:
        {
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MyDlg);
        }
        break;
    case WM_KEYDOWN:
        {
            switch (wParam)
            {
            case VK_LEFT: // Decrease pw by 10%
                pw *= 0.9;
                break;
            case VK_RIGHT: // Increase pw by 10%
                pw *= 1.1;
                break;
            case VK_UP: // Decrease pn by 10%
                pn *= 0.9;
                break;
            case VK_DOWN: // Increase pn by 10%
                pn *= 1.1;
                break;
            default:
                break;
            }

            // Trigger a repaint of the window
            InvalidateRect(hWnd, NULL, true);
            break;
        }
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

// Message handler for about box.
INT_PTR CALLBACK MyDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        // Convert float values to strings
        TCHAR buffer[256];
        _stprintf_s(buffer, _T("%f"), pw); // More secure print compared to printf
        SetDlgItemText(hDlg, IDC_EDIT1, buffer);

        _stprintf_s(buffer, _T("%f"), pn); // More secure print compared to printf
        SetDlgItemText(hDlg, IDC_EDIT3, buffer);

        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            TCHAR buffer[256];
            GetDlgItemText(hDlg, IDC_EDIT1, buffer, 256);
            pw = _wtof(buffer); // Takes a string, converts to float and return double equivalent

            GetDlgItemText(hDlg, IDC_EDIT3, buffer, 256);
            pn = _wtof(buffer); // Takes a string, converts to float and return double equivalent

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
