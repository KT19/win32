//-----------------Header.h
//���̕\��
void DispScore(HDC hdc,int combo,int rest_time);
//�^�C�v�����̕\��
void DispType(HDC hdc);
//������
void Init(void);
//�Ԉ���������̃J�E���g
void Count_Miss_Type(char str);
//�����𗎂Ƃ����x��ς���
void Change_FallType(int *speed,int time);
//�~�X�̔���
BOOL MissType(char type);
//�^�C�v�����̐���(�ʒu�A����)
BOOL Generate_Type(int *random,char str);
//������������������
BOOL Check_Type(char str,int combo);
//���_�̉��Z
void AddScore(int combo);
//�����̈ʒu�̃Z�b�g
void Set_Iti(int iti);
//�����𗎂Ƃ�
void FallType(int speed);
//GameOver
void End(HDC hdc);
//�F�X�\��
void Disp_Explain(HDC hdc);
//�J�n
void Start(void);
//�J�E���g�_�E��
void Count_Down(DWORD time);
//�J�E���g��~
void LetsStart(void);
//�J�E���g�_�E���̕\��
void Disp_Count(HDC hdc);