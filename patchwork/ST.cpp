#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

#include "ST.h"

//using namespace std;

/*
* 使用コンストラクタ
*/
ST::ST(){
	_a = new Item[_MaxX][_MaxY];
}

ST::ST(int x, int y){
//	_MaxX = x; _MaxY = y;
	_a = new Item[_MaxX][_MaxY];
}


ST::~ST(){
	delete[] _a;
}

void ST::init(const char fname[]){
	printf("-----ST::init-----\n");
	std::ifstream ifs(fname);
	char str[_MaxY+1];

	if(!ifs.is_open()){
		printf("%f is not found\n", fname);
		exit(1);
	}

	int i,j;
	int notChecked = -1;
	for(i=0;i<_MaxX;i++){
		ifs.getline(str,_MaxY+2);
		for(j=0;j<_MaxY;j++){
			_a[i][j].setInt(i, j, notChecked);
			_a[i][j].setSymbol(str[j]);
			printf("%c", _a[i][j].key.first);
//			_a[i][j].dumpItem(std::cout);
		}
		printf("\n");
	}
	ifs.close();

}

/*
* 参照渡しでのItemのArea変更
*/
//void ST::setArea(Item& item, int target){
void ST::setArea(Point point, int target){
	_a[point.first][point.second].setArea(target);
//	getItem(point).setArea(target);
}
void ST::setSymbol(Point point, char target){
	_a[point.first][point.second].setSymbol(target);
}

Item ST::getHead(){
	printf("-----ST::getHead-----\n");
	return _a[0][0];
}

/*
* 着目するPointの上下左右にあるPointを返す
* 配列外に出る時は自身を返す
*/
Point ST::getUp(Point& point){
	if(point.first==0){ return point; }
	return Point(point.first-1,point.second);
}
Point ST::getDown(Point& point){
	if(point.first==_MaxX-1){ return point; }
	return Point(point.first+1,point.second);
}
Point ST::getLeft(Point& point){
	if(point.second==0){ return point; }
	return Point(point.first,point.second-1);
}
Point ST::getRight(Point& point){
	if(point.second==_MaxY-1){ return point; }
	return Point(point.first,point.second+1);
}

/*
* 動作テスト

void ST::testMove(){
	printf("-----ST::testMove-----\n");
	struct Item itm = getHead(); cout << "getHead" << endl; itm.dumpItem(cout);
	itm=getDown(itm); cout << "getDown" << endl; itm.dumpItem(cout);
	itm=getRight(itm); cout << "getRight" << endl; itm.dumpItem(cout);
	itm=getDown(itm); cout << "getDown" << endl; itm.dumpItem(cout);
	itm=getRight(itm); cout << "getRight" << endl; itm.dumpItem(cout);
	itm=getDown(itm); cout << "getDown" << endl; itm.dumpItem(cout);
	itm=getRight(itm); cout << "getRight" << endl; itm.dumpItem(cout);
	itm=getDown(itm); cout << "getDown" << endl; itm.dumpItem(cout);
	itm=getRight(itm); cout << "getRight" << endl; itm.dumpItem(cout);
	itm=getDown(itm); cout << "getDown" << endl; itm.dumpItem(cout);
	itm=getRight(itm); cout << "getRight" << endl; itm.dumpItem(cout);
	itm=getUp(itm); cout << "getUp" << endl; itm.dumpItem(cout);
	itm=getUp(itm); cout << "getUp" << endl; itm.dumpItem(cout);
	itm=getUp(itm); cout << "getUp" << endl; itm.dumpItem(cout);
	itm=getUp(itm); cout << "getUp" << endl; itm.dumpItem(cout);
	itm=getUp(itm); cout << "getUp" << endl; itm.dumpItem(cout);
	itm=getUp(itm); cout << "getUp" << endl; itm.dumpItem(cout);
}
*/
Item ST::getItem(Point pt){
	return _a[pt.first][pt.second];
}
char ST::getSymbol(Point pt){
	return _a[pt.first][pt.second].key.first;
}

//TODO
bool ST::checkItem(Point pt, Key key){
	printf("-----ST::checkItem-----\n");
	if(_a[pt.first][pt.second].key==key){
		return true;
	} else {
		return false;
	}
}

//TODO
void ST::output(const char fname[], int max, int time){
	printf("-----ST::output-----\n");
	std::ofstream ofs(fname);
	ofs << "max: "  << max << std::endl;
	ofs << "exec time: " << time << std::endl;
	ofs << "original: " << std::endl;
	int i,j;
	for(i=0;i<_MaxX;i++){
		for(j=0;j<_MaxY;j++){
			ofs << _a[i][j].key.first << ", " ;
		}
		ofs << "\b\b  " << std::endl;
	}
	ofs << "blackedArea: " << std::endl;
	ST::dumpArea(ofs);
	ofs << "ans: " << std::endl;
	ST::getAns(ofs, max);
	ofs.close();
}

void ST::outputAns(int max){
	printf("-----ST::outputAns-----\n");
	std::ofstream ofs("out.txt");
	ST::getAns(ofs, max);
	ofs.close();
}

/*
* 最大面積maxを取得し面積maxの全領域に印をつける
*/
int ST::getMaxArea(){
	printf("-----ST::getMaxArea-----\n");
	int i,j;
	int n = 0;
	struct Item head = ST::getHead();
	for(i=0;i<_MaxX;i++){
		for(j=0;j<_MaxY;j++){
			if(!_a[i][j].isChecked()){
				n = std::max(n, ST::getArea(Point(i,j), n));
			}
		}
	}
	return n;
}
/*
* currentPtの地点を含む領域の面積を取得
* 面積がその時点でのmax以上ならば領域に印をつける
*/
int ST::getArea(Point currentPt, int n){
//	printf("-----ST::getArea-----\n");
	int tmp = 0;
	char currentSymbol = ST::getItem(currentPt).key.first;
	printf("-ST::getArea find phase-----\n");
	tmp = ST::searchAndReplace(currentPt, Key(currentSymbol, -1), 0);
	printf("tmp = %d\n", tmp);
	if(n <= tmp){
		printf("-ST::getArea replace phase-----\n");
		ST::searchAndReplace(currentPt, Key(currentSymbol, 0), tmp);
		n = tmp;
	}
	printf("-ST::getArea::end ans = %d -\n", n);	
	return n;
}
/*
* currentPtの周囲1マスのうち、探索キーを持つものを探す
* キーを持つならば印をつけた後再帰呼び出し
*/
int ST::searchAndReplace(Point pt, Key searchKey, int target){
	printf("-----ST::searchAndReplace-----\n");
	int count = 0;
	
	_a[pt.first][pt.second].dumpItem(std::cout);
	ST::setArea(pt, target);

	struct Item item = ST::getItem(pt);
	item.dumpItem(std::cout);
	if(ST::getItem(ST::getUp(pt)).hasKey(searchKey)){
		count += ST::searchAndReplace(ST::getUp(pt), searchKey, target);
	}
	if(ST::getItem(ST::getDown(pt)).hasKey(searchKey)){
		count += ST::searchAndReplace(ST::getDown(pt), searchKey, target);
	}
	if(ST::getItem(ST::getLeft(pt)).hasKey(searchKey)){
		count += ST::searchAndReplace(ST::getLeft(pt), searchKey, target);
	}
	if(ST::getItem(ST::getRight(pt)).hasKey(searchKey)){
		count += ST::searchAndReplace(ST::getRight(pt), searchKey, target);
	}
	return count+1;
}

/*
* 面積maxの領域を塗りつぶし
*/
void ST::blackOut(int max){
	printf("-----ST::blackOut-----\n");
	int i,j;
	for(i=0;i<_MaxX;i++){
		for(j=0;j<_MaxY;j++){
			if(_a[i][j].key.second==max){
				ST::setSymbol(Point(i,j),'_');
			}
		}
	}
}


std::ostream& ST::dumpArea(std::ostream& s) const{
	printf("-----ST::dumpArea-----\n");
	int i,j;
	for(i=0;i<_MaxX;i++){
		for(j=0;j<_MaxY;j++){
			s << std::setw(3) << _a[i][j].getArea() << ", ";

		}
		s << std::endl;
	}

	return s;
}

std::ostream& ST::getAns(std::ostream& s, int max) const{
	printf("-----ST::getAns-----\n");
	int i,j;
	for(i=0;i<_MaxX;i++){
		int count= 0;
		for(j=0;j<_MaxY;j++){
			if(_a[i][j].getArea()==max){ count++; }
		}
		s << count << std::endl;
	}
	return s;
}
