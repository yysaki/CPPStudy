#ifndef _INCLUDED_ITEM
#define _INCLUDED_ITEM

#include <iostream>
#include <utility>

/*
*	struct Item item = Item(Point(0,1),Key('A',-1));
*/

typedef std::pair<int, int> Point;
typedef std::pair<char, int> Key;

struct Item{
	/*
	* Point(x, y);
	* _a[x][y]�̈ʒu
	* int x, y (0<= x,y < MaxX,Y)
	*/
	Point point;
	/*
	* Key(symbol, area);
	* char symbol = {A,B}
	* int area = {�����l -1, �T���� 0, �ʐς̒l n(>0)
	*/
	Key key;
	Item(){};
	Item(Point point, Key key){
		this->point = point;
		this->key = key;
	}
	
	void setItem(Point point, Key key){
		this->point = point;
		this->key = key;
	}

	std::ostream& dumpItem(std::ostream& s) const{
		printf("-Item::dumpItem- ");
		s << "_a[" << point.first << "][" << point.second
			<< "]= {"<< key.first << ", " << key.second << "} " << std::endl;
		return s;
	}

	void setInt(int& x,int& y, int& area){
		point.first = x;	point.second = y;
		key.second = area;
	}

	void setSymbol(char& symbol){
		this->key.first = symbol;
	}

	void setArea(int& area){
		this->key.second = area;
	}
/*
* area!=-1�̎�True
*/
	bool isChecked(){
		if(key.second!=-1) {
			return true;
		}else{
			return false;
		}
	}
/*
* searchKey������True
*/
	bool hasKey(Key searchKey){
		printf("-Item::hasKey- ");
		printf("key.area = %d, searchKey.area = %d\n",key.second, searchKey.second);
		
		if(key==searchKey) {
			return true;
		}else{
			return false;
		}
	}


	Point getPoint(){
		return point;
	}

	int getArea(){
		return this->key.second;
	}

/*
* �[�ɒ�����Item�𔻒�
*/
	bool equals(Item item){
		if(point==item.point && key==item.key){
			return true;
		}else{
			return false;
		}
	}

};

#endif //_INCLUDED_ITEM
