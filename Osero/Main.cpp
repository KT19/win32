#include<windows.h>
#include<tchar.h>
#include<future>
#include"Header.h"
#include"Define.h"

//�O���[�o���ϐ�
TCHAR szClassName[] =_T("Default Class Name"); //�E�B���h�E�N���X
extern bool Sharp_Board[BOARD_SIZE_Y+1][BOARD_SIZE_X+1]; //�Ֆʂ̌`��ێ�(�s�A��)
extern int Board[BOARD_SIZE_Y+1][BOARD_SIZE_X+1]; //���ۂ̔Ֆ�
extern int G_Disp_Board[BOARD_SIZE_Y+1][BOARD_SIZE_X+1]; 
extern bool Flash_Board[BOARD_SIZE_Y+1][BOARD_SIZE_X+1]; //���点��ꏊ
HINSTANCE G_hInst;
char Moziretu[30] = "�I�����Ă�������";
BOOL G_Flag = FALSE;

//�v���g�^�C�v�錾
ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst,int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp);

//�J�n
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpsCmdLine,int nCmdShow)
{
	MSG msg;
	BOOL bRet;
	
	G_hInst = hInst;

	if(!InitApp(hInst))
		return FALSE;
	if(!InitInstance(hInst,nCmdShow))
		return FALSE;

	//���b�Z�[�W���擾
	while((bRet=GetMessage(&msg,NULL,0,0)) != 0) {
		if(bRet == -1) {
			break;
		}
		else {
			//���b�Z�[�W����������
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//�E�B���h�E�N���X�̓o�^
ATOM InitApp(HINSTANCE hInst) 
{
	WNDCLASSEX wc;

	wc.cbSize=sizeof(WNDCLASSEX); //�\���̃T�C�Y
	wc.style=CS_HREDRAW | CS_VREDRAW; //�N���X�̃X�^�C��
	wc.lpfnWndProc=WndProc; //�E�B���h�E�v���V�[�W��
	wc.cbClsExtra=0; //�⏕������
	wc.cbWndExtra=0; //�⏕������
	wc.hInstance=hInst; //�C���X�^���X�n���h��
	wc.hIcon=(HICON)LoadImage( //�A�C�R��
		NULL,MAKEINTRESOURCE(IDI_APPLICATION),IMAGE_ICON,
		0,0,LR_DEFAULTSIZE | LR_SHARED
		); 
	wc.hIconSm=wc.hIcon; //�q�A�C�R��
	wc.hCursor=(HCURSOR)LoadImage( //�}�E�X�J�[�\��
		NULL,MAKEINTRESOURCE(IDC_ARROW),IMAGE_CURSOR,
		0,0,LR_DEFAULTSIZE | LR_SHARED
		);
	wc.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH); //�E�B���h�E�w�i
	wc.lpszMenuName=NULL;
	wc.lpszClassName=szClassName; //�N���X��

	return (RegisterClassEx(&wc));
}

//�E�B���h�E�̐���
BOOL InitInstance(HINSTANCE hInst,int nCmdShow)
{
	HWND hWnd;

	hWnd=CreateWindowEx(
		WS_EX_COMPOSITED, //�_�u���o�b�t�@
		szClassName, //�E�B���h�E�N���X��
		_T("Osero(Hard)"), //�^�C�g���o�[�ɕ\�����镶����
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, //�E�B���h�E�X�^�C��
		WINDOW_X, //�E�B���h�E��\������ʒu(X���W)
		WINDOW_Y, //�E�B���h�E��\������ʒu(Y���W)
		WINDOW_WIDTH, //�E�B���h�E�̕�
		WINDOW_HEIGHT, //�E�B���h�E�̍���
		NULL, //�e�E�B���h�E�̃n���h��
		NULL, //���j���[�n���h��
		hInst, //�C���X�^���X�n���h��
		NULL //���̑��̍쐬�f�[�^
		);

	if(!hWnd)
		return FALSE;

	//�E�B���h�E��\������
	ShowWindow(hWnd,nCmdShow); //�E�B���h�E�̕\�����
	UpdateWindow(hWnd); //�E�B���h�E���X�V

	return TRUE;
}

//�E�B���h�E�v���V�[�W��
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
		//������
		play_off = true;
		fin = false;
		play_ok = false;
		init.Initialize();
		hButton1 = CreateWindow(
			_T("BUTTON"),
			_T("��U"),
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
			_T("��U"),
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
			_T("�������"),
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
	case WM_LBUTTONDOWN: //�}�E�X�J�[�\���̍��{�^���������ꂽ�Ƃ�
		pos.x = LOWORD(lp);
		pos.y = HIWORD(lp);
		if(!fin && play_ok) {
			mouse_x = pos.x-50;
			mouse_y = pos.y-30;
			if(mouse_x < 0 || mouse_y < 0) //�Ֆʂ̊O�ł͉������Ȃ�
				break;
			mouse_x /= SEL_SIZE;
			mouse_y /= SEL_SIZE;
			if(mouse_x >= BOARD_SIZE_X || mouse_y >= BOARD_SIZE_Y)
				break;
			mouse.x = mouse_y;
			mouse.y = mouse_x;

			if(sys.Can_Put(mouse,YOU)) { //�u��
				play_ok = false;
				sys.Turn_Over(mouse,YOU);

				for(int i = 0;i < BOARD_SIZE_Y+1;i++) {
					for(int j = 0;j < BOARD_SIZE_X+1;j++) {
						G_Disp_Board[i][j] = Board[i][j];
					}
				}

				//�����Ă���������
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

				cpu.run(hWnd,&play_ok,&fin); //�G�̔�
			}
		}

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);
		SetBkMode(hdc,TRANSPARENT);
		SetTextColor(hdc,RGB(255,0,0));
		hFont = CreateFontIndirect(&logfont); //�_���t�H���g�쐬
		SelectObject(hdc,hFont);
		DrawText(hdc,Moziretu,-1,&char_rect,DT_CENTER);
		disp.Show_Score(hdc);
		DeleteObject(hFont); //�_���t�H���g�̍폜
		disp.Show_Board(hdc);

		EndPaint(hWnd,&ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd,msg,wp,lp);
}
