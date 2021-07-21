//----------------System.cpp
#ifndef READ
#include<windows.h>
#include<tchar.h>
#include"Global.h"
#include"Define.h"
#endif

//グローバル変数
extern char G_Type_String[TYPE_MAX+1]; //タイプする文字
extern char G_Score[SCORE_MAX+1]; //スコアの表示
extern int G_Miss_Type[ALPHABET+1]; //間違った文字のカウント
extern RECT G_Dis_Rect[TYPE_MAX+1]; //表示文字の座標
extern char G_Explain[1000]; //説明文
extern char G_Count_Down[3]; //カウントダウン

//情報の表示
void DispScore(HDC hdc,int combo,int rest_time)
{
	char val[100];
	RECT rc;

	rc.top = 200;
	rc.bottom = 300;
	rc.left = 630;
	rc.right = 800;

	//残り時間は右上に
	if(rest_time/60000 != 0) {
	wsprintf(val,"残り\n%d分%d秒",rest_time/60000,(rest_time%60000)/1000);
	} else {
		wsprintf(val,"残り\n%d秒",(rest_time%60000)/1000);
	}
	SetTextColor(hdc,RGB(255,0,0));
	DrawText(hdc,val,-1,&rc,DT_CENTER);

	//スコアは右真ん中に
	rc.top = 350;
	rc.bottom = 450;
	SetTextColor(hdc,RGB(0x00,0x64,0x00));
	DrawText(hdc,G_Score,-1,&rc,DT_CENTER);
	//Scoreという文字をスコアの左上に
	rc.top = 330;
	rc.bottom=430;
	DrawText(hdc,"Score:",-1,&rc,DT_LEFT);

	//コンボ数は右下に
	SetTextColor(hdc,RGB(0,0,255));
	rc.top = 500;
	rc.bottom = 600;
	wsprintf(val,"combo\n%d",combo);
	DrawText(hdc,val,-1,&rc,DT_CENTER);

	return;
}

//タイプ文字の表示
void DispType(HDC hdc)
{
	char val[10];
	int i;

	//タイプ文字列を表示
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

//初期化
void Init(void)
{
	int i;

	//表示文字
	for(i = 0;i < TYPE_MAX+1;i++)
		G_Type_String[i] = '\0';
	//表示スコア
	for(i = 0;i < SCORE_MAX+1;i++)
		G_Score[i] = '0';
	//ミスタイプ
	for(i = 0;i < ALPHABET+1;i++)
		G_Miss_Type[i] = 0;
	//位置の座標
	for(i = 0;i < TYPE_MAX;i++)
		G_Dis_Rect[i].top = G_Dis_Rect[i].bottom = G_Dis_Rect[i].left = G_Dis_Rect[i].right = 0;

	//説明文
	wsprintf(G_Explain,"CTRL+Sでスタート\nCTRL+Qで終了\n下のタイプ領域でのみタイプ可能(文字の色が変わったときのみ)\n残り時間やスコアなどは⇒\n途中で落下速度が速くなる\nタイプ領域内で押せなくてもスコアにならないだけだが、\nミスするとミスをカウント+コンボがなくなる\n3分で終了\n小文字、大文字の区別はなし");

	//カウントダウン
	wsprintf(G_Count_Down,"");

	//ミス
	G_Miss_Num = 0;

	return;
}

//間違った文字のカウント
void Count_Miss_Type(char str)
{

	//カウント
	G_Miss_Type[str - 'A']++;

	return;
}

//文字を落とす速度を変える
void Change_FallType(int *speed,int time) //(速度,時間)
{
	*speed = 500/time*CYCLE/1000;

	return;
}

//ミスの判定
BOOL MissType(char type)
{
	int i;
	
	for(i = 0;i < TYPE_MAX;i++) {
		if(TYPE_FIELD_OK(G_Dis_Rect[i]) && G_Type_String[i] == type)  //表示文字列にある
			return TRUE;
	}

	return FALSE;
}

//タイプ文字の生成
BOOL Generate_Type(int *random,char str) //(位置、文字)
{
	int check,i;
	
	//適当な位置に適当な文字を生成
	for(i = 0;i < TYPE_MAX;i++) 
		if(G_Type_String[i] == str)
			return FALSE;

	//その場所にすでに置かれていたら次の位置
	for(check = 0;G_Type_String[*random%TYPE_MAX] != '\0';++(*random))
	{
		check++;

		if(check == TYPE_MAX)
			break;
	}

	//全ての位置が埋まっているとき
	if(check == TYPE_MAX)
		return FALSE; //生成失敗
 
	G_Type_String[*random%TYPE_MAX] = str; //文字セット

	return TRUE;
}

//得点の加算
void AddScore(int combo)
{
	int score;

	score = atoi(G_Score); //得点(文字->数値)
	score += 1+combo*(combo/COMBO_BONUS); //加算(1type1点+10コンボ数/2を加算)
	if(score >= 999999)
		score = 999999;
	wsprintf(G_Score,"%d",score); //得点(数値->文字)

	return;
}

//正しく押せたか判定
BOOL Check_Type(char str,int combo)
{
	int i;

	if(MissType(str)) { //正しく押せた
		for(i = 0;i < TYPE_MAX;i++) {
			if(G_Type_String[i] == str) {
			
				AddScore(combo); //スコアの加算
				G_Type_String[i] = '\0'; //空にする
				G_Dis_Rect[i].left = G_Dis_Rect[i].right = G_Dis_Rect[i].top = G_Dis_Rect[i].bottom = 0; //位置のリセット
				
			}
		}

		return TRUE;
	} else { //正しく押せてない
		if(str - 'A' >= 0 && str-'A' < ALPHABET)
			Count_Miss_Type(str); //間違ったキーを加算
		G_Miss_Num++;
	}

	return FALSE;
}


//文字の位置をセット
void Set_Iti(int iti)
{
	G_Dis_Rect[iti].left = 10+iti*55-2;
	G_Dis_Rect[iti].right = 10+iti*55+TYPE_FONT+2;
	G_Dis_Rect[iti].top = 10;
	G_Dis_Rect[iti].bottom = 10+TYPE_FONT;

	return;
}

//文字を落とす
void FallType(int speed)
{
	int i;

	for(i = 0;i < TYPE_MAX;i++) {
		if(G_Dis_Rect[i].top != 0) {
			G_Dis_Rect[i].top += speed;
			G_Dis_Rect[i].bottom += speed;
			if(G_Dis_Rect[i].top >= TYPE_OK_BOTTOM + 10) { //落とした結果タイプ領域の外に出た
				G_Type_String[i] = '\0'; //空にする
				G_Dis_Rect[i].left = G_Dis_Rect[i].right = G_Dis_Rect[i].top = G_Dis_Rect[i].bottom = 0; //位置のリセット
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


	//タイプ文字を消す
	for(i = 0;i < TYPE_MAX;i++)
		G_Type_String[i] = '\0';

	rc.top = 20;
	rc.bottom = 70;
	rc.left = 50;
	rc.right = 450;

	wsprintf(ex,"Score:%s\nミスタイプ:%d",G_Score,G_Miss_Num);
	DrawText(hdc,ex,-1,&rc,DT_CENTER);

	rc.top = 100;
	rc.bottom = 150;
	wsprintf(ex,"それぞれのミス数↓");
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

//色々表示
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

//開始する準備
void Start(void)
{
	wsprintf(G_Explain,"");

	return;
}

//カウントダウン
void Count_Down(DWORD time)
{
	wsprintf(G_Count_Down,"%d",3-(time/1000));

	return;
}

//カウント停止
void LetsStart(void)
{
	wsprintf(G_Count_Down,"");

	return;
}

//カウントダウンの表示
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