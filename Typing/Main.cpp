//------------------Main.cpp
#define READ //二重インクルード防止
#include<windows.h>
#include<tchar.h>
#include<stdlib.h>
#include<time.h>
#include"Define.h"
#include"Header.h"

//グローバル変数
TCHAR szClassName[] =_T("Default Class Name"); //ウィンドウクラス

//プロトタイプ宣言
ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst,int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp);

//開始
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpsCmdLine,int nCmdShow)
{
	MSG msg;
	BOOL bRet;

	srand((unsigned)time(NULL)); //乱数セット

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
		_T("Sample Program"), //タイトルバーに表示する文字列
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
	HDC hdc;
	HBRUSH hBrush,hBrush2,hBrush3;
	PAINTSTRUCT ps;
	static LOGFONT logfont,logfont2,logfont3,logfont4;
	static HFONT hFont,hFont2,hFont3,hFont4;
	static int rest_time,combo,speed,fall_time;
	static BOOL stop,count_down,flag_s,disp_toku,restart;
	RECT rc;
	int iti;
	char mozi;
	static DWORD time1,time2;

	switch(msg)
	{
	case WM_CREATE:
		Init(); //グローバル変数初期化

		//フォントの作成
		ZeroMemory(&logfont,sizeof(logfont));
		ZeroMemory(&logfont2,sizeof(logfont2));
		ZeroMemory(&logfont3,sizeof(logfont3));
		ZeroMemory(&logfont4,sizeof(logfont4));
		logfont.lfHeight = TYPE_FONT;
		logfont.lfWidth = TYPE_FONT-10;
		logfont.lfCharSet = DEFAULT_CHARSET;
		logfont2.lfHeight = INF_FONT+4;
		logfont2.lfWidth = INF_FONT;
		logfont2.lfCharSet = DEFAULT_CHARSET;
		logfont3.lfHeight = 30;
		logfont3.lfWidth = 24;
		logfont3.lfCharSet = DEFAULT_CHARSET;
		logfont4.lfHeight = 20;
		logfont4.lfWidth = 14;
		logfont4.lfCharSet = DEFAULT_CHARSET;
		
		//落下速度
		fall_time = 6;
		Change_FallType(&speed,fall_time);


		rest_time = 180000; //3分固定

		combo = 0; //コンボ数

		stop = TRUE; //停止

		count_down = FALSE;
		flag_s = TRUE;
		disp_toku = FALSE;
		restart = FALSE;
		
		break;
	case WM_KEYDOWN:
		if((GetKeyState(VK_CONTROL) & 0x80) && (wp ==0x51)) //終了
			DestroyWindow(hWnd);

		if((GetKeyState(VK_CONTROL) & 0x80) && (wp== 0x53) && flag_s) { //開始
			Start();
			SetTimer(hWnd,TIMER_ID,CYCLE,NULL);
			time1 = timeGetTime(); //開始時刻
			flag_s = FALSE; //もう一度押せないようにする
			count_down = TRUE; //カウントダウン
		}
			
		break;
	case WM_LBUTTONDOWN:

		if(restart == TRUE && LOWORD(lp)>=400 && LOWORD(lp)<=600 && HIWORD(lp)>=400 && HIWORD(lp)<=500) {
			KillTimer(hWnd,TIMER_ID); //タイマを切る
			SendMessage(hWnd,WM_CREATE,wp,lp); //ウィンドウが作られた状態へ(つまり、初期化)
			InvalidateRect(hWnd,NULL,TRUE);
		}
		break;
	case WM_CHAR:

		if(stop) { //開始していないとき
			//何もしない

		} else { //開始

			if(Check_Type(toupper(wp),combo)) {
				combo++;
			} else {
				combo = 0;
			}
		}
		InvalidateRect(hWnd,NULL,TRUE); //再描画
		break;
	case WM_TIMER:
		if(wp != TIMER_ID)
			break;

		//カウント中
		if(count_down) {
			time2 = timeGetTime();
			Count_Down(time2-time1);
			if(time2-time1 >= 3000) {
				count_down = FALSE;
				LetsStart(); //カウント終了
				time1 = timeGetTime();
				stop = FALSE;
			}
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		}

		if(rest_time <= 0) { //終わりの時
			disp_toku = TRUE;
			stop = TRUE;
			restart = TRUE;
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		}

		rest_time -= CYCLE; //残り時間を引く

		if(rand()%2){
			iti = rand()%TYPE_MAX; //位置の設定
			mozi = 'A'+rand()%ALPHABET; //文字
			if(Generate_Type(&iti,mozi)) { //生成できたとき
				Set_Iti(iti%TYPE_MAX); //文字位置をセット
			}
		}
		FallType(speed); //文字を落とす

		time2 = timeGetTime();
		if(time2-time1 >= 45000) { //45秒ごとに加速
			fall_time--;
			Change_FallType(&speed,fall_time);
			
			time1 = timeGetTime();
		}


		InvalidateRect(hWnd,NULL,TRUE); //再描画
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);

		SetBkMode(hdc,TRANSPARENT);

		//色々表示
		hFont3 = CreateFontIndirect(&logfont3);
		Disp_Explain(hdc);
		SelectObject(hdc,hFont3);
		Disp_Count(hdc);

		//情報の表示部は色を変更
		hBrush = CreateSolidBrush(RGB(0x7f,0xff,0xd4));
		SelectObject(hdc,hBrush);
		Rectangle(hdc,600,0,800,WINDOW_HEIGHT);

		//情報の表示
		hFont2 = CreateFontIndirect(&logfont2);
		SelectObject(hdc,hFont2);
		DispScore(hdc,combo,rest_time);
		
		//タイプ領域の表示
		hBrush2 = CreateSolidBrush(RGB(0xe0,0xff,0xff));
		SelectObject(hdc,hBrush2);
		Rectangle(hdc,TYPE_OK_LEFT,TYPE_OK_TOP,TYPE_OK_RIGHT-60,TYPE_OK_BOTTOM);
		
		//タイプ文字の表示
		hFont = CreateFontIndirect(&logfont);
		SelectObject(hdc,hFont);
		DispType(hdc);
		if(disp_toku) {
		//得点などの表示
			hFont4 = CreateFontIndirect(&logfont4);
			SelectObject(hdc,hFont4);
			SetTextColor(hdc,RGB(120,18,200));
			End(hdc);
			DeleteObject(hFont4);

			//再スタートの表示
			hBrush3 = CreateSolidBrush(RGB(0,200,200));
			SelectObject(hdc,hBrush3);
			Rectangle(hdc,400,400,600,500);
			DeleteObject(hBrush3);
			rc.top = 430;
			rc.bottom = 500;
			rc.left = 400;
			rc.right = 600;
			SetTextColor(hdc,RGB(0,0,0));
			DrawText(hdc,"再スタート",-1,&rc,DT_CENTER);
		}
		
		//破棄
		DeleteObject(hFont);
		DeleteObject(hBrush2);
		DeleteObject(hFont2);
		DeleteObject(hFont3);
		DeleteObject(hBrush);

		EndPaint(hWnd,&ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd,msg,wp,lp);
}
