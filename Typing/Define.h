//--------------------Define.h
//�萔
#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)
#define WINDOW_X ((GetSystemMetrics(SM_CXSCREEN)-WINDOW_WIDTH)/2)
#define WINDOW_Y ((GetSystemMetrics(SM_CYSCREEN)-WINDOW_HEIGHT)/2)
#define TYPE_MAX (10) //��x�ɕ\�����镶���̍ő吔
#define ALPHABET (26) //�A���t�@�x�b�g�̐�
#define SCORE_MAX (6) //�X�R�A�̌�
#define COMBO_BONUS (10) //�R���{�ɂ���ē_���̉��Z
#define TYPE_FONT (32) //�t�H���g�̑傫��
#define INF_FONT (14) //���̃t�H���g
#define TYPE_OK_LEFT (5) //�^�C�v�L���̈�
#define TYPE_OK_RIGHT (650) //�^�C�v�L���̈�
#define TYPE_OK_TOP (400) //�^�C�v�L���ǎ�
#define TYPE_OK_BOTTOM (500) //�^�C�v�̈�
#define TYPE_FIELD_OK(ok) (ok.bottom >= TYPE_OK_TOP && ok.top <= TYPE_OK_BOTTOM) //�L���̈�
#define TIMER_ID (1) //�^�C�}ID
#define CYCLE (30)