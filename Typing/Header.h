//-----------------Header.h
//情報の表示
void DispScore(HDC hdc,int combo,int rest_time);
//タイプ文字の表示
void DispType(HDC hdc);
//初期化
void Init(void);
//間違った文字のカウント
void Count_Miss_Type(char str);
//文字を落とす速度を変える
void Change_FallType(int *speed,int time);
//ミスの判定
BOOL MissType(char type);
//タイプ文字の生成(位置、文字)
BOOL Generate_Type(int *random,char str);
//正しく押せたか判定
BOOL Check_Type(char str,int combo);
//得点の加算
void AddScore(int combo);
//文字の位置のセット
void Set_Iti(int iti);
//文字を落とす
void FallType(int speed);
//GameOver
void End(HDC hdc);
//色々表示
void Disp_Explain(HDC hdc);
//開始
void Start(void);
//カウントダウン
void Count_Down(DWORD time);
//カウント停止
void LetsStart(void);
//カウントダウンの表示
void Disp_Count(HDC hdc);