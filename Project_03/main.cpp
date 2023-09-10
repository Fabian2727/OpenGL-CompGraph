//RENDERING CUBE
#include <iostream>
//#include <stdlib.h>
using namespace std;

const int N = 20;
const int M = 20;
const int S = 2;
char mat [S*N][S*M];

class point3d {
  private:
    int x3d;
    int y3d;
    int z3d;
  
  public:
    point3d (int x_val = 0, int y_val = 0, int z_val = 0) 
	{
        x3d = x_val;
        y3d = y_val;
        z3d = z_val;
    }

    int getX3d() const 
	{
        return x3d;
    }

    int getY3d() const 
	{
        return y3d;
    }

    int getZ3d() const 
	{
        return z3d;
    }

    void setX3d(int x_val) 
	{
        x3d = x_val;
    }
	
	void setall3d(int x, int y, int z)
	{
		x3d = x;
		y3d = y;
		z3d = z;
	}
	
    void setY3d(int y_val) 
	{
        y3d = y_val;
    }

    void setZ3d(int z_val) 
	{
        z3d = z_val;
    }
};

class point2d {
  private:
    int x2d;
    int y2d;
  public:
    point2d (int x_val = 0, int y_val = 0) 
	{
        x2d = x_val;
        y2d = y_val;
    }

    int getX2d() const 
	{
        return x2d;
    }

    int getY2d() const 
	{
        return y2d;
    }

    void setX2d(int x_val) 
	{
        x2d = x_val;
    }

    void setY2d(int y_val) 
	{
        y2d = y_val;
    }
};

void drawLineDown(int x0, int y0, int x1, int y1)
{
	//líneas que empiezan en el origen con una pendiente entre 0 y 1
	
	int dx = x1 - x0; //distancias que separan los puntos extremos de la línea sobre cada eje
    int dy = y1 - y0;
    int yi = 1;	//desplazamiento del eje
    if (dy < 0)
    {
    	yi = -1;
        dy = -dy;
	}
    int D = (2 * dy) - dx;
    int y = y0;

    for (int x = x0; x<x1; x++)
    {
    	mat[x][y] = '*';
        if (D > 0)
        {
        	y = y + yi;
            D = D + (2 * (dy - dx));
		} 
        else
        {
        	D = D + 2*dy;
		}
	}
}

void drawLineTop(int x0, int y0, int x1, int y1)
{
	//Cambiando los ejes x e y se puede escribir una aplicación para pendientes pronunciadas positivas o negativas como
	
	int dx = x1 - x0; //distancias que separan los puntos extremos de la línea sobre cada eje
    int dy = y1 - y0; 
    int xi = 1;
    if (dx < 0)
    {
    	xi = -1;
        dx = -dx;
	}
    int D = (2 * dx) - dy;
    int x = x0;

    for (int y = y0; y<y1; y++)
    {
    	mat[x][y] = '*';
        if (D > 0)
        {
        	x = x + xi;
            D = D + (2 * (dx - dy));
		}
        else
        {
        	D = D + 2*dx;
		}
	}
}
    
void drawLine(int x0, int y0, int x1, int y1)
{
	//necesita detectar si x1 > x0 o y1 > y0 e invertir las coordenadas de entrada antes de dibujar
	if (abs(y1 - y0) < abs(x1 - x0))
	{
		if (x0 > x1) //Se busca cuál punto es mayor en X	El trazado se realiza sobre un bucle, elegido para el eje de mayor medida
		{
			drawLineDown(x1, y1, x0, y0);
		}
        else
        {
        	drawLineDown(x0, y0, x1, y1);
		}
	} 
    else
    {
    	if (y0 > y1) //Se busca cuál punto es mayor en Y	El trazado se realiza sobre un bucle, elegido para el eje de mayor medida
    	{
    		drawLineTop(x1, y1, x0, y0);
		}
        else
        {
        	drawLineTop(x0, y0, x1, y1);
		}
	}   
	//Después de dibujar en ese punto, se corre una posicion más para volver a graficar en el siguiente punto
}
    

void convert (int *x, int *y, int *z, int c1, int c2, int c3)
{
	//cout << *x << " " << *y << " " << *z << endl;
	int div = c3 - *z;
	*x = ((c1- (*x)) * (N/ div));
	*x = N - *x;
	//cout << c1- (*x) << " ";
	*y = ((c2- *y) * (M/div));
	//*y = M - *y;
	//cout << c2- *y << " ";
	*z = ((c3- *z) / (c3 - *z));
	//cout << c3- *x << " ";
	
	//cout << endl;
	//cout << *x << " " << *y << " " << *z << endl;
}

void print(int x, int y, int z)
{
	cout << x << " " << y << " " << z << endl;
}

int main(){
	
	int camx = -2 ,camy = 2 ,camz = 5;
	char key; int x;
	
	while (true)
	{
		int ax = 1, ay = 1 , az = 1;	int ex = 1, ey = 1 , ez = -1;
		int bx = -1, by = 1 , bz = 1;	int fx = -1, fy = 1 , fz = -1;
		int cx = 1, cy = -1 , cz = 1;	int gx = 1, gy = -1 , gz = -1;
		int dx = -1, dy = -1 , dz = 1;	int hx = -1, hy = -1 , hz = -1;
		
		convert(&ax,&ay,&az,camx,camy,camz);
		convert(&bx,&by,&bz,camx,camy,camz);
		convert(&cx,&cy,&cz,camx,camy,camz);
		convert(&dx,&dy,&dz,camx,camy,camz);
		convert(&ex,&ey,&ez,camx,camy,camz);
		convert(&fx,&fy,&fz,camx,camy,camz);
		convert(&gx,&gy,&gz,camx,camy,camz);
		convert(&hx,&hy,&hz,camx,camy,camz);
		
		for (int i = 0; i<S*N; i++)
		{
			for (int j = 0; j<S*M; j++)
			{
				if((i == ax && j == ay) || (i == bx && j == by) || (i == cx && j == cy) || (i == dx && j == dy) || (i == ex && j == ey) || (i == fx && j == fy) || (i == gx && j == gy) || (i == hx && j == hy))
				{
					mat[i][j] = '*';
				}
				else
				{
					mat[i][j] = ' ';
				}
			}
		}

		drawLine(ax,ay,bx,by);
		drawLine(ax,ay,cx,cy);
		drawLine(cx,cy,dx,dy);
		drawLine(gx,gy,hx,hy);
		drawLine(ex,ey,fx,fy);
		drawLine(ex,ey,gx,gy);
		drawLine(fx,fy,hx,hy);
		drawLine(bx,by,dx,dy);
		drawLine(cx,cy,gx,gy);
		drawLine(dx,dy,hx,hy);
		drawLine(ax,ay,ex,ey);
		drawLine(bx,by,fx,fy);
		
		for (int i = 0; i<S*N; i++)
		{
			for (int j = 0; j<S*M; j++)
			{
				cout << mat[i][j]<<" ";
			}
			cout << endl;
		}
	//	print(ax,ay,az);	print(bx,by,bz);	print(cx,cy,cz);	print(dx,dy,dz);
	//	print(ex,ey,ez);	print(fx,fy,fz);	print(gx,gy,gz);	print(hx,hy,hz);

    cout << "Ingrese la accion deseada: "; cin>> key;	
		if (key == 'q' or key == 'Q')
		{
			camy++;
		}
		else if (key == 'a' or key == 'A')
		{
			camy--;
		}
		else if (key == 'w' or key == 'W')
		{
			camx++;
		}
		else if (key == 's' or key == 'S')
		{
			camx--;
		}
		else if (key == 'e' or key == 'E')
		{
			camz++;
		}
		else if (key == 'd' or key == 'D')
		{
			camz--;
		}	
		system("cls");
	}
}