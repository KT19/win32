#include<tchar.h>
#include<iostream>
#include<cstdio>
#include<windows.h>
#include<vector>
#include<random>
#include<thread>
#include<mutex>
#include"Header.h"
#include"Define.h"

using namespace std;

//�O���[�o���ϐ�
bool Sharp_Board[BOARD_SIZE_Y+1][BOARD_SIZE_X+1];//�Ֆʂ̌`(�s�A��)
int Board[BOARD_SIZE_Y+1][BOARD_SIZE_X+1]; //���ۂ̔Ֆ�
int G_Disp_Board[BOARD_SIZE_Y+1][BOARD_SIZE_X+1]; //�\���p�̔Ֆ�
bool Flash_Board[BOARD_SIZE_Y+1][BOARD_SIZE_X+1]; //���点��ꏊ
extern char Moziretu[30];
extern BOOL G_Flag;
mutex mtx;

//�\����
typedef struct
{
	double win_kazu; //��������
	double kei; //���̎���w������
	double score; //�X�R�A
}Prop;

//������
void Init::Initialize(void)
{
	for(int i = 0;i < BOARD_SIZE_Y+1;i++) {
		for(int j = 0;j < BOARD_SIZE_X+1;j++) {
			Board[i][j] = 0;
			Sharp_Board[i][j] = true;
			Flash_Board[i][j] = false;
		}
	}

	Board[3][3] = AI;
	Board[3][4] = YOU;
	Board[4][3] = YOU;
	Board[4][4] = AI;

	for(int i = 0;i < BOARD_SIZE_Y+1;i++) {
		for(int j = 0;j < BOARD_SIZE_X+1;j++) {
			G_Disp_Board[i][j] = Board[i][j];
		}
	}

	wsprintf(Moziretu,"�I�����Ă�������");

	return;
}
//�R���s���[�^
//�u���ꏊ������
POINT CPU::Where_Put(void)
{
	Prop Count[BOARD_SIZE_Y+1][BOARD_SIZE_X+1]; //�����̃J�E���g
	int Re_Board[BOARD_SIZE_Y+1][BOARD_SIZE_X+1]; //���Z�b�g�p
	POINT point,mem_point;
	random_device ran; //�䌈��I�ȗ���
	mt19937 mt(ran()); //�����Z���k�E�c�C�X�^
	vector<POINT> vec;
	System sys;
	int win;
	double max;
	bool flag = false;
	int kazu;
	bool flag_put;
	int level = 0;
	int Kei;
	int hand;

	//������
	for(int i = 0;i < BOARD_SIZE_Y;i++) {
		for(int j = 0;j < BOARD_SIZE_X;j++) {
			Count[i][j].score = 500.0;
			Count[i][j].win_kazu = Count[i][j].kei = 0.0;
			Re_Board[i][j] = Board[i][j];
		}
	}	

	//�u���邩�ǂ������m�F
	flag_put = false;
	for(int i = 0;i < BOARD_SIZE_Y;i++) {
		for(int j = 0;j < BOARD_SIZE_X;j++) {
			point.x = i;
			point.y = j;
			if(sys.Can_Put(point,AI))
				flag_put = true;

			if(Board[i][j] == YOU || Board[i][j] == AI)
				level++;
		}
	}
	if(flag_put == false)
	{
		point.x = -1;
		point.y = -1;

		return point;
	}
	hand = level-3; //����ڂ����擾

	//�����e�J����
	//���s�񐔂��萔�ɂ���ĕύX����
//	Kei = INTELIGENCE*(level/5+1+level/20); //���̎��u�����v�̐�
	Kei = INTELIGENCE*hand;

	for(int i = 0;i < Kei;i++) {
		flag = false;
		//�v���C�A�E�g�܂ŌJ��Ԃ�
		for(int nc = 0;nc < 61;nc++) {
			//ai�ɂ��Ēu����ꏊ�𒲂ׂ�
			for(int x = 0;x < BOARD_SIZE_Y*BOARD_SIZE_X;x++) {
				point.x = x/BOARD_SIZE_Y;
				point.y = x%BOARD_SIZE_X;
				if(sys.Can_Put(point,AI)) {
					if(!flag) { //����̂�
						for(int rt = 0;rt < Count[point.x][point.y].score;rt++)
							vec.push_back(point); //�u����ꏊ���擾
					}
					else {
						vec.push_back(point); //�u����ꏊ���擾
					}
				}
			}

			kazu = vec.size(); //�v�f�����擾

			if(kazu != 0) {

				//�����œK���ȏꏊ�ɒu��(�G)
				point = vec[mt()%kazu];
				sys.Turn_Over(point,AI); //�Ƃ肠�����u��

				if(!flag) {
					mem_point = point; //���W��ۑ�
					flag = true;
				}
				vec.clear(); //��x�v�f�����폜
				if(sys.Game_Over())
					break;
			} else { //�܂�u���Ȃ�
			}

			//�v���C���ɂ��Ēu����ꏊ�𒲂ׂ�
			for(int x = 0;x < BOARD_SIZE_Y;x++) {
				for(int y = 0;y < BOARD_SIZE_X;y++) {
					point.x = x;
					point.y = y;
					if(sys.Can_Put(point,YOU)) {
						vec.push_back(point); //�u����ꏊ���擾
						if((x == 0 && y == 0) || (x == 0 && y == 7) || (x == 7 && y == 0) || (x == 7 && y == 7))
							vec.push_back(point);
					}
				}
			}
		
			kazu = vec.size(); //�v�f�����擾

			if(kazu != 0) {
				//�����œK���ȏꏊ�ɒu��(����)
				point = vec[mt()%kazu];
				sys.Turn_Over(point,YOU); //�u��
				vec.clear(); //�폜
				if(sys.Game_Over())
					break;
			}

		}

		win = sys.Which_Win(); //����������������
		Count[mem_point.x][mem_point.y].kei += 1.0;
		if(win == CPU_WIN) {
			Count[mem_point.x][mem_point.y].win_kazu += 1.0;
		}
			Count[mem_point.x][mem_point.y].score = 1.0+((Count[mem_point.x][mem_point.y].win_kazu/Count[mem_point.x][mem_point.y].kei)+0.7*sqrt(2*log(1.0*Kei)/(Count[mem_point.x][mem_point.y].kei)))*10; //�w����W���J�E���g
		//�Ֆʂ�߂�
			for(int i = 0;i < BOARD_SIZE_Y*BOARD_SIZE_X;i++) {
				Board[i/BOARD_SIZE_Y][i%BOARD_SIZE_X] = Re_Board[i/BOARD_SIZE_Y][i%BOARD_SIZE_X];
		}
	}

	max = 0.0;
	//�ł����s�񐔂̍��������ꏊ��Ԃ�
	for(int x = 0;x < BOARD_SIZE_Y;x++) {
		for(int y = 0;y < BOARD_SIZE_X;y++) {
			if(Count[x][y].kei > max) {
				point.x = x;
				point.y = y;
				max = Count[x][y].kei;
			}
		}
	}
	//�܂�A�ǂ��ɂ����Ă����ĂȂ��Ƃ�
	if(max == 0.0) {
		vec.clear(); //�v�f���폜
		for(int i = 0;i < BOARD_SIZE_Y;i++) {
			for(int j = 0;j < BOARD_SIZE_X;j++) {
				point.x = i;
				point.y = j;
				if(sys.Can_Put(point,AI))
					vec.push_back(point); //�v�f���L��
			}
		}
		//�����_���ɂǂ����ɒu��
		kazu = vec.size();
		point = vec[mt()%kazu];
	}

	return point;
}

//�R���s���[�^�̔�
void CPU::run(HWND hWnd,bool *play_ok,bool *fin)
{
	thread th(&CPU::Turn,this,hWnd,ref(play_ok),ref(fin));
	
	th.detach();

	return;
}

void CPU::Turn(HWND hWnd,bool *play_ok,bool *fin)
{
	POINT point; //�u���ꏊ
	Player player;
	System sys;
	bool re = true;

	do {
		wsprintf(Moziretu,"�v�l���E�E�E�E");
		InvalidateRect(hWnd,NULL,TRUE);

		point = Where_Put();
		if(point.x != -1 && point.y != -1) {
			sys.Turn_Over(point,AI);
			for(int i = 0;i < BOARD_SIZE_Y+1;i++) {
				for(int j = 0;j < BOARD_SIZE_X+1;j++) {
					G_Disp_Board[i][j] = Board[i][j];
				}
			}
		}
		else {
			wsprintf(Moziretu,"pass");
			InvalidateRect(hWnd,NULL,TRUE);
			this_thread::sleep_for(chrono::seconds(1));
		}
		InvalidateRect(hWnd,NULL,TRUE);

		if(sys.Game_Over()) {
			mtx.lock();
			*fin = true;
			mtx.unlock();
			if(sys.Which_Win() == YOU)
				wsprintf(Moziretu,"You Win");
			else if(sys.Which_Win() == AI)
				wsprintf(Moziretu,"You Lose");
			else
				wsprintf(Moziretu,"Draw");
			InvalidateRect(hWnd,NULL,TRUE); //�ĕ`��

			return;
		} else {
			re = player.Turn(); //�v���C���[�̔�
			InvalidateRect(hWnd,NULL,TRUE); //�ĕ`��
			if(re == false)
				this_thread::sleep_for(chrono::milliseconds(1000));
		}
	}while(re == false); //���Ȃ킿�v���C���[���u���Ȃ�

	mtx.lock();

	*play_ok = true;

	mtx.unlock();

	return;
}

//�v���C���[
bool Player::Turn(void)
{
	System sys;
	POINT point;

	wsprintf(Moziretu,"Your Turn");

	sys.Blight_Put(YOU); //���点��

	for(int i = 0;i < BOARD_SIZE_Y;i++) {
		for(int j = 0;j < BOARD_SIZE_X;j++) {
			point.x = i;
			point.y = j;
			if(sys.Can_Put(point,YOU))
				return true;
		}
	}

	wsprintf(Moziretu,"Pass");

	return false;
}
//�u��
void Player::Possible(POINT po)
{
	System sys;

	if(Flash_Board[po.x][po.y])
		sys.Turn_Over(po,YOU);

	return;
}

//�V�X�e��
//�Q�[���I�[�o�[
bool System::Game_Over(void)
{
	POINT point;
	for(int i = 0;i < BOARD_SIZE_Y;i++) {
		for(int j = 0;j < BOARD_SIZE_X;j++) {
			point.x = i;
			point.y = j;
			if(Can_Put(point,AI))
				return false;
			if(Can_Put(point,YOU))
				return false;
		}
	}

	return true;
}
//�ǂ�������������
int System::Which_Win(void)
{
	int cpu = 0,you = 0;

	for(int i = 0;i < BOARD_SIZE_Y;i++) {
		for(int j = 0;j < BOARD_SIZE_X;j++) {
			if(Board[i][j] == YOU)
				you++;
			if(Board[i][j] == AI)
				cpu++;
		}
	}

	if(cpu == you)
		return SAME_WIN;

	return cpu > you ? CPU_WIN : YOU_WIN;
}
//�u����ꏊ�����点��
void System::Blight_Put(int sel)
{
	POINT point;
	vector<POINT> pos;

	for(int i = 0;i < BOARD_SIZE_Y;i++) {
		for(int j = 0;j < BOARD_SIZE_X;j++) {
		/*	Flash_Board[j][i] = false;
			point.x = i;
			point.y = j;
			if(Can_Put(point,sel))  
				pos.push_back(point); //�u����ꏊ���i�[
		*/
			point.x = i;
			point.y = j;
			mtx.lock();

			if(Can_Put(point,sel))
				Flash_Board[i][j] = true;
			else
				Flash_Board[i][j] = false;
			
			mtx.unlock();
		}
	}

/*	while(!pos.empty()) {
		point = pos.back(); //�v�f�����o��
		pos.pop_back(); //����
		Flash_Board[point.x][point.y] = true;
	}
*/
	return;
}
//�u���邩
bool System::Can_Put(POINT po,int sel)
{
	bool flag = false;

	if(po.x < 0 || po.y < 0 || po.x >= BOARD_SIZE_Y || po.x >= BOARD_SIZE_Y || Board[po.x][po.y] != 0)
		return false;
	//�u���������
	//�㉺���E�̂����ꂩ�ɓG�̐΂��u����Ă���
	//�܂��A�΂߂����l
	//��
/*	for(int i = po.x;i >= 0;i--) {
		if(i - 1 >= 0 && Sharp_Board[i-1][po.y] == true && Board[i-1][po.y] != 0 && Board[i-1][po.y] != sel) {
			flag = true;
		} else {
			if(flag && i-1 >= 0 && Board[i-1][po.y] == sel)
				return true;
			else
				break;
		}
	}
*/
	for(int i = po.x-1;i >= 0;i--)
	{
		if(Board[i][po.y] != 0 && Board[i][po.y] != sel)
			flag = true;
		else
		{
			if(flag && Board[i][po.y] == sel)
				return true;
			else
				break;
		}
	}
	//��
	flag = false;
/*	for(int i = po.x;i < BOARD_SIZE_Y;i++) {
		if(i+1 < BOARD_SIZE_Y && Sharp_Board[i+1][po.y] == true && Board[i+1][po.y] != 0 && Board[i+1][po.y] != sel) {
			flag = true;
		} else {
			if(flag && i+1 < BOARD_SIZE_Y && Board[i+1][po.y] == sel)
				return true;
			else
				break;
		}
	}*/
	for(int i = po.x+1;i < BOARD_SIZE_Y;i++) {
		if(Board[i][po.y] != 0 && Board[i][po.y] != sel)
			flag = true;
		else {
			if(flag && Board[i][po.y] == sel)
				return true;
			else
				break;
		}
	}
	//��
	flag = false;
/*	for(int j = po.y;j >= 0;j--) {
		if(j-1 >= 0 && Sharp_Board[po.x][j-1] == true && Board[po.x][j-1] != 0 && Board[po.x][j-1] != sel) {
			flag = true;
		} else {
			if(flag && j-1 >= 0 && Board[po.x][j-1] == sel)
				return true;
			else
				break;
		}
	}*/
	for(int j = po.y-1;j >= 0;j--) {
		if(Board[po.x][j] != 0 && Board[po.x][j] != sel)
			flag = true;
		else
		{
			if(flag && Board[po.x][j] == sel)
				return true;
			else
				break;
		}
	}
	//�E
	flag = false;
/*	for(int j = po.y;j < BOARD_SIZE_X;j++) {
		if(j+1 < BOARD_SIZE_X && Sharp_Board[po.x][j+1] == true && Board[po.x][j+1] != 0 && Board[po.x][j+1] != sel) {
			flag = true;
		} else {
			if(flag && j+1 < BOARD_SIZE_X && Board[po.x][j+1] == sel)
				return true;
			else
				break;
		}
	}*/
	for(int j = po.y+1;j < BOARD_SIZE_X;j++) {
		if(Board[po.x][j] != 0 && Board[po.x][j] != sel)
			flag = true;
		else
		{
			if(flag && Board[po.x][j] == sel)
				return true;
			else 
				break;
		}
	}
	//����
	flag = false;
/*	for(int j = po.y,i = po.x;j >= 0 && i >= 0;j--,i--) {
		if(i-1 >= 0 && j-1 >= 0 && Sharp_Board[i-1][j-1] == true && Board[i-1][j-1] != 0 && Board[i-1][j-1] != sel) {
			flag = true;
		} else {
			if(flag && i-1 >= 0 && j-1 >= 0 && Board[i-1][j-1] == sel)
				return true;
			else
				break;
		}
	}*/
	for(int i = po.x-1,j = po.y-1;j >= 0 && i >= 0;i--,j--) {
		if(Board[i][j] != 0 && Board[i][j] != sel)
			flag = true;
		else
		{
			if(flag && Board[i][j] == sel)
				return true;
			else
				break;
		}
	}
	//�E��
	flag = false;
/*	for(int j = po.y,i = po.x;j < BOARD_SIZE_X && i >= 0;j++,i--) {
		if(i-1 >= 0 && j+1 < BOARD_SIZE_X && Sharp_Board[i-1][j+1] == true && Board[i-1][j+1] != 0 && Board[i-1][j+1] != sel) {
			flag = true;
		} else {
			if(flag && i-1 >= 0 && j+1 < BOARD_SIZE_X && Board[i-1][j+1] == sel)
				return true;
			else
				break;
		}
	}*/
	for(int i = po.x-1,j = po.y+1;i >= 0 && j < BOARD_SIZE_X;i--,j++)
	{
		if(Board[i][j] != 0 && Board[i][j] != sel)
			flag = true;
		else
		{
			if(flag && Board[i][j] == sel)
				return true;
			else
				break;
		}
	}
	//�E��
	flag = false;
/*	for(int j = po.y,i = po.x;j < BOARD_SIZE_X && i < BOARD_SIZE_Y;j++,i++) {
		if(i+1 < BOARD_SIZE_Y && j+1 < BOARD_SIZE_Y && Sharp_Board[i+1][j+1] == true && Board[i+1][j+1] != 0 && Board[i+1][j+1] != sel) {
			flag = true;
		} else {
			if(flag && i+1 < BOARD_SIZE_Y && j+1 < BOARD_SIZE_X && Board[i+1][j+1] == sel)
				return true;
			else
				break;
		}
	}*/
	for(int i = po.x+1,j = po.y+1;i < BOARD_SIZE_Y && j < BOARD_SIZE_X;i++,j++)
	{
		if(Board[i][j] != 0 && Board[i][j] != sel)
			flag = true;
		else
		{
			if(flag && Board[i][j] == sel)
				return true;
			else
				break;
		}
	}
	//����
	flag = false;
/*	for(int j = po.y,i = po.x;j >= 0 && i < BOARD_SIZE_Y;j--,i++) {
		if(i+1 < BOARD_SIZE_Y && j-1 >= 0 && Sharp_Board[i+1][j-1] == true && Board[i+1][j-1] != 0 && Board[i+1][j-1] != sel) {
			flag = true;
		} else {
			if(flag && i+1 < BOARD_SIZE_Y && j-1 >= 0 && Board[i+1][j-1] == sel)
				return true;
			else
				break;
		}
	}*/
	for(int i = po.x+1,j = po.y-1;i < BOARD_SIZE_Y && j >= 0;i++,j--)
	{
		if(Board[i][j] != 0 && Board[i][j] != sel)
			flag = true;
		else
		{
			if(flag && Board[i][j] == sel)
				return true;
			else
				break;
		}
	}

	return false;
};
//�Ђ�����Ԃ�
void System::Turn_Over(POINT po,int sel) //���W�ƍ��̑���v���C���[��n��
{
	int count;
	int i;
	int row,col;
	//�u��
	Board[po.x][po.y] = sel;


	//�㑤�ɂ���
	count = 0;
	for(i = po.x-1;i >= 0;i--) {
		if(Board[i][po.y] != 0 && Board[i][po.y] != sel)
			count++;
		else
			break;
	}
	if(i>=0 && count > 0 && Board[i][po.y] == sel)
		for(int j = po.x-1;j >= 0 && count > 0;j--) {
			Board[j][po.y] = sel;
			count--;
		}

	//�����ɂ���
	count = 0;
	for(i = po.x+1;i < BOARD_SIZE_Y;i++) {
		if(Board[i][po.y] != 0 && Board[i][po.y] != sel) {
			count++;
		} else {
			break;
		}
	}
	if(i < BOARD_SIZE_Y && count > 0 && Board[i][po.y] == sel)
		for(int j = po.x+1;j < BOARD_SIZE_Y && count > 0;j++) {
			Board[j][po.y] = sel;
			count--;
		}

	//�����ɂ���
	count = 0;
	for(i = po.y-1;i >= 0;i--) {
		if(Board[po.x][i] != 0 && Board[po.x][i] != sel)
			count++;
		else
			break;
	}
	if(i >= 0 && count > 0 && Board[po.x][i] == sel) {
		for(int j = po.y-1;j >= 0 && count > 0;j--) {
			Board[po.x][j] = sel;
			count--;
		}
	}

	//�E���ɂ���
	count = 0;
	for(i = po.y+1;i < BOARD_SIZE_X;i++) {
		if(Board[po.x][i] != 0 && Board[po.x][i] != sel) {
			count++;
		}
		else
			break;
	}
	if(i < BOARD_SIZE_X && count > 0 && Board[po.x][i] == sel) {
		for(int j = po.y+1;j < BOARD_SIZE_X && count > 0;j++) {
			Board[po.x][j] = sel;
			count--;
		}
	}

	//����ɂ���
	count = 0;
	for(row = po.x-1,col = po.y-1;row >= 0 && col >= 0;row--,col--) {
		if(Board[row][col] != 0 && Board[row][col] != sel)
			count++;
		else
			break;
	}
	if(row >= 0 && col >= 0 && count > 0 && Board[row][col] == sel) {
		for(row = po.x-1,col = po.y-1;row >= 0 && col >= 0 && count > 0;row--,col--) {
			Board[row][col] = sel;
			count--;
		}
	}

	//�E��ɂ���
	count = 0;
	for(row = po.x-1,col = po.y+1;col < BOARD_SIZE_X && row >= 0;col++,row--) {
		if(Board[row][col] != 0 && Board[row][col] != sel)
			count++;
		else
			break;
	}
	if(row >= 0 && col < BOARD_SIZE_X && count > 0 && Board[row][col] == sel) {
		for(row = po.x-1,col = po.y+1;col < BOARD_SIZE_X && row >= 0 && count > 0;col++,row--) {
			Board[row][col] = sel;
			count--;
		}
	}

	//�����ɂ���
	count = 0;
	for(row = po.x+1,col = po.y-1;row < BOARD_SIZE_Y && col >= 0;row++,col--) {
		if(Board[row][col] != 0 && Board[row][col] != sel)
			count++;
		else
			break;
	}
	if(row < BOARD_SIZE_Y && col >= 0 && count > 0 && Board[row][col] == sel) {
		for(row = po.x+1,col = po.y-1;row < BOARD_SIZE_Y && col >= 0 && count > 0;row++,col--) {
			Board[row][col] = sel;
			count--;
		}
	}

	//�E���ɂ���
	count = 0;
	for(row = po.x+1,col = po.y+1;row < BOARD_SIZE_Y && col < BOARD_SIZE_X;row++,col++) {
		if(Board[row][col] != 0 && Board[row][col] != sel)
			count++;
		else
			break;
	}
	if(row < BOARD_SIZE_Y && col < BOARD_SIZE_X && count > 0 && Board[row][col] == sel) {
		for(row = po.x+1,col = po.y+1;row < BOARD_SIZE_Y && col < BOARD_SIZE_X && count > 0;row++,col++) {
			Board[row][col] = sel;
			count--;
		}
	}

	return;
}

//�\���p
void Disp::Show_Board(HDC hdc)
{
	HBRUSH hBrush;
	HPEN hPen;

	//�Ֆʂ̕\��
	hBrush = CreateSolidBrush(RGB(242,116,140)); //�u���V���g��
	SelectObject(hdc,hBrush);
	hPen = CreatePen(PS_DASH,3,RGB(124,83,53));
	SelectObject(hdc,hPen); //�y�����g��
	for(int i = 0;i < BOARD_SIZE_Y;i++) {
		for(int j = 0;j < BOARD_SIZE_X;j++) {
			if(Sharp_Board[i][j]) {
				Rectangle(hdc,50+j*SEL_SIZE,20+10+i*SEL_SIZE,50+j*SEL_SIZE+SEL_SIZE,20+10+i*SEL_SIZE+SEL_SIZE);
			}
		}
	}
	DeleteObject(hPen); //�y���̔j��
	DeleteObject(hBrush); //�u���V�̔j��

	//�Ֆʂ����点��
	hBrush = CreateSolidBrush(RGB(0,255,0));
	SelectObject(hdc,hBrush);
	hPen = CreatePen(PS_SOLID,1,RGB(0,255,0));
	SelectObject(hdc,hPen);
	for(int i = 0;i < BOARD_SIZE_Y;i++) {
		for(int j = 0;j < BOARD_SIZE_X;j++) {
			if(Flash_Board[i][j])
				Ellipse(hdc,70+j*SEL_SIZE,20+30+i*SEL_SIZE,70+j*SEL_SIZE+20,20+30+i*SEL_SIZE+20);
		}
	}
	DeleteObject(hPen);
	DeleteObject(hBrush);
	//�΂̕\��
	for(int i = 0;i < BOARD_SIZE_Y;i++) {
		for(int j = 0;j < BOARD_SIZE_X;j++) {
			if(G_Disp_Board[i][j] == YOU) {
				hBrush = CreateSolidBrush(RGB(25,25,25));
				SelectObject(hdc,hBrush);
				hPen = CreatePen(PS_SOLID,1,RGB(25,25,25));
				SelectObject(hdc,hPen);
				Ellipse(hdc,50+j*SEL_SIZE,20+10+i*SEL_SIZE,50+j*SEL_SIZE+SEL_SIZE,20+10+i*SEL_SIZE+SEL_SIZE);
				DeleteObject(hPen);
				DeleteObject(hBrush);
			}
			if(G_Disp_Board[i][j] == AI) {
				hBrush = CreateSolidBrush(RGB(250,250,250));
				SelectObject(hdc,hBrush);
				hPen = CreatePen(PS_SOLID,1,RGB(250,250,250));
				SelectObject(hdc,hPen);
				Ellipse(hdc,50+j*SEL_SIZE,20+10+i*SEL_SIZE,50+j*SEL_SIZE+SEL_SIZE,20+10+i*SEL_SIZE+SEL_SIZE);
				DeleteObject(hPen);
				DeleteObject(hBrush);
			}
		}
	}

	return;
}
//�X�R�A�̕\��
void  Disp::Show_Score(HDC hdc)
{
	RECT rect;
	static char stone_score[30];
	int you = 0,cpu = 0;

	rect.top = 450;
	rect.bottom = 600;
	rect.left = 500;
	rect.right = 800;

	for(int i = 0;i < BOARD_SIZE_Y;i++) {
		for(int j = 0;j < BOARD_SIZE_X;j++) {
			if(G_Disp_Board[i][j] == YOU)
				you++;
			if(G_Disp_Board[i][j] == AI)
				cpu++;
		}
	}

	wsprintf(stone_score,"You:%d,Cpu:%d",you,cpu);

	DrawText(hdc,stone_score,-1,&rect,DT_CENTER);

	return;
}

