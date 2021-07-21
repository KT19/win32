#include"Define.h"

//初期化
class Init
{
public:
	void Initialize(void); //初期化
};
//敵側
class CPU
{
private:
	POINT Where_Put(void); //何処に置くか
public:
	void Turn(HWND,bool *,bool *); //敵の番
	void run(HWND,bool *,bool *);
};

//自分
class Player
{
public:
	bool Turn(void); //自分の番
	void Possible(POINT); //置く
};

//システム
class System
{
public:
	int Which_Win(void); //どっちが勝ったか
	void Blight_Put(int); //置ける場所を光らせる
	bool Game_Over(void); //ゲームが終わったか
	bool Can_Put(POINT,int); //置けるか
	void Turn_Over(POINT,int); //ひっくり返す
	void Play(int); //処理
};

//表示用
class Disp
{
public:
	void Show_Board(HDC); //盤面の表示
	void Show_Score(HDC); //スコアの表示
};