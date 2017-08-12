/* 
	viewport entity
*/

#ifndef WINDOW_H
#define WINDOW_H

class Window{
	private:
		int x_min, y_min, x_max, y_max;

	public:		
		void setXMin(int x_min);
		void setYMin(int y_min);
		void setXMax(int x_max);
		void setYMax(int y_max);
		int getXMin();
		int getXMax();
		int getYMin();
		int getYMax();	
};

#endif