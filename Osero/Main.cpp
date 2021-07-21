#include<windows.h>
#include<tchar.h>
#include<future>
#include"Header.h"
#include"Define.h"

//グローバル変数
TCHAR szClassName[] =_T("Default Class Name"); //ウィンドウクラス
extern bool Sharp_Board[BOARD_SIZE_Y+1][BOARD_SIZE_X+1]; //盤面の形を保持(行、列)
extern int Board[BOARD_SIZE_Y+1][BOARD_SIZE_X+1]; //実際の盤面
extern int G_Disp_Board[BOARD_SIZE_Y+1][BOARD_SIZE_X+1]; 
extern bool Flash_Board[BOARD_SIZE_Y+1][BOARD_SIZE_X+1]; //光らせる場所
HINSTANCE G_hInst;
char Moziretu[30] = "選択してください";
BOOL G_Flag = FALSE;

//プロトタイプ宣言
ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst,int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp);

//開始
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpsCmdLine,int nCmdShow)
{
	MSG msg;
	BOOL bRet;
	
	G_hInst = hInst;

	if(!InitApp(hInst))
		return FALSE;
	if(!InitInstance(hInst,nCmdShow))
		return FALSE;

	//メッセージを取得
	while((bRet=GetMessage(&msg,NULL,0,0)) != 0) {
		if(bRet == -1) {
			break;
		}
		else {
			//メッセージを処理する
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//ウィンドウクラスの登録
ATOM InitApp(HINSTANCE hInst) 
{
	WNDCLASSEX wc;

	wc.cbSize=sizeof(WNDCLASSEX); //構造体サイズ
	wc.style=CS_HREDRAW | CS_VREDRAW; //クラスのスタイル
	wc.lpfnWndProc=WndProc; //ウィンドウプロシージャ
	wc.cbClsExtra=0; //補助メモリ
	wc.cbWndExtra=0; //補助メモリ
	wc.hInstance=hInst; //インスタンスハンドル
	wc.hIcon=(HICON)LoadImage( //アイコン
		NULL,MAKEINTRESOURCE(IDI_APPLICATION),IMAGE_ICON,
		0,0,LR_DEFAULTSIZE | LR_SHARED
		); 
	wc.hIconSm=wc.hIcon; //子アイコン
	wc.hCursor=(HCURSOR)LoadImage( //マウスカーソル
		NULL,MAKEINTRESOURCE(IDC_ARROW),IMAGE_CURSOR,
		0,0,LR_DEFAULTSIZE | LR_SHARED
		);
	wc.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH); //ウィンドウ背景
	wc.lpszMenuName=NULL;
	wc.lpszClassName=szClassName; //クラス名

	return (RegisterClassEx(&wc));
}

//ウィンドウの生成
BOOL InitInstance(HINSTANCE hInst,int nCmdShow)
{
	HWND hWnd;

	hWnd=CreateWindowEx(
		WS_EX_COMPOSITED, //ダブルバッファ
		szClassName, //ウィンドウクラス名
		_T("Osero(Hard)"), //タイトルバーに表示する文字列
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, //ウィンドウスタイル
		WINDOW_X, //ウィンドウを表示する位置(X座標)
		WINDOW_Y, //ウィンドウを表示する位置(Y座標)
		WINDOW_WIDTH, //ウィンドウの幅
		WINDOW_HEIGHT, //ウィンドウの高さ
		NULL, //親ウィンドウのハンドル
		NULL, //メニューハンドル
		hInst, //インスタンスハンドル
		NULL //その他の作成データ
		);

	if(!hWnd)
		return FALSE;

	//ウィンドウを表示する
	ShowWindow(hWnd,nCmdShow); //ウィンドウの表示状態
	UpdateWindow(hWnd); //ウィンドウを更新

	return TRUE;
}

//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
	int mouse_x,mouse_y;
	POINT pos,mouse;
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT char_rect;
	static bool play_off = true,fin = false;
	static bool play_ok = false;
	static System sys;
	static Player player;
	static CPU cpu;
	static HWND hButton1,hButton2,hButton3;
	static Disp disp;
	static Init init;
	static LOGFONT logfont;
	HFONT hFont;

	switch(msg)
	{
	case WM_CREATE:
		//初期化
		play_off = true;
		fin = false;
		play_ok = false;
		init.Initialize();
		hButton1 = CreateWindow(
			_T("BUTTON"),
			_T("先攻"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			600,100,
			100,50,
			hWnd,
			(HMENU)(INT_PTR)BUTTON_ID1,
			G_hInst,
			NULL
			);
		
		hButton2 = CreateWindow(
			_T("BUTTON"),
			_T("後攻"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			600,160,
			100,50,
			hWnd,
			(HMENU)(INT_PTR)BUTTON_ID2,
			G_hInst,
			NULL
			);

		hButton3 = CreateWindow(
			_T("BUTTON"),
			_T("もう一回"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			600,220,
			100,50,
			hWnd,
			(HMENU)(INT_PTR)BUTTON_ID3,
			G_hInst,
			NULL
			);

		char_rect.top = 350;
		char_rect.left = 500;
		char_rect.bottom = 450;
		char_rect.right = 800;

		ZeroMemory(&logfont,sizeof(logfont));
		logfont.lfCharSet = DEFAULT_CHARSET;
		logfont.lfHeight = 24;
		logfont.lfWidth = 12;

		break;
	case WM_COMMAND:
		if(fin || play_off) {
			switch(LOWORD(wp))
			{
			case BUTTON_ID1:
				player.Turn();
				InvalidateRect(hWnd,NULL,TRUE);
				play_off = false;
				play_ok = true;
				break;
			case BUTTON_ID2:
				cpu.run(hWnd,&play_ok,&fin);
				InvalidateRect(hWnd,NULL,TRUE);
				play_off = false;
				break;
			case BUTTON_ID3:
				SendMessage(hWnd,WM_CREATE,wp,lp);
				InvalidateRect(hWnd,NULL,TRUE);
				break;
			}
		}
		break;
	case WM_LBUTTONDOWN: //マウスカーソルの左ボタンが押されたとき
		pos.x = LOWORD(lp);
		pos.y = HIWORD(lp);
		if(!fin && play_ok) {
			mouse_x = pos.x-50;
			mouse_y = pos.y-30;
			if(mouse_x < 0 || mouse_y < 0) //盤面の外では何もしない
				break;
			mouse_x /= SEL_SIZE;
			mouse_y /= SEL_SIZE;
			if(mouse_x >= BOARD_SIZE_X || mouse_y >= BOARD_SIZE_Y)
				break;
			mouse.x = mouse_y;
			mouse.y = mouse_x;

			if(sys.Can_Put(mouse,YOU)) { //置く
				play_ok = false;
				sys.Turn_Over(mouse,YOU);

				for(int i = 0;i < BOARD_SIZE_Y+1;i++) {
					for(int j = 0;j < BOARD_SIZE_X+1;j++) {
						G_Disp_Board[i][j] = Board[i][j];
					}
				}

				//光っているやつを消す
				for(int i = 0;i < BOARD_SIZE_Y;i++) {
					for(int j = 0;j < BOARD_SIZE_X;j++) {
						Flash_Board[i][j] = false;;
					}
				}

				if(sys.Game_Over()) {
					fin = true;
					if(sys.Which_Win() == YOU)
						wsprintf(Moziretu,"You Win");
					else if(sys.Which_Win() == AI)
						wsprintf(Moziretu,"You Lose");
					else
						wsprintf(Moziretu,"Draw");

					InvalidateRect(hWnd,NULL,TRUE);

					break;
				}

				cpu.run(hWnd,&play_ok,&fin); //敵の番
			}
		}

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);
		SetBkMode(hdc,TRANSPARENT);
		SetTextColor(hdc,RGB(255,0,0));
		hFont = CreateFontIndirect(&logfont); //論理フォント作成
		SelectObject(hdc,hFont);
		DrawText(hdc,Moziretu,-1,&char_rect,DT_CENTER);
		disp.Show_Score(hdc);
		DeleteObject(hFont); //論理フォントの削除
		disp.Show_Board(hdc);

		EndPaint(hWnd,&ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd,msg,wp,lp);
}
