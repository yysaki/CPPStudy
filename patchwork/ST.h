#ifndef _INCLUDED_ST
#define _INCLUDED_ST

#define _MaxX 600
#define _MaxY 600

#include <string>
#include <utility>

#include "Item.h"


/*
*	記号表_a: item型の二次元配列 の操作
*   { {0,0,'A',-1}, {0,1,'A',-1}, ...}
*/
class ST{
private:
//	int _MaxX, _MaxY;
	struct Item (*_a)[_MaxY];

public:
	ST();
	//コンストラクタ 
    ST(int x, int y);
	//デストラクタ _aをdelete
    ~ST();

	void init(const char fname[]);
	void output(const char fname[], int max, int time);
	void outputAns(int max);

	void setArea(Point point, int target);
	void setSymbol(Point point, char target);

/*
* 着目するPointの上下左右にあるPointを返す
* 配列外に出る時は自身を返す
*/
	Point getUp(Point& point);
	Point getDown(Point& point);
	Point getLeft(Point& point);
	Point getRight(Point& point);

/*
* 動作テスト
	void testMove();
*/
/*
* _a[0][0]を返す
*/
	Item getHead();
/*
* a[x][y]を返す
*/
	Item getItem(Point pt);
	char getSymbol(Point pt);

/*
* _a[x][y]が{symbol, area}を持つときTrue
*/
	bool checkItem(Point pt, Key key);

/*
* 最大面積maxを取得し面積maxの全領域に印をつける
*/
	int getMaxArea();
/*
* currentPtの地点を含む領域の面積を取得
* 取得面積がその時点でのmax以上ならば領域に印をつける
*/
	int getArea(Point currentPt, int n);
/*
* currentPtの周囲1マスのうち、探索キーを持つものを探す
* キーを持つならば印をつけた後再帰呼び出し
*/
	int searchAndReplace(Point pt, Key searchKey, int target);
/*
* 面積maxの領域を塗りつぶし
*/
	void blackOut(int max);
/*
* Areaの出力
*/
	std::ostream& dumpArea(std::ostream& s) const;
/*
* 解答形式で出力
*/
	std::ostream& getAns(std::ostream& s, int max) const;
};


#endif //_INCLUDED_ST
