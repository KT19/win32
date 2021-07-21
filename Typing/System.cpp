//----------------System.cpp
#ifndef READ
#include<windows.h>
#include<tchar.h>
#include"Global.h"
#include"Define.h"
#endif

//�O���[�o���ϐ�
extern char G_Type_String[TYPE_MAX+1]; //�^�C�v���镶��
extern char G_Score[SCORE_MAX+1]; //�X�R�A�̕\��
extern int G_Miss_Type[ALPHABET+1]; //�Ԉ���������̃J�E���g
extern RECT G_Dis_Rect[TYPE_MAX+1]; //�\�������̍��W
extern char G_Explain[1000]; //������
extern char G_Count_Down[3]; //�J�E���g�_�E��

//���̕\��
void DispScore(HDC hdc,int combo,int rest_time)
{
	char val[100];
	RECT rc;

	rc.top = 200;
	rc.bottom = 300;
	rc.left = 630;
	rc.right = 800;

	//�c�莞�Ԃ͉E���
	if(rest_time/60000 != 0) {
	wsprintf(val,"�c��\n%d��%d�b",rest_time/60000,(rest_time%60000)/1000);
	} else {
		wsprintf(val,"�c��\n%d�b",(rest_time%60000)/1000);
	}
	SetTextColor(hdc,RGB(255,0,0));
	DrawText(hdc,val,-1,&rc,DT_CENTER);

	//�X�R�A�͉E�^�񒆂�
	rc.top = 350;
	rc.bottom = 450;
	SetTextColor(hdc,RGB(0x00,0x64,0x00));
	DrawText(hdc,G_Score,-1,&rc,DT_CENTER);
	//Score�Ƃ����������X�R�A�̍����
	rc.top = 330;
	rc.bottom=430;
	DrawText(hdc,"Score:",-1,&rc,DT_LEFT);

	//�R���{���͉E����
	SetTextColor(hdc,RGB(0,0,255));
	rc.top = 500;
	rc.bottom = 600;
	wsprintf(val,"combo\n%d",combo);
	DrawText(hdc,val,-1,&rc,DT_CENTER);

	return;
}

//�^�C�v�����̕\��
void DispType(HDC hdc)
{
	char val[10];
	int i;

	//�^�C�v�������\��
	for(i = 0;i < TYPE_MAX;i++) {
		if(G_Type_String[i] != '\0') {
			val[0] = G_Type_String[i];
			val[1] = '\0';
			if(TYPE_FIELD_OK(G_Dis_Rect[i]))
				SetTextColor(hdc,RGB(183,196,22));
			else
				SetTextColor(hdc,RGB(239,117,188));
			DrawText(hdc,val,-1,&G_Dis_Rect[i],DT_CENTER);
		}
	}

	return;
}

//������
void Init(void)
{
	int i;

	//�\������
	for(i = 0;i < TYPE_MAX+1;i++)
		G_Type_String[i] = '\0';
	//�\���X�R�A
	for(i = 0;i < SCORE_MAX+1;i++)
		G_Score[i] = '0';
	//�~�X�^�C�v
	for(i = 0;i < ALPHABET+1;i++)
		G_Miss_Type[i] = 0;
	//�ʒu�̍��W
	for(i = 0;i < TYPE_MAX;i++)
		G_Dis_Rect[i].top = G_Dis_Rect[i].bottom = G_Dis_Rect[i].left = G_Dis_Rect[i].right = 0;

	//������
	wsprintf(G_Explain,"CTRL+S�ŃX�^�[�g\nCTRL+Q�ŏI��\n���̃^�C�v�̈�ł̂݃^�C�v�\(�����̐F���ς�����Ƃ��̂�)\n�c�莞�Ԃ�X�R�A�Ȃǂ́�\n�r���ŗ������x�������Ȃ�\n�^�C�v�̈���ŉ����Ȃ��Ă��X�R�A�ɂȂ�Ȃ����������A\n�~�X����ƃ~�X���J�E���g+�R���{���Ȃ��Ȃ�\n3���ŏI��\n�������A�啶���̋�ʂ͂Ȃ�");

	//�J�E���g�_�E��
	wsprintf(G_Count_Down,"");

	//�~�X
	G_Miss_Num = 0;

	return;
}

//�Ԉ���������̃J�E���g
void Count_Miss_Type(char str)
{

	//�J�E���g
	G_Miss_Type[str - 'A']++;

	return;
}

//�����𗎂Ƃ����x��ς���
void Change_FallType(int *speed,int time) //(���x,����)
{
	*speed = 500/time*CYCLE/1000;

	return;
}

//�~�X�̔���
BOOL MissType(char type)
{
	int i;
	
	for(i = 0;i < TYPE_MAX;i++) {
		if(TYPE_FIELD_OK(G_Dis_Rect[i]) && G_Type_String[i] == type)  //�\��������ɂ���
			return TRUE;
	}

	return FALSE;
}

//�^�C�v�����̐���
BOOL Generate_Type(int *random,char str) //(�ʒu�A����)
{
	int check,i;
	
	//�K���Ȉʒu�ɓK���ȕ����𐶐�
	for(i = 0;i < TYPE_MAX;i++) 
		if(G_Type_String[i] == str)
			return FALSE;

	//���̏ꏊ�ɂ��łɒu����Ă����玟�̈ʒu
	for(check = 0;G_Type_String[*random%TYPE_MAX] != '\0';++(*random))
	{
		check++;

		if(check == TYPE_MAX)
			break;
	}

	//�S�Ă̈ʒu�����܂��Ă���Ƃ�
	if(check == TYPE_MAX)
		return FALSE; //�������s
 
	G_Type_String[*random%TYPE_MAX] = str; //�����Z�b�g

	return TRUE;
}

//���_�̉��Z
void AddScore(int combo)
{
	int score;

	score = atoi(G_Score); //���_(����->���l)
	score += 1+combo*(combo/COMBO_BONUS); //���Z(1type1�_+10�R���{��/2�����Z)
	if(score >= 999999)
		score = 999999;
	wsprintf(G_Score,"%d",score); //���_(���l->����)

	return;
}

//������������������
BOOL Check_Type(char str,int combo)
{
	int i;

	if(MissType(str)) { //������������
		for(i = 0;i < TYPE_MAX;i++) {
			if(G_Type_String[i] == str) {
			
				AddScore(combo); //�X�R�A�̉��Z
				G_Type_String[i] = '\0'; //��ɂ���
				G_Dis_Rect[i].left = G_Dis_Rect[i].right = G_Dis_Rect[i].top = G_Dis_Rect[i].bottom = 0; //�ʒu�̃��Z�b�g
				
			}
		}

		return TRUE;
	} else { //�����������ĂȂ�
		if(str - 'A' >= 0 && str-'A' < ALPHABET)
			Count_Miss_Type(str); //�Ԉ�����L�[�����Z
		G_Miss_Num++;
	}

	return FALSE;
}


//�����̈ʒu���Z�b�g
void Set_Iti(int iti)
{
	G_Dis_Rect[iti].left = 10+iti*55-2;
	G_Dis_Rect[iti].right = 10+iti*55+TYPE_FONT+2;
	G_Dis_Rect[iti].top = 10;
	G_Dis_Rect[iti].bottom = 10+TYPE_FONT;

	return;
}

//�����𗎂Ƃ�
void FallType(int speed)
{
	int i;

	for(i = 0;i < TYPE_MAX;i++) {
		if(G_Dis_Rect[i].top != 0) {
			G_Dis_Rect[i].top += speed;
			G_Dis_Rect[i].bottom += speed;
			if(G_Dis_Rect[i].top >= TYPE_OK_BOTTOM + 10) { //���Ƃ������ʃ^�C�v�̈�̊O�ɏo��
				G_Type_String[i] = '\0'; //��ɂ���
				G_Dis_Rect[i].left = G_Dis_Rect[i].right = G_Dis_Rect[i].top = G_Dis_Rect[i].bottom = 0; //�ʒu�̃��Z�b�g
			}
		}
	}

	return;
}

//GameOver
void End(HDC hdc)
{
	int i,j;
	RECT rc;
	char miss[50];
	char ex[1000];


	//�^�C�v����������
	for(i = 0;i < TYPE_MAX;i++)
		G_Type_String[i] = '\0';

	rc.top = 20;
	rc.bottom = 70;
	rc.left = 50;
	rc.right = 450;

	wsprintf(ex,"Score:%s\n�~�X�^�C�v:%d",G_Score,G_Miss_Num);
	DrawText(hdc,ex,-1,&rc,DT_CENTER);

	rc.top = 100;
	rc.bottom = 150;
	wsprintf(ex,"���ꂼ��̃~�X����");
	DrawText(hdc,ex,-1,&rc,DT_CENTER);


	rc.top = 200;
	rc.bottom = 300;
	for(i = 0;i < ALPHABET/2;i++) {
		rc.left = 5+i*30;
		rc.right = 5+((G_Miss_Type[i]/10)+1)*30+i*30;
		wsprintf(miss,"%c\n%d",'A'+i,G_Miss_Type[i]);
		DrawText(hdc,miss,-1,&rc,DT_CENTER);
	}
	rc.top = 350;
	rc.bottom = 450;
	for(i = ALPHABET/2,j=0;i < ALPHABET;i++,j++) {
		rc.left = 5+j*30;
		rc.right = 5+((G_Miss_Type[i]/10)+1)*30+j*30;
		wsprintf(miss,"%c\n%d",'A'+i,G_Miss_Type[i]);
		DrawText(hdc,miss,-1,&rc,DT_CENTER);
	}

	return;
}

//�F�X�\��
void Disp_Explain(HDC hdc)
{
	RECT rc;
	rc.top = 10;
	rc.bottom = 450;
	rc.left = 10;
	rc.right = 500;

	SetTextColor(hdc,RGB(255,0,0));
	DrawText(hdc,G_Explain,-1,&rc,DT_VCENTER);

	return;
}

//�J�n���鏀��
void Start(void)
{
	wsprintf(G_Explain,"");

	return;
}

//�J�E���g�_�E��
void Count_Down(DWORD time)
{
	wsprintf(G_Count_Down,"%d",3-(time/1000));

	return;
}

//�J�E���g��~
void LetsStart(void)
{
	wsprintf(G_Count_Down,"");

	return;
}

//�J�E���g�_�E���̕\��
void Disp_Count(HDC hdc)
{
	RECT rc;

	rc.top = 200;
	rc.bottom = 300;
	rc.left = 250;
	rc.right = 350;
	SetTextColor(hdc,RGB(255,255,255));
	DrawText(hdc,G_Count_Down,-1,&rc,DT_CENTER);

	return;
}