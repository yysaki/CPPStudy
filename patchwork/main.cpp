#include <iostream>
#include <fstream>
#include <time.h>    // clock, CLOCKS_PER_SEC


#include "Item.h"
#include "ST.h"

//MaxX,Y‚ÌéŒ¾‚ÍST.h‚Å

using namespace std;

int main();

int main(){
	clock_t start, end;
	start = clock();

    const char fname[256] = "input.txt";

/////////////////////////////init
	ST st;
	st.init(fname);
	struct Item item = Item(Point(0,1),Key('A',-1));

/////////////////////////////search & black out

	int max;
	max = st.getMaxArea();

	cout << "////max = " << max << "///" << endl;	
	st.blackOut(max);

/////////////////////////////output ans

	st.dumpArea(std::cout);
	st.getAns(std::cout, max);

	end = clock();
	cout << "--- exec time: " << end-start << "---" << endl;
	
	st.outputAns(max);
	st.output("out.csv", max, end-start);
	return 0;
}
