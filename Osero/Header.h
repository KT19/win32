#include"Define.h"

//������
class Init
{
public:
	void Initialize(void); //������
};
//�G��
class CPU
{
private:
	POINT Where_Put(void); //�����ɒu����
public:
	void Turn(HWND,bool *,bool *); //�G�̔�
	void run(HWND,bool *,bool *);
};

//����
class Player
{
public:
	bool Turn(void); //�����̔�
	void Possible(POINT); //�u��
};

//�V�X�e��
class System
{
public:
	int Which_Win(void); //�ǂ�������������
	void Blight_Put(int); //�u����ꏊ�����点��
	bool Game_Over(void); //�Q�[�����I�������
	bool Can_Put(POINT,int); //�u���邩
	void Turn_Over(POINT,int); //�Ђ�����Ԃ�
	void Play(int); //����
};

//�\���p
class Disp
{
public:
	void Show_Board(HDC); //�Ֆʂ̕\��
	void Show_Score(HDC); //�X�R�A�̕\��
};