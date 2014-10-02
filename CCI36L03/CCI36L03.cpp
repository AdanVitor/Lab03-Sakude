
#define UNICODE
#define _UNICODE
#include "stdafx.h"
#include <windows.h>
#include "grafbase.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "PolyFill.h"

#define Max(x,y) ((x>y)?x:y)
#define Arred(x) ((int)(x+0.5))   // only for x>=0
#define PI 3.1415926535897932384626433832795
#define ENTER 13
#define BACKSPACE 8
#define ESC 27
#define NO_ACTION -1
#define L_MOUSE_MOVE_DOWN 0
#define L_MOUSE_DOWN 1
#define R_MOUSE_DOWN 2
#define L_DOUBLE_CLICK 3
#define L_MOUSE_UP 4

#define CONSOLE_SIZE_X 640  // initial input line size in pixel
#define START_TEXT_X 5   // input line box initial position X
#define END_TEXT_X (CONSOLE_SIZE_X-2) // size of input line box in X
static SHORT numXpixels = 500;//640;			// size of window in X
static SHORT numYpixels = 500;//480             // size of window in Y
int start_text_y = numYpixels - 20;
int end_text_y = 21 + start_text_y;					// size of input line in Y

int key_input = NO_ACTION, mouse_action = NO_ACTION;
int mouse_x, mouse_y;
static BOOL graphics = TRUE;                /* Boolean, enable graphics?  */
/*São os padrões disponíveis: optou-se por utilizar um tamanho variável para facilitar a construção do padrão, deixando mais destacado*/
int pattern0[] = { 1, 1, 1, 1 }; // sem padrão.
int pattern1[] = { 1, 1, 1, 1, 0, 0, 0, 0 }; // tracejado.
int pattern2[] = { 1, 1, 1, 1, 0, 0, 0 , 0 , 1 , 0 , 0 , 0 , 0}; // traço ponto.
int pattern3[] = { 0, 1, 0, 0, 0, 0, 1, 0 }; // pontilhado.
int patternSizeFill;
int *patternFill;
int chosenPatternFill = 1;

//static short draw_color = MY_WHITE;        /* Current drawing color.     */
char buffer[200] = "";					// string buffer for keyboard input


static COLORREF win_draw_color = RGB(255, 255, 255);  // current draw color
static HBRUSH blackBrush;
HDC  hdc;                                   /* Presentation Space Handle */
HWND WinHandle = NULL;                /* Client area window handle */
/* Foreground colors default to standard EGA palette.                */
/* No map is necessary unless a non-standard palette is implemented. */
static  HPEN hpen;
// 16 Predefined colors
typedef enum {
	MY_BLACK, MY_BLUE, MY_GREEN, MY_CYAN, MY_RED, MY_MAGENTA,
	MY_BROWN, MY_LIGHTGRAY, MY_DARKGRAY, MY_LIGHTBLUE, MY_LIGHTGREEN,
	MY_LIGHTCYAN, MY_LIGHTRED, MY_LIGHTMAGENTA, MY_YELLOW, MY_WHITE
} my_color;
/* Define RGB color settings for MY enumerated colors */
static COLORREF color_trans_map[] =
{
	RGB(0, 0, 0),//MY_BLACK
	RGB(0, 0, 255),//MY_BLUE,
	RGB(0, 127, 0),//MY_GREEN,
	RGB(0, 233, 233),//MY_CYAN,
	RGB(255, 0, 0),//MY_RED,
	RGB(255, 0, 255),//MY_MAGENTA,
	RGB(153, 51, 0),//MY_BROWN,
	RGB(175, 175, 175),//MY_LIGHTGRAY,
	RGB(70, 70, 70),//MY_DARKGRAY,
	RGB(51, 51, 255),//MY_LIGHTBLUE,
	RGB(0, 255, 0),//MY_LIGHTGREEN,
	RGB(51, 255, 255),//MY_LIGHTCYAN,
	RGB(255, 25, 25),//MY_LIGHTRED,
	RGB(255, 65, 255),//MY_LIGHTMAGENTA,
	RGB(255, 255, 0),//MY_YELLOW,
	RGB(255, 255, 255),//MY_WHITE,
};


/*******************************************************************
                       Function Prototypes
************************************************************************/
HMENU menu, menu_draw, menu_pattern , menu_actions;
void menuBar();
void DrawXorPixel(int x, int y);


/****************************************************************************
*  Set the X dimension of the current window in pixels.                 *
****************************************************************************/
void SetMaxX(int maxX)
{
	numXpixels = maxX;
}
/****************************************************************************
*  Set the X dimension of the current window in pixels.                 *
****************************************************************************/
void SetMaxY(int maxY)
{
	numYpixels = maxY;
	start_text_y = numYpixels - 20;
	end_text_y = 21 + start_text_y;
}

/****************************************************************************
*  Draws a pixel at the specified point on the screen.                      *
*  Caution!! GpiSetPel has been found to crash programs at some locations!  *
****************************************************************************/
void DrawPixel(int x, int y)
{

	SetPixel(hdc, x, y, win_draw_color);
}





/****************************************************************************
*                                                                           *
*  Name       :  InitGraphics()                                             *
*                                                                           *
*  Description:   Initializes the process for Window services               *
*  Concepts   : - obtains anchor block handle							    *
*  - creates the main frame window which creates the                        *
*  main client window                                                       *
*                                                                           *                                                                           *
*                                                                           *
****************************************************************************/




wchar_t wind_class[] = L"Window Application";
wchar_t wind_name[] = L"Lab 01 CCI-36     ";
void InitGraphics()
{

	HINSTANCE hInst = NULL;
	HWND hWnd;
	WNDCLASS    wc;
	LPCWSTR window_class = (LPCWSTR)wind_class;
	//
	LPCWSTR window_name = (LPCWSTR)wind_name;
	// Fill up window structure

	wc.lpszClassName = window_class;  // registration name
	wc.hInstance = hInst;				// application instance
	wc.lpfnWndProc = (WNDPROC)WinProc; // event handling function
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // cursor type
	wc.hIcon = NULL;
	wc.lpszMenuName = NULL;						// menu, if any
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // background color
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;		// window style
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	menuBar();

	/* Registeer window class      */
	//GlobalAddAtom(window_class);

	if (!RegisterClass(&wc))
	{
		printf(" Error in RegisterClass...\n");
		exit(1);
	}

	// Create window
	hWnd = CreateWindow(
		window_class,           // Desktop window class name
		L"Lab 1 CCI-36",       // window name
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,  // Window class style
		0, 0,                //window  top, left corner(origin)
		500, 500,                   // window X,Y size
		(HWND)NULL,                   // Parent window         /
		(HMENU)menu,				// handle to menu
		(HINSTANCE)hInst,			// handle to application instance
		(LPVOID)NULL);  //  pointer to window-creation data

	if ((hWnd == NULL))
	{

		printf("error in CreateWindow ...\n ");
		exit(1);

	}

	// Sets the visibility state of window
	ShowWindow(hWnd, SW_SHOW);

	// store window handle device context
	WinHandle = hWnd;
	hdc = GetDC(WinHandle);
	// set hpen, blackbrush for clearing window, color for text and text background
	hpen = CreatePen(PS_SOLID, 1, win_draw_color);
	SelectObject(hdc, hpen);
	blackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(255, 255, 255));

}


/****************************************************************************
*  Reset display to default mode.                                           *
****************************************************************************/
void CloseGraphics(void)
{

	// Delete pen and destroy window
	DeleteObject(hpen);
	ReleaseDC(WinHandle, hdc);
	DestroyWindow(WinHandle);          /* Tidy up...                 */
}




/****************************************************************************
*  Returns the X dimension of the current window in pixels.                 *
****************************************************************************/
int GetMaxX(void)
{
	return numXpixels;
}

/****************************************************************************
*  Returns the Y dimension of the current window in pixels.                 *
****************************************************************************/
int GetMaxY(void)
{
	return numYpixels;
}


/****************************************************************************
*  Set current graphics drawing color.                                      *
****************************************************************************/
void SetGraphicsColor(int new_color, int width)
{
	HPEN hpenOld;
	if (graphics)
	{
		// test to avoid unnecessay pen changing
		if (win_draw_color != color_trans_map[new_color])
		{
			// get COLORREF from defined palette
			win_draw_color = color_trans_map[new_color];

			// create a pen with that color
			hpen = CreatePen(PS_SOLID, width, win_draw_color);
			hpenOld = (HPEN)SelectObject(hdc, hpen);
			//   delete old pen
			DeleteObject(hpenOld);
		}
	}

}




/****************************************************************************
*  Returns the color value of the pixel at the specified point on the       *
*  screen.                                                                  *
****************************************************************************/
int GetPixel(int x, int y)
{
	return (int)GetPixel(hdc, x, y);
}



void CheckGraphicsMsg(void)
{
	MSG msg;
	/*Peek Message from message queue */
	if (PeekMessage(&msg, WinHandle, 0L, 0L, PM_REMOVE))
		//  Translate keyboard hit to virtual key code and send to message queue
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}
void ClearString(char *str)
{
	str[0] = 0x00;
}
void EraseMessage()
{
	RECT rec = { START_TEXT_X, start_text_y,
		END_TEXT_X, end_text_y };
	HBRUSH backgrdBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	// Clear input input box
	FillRect(hdc, &rec, backgrdBrush);
}
void  PrintMessage(char *buffer)
{
	// Write input text in the graphics window
	// Input line is in the upper portion of the graphics window
	if (strlen(buffer) > 0)
		TextOutA(hdc, START_TEXT_X, start_text_y, (LPCSTR)buffer, strlen(buffer));

}




/****************************************************************************
*  Mouse Handler for Win 95                                                   *
****************************************************************************/
int menu_item , hasEvent;
static LRESULT CALLBACK WinProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;

	char str[3] = " ";
	switch (messg)
	{

	case WM_COMMAND:
		menu_item = LOWORD(wParam);
		hasEvent = 1;

		break;

	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		PrintMessage(buffer);
		// Draw everything
		//	  RefreshScreen();

		ValidateRect(hWnd, NULL);

		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
	{
					// Win32 keyboard message: lParam=key code wParam= virtual key (ASCII) code

					if (!(LOWORD(wParam) & KF_UP) &&
						!(LOWORD(wParam) & KF_ALTDOWN))

					{
						//  take keyboard input
						key_input = (char)LOWORD(wParam);



						if (key_input == ENTER) // Enter
						{
							EraseMessage();
							//buffer[strlen(buffer)]=0x00;
						}
						else if (key_input == BACKSPACE) // BackSpace
						{
							if (strlen(buffer) > 0)
							{
								int len = strlen(buffer) - 1;
								// Clear last character in buffer
								buffer[len] = ' ';
								// Clear characters in input box
								strcat_s(buffer, "   ");
								PrintMessage(buffer);
								buffer[len] = 0x00; // put end string
							}
						}
						else if (key_input > 31 && key_input < 130)
						{
							int leng = strlen(buffer);
							EraseMessage();
							str[0] = key_input;
							strcat_s(buffer, str); // add char
							// display, update input box
							PrintMessage(buffer);
						}
						else if (key_input != ESC) //ESC
							key_input = -1;
						break;
					}
	}
	case WM_SIZE:
		// resize
		SetMaxX(LOWORD(lParam));  // width of client area
		SetMaxY(HIWORD(lParam));
		PostMessage(WinHandle, WM_PAINT, wParam, lParam);

		break;
	case WM_MOUSEMOVE:

	{     key_input = wParam;
	if (key_input == MK_LBUTTON)

	{
		EraseMessage();
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		key_input = wParam;
		printf_s(buffer, " x = %d y = %d", mouse_x, mouse_y);
		PrintMessage(buffer);
		mouse_action = L_MOUSE_MOVE_DOWN;
		ClearString(buffer);
	}
	break;
	}
	case WM_LBUTTONDOWN:

	{

						   EraseMessage();
						   mouse_x = LOWORD(lParam);
						   mouse_y = HIWORD(lParam);
						   key_input = wParam;
						   printf_s(buffer, " x = %d y = %d", mouse_x, mouse_y);
						   PrintMessage(buffer);
						   mouse_action = L_MOUSE_DOWN;
						   ClearString(buffer);
						   break;
	}
	case WM_LBUTTONUP:

	{

						 EraseMessage();
						 mouse_x = LOWORD(lParam);
						 mouse_y = HIWORD(lParam);
						 key_input = wParam;
						 sprintf_s(buffer, " x = %d y = %d", mouse_x, mouse_y);
						 PrintMessage(buffer);
						 mouse_action = L_MOUSE_UP;
						 ClearString(buffer);
						 break;
	}

	case WM_RBUTTONDOWN:
	{
						   EraseMessage();
						   key_input = wParam;
						   mouse_x = LOWORD(lParam);
						   mouse_y = HIWORD(lParam);

						   sprintf_s(buffer, " x = %d y = %d", mouse_x, mouse_y);
						   PrintMessage(buffer);

						   mouse_action = R_MOUSE_DOWN;
						   ClearString(buffer);

						   break;
	}
	case WM_LBUTTONDBLCLK:
		EraseMessage();
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		key_input = wParam;
		sprintf_s(buffer, " x = %d y = %d", mouse_x, mouse_y);
		PrintMessage(buffer);
		mouse_action = L_DOUBLE_CLICK;
		ClearString(buffer);
		key_input = wParam;

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return(DefWindowProc(hWnd, messg, wParam, lParam));
		break;
	}

	return 0;

}



/*Essa função desenha uma linha usando o algoritmo DDA*/
void DrawLine(int x1, int y1, int x2, int y2 , int pattern[] , int patternSize)
{  // i,length são inteiros   x,y,dx,dy são reais

	int length = Max(abs(x2 - x1), abs(y2 - y1));


	if (length>0)
	{
		float dx = ((float)(x2 - x1)) / length;
		float dy = ((float)(y2 - y1)) / length;
		float x = (float)x1;
		float y = (float)y1;
		for (int i = 0; i <= length; i++)
		{
			if (pattern[i%patternSize]){
				DrawXorPixel(Arred((int)x), Arred((int)y));
			}
			x = x + dx;    // dx = 1 ou -1 ou m
			y = y + dy;   // yx = 1 ou -1 ou 1/m
		}
	}
}

/*Essa função desenha uma linha, usando o algoritmo de Bresenham*/
void BresenhamLine(int x0, int y0, int x1, int y1 , int pattern[] ,int patternSize)
{
	int x = x0;
	int y = y0;
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int incx, incy;
	int d, incrE, incrNE;

	incx = ((x0 > x1) ? -1 : (x0 < x1) ? 1 : 0);
	incy = ((y0 > y1) ? -1 : (y0 < y1) ? 1 : 0);

	if (dx > dy){  // inclinacao menor que 45 graus
		d = 2 * dy - dx;
		incrE = 2 * dy;
		incrNE = 2 * (dy - dx);
		for (int i = 0; i < dx; i++){
			if (pattern[i%patternSize]){
				DrawXorPixel(x, y);
			}

			if (d <= 0){
				d += incrE;
			}
			else{
				d += incrNE;
				y += incy;
			}
			x += incx;
		}
	}
	else{
		d = 2 * dx - dy; //inclinação maior que 45 graus
		incrE = 2 * dx;
		incrNE = 2 * (dx - dy);
		for (int i = 0; i < dy; i++){
			if (pattern[i%patternSize]){
				DrawXorPixel(x, y);
			}
			if (d <= 0){
				d += incrE;
			}
			else{
				d += incrNE;
				x += incx;
			}
			y += incy;
		}
	}

}



/*Essa função desenha os oito pontos simétricos da circunferência: ela recebe como parâmetros
os pontos do centro da circunferência: xc, yc
os pontos do primeiro octante: x , y
o número do ponto que está sendo desenhado no primeiro octante: numberPoint
o total de números desenhados no primeiro octatnte: pointsNumber
o pattern: pattern[]
o tamanho do pattern: patternSize*/
void PlotCirclePattern(int xc, int yc, int x, int y, int numberPoint, int pointsNumber , int pattern[] ,int patternSize){
	// First octant - starting with y axis and with clockwise

	if (pattern[numberPoint % patternSize]){
		DrawXorPixel(xc + x, yc + y);
	}
	// Second octant
	if (pattern[(2 * pointsNumber - numberPoint) % patternSize]){
		DrawXorPixel(xc + y, yc + x);
	}
	// Third octant
	if (pattern[(2 * pointsNumber + numberPoint) % patternSize]){
		DrawXorPixel(xc + y, yc - x);
	}
	// Fourth octant
	if (pattern[(4 * pointsNumber - numberPoint) % patternSize ]){
		DrawXorPixel(xc + x, yc - y);
	}
	// Fifth octant
	if (pattern[(4 * pointsNumber + numberPoint) % patternSize]){
		DrawXorPixel(xc - x, yc - y);
	}
	// Sixth octant
	if (pattern[(6 * pointsNumber - numberPoint) % patternSize]){
		DrawXorPixel(xc - y, yc - x);
	}
	// Seventh octant
	if (pattern[(6 * pointsNumber + numberPoint) % patternSize]){
		DrawXorPixel(xc - y, yc + x);
	}
	// Eighth octant
	if (pattern[(8 * pointsNumber - numberPoint) % patternSize]){
		DrawXorPixel(xc - x, yc + y);
	}
}

/*Função que efetivamente desenha o círculo, conforme explicado no comentário da função CircleBresenham*/
void CircleWithPattern(int xc, int yc , int r , int pointsNumber ,int pattern[] , int patternSize){

	int x, y, d, deltaE, deltaSE;
	x = 0; y = r;
	d = 1 - r; deltaE = 3; deltaSE = -2 * r + 5;
	int i = 0;

	PlotCirclePattern(xc, yc, x, y, i, pointsNumber,pattern,patternSize); // Plot 8 symetrical circle points

	while (y>x)   // Draw a eighth of a circle
	{
		if (d<0)
		{
			d += deltaE;
			deltaE += 2;
			deltaSE += 2;
		}
		else {
			d += deltaSE;
			deltaE += 2;
			deltaSE += 4;
			y--;
		}
		x++;
		i++;
		PlotCirclePattern(xc, yc, x, y,i, pointsNumber , pattern, patternSize); // Plot 8 symetrical circle points
	}

}

/*Essa função é responsável por desenhar um Círculo, usando o algoritmo de Bresenham, no qual se desenha um oitavo do círculo e
os demais pontos são desenhados por simetria
Nesse ponto cabe uma ressalva: essa função acaba por chamar uma outra função CircleWithPattern, que efetivamente desenha o círculo.
Essa função CircleBresenham é chamada somente para contar o número de pontos desenhados no primeiro octant e então passar esse parâmetro para
a função CircleWithPattern, que necessita dessa informação para poder fazer o padrão.*/
void CircleBresenham(int xc, int yc, int r , int pattern[] , int patternSize)

{
	int x, y, d, deltaE, deltaSE;
	x = 0; y = r;
	d = 1 - r; deltaE = 3; deltaSE = -2 * r + 5;
	int pointsNumber = 1;

	while (y>x)   // Draw a octant in clockwise
	{
		if (d<0)
		{
			d += deltaE;
			deltaE += 2;
			deltaSE += 2;
		}
		else {
			d += deltaSE;
			deltaE += 2;
			deltaSE += 4;
			y--;
		}
		x++;
		pointsNumber++;
	}

	CircleWithPattern(xc, yc, r,pointsNumber , pattern, patternSize);


}

/*Desenha um pixel XOR*/
void DrawXorPixel(int x, int y)
{
	unsigned int mask = 0x00FFFFFF;

	COLORREF cor = GetPixel(hdc, x, y);

	cor ^= mask; // bit-bit Xor operation mask with color
	SetPixel(hdc, x, y, cor);
}

/*Função para desenhar uma linha XOR: o algoritmo usado é o DDA*/
void DrawXorLine(int x1, int y1, int x2, int y2 ,int pattern[], int patternSize){
	int i, length; float  x, y, dx, dy;

	length = Max(abs(x2 - x1), abs(y2 - y1));

	if (length>0)
	{
		dx = ((float)(x2 - x1)) / length;
		dy = ((float)(y2 - y1)) / length;
		x = (float)x1; y = (float)y1;
		for (i = 0; i <= length; i++)
		{
			DrawXorPixel(Arred(x), Arred(y));
			x = x + dx;    // dx = 1 ou -1 ou m
			y = y + dy;   // yx = 1 ou -1 ou 1/m
		}
	}
}


void EraseLine(int x0, int y0, int x1, int y1){
	SetGraphicsColor(255, 1);
	int x = x0;
	int y = y0;
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int incx, incy;
	int d, incrE, incrNE;

	incx = ((x0 > x1) ? -1 : (x0 < x1) ? 1 : 0);
	incy = ((y0 > y1) ? -1 : (y0 < y1) ? 1 : 0);

	if (dx > dy){  // inclinacao menor que 45 graus
		d = 2 * dy - dx;
		incrE = 2 * dy;
		incrNE = 2 * (dy - dx);
		for (int i = 0; i < dx; i++){
			
			DrawPixel(x, y);
			

			if (d <= 0){
				d += incrE;
			}
			else{
				d += incrNE;
				y += incy;
			}
			x += incx;
		}
	}
	else{
		d = 2 * dx - dy; //inclinação maior que 45 graus
		incrE = 2 * dx;
		incrNE = 2 * (dx - dy);
		for (int i = 0; i < dy; i++){
			
			DrawPixel(x, y);
			if (d <= 0){
				d += incrE;
			}
			else{
				d += incrNE;
				x += incx;
			}
			y += incy;
		}
	}

}





/*Essa função é chamada na main, sendo responsável pela escolha da figura e algoritmo: Linha (DDA (DrawLine) , DrawXorLine (implementado com o DDA) e BresenhamLine)
ou circunferência: Circle Bresenham*/
void chooseDraw(int chosenDraw, int chosenPattern, int p0_x, int p0_y, int p1_x, int p1_y) {
	int* pattern = pattern0;
	int patternSize = 0;
	switch (chosenPattern){
	case 1: pattern = pattern0;
		patternSize = 4;
		break;
	case 2: pattern = pattern1;
		patternSize = 8;
		break;
	case 3: pattern = pattern2;
		patternSize = 13;
		break;
	case 4: pattern = pattern3;
		patternSize = 8;
		break;
	}

	switch (chosenDraw){
	case 1:
		DrawLine(p0_x, p0_y, p1_x, p1_y,pattern,patternSize);
		break;
	case 2:
		DrawXorLine(p0_x, p0_y, p1_x, p1_y,pattern,patternSize);
		break;
	case 3:
		BresenhamLine(p0_x, p0_y, p1_x, p1_y,pattern, patternSize);
		break;
	case 4:
		double dx = p0_x - p1_x;
		double dy = p0_y - p1_y;
		double radio = sqrt(pow(dx, 2)+pow(dy, 2));
		CircleBresenham(p0_x, p0_y, Arred((int)radio), pattern, patternSize);
		break;
	}
}





/*Função que limpa a tela - ela é chamada quando se aperta o botão Erase Draw*/
void ClearGraphicsScreen(void)
{
	POINT p1 = { 0, 0 };;
	RECT rc;
	// Fill up screen with a black rectangle
	SetRect(&rc, p1.x, p1.y, GetMaxX(), GetMaxY());
	SelectObject(hdc, blackBrush);
	FillRect(hdc, &rc, blackBrush);
	SelectObject(hdc, hpen);

}

void InsertVertex(polygon_type &poly, int x, int y) {	// insert x,y as the last element
	if (poly.n<MAX_POLY) {
		poly.vertex[poly.n].x=x;
		poly.vertex[poly.n].y=y;
		poly.n++;
	}
}

void GetPoint(polygon_type polygon, int k , int &x , int &y ) {
	x = polygon.vertex[k].x;
	y = polygon.vertex[k].y;
}

void PolyInsert(edge_list_type &list, int x1, int y1, int x2, int y2) { // insert line segment in edge struct, if not horizontal
	if (y1!=y2) {  
		int YM=Max(y1,y2),J1=list.n+1;
		while (J1!=1 &&  list.edge[J1-1].Ymax<YM) {
			list.edge[J1]=list.edge[J1-1];
			J1--;
		}
		list.edge[J1].Ymax=YM;
		list.edge[J1].dx = -1*(float)(x2-x1)/(y2-y1);
		if (y1>y2) {
			list.edge[J1].Ymin=y2;
			list.edge[J1].Xinter=(float)x1;
		} else {
			list.edge[J1].Ymin=y1;
			list.edge[J1].Xinter=(float)x2;
		}
		list.n++;
	}
}




void LoadPolygon(polygon_type &polygon, edge_list_type &list, int &num_Edges) {
	int x1,x2,y1,y2,k=0;
	list.n = 0;
	GetPoint(polygon, k,x1,y1);
	num_Edges = 0;
	for ( ; k < polygon.n-1; k++ ) {
		GetPoint(polygon, k%polygon.n+1,x2,y2);
		if (y1==y2) x1 = x2 ;
		else {
			PolyInsert(list, x1,y1,x2,y2);
			num_Edges+=1;
			x1=x2;
			y1=y2;
		}
	}
} 

void Include(edge_list_type &list, int &end_Edge, int final_Edge, int scan) {	// include all edges that intersept y_scan
	while (end_Edge < final_Edge && list.edge[end_Edge+1].Ymax >= scan) {
		end_Edge++;
   }
}

void XSort(edge_list_type &list, int start_Edge, int last_Edge) {  
	int L,k ; bool sorted=false;
	edge_type temp;
	// Use bubble sort
	for ( k = start_Edge ; k < last_Edge; k++ ) {
		L = k + 1; 
		while (L > start_Edge && list.edge[L].Xinter < list.edge[L-1].Xinter) {
			temp=list.edge[L];
			list.edge[L]=list.edge[L-1];
			list.edge[L-1]=temp;
			L--;
		}
	}
} 

boolean shouldBeDrawn(int x, int y){
	switch (chosenPatternFill){
	case 1: patternFill = pattern0;
		patternSizeFill = 4;
		break;
	case 2: patternFill = pattern1;
		patternSizeFill = 8;
		SetGraphicsColor(1, 1);
		return (patternFill[((int)sqrt(double(x*100)))%patternSizeFill]); 
	case 3: patternFill = pattern2;
		patternSizeFill = 13;
		SetGraphicsColor(2, 1);
		return (patternFill[((int)sqrt(double(x*100)))%patternSizeFill]); 
	case 4: patternFill = pattern3;
		patternSizeFill = 8;
		SetGraphicsColor(3, 1);
		return (patternFill[((int)sqrt(double(x*100)))%patternSizeFill]);
	case 5: patternFill = pattern1;
		patternSizeFill = 8;
		SetGraphicsColor(4, 1);
		return patternFill[x*y%patternSizeFill];
	case 6: patternFill = pattern2;
		patternSizeFill = 13;
		SetGraphicsColor(5, 1);
		return patternFill[x*y%patternSizeFill];
	case 7: patternFill = pattern3;
		patternSizeFill = 8;
		SetGraphicsColor(6, 1);
		return patternFill[x*y%patternSizeFill];
	case 8: patternFill = pattern1;
		patternSizeFill = 8;
		SetGraphicsColor(7, 1);
		return patternFill[x%patternSizeFill]&&patternFill[y%patternSizeFill];
	case 9: patternFill = pattern2;
		patternSizeFill = 13;
		SetGraphicsColor(8, 1);
		return patternFill[x%patternSizeFill]&&patternFill[y%patternSizeFill];
	case 11: patternFill = pattern1;
		patternSizeFill = 8;
		SetGraphicsColor(9, 1);
		return patternFill[x%patternSizeFill]||patternFill[y%patternSizeFill];
	case 12: patternFill = pattern2;
		patternSizeFill = 13;
		SetGraphicsColor(10, 1);
		return patternFill[x%patternSizeFill]||patternFill[y%patternSizeFill];
	case 14: patternFill = pattern1;
		patternSizeFill = 8;
		SetGraphicsColor(11, 1);
		return patternFill[x%patternSizeFill]^patternFill[y%patternSizeFill];
	case 15: patternFill = pattern2;
		patternSizeFill = 13;
		SetGraphicsColor(12, 1);
		return patternFill[x%patternSizeFill]^patternFill[y%patternSizeFill];
	case 17: patternFill = pattern1;
		patternSizeFill = 8;
		SetGraphicsColor(13, 1);
		return patternFill[((int)(100*sin((double)x*y)))%patternSizeFill];
	case 18: patternFill = pattern2;
		patternSizeFill = 13;
		SetGraphicsColor(14, 1);
		return patternFill[((int)(100*sin((double)x*y)))%patternSizeFill];
	case 19: patternFill = pattern3;
		patternSizeFill = 8;
		SetGraphicsColor(15, 1);
		return patternFill[((int)(100*sin((double)x*y)))%patternSizeFill];
	case 20: patternFill = pattern1;
		patternSizeFill = 8;
		SetGraphicsColor(2, 1);
		return patternFill[((int)(100*sin((double)x)))%patternSizeFill]&&patternFill[((int)(100*sin((double)y)))%patternSizeFill];
	case 21: patternFill = pattern2;
		patternSizeFill = 13;
		SetGraphicsColor(3, 1);
		return patternFill[((int)(100*sin((double)x)))%patternSizeFill]&&patternFill[((int)(100*sin((double)y)))%patternSizeFill];
	case 22: patternFill = pattern3;
		patternSizeFill = 8;
		SetGraphicsColor(4, 1);
		return patternFill[((int)(100*sin((double)x)))%patternSizeFill]&&patternFill[((int)(100*sin((double)y)))%patternSizeFill];
	case 23: patternFill = pattern1;
		patternSizeFill = 8;
		SetGraphicsColor(5, 1);
		return patternFill[((int)(100*sin((double)x)))%patternSizeFill]^patternFill[((int)(100*sin((double)y)))%patternSizeFill];
	case 24: patternFill = pattern2;
		patternSizeFill = 13;
		SetGraphicsColor(6, 1);
		return patternFill[((int)(100*sin((double)x)))%patternSizeFill]^patternFill[((int)(100*sin((double)y)))%patternSizeFill];
	case 25: patternFill = pattern3;
		patternSizeFill = 8;
		SetGraphicsColor(7, 1);
		return patternFill[((int)(100*sin((double)x)))%patternSizeFill]^patternFill[((int)(100*sin((double)y)))%patternSizeFill];
	case 26: patternFill = pattern1;
		patternSizeFill = 8;
		SetGraphicsColor(8, 1);
		return patternFill[((int)(100*sin((double)x)))%patternSizeFill]^patternFill[((int)(10000*sin((double)y)))%patternSizeFill];
	case 27: patternFill = pattern2;
		patternSizeFill = 13;
		SetGraphicsColor(9, 1);
		return patternFill[((int)(100*sin((double)x)))%patternSizeFill]^patternFill[((int)(10000*sin((double)y)))%patternSizeFill];
	case 28: patternFill = pattern3;
		patternSizeFill = 8;
		SetGraphicsColor(10, 1);
		return patternFill[((int)(100*sin((double)x)))%patternSizeFill]^patternFill[((int)(10000*sin((double)y)))%patternSizeFill];
	}
}

void FillIn ( int x1 , int x2 , int y ) {
	int px,py;
	py = y  % 8 ;
	if (x1!=x2) {
		for (int x = x1 ; x <= x2 ; x++ ) {
			px=x%8;
			if(shouldBeDrawn(x,y)) {
				DrawPixel(x,y);
			}
		}
	} 
}



void FillScan (edge_list_type &list, int end_Edge , int start_Edge , int scan ) {
	int NX , J , K;
	NX = (end_Edge - start_Edge + 1)/2;
	J = start_Edge;
	for ( K = 1 ; K <= NX ; K++ ) {
		FillIn ((int)list.edge[J].Xinter, (int)list.edge[J+1].Xinter, scan);
		J += 2;
	}
}

void UpdateXValues(edge_list_type &list, int last_Edge , int &start_Edge , int scan) {
	int K1;
	for ( K1 = start_Edge ; K1 <= last_Edge ; K1++) {
		if ( list.edge[K1].Ymin < scan ) {
			list.edge[K1].Xinter += list.edge[K1].dx ;
		} else {  // remove edge
			start_Edge++;
			if ( start_Edge <= K1 ) 
				for ( int i = K1 ; i >= start_Edge ; i--)
					list.edge[i]=list.edge[i-1]; 
		}
	}
}
	
void FillPolygon (polygon_type &polygon, edge_list_type &list ) {
	int Edges , start_Edge , end_Edge , scan;
	LoadPolygon(polygon,list, Edges);
	if (Edges==2) return;
	scan = list.edge[1].Ymax ;
	start_Edge = 1 ;
	end_Edge = 1 ;
	Include(list, end_Edge, Edges, scan);
	while ( end_Edge != start_Edge - 1 ) {
		XSort(list, start_Edge, end_Edge);
		FillScan(list, end_Edge, start_Edge, scan);
		scan--;
		UpdateXValues(list, end_Edge, start_Edge, scan);
		Include(list, end_Edge, Edges, scan);
	}
}

bool Empty(int x,int y) {  
	return (GetPixel(x,y)==0);
}

void FloodFillRec(int x,int y) {
	if (Empty(x,y)) {
		if (patternFill[x%patternSizeFill]){
				DrawPixel(x,y);
		}
		if (Empty(x+1,y)) 	FloodFillRec(x+1,y);
		if (Empty(x-1,y))	FloodFillRec(x-1,y);
		if (Empty(x,y+1))	FloodFillRec(x,y+1);
		if (Empty(x,y-1))	FloodFillRec(x,y-1);
	}
}

void FloodFillRecursive(polygon_type poly) {  
    int x_seed=0,y_seed=0;
	for (int i=0; i<poly.n; i++) {
		x_seed+=poly.vertex[i].x;
		y_seed+=poly.vertex[i].y;
	}
	x_seed/=poly.n;
	y_seed/=poly.n;
    FloodFillRec(x_seed,y_seed);
}

void DrawPoly(polygon_type poly){
	int pattern[] = { 1, 1, 1, 1 };
	DrawLine(poly.vertex[poly.n-1].x,poly.vertex[poly.n-1].y,poly.vertex[0].x,poly.vertex[0].y,pattern,4);
	
}

void Circle(int x, int y) {
	FloodFillRec(x,y);
}


/*Function that create the menu used in the application*/
void menuBar(){
	menu = CreateMenu();
	menu_draw = CreatePopupMenu();
	menu_pattern = CreatePopupMenu();
	menu_actions = CreatePopupMenu();
	/*Menu para para vários tipos de ação, como por exemplo, selecionar uma aresta*/
	AppendMenu(menu, MF_POPUP, (UINT)menu_actions, (LPCTSTR)L"&Actions");
	InsertMenu(menu_actions, 0, MF_STRING, 51, (LPCTSTR)L"&Select and Unselect polygon by a edge");
	InsertMenu(menu_actions, 0, MF_STRING, 52, (LPCTSTR)L"&Erase selected polygon");
	// Creating the sub menus
	// Menu Draw: responsável pela escolha do desenho
	/*The XorLine, DDA Line, Bresenham Line desenham uma linha, a diferença é só o algoritmo implementado*/
	AppendMenu(menu, MF_POPUP, (UINT)menu_draw, (LPCTSTR)L"&Fill");
	InsertMenu(menu_draw, 0, MF_STRING, 41, (LPCTSTR)L"&Fill Polygon");
	AppendMenu(menu_draw, MF_STRING, 42, (LPCTSTR)L"&Fill Polygon Recursive");
	AppendMenu(menu_draw, MF_STRING, 43, (LPCTSTR)L"&Circle");
	/* Menu Pattern: responsável pela escolha do padrão
	- Sem padrão
	- Tracejado
	- Tracejado Pontilhado
	- Pontilhado*/
	AppendMenu(menu, MF_POPUP, (UINT)menu_pattern, (LPCTSTR)L"&Pattern");
	InsertMenu(menu_pattern, 0, MF_STRING, 1, (LPCTSTR)L"Sem Padrao");
	AppendMenu(menu_pattern, MF_STRING, 2, (LPCTSTR)L"Linhas Verticais Crescimento Exponencial");
	AppendMenu(menu_pattern, MF_STRING, 3, (LPCTSTR)L"Linhas Verticais  Alternadas Crescimento Exponencial");
	AppendMenu(menu_pattern, MF_STRING, 4, (LPCTSTR)L"Linhas Verticais  Pequeno Espaçamento Crescimento Exponencial");
	AppendMenu(menu_pattern, MF_STRING, 5, (LPCTSTR)L"Multiplicativo Pequeno");
	AppendMenu(menu_pattern, MF_STRING, 6, (LPCTSTR)L"Multiplicativo Grande");
	AppendMenu(menu_pattern, MF_STRING, 7, (LPCTSTR)L"Pontilhado");
	AppendMenu(menu_pattern, MF_STRING, 8, (LPCTSTR)L"Quadrados Fundo Preto");
	AppendMenu(menu_pattern, MF_STRING, 9, (LPCTSTR)L"Quadrados e Tracejados Fundo Preto");
	AppendMenu(menu_pattern, MF_STRING, 11, (LPCTSTR)L"Quadrados Fundo Azul");
	AppendMenu(menu_pattern, MF_STRING, 12, (LPCTSTR)L"Janelas Fundo Verde");
	AppendMenu(menu_pattern, MF_STRING, 14, (LPCTSTR)L"Xadrez");
	AppendMenu(menu_pattern, MF_STRING, 15, (LPCTSTR)L"Mosaico");
	AppendMenu(menu_pattern, MF_STRING, 17, (LPCTSTR)L"Ruído Rosa Forte");
	AppendMenu(menu_pattern, MF_STRING, 18, (LPCTSTR)L"Ruído Amarelo Moderado");
	AppendMenu(menu_pattern, MF_STRING, 19, (LPCTSTR)L"Ruído Branco Forte");
	AppendMenu(menu_pattern, MF_STRING, 20, (LPCTSTR)L"Malha Senoidal Linha Grossa");
	AppendMenu(menu_pattern, MF_STRING, 21, (LPCTSTR)L"Malha Senoidal");
	AppendMenu(menu_pattern, MF_STRING, 22, (LPCTSTR)L"Malha Senoidal Linha Fina");
	AppendMenu(menu_pattern, MF_STRING, 23, (LPCTSTR)L"Pseudo Fractal I");
	AppendMenu(menu_pattern, MF_STRING, 24, (LPCTSTR)L"Onda Superfície");
	AppendMenu(menu_pattern, MF_STRING, 25, (LPCTSTR)L"Pseudo Fractal II");
	AppendMenu(menu_pattern, MF_STRING, 26, (LPCTSTR)L"Psicodélico I");
	AppendMenu(menu_pattern, MF_STRING, 27, (LPCTSTR)L"Psicodélico II");
	AppendMenu(menu_pattern, MF_STRING, 28, (LPCTSTR)L"Psicodélico III");
	/*Fornece a opção de limpar a tela*/
	InsertMenu(menu, 0, MF_STRING, 39, (LPCTSTR)L"&Erase Draw");
}


/*Declaração de algumas classes */





class Point{
public:
	int x, y;

public:
	void setPoint(int x, int y){
		this->x = x;
		this->y = y;
	}

	int getX(){
		return x;
	}
	int getY(){
		return y;
	}

};


class Edge{
public:
	Point point1, point2;
public:
	void getPoints(Point* p1, Point* p2){
		p1 = &point1;
		p2 = &point2;
	}

	void setFirstPoint(Point initialPoint){
		point1 = initialPoint;
	}
	void setSecondPoint(Point finalPoint){
		point2 = finalPoint;
	}

	Point getFirstPoint(){
		return point1;
	}

	Point getSecondPoint(){
		return point2;
	}
};

struct Vertex{
	Point point;
	Vertex* prox;
};

struct EdgeNode{
	Edge edge;
	EdgeNode* prox;
};



/*Classe responsável por guardar cada polígono - POR ENQUANTO SÓ TÁ GUARDANDO POLÍGONO NÃO ARESTA E CIRCUNFERENCIA*/
class Figure {
public:
	Vertex* start;
	EdgeNode* edgeStart;
	bool isSelected = false;

	bool selected(){
		return isSelected;
	}

	Figure(){
		start = NULL;
		edgeStart = NULL;
	}

	

	void addVertex(int x, int y){
		Point point;
		point.setPoint(x, y);
		Vertex* p;
		p = (Vertex*)malloc(sizeof(Vertex));
		p->point = point;
		p->prox = start;
		start = p;
	}
	void printVertex(){
		Vertex* p = start;
		int count = 0;
		while (p != NULL){
			printf(" point: %d %d", (p->point).getX(), (p->point).getY());
			count++;
			p = p->prox;
		}
		printf(" Numero de pontos: %d ", count);
	}

	void finishPolygon(){
		addEdges();

	}

	void addEdges(){
		Vertex* p = start;
		Edge* previousEdge = NULL;
		Edge* nextEdge = NULL;
		int count = 0;
		while (p != NULL){
			if (count % 2 == 0){
				if(p->prox != NULL){
					previousEdge = new Edge();
					(*previousEdge).setFirstPoint(p->point);
				}
				if (nextEdge != NULL){
					(*nextEdge).setSecondPoint(p->point);
					addNode(nextEdge);
				}
			}
			else{
				if (p->prox != NULL){
					nextEdge = new Edge();
					(*nextEdge).setFirstPoint(p->point);
				}
				(*previousEdge).setSecondPoint(p->point);
				addNode(previousEdge);
			}
			p = p->prox;
			count++;
		}
	}


	void addNode(Edge* newEdge){
		EdgeNode* edge = (EdgeNode*)malloc(sizeof(EdgeNode));
		edge->edge = (*newEdge);
		edge->prox = edgeStart;
		edgeStart = edge;
	}
	void printEdges()  {
		EdgeNode* aux = edgeStart;
		while (aux != NULL){
			Edge edge = aux->edge;
			Point point1 = edge.getFirstPoint();
			Point point2 = edge.getSecondPoint();
			printf("Edge: %d  %d , %d %d\n", point1.getX(), point1.getY(), point2.getX(), point2.getY());
			aux = aux->prox;
		}
	}
	/*função que indica se alguma aresta desse polígono está próximo do ponto x e y */
	bool isNear(int x, int y){
		EdgeNode* aux = edgeStart;
		while (aux != NULL){
			Edge edge = aux->edge;
			Point p1 = edge.getFirstPoint();
			Point p2 = edge.getSecondPoint();
			double p1x = p1.getX();
			double p1y = p1.getY();
			double p2x = p2.getX();
			double p2y = p2.getY();

			float d = 5;
			float dist2, xmin, ymin, xmax, ymax;

			xmin = Min(p1x, p2x); ymin = Min(p1y, p2y); xmax = Max(p1x, p2x); ymax = Max(p1y, p2y);
			dist2 = pow((x - p1x)*(p2y - p1y) - (y - p1y)*(p2x - p1x), 2) / (pow(p2x - p1x,2) + pow(p2y - p1y,2));

			printf(" distancia: %f", dist2);

			if ((dist2 <= d*d) && (xmin - d <= x) && (x <= xmax + d) && (ymin - d <= y) && (y <= ymax + d)){
				return true;
			}
			aux = aux->prox;
		}
		return false;
	}

	void pick(){
		redrawFigure(false);
	}


	void redrawFigure(bool eraseFigure){
		int* erasePattern;
		int  eraseLength;
		int* newSelectPattern;
		int  newLength;
		if (!isSelected){
			erasePattern = pattern0;
			eraseLength = 4;
			newSelectPattern = pattern3;
			newLength = 8;
			isSelected = true;
		}
		else{
			isSelected = false;
			erasePattern = pattern3;
			eraseLength = 8;
			newSelectPattern = pattern0;
			newLength = 4;
		}
		EdgeNode* aux = edgeStart;
		while (aux != NULL){
			Edge edge = aux->edge;
			Point point1 = edge.getFirstPoint();
			Point point2 = edge.getSecondPoint();
			DrawLine(point1.getX(), point1.getY(), point2.getX(), point2.getY(), erasePattern, eraseLength);
			if (!eraseFigure){
				DrawLine(point1.getX(), point1.getY(), point2.getX(), point2.getY(), newSelectPattern, newLength); /*desenhando a nova linha*/
			}
			else{
				/*ESSA FUNÇÃO ERASE LINE NÃO TÁ DANDO CERTO, NÃO SEI PORQUE - A IDÉIA QUE EU TIVE ERA SETAR A COR PRA PRETO
				E DESENHAR A LINHA NÃO SEI POR QUE NÃO TÁ DANDO CERTO*/
				EraseLine(point1.getX(), point1.getY(), point2.getX(), point2.getY());
			}
			aux = aux->prox;
		}
	}

	void eraseFigure(){
		redrawFigure(true);
	}


};

struct FigureNode{
	Figure* figure;
	FigureNode* prox;
};

/*Classe responsável por guardar as figuras*/
class ScreenFigures{
public:
	FigureNode* start;
	ScreenFigures(){
		start = NULL;
	}
	void addFigure(Figure* figure){
		FigureNode* node;
		node = (FigureNode*)malloc(sizeof(FigureNode));
		node->figure = figure;
		node->prox = start;
		start = node;
	}

	int getFiguresNumber(){
		int counter = 0;
		FigureNode* aux = start;
		while (aux != NULL){
			counter++;
			aux = aux->prox;
		}
		return counter;
	}

	void printEdges(){
		FigureNode* aux = start;
		while (aux != NULL){
			printf("poligono:\n");
			(*(aux->figure)).printEdges();
			aux = aux->prox;
		}
	}

	void pickFigure(int mouseX, int mouseY){
		FigureNode* aux = start;
		while (aux != NULL){
			if ((*aux->figure).isNear(mouseX, mouseY)){
				(*aux->figure).pick();
			}
			else{
				printf("false");
			}
			aux = aux->prox;
		}
	}

	void eraseSelectedPolygonals(){
		FigureNode* aux = start;
		FigureNode* previous = start;
		while (aux != NULL){
			if ((*aux->figure).selected()){
				printf("chamou a função");
				(*aux->figure).eraseFigure();
			}
			/*
			FALTA TIRAR O POLIÍGONO DA FILA 
			*/
			previous = aux;
			aux = aux->prox;
		}
	}

};




/*Função principal: implementa o efeito elástico, tanto nas linhas, quanto na circunferência*/
void main()
{
	SetGraphicsColor(15, 1);
	//int p0_x, p0_y, p1_x, p1_y, cor, color = MY_MAGENTA;

	int pattern[] = { 1, 1, 1, 1 };
	polygon_type polygon;
	polygon.n = 0;
	edge_list_type list;
	int p0_x, p0_y, p1_x,p1_y, x_1,y_1, x_2,y_2,xc,yc; 
	bool isAction = false;
	bool pressed = false;

	int chosenAlgorithm = 1;
	InitGraphics();

	ScreenFigures screenFigures;
	Figure* savepoly;

	while (key_input != ESC) {	// ESC exits the program
		
		CheckGraphicsMsg();
		if (hasEvent){
			isAction = false;
			if (menu_item == 52){
				screenFigures.eraseSelectedPolygonals();
			}
			else if (menu_item >= 50){
				isAction = true;
			}
			else if (menu_item >= 40){
				chosenAlgorithm = menu_item - 40;

			}
			else if (menu_item < 39){
					chosenPatternFill = menu_item;
				}
			else{
				ClearGraphicsScreen();
		    }
			hasEvent = 0;
		}
		patternFill = pattern0;
		patternSizeFill = 4;
		
		if (!isAction){
			if (mouse_action == L_MOUSE_DOWN) {  // Pick first point up
				if (polygon.n == 0) {
					p0_x = p1_x = mouse_x;
					p0_y = p1_y = mouse_y;
					if (chosenAlgorithm != 3) {
						savepoly = new Figure();
						screenFigures.addFigure(savepoly);
						InsertVertex(polygon, p0_x, p0_y);
						(*savepoly).addVertex(p0_x, p0_y);
					}
					else {
						xc = mouse_x;
						yc = mouse_y;
					}
				}
			}
			if (mouse_action == L_MOUSE_MOVE_DOWN) {  // Example of elastic line
				if (chosenAlgorithm != 3) {
					if (p1_x != mouse_x || p1_y != mouse_y) {  // Erase previous line. NOTE: using XOR line
						BresenhamLine(p0_x, p0_y, p1_x, p1_y, pattern, 4);
						p1_x = mouse_x;
						p1_y = mouse_y;  // Draw new line
						BresenhamLine(p0_x, p0_y, p1_x, p1_y, pattern, 4);
						x_1 = p0_x;
						y_1 = p0_y;
						x_2 = p1_x;
						y_2 = p1_y;
					}
				}
				else {
					if (p1_x != mouse_x || p1_y != mouse_y)  	// test if x or y changed
					{
						// Erase previous line.
						chooseDraw(4, 1, p0_x, p0_y, p1_x, p1_y);
						p1_x = mouse_x; p1_y = mouse_y;
						chooseDraw(4, 1, p0_x, p0_y, p1_x, p1_y);
						//mouse_action=NO_ACTION;
					}
				}
			}
			else if (mouse_action == L_MOUSE_UP) {//  SetGraphicsColor((int)MY_LIGHTGREEN);
				//DrawLine(p0_x,p0_y,p1_x,p1_y,pattern,4);
				p0_x = p1_x = mouse_x;
				p0_y = p1_y = mouse_y;
				if (chosenAlgorithm != 3) {
					if (polygon.n > 0 &&
						(polygon.vertex[polygon.n - 1].x != p0_x
						|| polygon.vertex[polygon.n - 1].y != p0_y)){

						InsertVertex(polygon, p0_x, p0_y);
						(*savepoly).addVertex(p0_x, p0_y);
					}

				}
				else {
					double dx = xc - p1_x;
					double dy = yc - p1_y;
					double radio = sqrt(pow(dx, 2) + pow(dy, 2));
					double radioproj = radio*sqrt(2.0) / 2;
					DrawPixel(xc + Arred((int)radio), yc);
					DrawPixel(xc - Arred((int)radio), yc);
					DrawPixel(xc, yc + Arred((int)radio));
					DrawPixel(xc, yc - Arred((int)radio));
					DrawPixel(xc + Arred((int)radioproj), yc + Arred((int)radioproj));
					DrawPixel(xc + Arred((int)radioproj), yc - Arred((int)radioproj));
					DrawPixel(xc - Arred((int)radioproj), yc + Arred((int)radioproj));
					DrawPixel(xc - Arred((int)radioproj), yc - Arred((int)radioproj));
				}

				mouse_action = NO_ACTION;
			}
			else if (mouse_action == R_MOUSE_DOWN) {
				if (chosenAlgorithm != 3) {
					DrawPoly(polygon);
					InsertVertex(polygon, polygon.vertex[0].x, polygon.vertex[0].y);
					(*savepoly).addVertex(polygon.vertex[0].x, polygon.vertex[0].y);
					(*savepoly).finishPolygon();
					(*savepoly).printVertex();
					(*savepoly).printEdges();
					
					/*if (chosenAlgorithm == 1)   FillPolygon(polygon, list);
					else if (chosenAlgorithm == 2)	FloodFillRecursive(polygon); */
				}
				else Circle(mouse_x, mouse_y);
				mouse_action = NO_ACTION;
				polygon.n = 0;

			}
		}
		else{
			if (menu_item == 51){
				if (mouse_action == L_MOUSE_DOWN) {  // Pick first point up
					pressed = true;
				}
				if (mouse_action == L_MOUSE_UP && pressed){
					pressed = false;
					printf("numero de figuras: %d\n", screenFigures.getFiguresNumber());
					/*screenFigures.printEdges();*/
					p0_x = mouse_x;
					p0_y = mouse_y;
					printf("p0x: %d , p0y: %d\n", p0_x, p0_y);
					screenFigures.pickFigure(p0_x , p0_y);
				}
			}
		}

		
	}
	CloseGraphics();

}


