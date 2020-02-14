/*
 * main.cpp
 *
 *  Created on: Mar 12, 2019
 *      Author: gilg
 */



#include <fstream>      // std::ifstream
#include <iostream>


#include <math.h>


#include <unistd.h>
#include <sys/time.h>


#include <GL/glut.h>


using namespace std;



suseconds_t differe(timeval tim1,timeval tim2)
{return (tim2.tv_sec - tim1.tv_sec)*1E6+tim2.tv_usec-tim1.tv_usec;}


inline double f(double x){return 0;}//initial conditions
inline double f1(double x){return 0;}//initial conditions

inline double g(double t){return 0;}//boundary conditions
inline double g1(double t){return 0;}//boundary conditions

double T	=10;
double l	=0.5;
double c	=0.2;

double tay	=0.00025;//0.01;//c
double h	=0.0005*1;//0.001;//0.1cm


long int		N=l/h+1;
long int		R=T/tay+1;


double **u;




void pressNormalKeys(unsigned char key, int xx, int yy)
{
        switch (key)
        {
              case 0x1b:
              {

            	  for(int k=0;k<R;k++){free(u[k]);}
            	  free(u);
            	  exit(255);}
        }
}


void display()// �������������� ���������������������� ��������������
{
	glClearColor(1.0, 1.0,1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	 glLoadIdentity();

	 glColor3ub(0,0,0);

	 static int k=0;

	 GLdouble line[N][2];


	 for(int i=0;i<N;i++)
	 {line[i][0]=i*h;line[i][1]=u[k][i];}


	 k++;
	 //usleep(10000);

	 glVertexPointer(2, GL_DOUBLE, 0, line);
	 glEnableClientState(GL_VERTEX_ARRAY);
	 glDrawArrays(GL_LINE_STRIP,0,N);



	 glutSwapBuffers();


	 if(k>T/tay)exit(255);
}












void opengl_init(int argc, char** argv)
{
	  glutInit(&argc, argv);
	  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	  glutInitWindowSize(1920, 1080);
	  glutInitWindowPosition(0, 0);
	  glutCreateWindow("graf");
	  glClearColor(0, 0, 0, 0);
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  glOrtho(0, l, -0.5, 0.5, -100, 100);
	  glutDisplayFunc(display);
	  glutIdleFunc(display);
	  glMatrixMode(GL_MODELVIEW);

	  glutKeyboardFunc(pressNormalKeys);


	  glutMainLoop();
}











int main(int argc, char** argv)
{

	cout<<"N= "<<N<<endl;
	cout<<"R= "<<R<<endl;
	cout<<"Courant number "<< c*tay/h<<endl;
	cout<<"memory allocated "<< (N*R*8)/1000000<<"MB"<<endl;

	timeval time__;
	timeval time__1;
	gettimeofday(&time__,NULL);

	//inital condition
	double* fi=(double* )malloc(sizeof(double)*N);for(int i=0;i<N;i++){fi[i]=f(h*i);}
	double* f1i=(double* )malloc(sizeof(double)*N);for(int i=0;i<N;i++){f1i[i]=f1(h*i);}
	// boundary conditions
	double* gk=(double* )malloc(sizeof(double)*R);for(int k=0;k<R;k++){gk[k]=g(tay*k);}
	double* g1k=(double* )malloc(sizeof(double)*R);for(int k=0;k<R;k++){g1k[k]=g1(tay*k);}


	double a=c*c*tay*tay/h/h;
	double b=2*(h*h-c*c*tay*tay)/h/h;

	//array of results
	u= (double** )malloc(sizeof(double*)*R);
	for(int k=0;k<R;k++){u[k]=(double*)malloc(sizeof(double)*N);}


	//FILE *fl =fopen("./test.txt","w+");



	fi[N/2]=0.1;
	for(int i=0;i<N;i++){u[0][i]=fi[i];u[1][i]=f1i[i]*tay+fi[i];}

	for(int k=1;k<R-1;k++)
	{
		for(int i=0;i<N;i++){u[k+1][i]=a*u[k][i+1]+b*u[k][i]+a*u[k][i-1]-u[k-1][i];}
		u[k+1][0]=gk[k+1];
		u[k+1][N-1]=g1k[k+1];
	}

	gettimeofday(&time__1,NULL);
	printf("%f \r\n",(double)differe(time__,time__1)/(double)1E6);

	/*
	for(int k=0;k<R-1;k++)
	{
		for(int i=0;i<N;i++){fprintf(fl,"%10.5f ",u[k][i]);}
		fprintf(fl,"\r\n");
	}
	fclose(fl);
	*/






	opengl_init(argc, argv);

	for(int k=0;k<R;k++){free(u[k]);}
	free(u);

	return 0xff;
}
