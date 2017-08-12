/* 
	generic view entity
*/

#ifndef VIEW_H
#define VIEW_H

class View{
	private:
		double x_min, y_min, x_max, y_max;

	public:		
		void setXMin(double x_min);
		void setYMin(double y_min);
		void setXMax(double x_max);
		void setYMax(double y_max);
		double getXMin();
		double getXMax();
		double getYMin();
		double getYMax();	

		double transformX(double x_w, View window);
		double transformY(double y_w, View window);
};

#endif