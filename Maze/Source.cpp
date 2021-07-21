//ウィンドウ部はPrograming Placeさんのページのものを使用しました
#include<windows.h>
#include<tchar.h>
#include<iostream>
#include<random>
#include<cmath>
#include<string>
#include<thread>

//定数
#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (645)
#define WINDOW_X ((GetSystemMetrics(SM_CXSCREEN)-WINDOW_WIDTH) / 2)
#define WINDOW_Y ((GetSystemMetrics(SM_CYSCREEN)-WINDOW_HEIGHT) / 2)
#define ROW (101) //迷路の縦幅
#define COL (101) //迷路の横幅
#define CELL_SIZE (6) //一マスの大きさ
#define SET_TIME (100) //100msec
#define BUTTON_ID1 (1)
#define BUTTON_ID2 (2)

//プロトタイプ
HWND Create(HINSTANCE hInst);
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp);
HWND CreateButton(HWND hWnd,int x,int y,int w,int h,LPCTSTR caption,int id);
void PrintMap(HWND hWnd); //マップの表示

using namespace std;

//グローバル変数
int** _map;
string _path;
HINSTANCE _Inst;

//迷路作成クラス
class Maze_Create {
private:
	static const int Line = ROW,Row = COL; //迷路の大きさ(行、列) 奇数にする
	void Break_Wall(int line,int row); //壁を壊す(行,列)
public:
	Maze_Create(); //コンストラクタ
	~Maze_Create(); //デストラクタ
	bool Create_map(); //迷路の作成(穴掘り法)
	bool S_G_Set(); //スタートとゴールのセット
};

//迷路の解法クラス
class Maze_Search {
private:
	int *vx,*vy; //方向ベクトル
	char *data_way,*un_way; //方向
	static const int Row = ROW,Col = COL;
	int Lim;
	int Init();
	int Heuristics(int,int); //ヒューリスティックス
	bool IDA(int,int,int,char);
	int s_row,s_col,g_row,g_col;
	string path;
public:
	Maze_Search(); //コンストラクタ
	~Maze_Search(); //デストラクタ
	void Search();
	int Get_S_Row();//ゲッター
	int Get_S_Col(); //ゲッター
};

//迷路のアニメーションクラス
class Maze_Animation {
private:
	static const int Time = SET_TIME;
public:
	void run(HWND,int,int);
	void Animation(HWND,int,int);
};

//Maze_Create
//コンストラクタ
Maze_Create :: Maze_Create() {
	//メモリの確保
	_map = new int*[Line];
	
	for(int i = 0;i < Line;i++) {
		_map[i] = new int[Row];
	}
};
//デストラクタ
Maze_Create :: ~Maze_Create() {
	for(int i = 0;i < Line;i++) {
		delete[] _map[i];
	}

	delete[] _map;
}

//壁壊し
void Maze_Create :: Break_Wall(int line,int row) 
{
	random_device rnd; //乱数
	int way1,way2,way3,way4;
	way1 = way2 = way3 = way4 = 0;

	while(true)
	{
		if(way1+way2+way3+way4 == 4)
			break;

		switch(rnd()%4+1) {
		case 1: //上
			if(line - 2 > 0 && _map[line-2][row] == 1) 
			{
				//通路にする
				_map[line-1][row] = 0;
				_map[line-2][row] = 0;
				//再帰呼び出し
				Break_Wall(line-2,row);
			} else {
				way1=1;
			}
			break;
		case 2: //左
			if(row - 2 > 0 && _map[line][row-2] == 1)
			{
				_map[line][row-1] = 0;
				_map[line][row-2] = 0;
				Break_Wall(line,row-2);
			} else {
				way2 = 1;
			}
			break;
		case 3: //下
			if(line+2 < Line-1 && _map[line+2][row] == 1) {
				_map[line+1][row] = 0;
				_map[line+2][row] = 0;
				Break_Wall(line+2,row);
			} else {
				way3 = 1;
			}
			break;
		case 4: //右
			if(row+2 < Row-1 && _map[line][row+2] == 1) {
				_map[line][row+1] = 0;
				_map[line][row+2] = 0;
				Break_Wall(line,row+2);
			} else {
				way4 = 1;
			}
			break;
		}
	}

	return;
}

//作成
bool Maze_Create :: Create_map() 
{
	int line,row;
	random_device rnd; //乱数

	
	//マップの初期化
	for(int i = 0;i < Line;i++) {
		for(int j = 0;j < Row;j++) {
			_map[i][j] = 1; //1:(壁) 0:(通路)
		}
	}
	//ランダムに壁を壊す場所を選択
	while(!((line = rnd() % Line)%2)) //行について
		;
	while(!((row = rnd() % Row)%2)) //列について
		;

	Break_Wall(line,row); //壁を壊す
	
	S_G_Set(); //スタートとゴールのセット

	cout<<"-----------------"<<endl;
	for(int i = 0;i < Line;i++) {
		for(int j = 0;j < Row;j++) {
			printf("%2d",_map[i][j]);
		}
		cout<<endl;
	}

	return true;
}
//スタートとゴールのセット
bool Maze_Create :: S_G_Set()
{
	int max = Line*Row;
	for(int i = 0;i < max;i++) {
		if(_map[i/Line][i%Row] == 0) {
			_map[i/Line][i%Row] = 2;
			break;
		}
	}

	for(int i = Line-1;i >= 0;i--) {
		for(int j = Row-1;j >= 0;j--) {
			if(_map[i][j] == 0) {
				_map[i][j] = 3;

				return true;
			}
		}
	}

	return false;
}

//Maze_Search
//コンストラクタ
Maze_Search :: Maze_Search()
{
	//方向ベクトルの定義
	vx = new int[4];
	vx[0] = 1;vx[1] = 0;vx[2] = -1;vx[3] = 0;
	vy = new int[4];
	vy[0] = 0;vy[1] = 1;vy[2] = 0;vy[3] = -1;

	//方向の定義
	data_way = new char[4];
	data_way[0]='d';data_way[1] = 'r';data_way[2] = 'u';data_way[3] = 'l';
	un_way = new char[4];
	un_way[0]='u';un_way[1]='l';un_way[2]='d';un_way[3]='r';

	Init();
}
//デストラクタ
Maze_Search :: ~Maze_Search()
{
	delete vx;
	delete vy;
	delete data_way;
}

//初期化
int Maze_Search :: Init()
{
	int max = Row*Col;
	for(int i = 0;i < max;i++)
		if(_map[i/Row][i%Col] == 2) {
			s_row = i/Row;
			s_col = i%Col;
		} else if(_map[i/Row][i%Col] == 3) {
			g_row = i/Row;
			g_col = i%Col;
		}

	path="";


	return Heuristics(s_row,s_col);
}

//ヒューリスティックス
int Maze_Search :: Heuristics(int nx,int ny)
{
	int a,b;
	a = abs(nx-g_row);
	b = abs(ny-g_col);

	return sqrt(a*a+b*b)+0.5; 
}

//探索
void Maze_Search :: Search()
{
	for(;!IDA(s_row,s_col,0,NULL);Lim++)  //反復深化
		;
	

	_path = path; //経路の保存
	
	return;
}

//IDA*
bool Maze_Search :: IDA(int nx,int ny,int n,char way)
{

	if(n+Heuristics(nx,ny) >= Lim) {
		return false;
	}
	if(nx < 0 || nx >= Row || ny < 0 || ny >= Col)
		return false;
	if(_map[nx][ny] == 1)
		return false;
	if(_map[nx][ny] == 3) { //ゴールだったら
		return true;
	}

	for(int i = 0;i < 4;i++) {
		if(way == un_way[i])
			continue;

		path += data_way[i];
		
		if(IDA(nx+vx[i],ny+vy[i],n+1,data_way[i]))
			return true;
		
		path.erase(--path.end());
	}

	return false;
}

//ゲッター
int Maze_Search::Get_S_Row() 
{
	return s_row;
}
int Maze_Search::Get_S_Col()
{
	return s_col;
}

//Maze_Animation
//並列処理
void Maze_Animation::run(HWND hWnd,int row,int col)
{
	thread th1(&Maze_Animation::Animation,this,hWnd,row,col);

	th1.detach();
}

void Maze_Animation::Animation(HWND hWnd,int row,int col)
{
	while(!_path.empty()) {
		switch(_path.front()) { //先頭の文字を取り出す
		case 'r':
			_map[row][++col] = 9;
			break;
		case 'u':
			_map[--row][col] = 9;
			break;
		case 'l':
			_map[row][--col] = 9;
			break;
		case 'd':
			_map[++row][col] = 9;
			break;
		default:
			break;
		}
		_path.erase(_path.begin()); //先頭の文字を削除
		InvalidateRect(hWnd,NULL,FALSE);
		Sleep(Time);
	}

	return;
}

//Main
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR pCmdLine,int showCmd)
{
	HWND hWnd;
	MSG msg;

	_Inst = hInst;
	//ウィンドウの作成
	hWnd = Create(hInst);
	if(hWnd == NULL) {
		MessageBox(NULL,_T("ウィンドウの作成に失敗しました"),_T("ERROR"),MB_OK);

		return 1;
	}
	//ウィンドウの表示
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);
	//メッセージループ
	while(1) {
		BOOL ret = GetMessage(&msg,NULL,0,0);
		if(ret == 0 || ret == -1)
			break;
		else {
			//メッセージ処理
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

//ウィンドウの作成
HWND Create(HINSTANCE hInst)
{
	WNDCLASSEX wc;

	//ウィンドウクラスの情報を設定
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = (HICON)LoadImage(
		NULL,MAKEINTRESOURCE(IDI_APPLICATION),IMAGE_ICON,
		0,0,LR_DEFAULTSIZE | LR_SHARED
		);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = (HCURSOR)LoadImage(
		NULL,MAKEINTRESOURCE(IDC_ARROW),IMAGE_CURSOR,
		0,0,LR_DEFAULTSIZE | LR_SHARED
		);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _T("Default Class Name");

	//ウィンドウクラスの登録
	if(RegisterClassEx(&wc) == 0)
		return 0;

	//ウィンドウの作成
	return CreateWindow(
		wc.lpszClassName,
		_T("～～～Maze～～～"),
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
		WINDOW_X,
		WINDOW_Y,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		NULL,
		NULL,
		hInst,
		NULL
		);
}

//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
	static HWND hButton1,hButton2;
	static Maze_Create mc;
	static bool flag = false;

	switch(msg)
	{
	case WM_CREATE:
		hButton1 = CreateButton(hWnd,650,100,100,50,_T("迷路作成"),BUTTON_ID1);
		InvalidateRect(hWnd,NULL,FALSE);

		return 0;
	case WM_COMMAND: //ボタンが押されたとき
		switch(LOWORD(wp))
		{
		case BUTTON_ID1:

			mc.Create_map(); //迷路の作成
			InvalidateRect(hWnd,NULL,FALSE);
			flag = true;

			//新たにボタンの生成
			hButton2 = CreateButton(hWnd,650,160,100,50,_T("迷路の解法"),BUTTON_ID2);

			break;
		case BUTTON_ID2:
			Maze_Search ms;
			Maze_Animation ma;

			ms.Search(); //迷路を解く
			if(MessageBox(hWnd,_T("手順が見つかりました\n再現します"),_T("Message"),MB_OK) == IDOK) {
				ma.run(hWnd,ms.Get_S_Row(),ms.Get_S_Col());
			}
			break;
		}

		return 0;
	case WM_PAINT:
		if(flag == true)
			PrintMap(hWnd);

		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;
	}

	return DefWindowProc(hWnd,msg,wp,lp);
}

//ボタンの作成
HWND CreateButton(HWND hWnd,int x,int y,int w,int h,LPCTSTR caption,int id)
{
	return CreateWindow(
		_T("BUTTON"),
		caption,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x,y,
		w,h,
		hWnd,
		(HMENU)(INT_PTR)id,
		_Inst,
		NULL
		);
}

//マップの表示
void PrintMap(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HBRUSH hBrush1,hBrush2,hBrush3,hBrush4,hBrush5;
	int wx = 0,wy = 0;
	int nx,ny;
	int px;

	hBrush1 = CreateSolidBrush(RGB(100,100,100)); //壁
	hBrush2 = CreateSolidBrush(RGB(200,200,200)); //通路
	hBrush3 = CreateSolidBrush(RGB(0,255,0)); //スタート
	hBrush4 = CreateSolidBrush(RGB(0,255,255)); //ゴール
	hBrush5 = CreateSolidBrush(RGB(255,0,0)); //ルート

	hDC = BeginPaint(hWnd,&ps);
	for(int i = 0;i < ROW;i++) {
		for(int j = 0;j < COL;j++) {
			px = _map[i][j];
			nx = wx+CELL_SIZE;
			ny = wy+CELL_SIZE;
			if(px == 1) { //壁の時
				SelectObject(hDC,hBrush1);
			}
			else if(px == 0) { //通路の時
				SelectObject(hDC,hBrush2);
			}
			else if(px == 2) { //スタートの時
				SelectObject(hDC,hBrush3);
			}
			else if(px == 3) { //ゴールの時
				SelectObject(hDC,hBrush4);
			}
			else if(px == 9) { //ルートの表示の時
				SelectObject(hDC,hBrush5);
			}
			Rectangle(hDC,wx,wy,nx,ny);
			wx = nx%(COL*CELL_SIZE);
			if(wx == 0)
				wy += CELL_SIZE;
		}
	}
	EndPaint(hWnd,&ps);
	DeleteObject(hBrush1);
	DeleteObject(hBrush2);
	DeleteObject(hBrush3);
	DeleteObject(hBrush4);
	DeleteObject(hBrush5);

	return;
}
