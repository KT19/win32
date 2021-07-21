//--------------------Define.h
//定数
#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)
#define WINDOW_X ((GetSystemMetrics(SM_CXSCREEN)-WINDOW_WIDTH)/2)
#define WINDOW_Y ((GetSystemMetrics(SM_CYSCREEN)-WINDOW_HEIGHT)/2)
#define TYPE_MAX (10) //一度に表示する文字の最大数
#define ALPHABET (26) //アルファベットの数
#define SCORE_MAX (6) //スコアの桁
#define COMBO_BONUS (10) //コンボによって点数の加算
#define TYPE_FONT (32) //フォントの大きさ
#define INF_FONT (14) //情報のフォント
#define TYPE_OK_LEFT (5) //タイプ有効領域
#define TYPE_OK_RIGHT (650) //タイプ有効領域
#define TYPE_OK_TOP (400) //タイプ有効良識
#define TYPE_OK_BOTTOM (500) //タイプ領域
#define TYPE_FIELD_OK(ok) (ok.bottom >= TYPE_OK_TOP && ok.top <= TYPE_OK_BOTTOM) //有効領域
#define TIMER_ID (1) //タイマID
#define CYCLE (30)