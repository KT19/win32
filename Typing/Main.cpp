//------------------Main.cpp
#define READ //��d�C���N���[�h�h�~
#include<windows.h>
#include<tchar.h>
#include<stdlib.h>
#include<time.h>
#include"Define.h"
#include"Header.h"

//�O���[�o���ϐ�
TCHAR szClassName[] =_T("Default Class Name"); //�E�B���h�E�N���X

//�v���g�^�C�v�錾
ATOM InitApp(HINSTANCE hInst);
BOOL InitInstance(HINSTANCE hInst,int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp);

//�J�n
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpsCmdLine,int nCmdShow)
{
	MSG msg;
	BOOL bRet;

	srand((unsigned)time(NULL)); //�����Z�b�g

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
		_T("Sample Program"), //�^�C�g���o�[�ɕ\�����镶����
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
		Init(); //�O���[�o���ϐ�������

		//�t�H���g�̍쐬
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
		
		//�������x
		fall_time = 6;
		Change_FallType(&speed,fall_time);


		rest_time = 180000; //3���Œ�

		combo = 0; //�R���{��

		stop = TRUE; //��~

		count_down = FALSE;
		flag_s = TRUE;
		disp_toku = FALSE;
		restart = FALSE;
		
		break;
	case WM_KEYDOWN:
		if((GetKeyState(VK_CONTROL) & 0x80) && (wp ==0x51)) //�I��
			DestroyWindow(hWnd);

		if((GetKeyState(VK_CONTROL) & 0x80) && (wp== 0x53) && flag_s) { //�J�n
			Start();
			SetTimer(hWnd,TIMER_ID,CYCLE,NULL);
			time1 = timeGetTime(); //�J�n����
			flag_s = FALSE; //������x�����Ȃ��悤�ɂ���
			count_down = TRUE; //�J�E���g�_�E��
		}
			
		break;
	case WM_LBUTTONDOWN:

		if(restart == TRUE && LOWORD(lp)>=400 && LOWORD(lp)<=600 && HIWORD(lp)>=400 && HIWORD(lp)<=500) {
			KillTimer(hWnd,TIMER_ID); //�^�C�}��؂�
			SendMessage(hWnd,WM_CREATE,wp,lp); //�E�B���h�E�����ꂽ��Ԃ�(�܂�A������)
			InvalidateRect(hWnd,NULL,TRUE);
		}
		break;
	case WM_CHAR:

		if(stop) { //�J�n���Ă��Ȃ��Ƃ�
			//�������Ȃ�

		} else { //�J�n

			if(Check_Type(toupper(wp),combo)) {
				combo++;
			} else {
				combo = 0;
			}
		}
		InvalidateRect(hWnd,NULL,TRUE); //�ĕ`��
		break;
	case WM_TIMER:
		if(wp != TIMER_ID)
			break;

		//�J�E���g��
		if(count_down) {
			time2 = timeGetTime();
			Count_Down(time2-time1);
			if(time2-time1 >= 3000) {
				count_down = FALSE;
				LetsStart(); //�J�E���g�I��
				time1 = timeGetTime();
				stop = FALSE;
			}
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		}

		if(rest_time <= 0) { //�I���̎�
			disp_toku = TRUE;
			stop = TRUE;
			restart = TRUE;
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		}

		rest_time -= CYCLE; //�c�莞�Ԃ�����

		if(rand()%2){
			iti = rand()%TYPE_MAX; //�ʒu�̐ݒ�
			mozi = 'A'+rand()%ALPHABET; //����
			if(Generate_Type(&iti,mozi)) { //�����ł����Ƃ�
				Set_Iti(iti%TYPE_MAX); //�����ʒu���Z�b�g
			}
		}
		FallType(speed); //�����𗎂Ƃ�

		time2 = timeGetTime();
		if(time2-time1 >= 45000) { //45�b���Ƃɉ���
			fall_time--;
			Change_FallType(&speed,fall_time);
			
			time1 = timeGetTime();
		}


		InvalidateRect(hWnd,NULL,TRUE); //�ĕ`��
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);

		SetBkMode(hdc,TRANSPARENT);

		//�F�X�\��
		hFont3 = CreateFontIndirect(&logfont3);
		Disp_Explain(hdc);
		SelectObject(hdc,hFont3);
		Disp_Count(hdc);

		//���̕\�����͐F��ύX
		hBrush = CreateSolidBrush(RGB(0x7f,0xff,0xd4));
		SelectObject(hdc,hBrush);
		Rectangle(hdc,600,0,800,WINDOW_HEIGHT);

		//���̕\��
		hFont2 = CreateFontIndirect(&logfont2);
		SelectObject(hdc,hFont2);
		DispScore(hdc,combo,rest_time);
		
		//�^�C�v�̈�̕\��
		hBrush2 = CreateSolidBrush(RGB(0xe0,0xff,0xff));
		SelectObject(hdc,hBrush2);
		Rectangle(hdc,TYPE_OK_LEFT,TYPE_OK_TOP,TYPE_OK_RIGHT-60,TYPE_OK_BOTTOM);
		
		//�^�C�v�����̕\��
		hFont = CreateFontIndirect(&logfont);
		SelectObject(hdc,hFont);
		DispType(hdc);
		if(disp_toku) {
		//���_�Ȃǂ̕\��
			hFont4 = CreateFontIndirect(&logfont4);
			SelectObject(hdc,hFont4);
			SetTextColor(hdc,RGB(120,18,200));
			End(hdc);
			DeleteObject(hFont4);

			//�ăX�^�[�g�̕\��
			hBrush3 = CreateSolidBrush(RGB(0,200,200));
			SelectObject(hdc,hBrush3);
			Rectangle(hdc,400,400,600,500);
			DeleteObject(hBrush3);
			rc.top = 430;
			rc.bottom = 500;
			rc.left = 400;
			rc.right = 600;
			SetTextColor(hdc,RGB(0,0,0));
			DrawText(hdc,"�ăX�^�[�g",-1,&rc,DT_CENTER);
		}
		
		//�j��
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
