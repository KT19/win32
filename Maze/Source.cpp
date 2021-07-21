//�E�B���h�E����Programing Place����̃y�[�W�̂��̂��g�p���܂���
#include<windows.h>
#include<tchar.h>
#include<iostream>
#include<random>
#include<cmath>
#include<string>
#include<thread>

//�萔
#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (645)
#define WINDOW_X ((GetSystemMetrics(SM_CXSCREEN)-WINDOW_WIDTH) / 2)
#define WINDOW_Y ((GetSystemMetrics(SM_CYSCREEN)-WINDOW_HEIGHT) / 2)
#define ROW (101) //���H�̏c��
#define COL (101) //���H�̉���
#define CELL_SIZE (6) //��}�X�̑傫��
#define SET_TIME (100) //100msec
#define BUTTON_ID1 (1)
#define BUTTON_ID2 (2)

//�v���g�^�C�v
HWND Create(HINSTANCE hInst);
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp);
HWND CreateButton(HWND hWnd,int x,int y,int w,int h,LPCTSTR caption,int id);
void PrintMap(HWND hWnd); //�}�b�v�̕\��

using namespace std;

//�O���[�o���ϐ�
int** _map;
string _path;
HINSTANCE _Inst;

//���H�쐬�N���X
class Maze_Create {
private:
	static const int Line = ROW,Row = COL; //���H�̑傫��(�s�A��) ��ɂ���
	void Break_Wall(int line,int row); //�ǂ���(�s,��)
public:
	Maze_Create(); //�R���X�g���N�^
	~Maze_Create(); //�f�X�g���N�^
	bool Create_map(); //���H�̍쐬(���@��@)
	bool S_G_Set(); //�X�^�[�g�ƃS�[���̃Z�b�g
};

//���H�̉�@�N���X
class Maze_Search {
private:
	int *vx,*vy; //�����x�N�g��
	char *data_way,*un_way; //����
	static const int Row = ROW,Col = COL;
	int Lim;
	int Init();
	int Heuristics(int,int); //�q���[���X�e�B�b�N�X
	bool IDA(int,int,int,char);
	int s_row,s_col,g_row,g_col;
	string path;
public:
	Maze_Search(); //�R���X�g���N�^
	~Maze_Search(); //�f�X�g���N�^
	void Search();
	int Get_S_Row();//�Q�b�^�[
	int Get_S_Col(); //�Q�b�^�[
};

//���H�̃A�j���[�V�����N���X
class Maze_Animation {
private:
	static const int Time = SET_TIME;
public:
	void run(HWND,int,int);
	void Animation(HWND,int,int);
};

//Maze_Create
//�R���X�g���N�^
Maze_Create :: Maze_Create() {
	//�������̊m��
	_map = new int*[Line];
	
	for(int i = 0;i < Line;i++) {
		_map[i] = new int[Row];
	}
};
//�f�X�g���N�^
Maze_Create :: ~Maze_Create() {
	for(int i = 0;i < Line;i++) {
		delete[] _map[i];
	}

	delete[] _map;
}

//�ǉ�
void Maze_Create :: Break_Wall(int line,int row) 
{
	random_device rnd; //����
	int way1,way2,way3,way4;
	way1 = way2 = way3 = way4 = 0;

	while(true)
	{
		if(way1+way2+way3+way4 == 4)
			break;

		switch(rnd()%4+1) {
		case 1: //��
			if(line - 2 > 0 && _map[line-2][row] == 1) 
			{
				//�ʘH�ɂ���
				_map[line-1][row] = 0;
				_map[line-2][row] = 0;
				//�ċA�Ăяo��
				Break_Wall(line-2,row);
			} else {
				way1=1;
			}
			break;
		case 2: //��
			if(row - 2 > 0 && _map[line][row-2] == 1)
			{
				_map[line][row-1] = 0;
				_map[line][row-2] = 0;
				Break_Wall(line,row-2);
			} else {
				way2 = 1;
			}
			break;
		case 3: //��
			if(line+2 < Line-1 && _map[line+2][row] == 1) {
				_map[line+1][row] = 0;
				_map[line+2][row] = 0;
				Break_Wall(line+2,row);
			} else {
				way3 = 1;
			}
			break;
		case 4: //�E
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

//�쐬
bool Maze_Create :: Create_map() 
{
	int line,row;
	random_device rnd; //����

	
	//�}�b�v�̏�����
	for(int i = 0;i < Line;i++) {
		for(int j = 0;j < Row;j++) {
			_map[i][j] = 1; //1:(��) 0:(�ʘH)
		}
	}
	//�����_���ɕǂ��󂷏ꏊ��I��
	while(!((line = rnd() % Line)%2)) //�s�ɂ���
		;
	while(!((row = rnd() % Row)%2)) //��ɂ���
		;

	Break_Wall(line,row); //�ǂ���
	
	S_G_Set(); //�X�^�[�g�ƃS�[���̃Z�b�g

	cout<<"-----------------"<<endl;
	for(int i = 0;i < Line;i++) {
		for(int j = 0;j < Row;j++) {
			printf("%2d",_map[i][j]);
		}
		cout<<endl;
	}

	return true;
}
//�X�^�[�g�ƃS�[���̃Z�b�g
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
//�R���X�g���N�^
Maze_Search :: Maze_Search()
{
	//�����x�N�g���̒�`
	vx = new int[4];
	vx[0] = 1;vx[1] = 0;vx[2] = -1;vx[3] = 0;
	vy = new int[4];
	vy[0] = 0;vy[1] = 1;vy[2] = 0;vy[3] = -1;

	//�����̒�`
	data_way = new char[4];
	data_way[0]='d';data_way[1] = 'r';data_way[2] = 'u';data_way[3] = 'l';
	un_way = new char[4];
	un_way[0]='u';un_way[1]='l';un_way[2]='d';un_way[3]='r';

	Init();
}
//�f�X�g���N�^
Maze_Search :: ~Maze_Search()
{
	delete vx;
	delete vy;
	delete data_way;
}

//������
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

//�q���[���X�e�B�b�N�X
int Maze_Search :: Heuristics(int nx,int ny)
{
	int a,b;
	a = abs(nx-g_row);
	b = abs(ny-g_col);

	return sqrt(a*a+b*b)+0.5; 
}

//�T��
void Maze_Search :: Search()
{
	for(;!IDA(s_row,s_col,0,NULL);Lim++)  //�����[��
		;
	

	_path = path; //�o�H�̕ۑ�
	
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
	if(_map[nx][ny] == 3) { //�S�[����������
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

//�Q�b�^�[
int Maze_Search::Get_S_Row() 
{
	return s_row;
}
int Maze_Search::Get_S_Col()
{
	return s_col;
}

//Maze_Animation
//���񏈗�
void Maze_Animation::run(HWND hWnd,int row,int col)
{
	thread th1(&Maze_Animation::Animation,this,hWnd,row,col);

	th1.detach();
}

void Maze_Animation::Animation(HWND hWnd,int row,int col)
{
	while(!_path.empty()) {
		switch(_path.front()) { //�擪�̕��������o��
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
		_path.erase(_path.begin()); //�擪�̕������폜
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
	//�E�B���h�E�̍쐬
	hWnd = Create(hInst);
	if(hWnd == NULL) {
		MessageBox(NULL,_T("�E�B���h�E�̍쐬�Ɏ��s���܂���"),_T("ERROR"),MB_OK);

		return 1;
	}
	//�E�B���h�E�̕\��
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);
	//���b�Z�[�W���[�v
	while(1) {
		BOOL ret = GetMessage(&msg,NULL,0,0);
		if(ret == 0 || ret == -1)
			break;
		else {
			//���b�Z�[�W����
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

//�E�B���h�E�̍쐬
HWND Create(HINSTANCE hInst)
{
	WNDCLASSEX wc;

	//�E�B���h�E�N���X�̏���ݒ�
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

	//�E�B���h�E�N���X�̓o�^
	if(RegisterClassEx(&wc) == 0)
		return 0;

	//�E�B���h�E�̍쐬
	return CreateWindow(
		wc.lpszClassName,
		_T("�`�`�`Maze�`�`�`"),
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

//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
	static HWND hButton1,hButton2;
	static Maze_Create mc;
	static bool flag = false;

	switch(msg)
	{
	case WM_CREATE:
		hButton1 = CreateButton(hWnd,650,100,100,50,_T("���H�쐬"),BUTTON_ID1);
		InvalidateRect(hWnd,NULL,FALSE);

		return 0;
	case WM_COMMAND: //�{�^���������ꂽ�Ƃ�
		switch(LOWORD(wp))
		{
		case BUTTON_ID1:

			mc.Create_map(); //���H�̍쐬
			InvalidateRect(hWnd,NULL,FALSE);
			flag = true;

			//�V���Ƀ{�^���̐���
			hButton2 = CreateButton(hWnd,650,160,100,50,_T("���H�̉�@"),BUTTON_ID2);

			break;
		case BUTTON_ID2:
			Maze_Search ms;
			Maze_Animation ma;

			ms.Search(); //���H������
			if(MessageBox(hWnd,_T("�菇��������܂���\n�Č����܂�"),_T("Message"),MB_OK) == IDOK) {
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

//�{�^���̍쐬
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

//�}�b�v�̕\��
void PrintMap(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HBRUSH hBrush1,hBrush2,hBrush3,hBrush4,hBrush5;
	int wx = 0,wy = 0;
	int nx,ny;
	int px;

	hBrush1 = CreateSolidBrush(RGB(100,100,100)); //��
	hBrush2 = CreateSolidBrush(RGB(200,200,200)); //�ʘH
	hBrush3 = CreateSolidBrush(RGB(0,255,0)); //�X�^�[�g
	hBrush4 = CreateSolidBrush(RGB(0,255,255)); //�S�[��
	hBrush5 = CreateSolidBrush(RGB(255,0,0)); //���[�g

	hDC = BeginPaint(hWnd,&ps);
	for(int i = 0;i < ROW;i++) {
		for(int j = 0;j < COL;j++) {
			px = _map[i][j];
			nx = wx+CELL_SIZE;
			ny = wy+CELL_SIZE;
			if(px == 1) { //�ǂ̎�
				SelectObject(hDC,hBrush1);
			}
			else if(px == 0) { //�ʘH�̎�
				SelectObject(hDC,hBrush2);
			}
			else if(px == 2) { //�X�^�[�g�̎�
				SelectObject(hDC,hBrush3);
			}
			else if(px == 3) { //�S�[���̎�
				SelectObject(hDC,hBrush4);
			}
			else if(px == 9) { //���[�g�̕\���̎�
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
