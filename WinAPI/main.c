#include <math.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define EYES_RED RGB(255, 0, 0)
#define EYES_GREEN RGB(0, 255, 0)
#define EYES_BLUE RGB(0, 0, 255)
#define EYES_BLACK RGB(0, 0, 0)

#define IMG_SIZE_X 600
#define IMG_SIZE_Y 709
#define EYE_RIGHT_X_DEFAULT 370
#define EYE_RIGHT_Y_DEFAULT 408
#define EYE_LEFT_X_DEFAULT 226
#define EYE_LEFT_Y_DEFAULT 408
#define EYE_MOVE_MAX 5
#define MOVE_STEP 5

HINSTANCE hInst;
UINT MessageCount = 0;
UINT Count = 0;
int posX = 0;
int posY = 0;

COLORREF eyeColour = EYES_BLACK;
int eyeRightX = EYE_RIGHT_X_DEFAULT, eyeRightY = EYE_RIGHT_Y_DEFAULT;
int eyeLeftX = EYE_LEFT_X_DEFAULT, eyeLeftY = EYE_LEFT_Y_DEFAULT;
int windowWidth = 0, windowHeight = 0;
int foxX = 0, foxY = 0;

// Function prototypes
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY,
                 POINT cursorPosition);
void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps);

// Simple helper to refresh the image
void refreshImg(HWND hWnd) {
     RECT rectFox;
     SetRect(&rectFox, foxX, foxY, foxX + IMG_SIZE_X, foxY + IMG_SIZE_Y);
     InvalidateRect(hWnd, &rectFox, FALSE);
}

// Application entry point. This is the same as main() in standard C.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
     MSG msg;
     BOOL bRet;
     WNDCLASS wcx;  // register class
     HWND hWnd;

     hInst = hInstance;  // Save the application-instance handle.
                         // Fill in the window class structure with parameters
                         // that describe the main window.

     wcx.style = CS_HREDRAW | CS_VREDRAW;          // redraw if size changes
     wcx.lpfnWndProc = (WNDPROC)MainWndProc;       // points to window procedure
     wcx.cbClsExtra = 0;                           // no extra class memory
     wcx.cbWndExtra = 0;                           // no extra window memory
     wcx.hInstance = hInstance;                    // handle to instance
     wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);  // predefined app. icon
     wcx.hCursor = LoadCursor(NULL, IDC_ARROW);    // predefined arrow
     wcx.hbrBackground = GetStockObject(WHITE_BRUSH);  // white background brush
     wcx.lpszMenuName = (LPCSTR) "MainMenu";           // name of menu resource
     wcx.lpszClassName = (LPCSTR) "MainWClass";        // name of window class

     // Register the window class.

     if (!RegisterClass(&wcx)) return FALSE;

     // create window of registered class

     hWnd = CreateWindow("MainWClass",         // name of window class
                         "ITU FOX ITU FOX",    // title-bar string
                         WS_OVERLAPPEDWINDOW,  // top-level window
                         200,                  // default horizontal position
                         25,                   // default vertical position
                         1000,                 // default width
                         700,                  // default height
                         (HWND)NULL,           // no owner window
                         (HMENU)NULL,          // use class menu
                         hInstance,            // handle to application instance
                         (LPVOID)NULL);        // no window-creation data
     if (!hWnd) return FALSE;

     // Show the window and send a WM_PAINT message to the window procedure.
     // Record the current cursor position.

     ShowWindow(hWnd, nCmdShow);
     UpdateWindow(hWnd);

     // loop of message processing
     while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
          if (bRet == -1) {
               // handle the error and possibly exit
          } else {
               TranslateMessage(&msg);
               DispatchMessage(&msg);
          }
     }
     return (int)msg.wParam;
}

LRESULT CALLBACK MainWndProc(HWND hWnd,      // handle to window
                             UINT uMsg,      // message identifier
                             WPARAM wParam,  // first message parameter
                             LPARAM lParam)  // second message parameter
{
     HDC hDC;
     PAINTSTRUCT ps;
     POINT cursorPosition;

     // init cursor position
     GetCursorPos(&cursorPosition);
     ScreenToClient(hWnd, &cursorPosition);

     switch (uMsg) {
          case WM_CREATE:
               break;

          // character input
          case WM_CHAR:
               switch (wParam) {
                    case 0x08:  // backspace
                    case 0x0A:  // linefeed
                    case 0x1B:  // escape
                         break;

                    case 0x09:  // tab
                         break;

                    default:
                         break;
               }
               break;

          // key input
          case WM_KEYDOWN:
               switch (wParam) {
                         // update posX and posY in order to move object
                    case VK_LEFT:  // left arrow
                    case 0x41:     // 'A'
                         foxX = max(foxX - MOVE_STEP, 0);
                         break;
                    case VK_RIGHT:  // right arrow
                    case 0x44:      // 'D'
                         foxX = min(foxX + MOVE_STEP, windowWidth - IMG_SIZE_X);
                         break;
                    case VK_UP:  // up arrow
                    case 0x57:   // 'W'
                         foxY = max(foxY - MOVE_STEP, 0);
                         break;
                    case VK_DOWN:  // down arrow
                    case 0x53:     // 'S'
                         foxY
                             = min(foxY + MOVE_STEP, windowHeight - IMG_SIZE_Y);
                         break;

                    // react on the other pressed keys
                    case VK_SPACE:  // space
                         foxX = 0;
                         foxY = 0;

                         // Abrupt change may leave fragments of the image
                         // so force redraw the whole window
                         InvalidateRect(hWnd, NULL, TRUE);
                         break;
                    case VK_BACK:  // backspace
                         break;
                    case VK_TAB:  // tab
                         break;
                    case 0x4B:  // 'K' -> Black eyes
                         eyeColour = EYES_BLACK;
                         break;
                    case 0x52:  // 'R' -> Red eyes
                         eyeColour = EYES_RED;
                         break;
                    case 0x47:  // 'G' -> Green eyes
                         eyeColour = EYES_GREEN;
                         break;
                    case 0x42:  // 'B' -> Blue eyes
                         eyeColour = EYES_BLUE;
                         break;

                         // more virtual codes can be found here:
                         // https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
               }

               refreshImg(hWnd);
               break;

          // get cursor position
          case WM_MOUSEMOVE:
               if (wParam & MK_LBUTTON) {  // left button is pressed
                    // Get cursor position
                    int mouseX = LOWORD(lParam);
                    int mouseY = HIWORD(lParam);

                    // Midpoint (for a more natural movement)
                    int midX = (EYE_RIGHT_X_DEFAULT + EYE_LEFT_X_DEFAULT) / 2;
                    int midY = (EYE_RIGHT_Y_DEFAULT + EYE_LEFT_Y_DEFAULT) / 2;

                    // Get distance from midpoint
                    int deltaX = mouseX - midX;
                    int deltaY = mouseY - midY;

                    // Ensure boundaries
                    deltaX = max(min(deltaX, EYE_MOVE_MAX), -EYE_MOVE_MAX);
                    deltaY = max(min(deltaY, EYE_MOVE_MAX), -EYE_MOVE_MAX);

                    // Update the eye position
                    eyeRightX = EYE_RIGHT_X_DEFAULT + deltaX;
                    eyeRightY = EYE_RIGHT_Y_DEFAULT + deltaY;
                    eyeLeftX = EYE_LEFT_X_DEFAULT + deltaX;
                    eyeLeftY = EYE_LEFT_Y_DEFAULT + deltaY;

                    refreshImg(hWnd);
               }
               break;

          // react on mouse clicks
          case WM_LBUTTONDOWN:
               SetCapture(hWnd);
               refreshImg(hWnd);
               break;
          case WM_LBUTTONUP:
               ReleaseCapture();

               // Reset eye position
               eyeRightX = EYE_RIGHT_X_DEFAULT;
               eyeRightY = EYE_RIGHT_Y_DEFAULT;
               eyeLeftX = EYE_LEFT_X_DEFAULT;
               eyeLeftY = EYE_LEFT_Y_DEFAULT;

               // Force redraw
               refreshImg(hWnd);
               break;

          // paint objects
          case WM_PAINT:
               hDC = BeginPaint(hWnd, &ps);

               // Determine window size
               RECT rect;
               GetClientRect(hWnd, &rect);
               windowWidth = rect.right - rect.left;
               windowHeight = rect.bottom - rect.top;

               paintObject(hWnd, hDC, ps, posX, posY, cursorPosition);
               paintPosition(hWnd, hDC, ps);
               EndPaint(hWnd, &ps);
               DeleteDC(hDC);
               break;

               //
               // Process other messages.
               //

          case WM_CLOSE:
               DestroyWindow(hWnd);
               return 0;

          case WM_DESTROY:
               PostQuitMessage(0);
               return 0;

          default:
               return DefWindowProc(hWnd, uMsg, wParam, lParam);
     }
}

void paintObject(HWND hWnd, HDC hDC, PAINTSTRUCT ps, int posX, int posY,
                 POINT cursorPosition) {
     HPEN hPen;
     HBRUSH hBrush;

     /* Draw a background rectangle */

     hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
     hBrush = CreateSolidBrush(RGB(255, 255, 255));

     SelectObject(hDC, hPen);
     SelectObject(hDC, hBrush);

     Rectangle(hDC, foxX, foxY, IMG_SIZE_X + foxX, IMG_SIZE_Y + foxY);

     DeleteObject(hPen);
     DeleteObject(hBrush);

     /* Define points of the fox */

     POINT triangles[][3] = {{{93, 65}, {259, 200}, {182, 183}},
                             {{182, 183}, {259, 200}, {219, 254}},
                             {{93, 65}, {182, 183}, {133, 296}},
                             {{182, 183}, {219, 254}, {133, 296}},
                             {{259, 200}, {219, 254}, {297, 213}},
                             {{302, 213}, {339, 201}, {378, 254}},
                             {{339, 201}, {378, 254}, {417, 186}},
                             {{339, 201}, {417, 186}, {507, 64}},
                             {{378, 254}, {417, 186}, {465, 301}},
                             {{465, 301}, {417, 186}, {507, 64}},
                             {{299, 213}, {219, 254}, {240, 386}},
                             {{300, 213}, {378, 254}, {360, 384}},
                             {{300, 213}, {240, 386}, {299, 442}},
                             {{300, 213}, {360, 384}, {300, 442}},
                             {{240, 386}, {219, 254}, {194, 355}},
                             {{360, 384}, {405, 356}, {378, 254}},
                             {{378, 254}, {405, 356}, {465, 301}},
                             {{194, 355}, {219, 254}, {133, 296}},
                             {{194, 355}, {186, 418}, {133, 296}},
                             {{405, 356}, {413, 418}, {465, 301}},
                             {{194, 355}, {186, 418}, {233, 431}},
                             {{194, 355}, {240, 386}, {218, 400}},
                             {{413, 418}, {405, 356}, {370, 431}},
                             {{405, 356}, {360, 384}, {384, 399}},
                             {{360, 384}, {351, 426}, {300, 442}},
                             {{360, 384}, {384, 399}, {370, 405}},
                             {{360, 384}, {351, 426}, {370, 405}},
                             {{300, 442}, {250, 427}, {240, 386}},
                             {{250, 427}, {240, 386}, {230, 405}},
                             {{218, 400}, {240, 386}, {230, 405}},
                             {{300, 442}, {250, 427}, {261, 479}},
                             {{300, 442}, {351, 426}, {340, 480}},
                             {{300, 442}, {261, 479}, {340, 480}},
                             {{300, 551}, {261, 479}, {340, 480}},
                             {{214, 400}, {204, 402}, {212, 417}},
                             {{212, 417}, {216, 426}, {232, 429}},
                             {{386, 400}, {396, 402}, {388, 417}},
                             {{388, 417}, {384, 426}, {366, 429}},
                             {{363, 429}, {356, 421}, {351, 426}},
                             {{245, 420}, {236, 429}, {250, 427}},
                             {{351, 426}, {348, 441}, {384, 426}},
                             {{250, 427}, {252, 440}, {216, 426}},
                             {{133, 296}, {186, 418}, {107, 408}},
                             {{465, 301}, {413, 418}, {490, 407}},
                             {{252, 440}, {216, 426}, {261, 479}},
                             {{348, 441}, {340, 480}, {384, 426}},
                             {{216, 426}, {261, 479}, {236, 522}},
                             {{384, 426}, {340, 480}, {364, 521}},
                             {{236, 522}, {216, 426}, {186, 418}},
                             {{364, 521}, {384, 426}, {413, 418}},
                             {{186, 418}, {236, 522}, {107, 408}},
                             {{413, 418}, {364, 521}, {490, 407}},
                             {{107, 408}, {236, 521}, {242, 581}},
                             {{490, 407}, {364, 521}, {357, 582}},
                             {{261, 479}, {236, 522}, {269, 572}},
                             {{261, 479}, {290, 533}, {269, 572}},
                             {{340, 480}, {364, 521}, {331, 572}},
                             {{340, 480}, {309, 535}, {331, 572}},
                             {{331, 572}, {309, 535}, {300, 552}},
                             {{270, 571}, {290, 535}, {300, 552}},
                             {{236, 522}, {269, 572}, {242, 581}},
                             {{364, 521}, {357, 582}, {331, 572}},
                             {{242, 581}, {269, 573}, {265, 602}},
                             {{357, 582}, {331, 572}, {333, 603}},
                             {{265, 602}, {269, 573}, {273, 585}},
                             {{333, 603}, {331, 572}, {327, 584}},
                             {{273, 583}, {265, 602}, {300, 600}},
                             {{327, 584}, {333, 603}, {300, 600}},
                             {{265, 602}, {333, 603}, {300, 615}},
                             {{270, 571}, {331, 572}, {300, 552}},
                             {{270, 571}, {273, 583}, {300, 600}},
                             {{300, 600}, {327, 584}, {331, 572}},
                             {{300, 600}, {270, 571}, {331, 572}},
                             {{215, 400}, {212, 417}, {238, 426}},
                             {{384, 399}, {386, 418}, {374, 423}}};

     COLORREF colours[]
         = {RGB(57, 47, 47),    RGB(87, 75, 75),    RGB(255, 238, 228),
            RGB(250, 226, 206), RGB(202, 96, 36),   RGB(202, 96, 36),
            RGB(87, 75, 75),    RGB(57, 47, 47),    RGB(250, 226, 206),
            RGB(255, 238, 228), RGB(240, 126, 58),  RGB(240, 126, 58),
            RGB(242, 116, 40),  RGB(242, 116, 40),  RGB(248, 170, 120),
            RGB(248, 170, 120), RGB(248, 202, 168), RGB(248, 202, 168),
            RGB(246, 174, 124), RGB(246, 174, 124), RGB(247, 120, 40),
            RGB(247, 158, 102), RGB(247, 120, 40),  RGB(247, 158, 102),
            RGB(211, 97, 43),   RGB(220, 102, 40),  RGB(160, 80, 50),
            RGB(211, 97, 43),   RGB(160, 80, 50),   RGB(220, 102, 40),
            RGB(240, 127, 57),  RGB(240, 127, 57),  RGB(250, 171, 122),
            RGB(250, 171, 122), RGB(104, 48, 23),   RGB(104, 48, 23),
            RGB(104, 48, 23),   RGB(104, 48, 23),   RGB(38, 38, 42),
            RGB(38, 38, 42),    RGB(0, 0, 0),       RGB(0, 0, 0),
            RGB(248, 200, 158), RGB(248, 200, 158), RGB(174, 81, 37),
            RGB(174, 81, 37),   RGB(240, 120, 52),  RGB(240, 120, 52),
            RGB(248, 152, 94),  RGB(248, 152, 94),  RGB(250, 215, 185),
            RGB(250, 215, 185), RGB(255, 238, 230), RGB(255, 238, 230),
            RGB(250, 153, 95),  RGB(250, 188, 144), RGB(250, 153, 95),
            RGB(250, 188, 144), RGB(250, 155, 100), RGB(250, 155, 100),
            RGB(251, 226, 206), RGB(251, 226, 206), RGB(246, 240, 230),
            RGB(246, 240, 230), RGB(250, 225, 208), RGB(250, 225, 208),
            RGB(246, 240, 230), RGB(246, 240, 230), RGB(250, 222, 196),
            RGB(31, 31, 31),    RGB(31, 31, 31),    RGB(31, 31, 31),
            RGB(61, 61, 61),    RGB(255, 255, 255), RGB(255, 255, 255)};

     /* Sanity check – same number of colours and points */

     int polyCount = sizeof(triangles) / sizeof(triangles[0]);
     if (sizeof(colours) / sizeof(COLORREF) != polyCount) {
          MessageBox(hWnd, "Number of colours and points do not match!",
                     "Error", MB_OK);
          return;
     }

     /* Fox */

     for (int i = 0; i < polyCount; ++i) {
          hPen = CreatePen(PS_SOLID, 1, colours[i]);
          hBrush = CreateSolidBrush(colours[i]);

          SelectObject(hDC, hPen);
          SelectObject(hDC, hBrush);

          // Adjust point to foxX and foxY
          for (int j = 0; j < 3; ++j) {
               triangles[i][j].x += foxX;
               triangles[i][j].y += foxY;
          }

          Polygon(hDC, triangles[i], 3);

          DeleteObject(hPen);
          DeleteObject(hBrush);
     }

     /* Eyes */

     hPen = CreatePen(PS_SOLID, 1, eyeColour);
     hBrush = CreateSolidBrush(eyeColour);

     SelectObject(hDC, hPen);
     SelectObject(hDC, hBrush);

     Ellipse(hDC, eyeRightX + foxX, eyeRightY + foxY, eyeRightX + foxX + 4,
             eyeRightY + foxY + 12);
     Ellipse(hDC, eyeLeftX + foxX, eyeLeftY + foxY, eyeLeftX + foxX + 4,
             eyeLeftY + foxY + 12);

     DeleteObject(hPen);
     DeleteObject(hBrush);

     return;
}

void paintPosition(HWND hWnd, HDC hDC, PAINTSTRUCT ps) {
     char text[40];  // buffer to store an output text
     HFONT font;     // new large font
     HFONT oldFont;  // saves the previous font

     font = CreateFont(25, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET,
                       OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                       FF_DONTCARE, 0);
     oldFont = (HFONT)SelectObject(hDC, font);
     sprintf(text, "Position -- x:%d, y:%d", foxX, foxY);
     TextOut(hDC, 50, 600, text, (int)strlen(text));
     SelectObject(hDC, oldFont);
     DeleteObject(font);
}
