//---------------------Global.h
#include"Define.h"
char G_Type_String[TYPE_MAX+1]; //タイプする文字
char G_Score[SCORE_MAX+1]; //スコアの表示
int G_Miss_Type[ALPHABET+1]; //間違った文字のカウント
RECT G_Dis_Rect[TYPE_MAX+1]; //表示文字の座標
char G_Explain[1000]; //説明文
char G_Count_Down[3]; //カウントダウン
int G_Miss_Num; //ミスした数