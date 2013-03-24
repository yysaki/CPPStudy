#ifndef _INCLUDED_ST
#define _INCLUDED_ST

#define _MaxX 600
#define _MaxY 600

#include <string>
#include <utility>

#include "Item.h"


/*
*	�L���\_a: item�^�̓񎟌��z�� �̑���
*   { {0,0,'A',-1}, {0,1,'A',-1}, ...}
*/
class ST{
private:
//	int _MaxX, _MaxY;
	struct Item (*_a)[_MaxY];

public:
	ST();
	//�R���X�g���N�^ 
    ST(int x, int y);
	//�f�X�g���N�^ _a��delete
    ~ST();

	void init(const char fname[]);
	void output(const char fname[], int max, int time);
	void outputAns(int max);

	void setArea(Point point, int target);
	void setSymbol(Point point, char target);

/*
* ���ڂ���Point�̏㉺���E�ɂ���Point��Ԃ�
* �z��O�ɏo�鎞�͎��g��Ԃ�
*/
	Point getUp(Point& point);
	Point getDown(Point& point);
	Point getLeft(Point& point);
	Point getRight(Point& point);

/*
* ����e�X�g
	void testMove();
*/
/*
* _a[0][0]��Ԃ�
*/
	Item getHead();
/*
* a[x][y]��Ԃ�
*/
	Item getItem(Point pt);
	char getSymbol(Point pt);

/*
* _a[x][y]��{symbol, area}�����Ƃ�True
*/
	bool checkItem(Point pt, Key key);

/*
* �ő�ʐ�max���擾���ʐ�max�̑S�̈�Ɉ������
*/
	int getMaxArea();
/*
* currentPt�̒n�_���܂ޗ̈�̖ʐς��擾
* �擾�ʐς����̎��_�ł�max�ȏ�Ȃ�Η̈�Ɉ������
*/
	int getArea(Point currentPt, int n);
/*
* currentPt�̎���1�}�X�̂����A�T���L�[�������̂�T��
* �L�[�����Ȃ�Έ��������ċA�Ăяo��
*/
	int searchAndReplace(Point pt, Key searchKey, int target);
/*
* �ʐ�max�̗̈��h��Ԃ�
*/
	void blackOut(int max);
/*
* Area�̏o��
*/
	std::ostream& dumpArea(std::ostream& s) const;
/*
* �𓚌`���ŏo��
*/
	std::ostream& getAns(std::ostream& s, int max) const;
};


#endif //_INCLUDED_ST
