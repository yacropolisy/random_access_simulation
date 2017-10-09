#ifndef INCLUDED_const_h
#define INCLUDED_const_h

#define ln2 0.693147
#define DeltaT 100 //単位時間　ms
#define T (3600*1000/DeltaT)  //　シミュレーション時間
#define M 144   //基地局数
#define N 1000  //端末数
#define TOP (T/S * 10)  //制御を行う基地局と時間スロットの組み合わせ数
#define Backoff_Indicator 20  //ランダムアクセスの衝突時のバックオフ時間の指標
#define S (60 * 1000 / DeltaT) //通信待機を行う時間スロット
#define Lambda_ed 0.0005    //Event Driven時のλ
#define Waittime (30 * 1000 / DeltaT)  //通信待機の時間

#define Interval0 (60 * 1000 / DeltaT)  //通信周期
#define Interval1 (15 * 1000 / DeltaT)
#define Interval2 (60 * 1000 / DeltaT)
#define Goal0 (60 * 1000 / DeltaT)  //目標値
#define Goal1 (90 * 1000 / DeltaT)
#define Goal2 (600 * 1000 / DeltaT)
#define Goal3 (3 * 1000 / DeltaT)
#define P0 1 //全車両に対するそのセンサ車両の割合
#define P1 0.05
#define P2 0.10
#define P3 0.15
/*
#define U00 60 * 1000 / DeltaT / Goal0  //効用関数モデルのU0
#define U01 60 * 1000 / DeltaT / Goal1
#define U02 60 * 1000 / DeltaT / Goal2
#define U03 60 * 1000 / DeltaT / Goal3
*/
#define U00 1
#define U01 1
#define U02 1
#define U03 1
#define Alpha0 (ln2/Goal0) //許容遅延、指数減衰の係数
#define Alpha1 (ln2/Goal1)
#define Alpha2 (ln2/Goal2)
#define Alpha3 (ln2/Goal3)

#define H 10  //taxiデータの読み込み開始時刻


#endif
