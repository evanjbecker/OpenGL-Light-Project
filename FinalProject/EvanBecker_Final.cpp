
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <iostream>
#include <conio.h> 
#include <string.h>

// Menu Functions
void createMenu(void);
void menu(int value);

// Menu Numbers
static int val = 3;
static int cval = 1;
static int eval = 1;
static int mode = 0;
static int l1col = 0;
static int l2col = 0;

// Menu Continued
static int win;
static int menuExit;
static int menuA;
static int menuB;
static int menuC;
static int menuD;
static int menuE;
static int menuF;
static int menuQ;

// Lighting Animation Control
static GLfloat curTimeX = 1.0;
static GLfloat curTimeY = 1.0;
static GLfloat delta = .005;

// Light 1
static GLfloat lpos1[]  = {.5, 0.0, 1.0, 1.0};
static GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
static GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };

// Light 2
static GLfloat lpos2[]  = {-.5, 0.0, 1.0, 1.0};
static GLfloat light2_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
static GLfloat light2_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // Diffuse Color is blue
static GLfloat light2_specular[] = { 1.0, 1.0, 1.0, 1.0 };

// Defined Colors
static GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
static GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
static GLfloat red[]   = {1.0, 0.0, 0.0, 1.0};
static GLfloat blue[]  = {0.0, 0.0, 1.0, 1.0};
static GLfloat green[]  = {0.0, 1.0, 0.0, 1.0};
static GLfloat yellow[]   = {1.0, 1.0, 0.0, 1.0};
static GLfloat purple[]  = {1.0, 0.0, 1.0, 1.0};
static GLfloat cyan[]  = {0.0, 1.0, 1.0, 1.0};

// Boolean flags
static GLint flag = 0;//used for color change
bool fullscreen = false;
bool mouseDown = false;
bool tog1 = false;
bool tog2 = false;

// Transformations
float xrot = 0.0f;
float yrot = 0.0f;
float xdiff = 0.0f;
float ydiff = 0.0f;
float xtrans = 0.0f;
float ytrans = 0.0f;
float yRotate = 0.0f;
float xRotate = 0.0f;
float xSclSt = 0.0f;
float ySclSt = 0.0f;
float xloc = 0.0f;
float yloc = 0.0f;
float scl = 1.0f;
float xStart = 0.0f;
float yStart = 0.0f;
int p = 0;

// .Off File Table Information
static GLint numVertices,numPolygons,numEdges;
static GLfloat **vdata;   //array for vertex data
static GLuint **pdata;    //array for polygon data (vertex indices)
static GLuint *psize;     //array for polygon size
static GLfloat **normals; //array for polygon normals
static GLfloat **cdata;//array for polygon color

// Mins and Maxs Function
#define min(x,y) ((x)<(y) ? (x) : (y))
#define max(x,y) ((x)>(y) ? (x) : (y))

// Read an off File (in this case, sample_polygon.off
void readOFF(){
	int n,j;
	int a,b;
	float x,y,z;
	float resize;
	char line[256];

	FILE* fin = NULL;

	while (fin == NULL) {
		printf("Opening sample_polygon.off...\n");
		fin =  fopen("sample_polygon.off","rb");
	};

	/* OFF format */
	while (fgets(line,256,fin) != NULL) {
		if (line[0]=='O' && line[1]=='F' && line[2]=='F')
			break;
	}

	fscanf(fin,"%d %d %d\n",&numVertices,&numPolygons,&numEdges);
	printf("Number of vertices  = %d\n",numVertices);
	printf("Number of polygons = %d\n",numPolygons);
	printf("Number of edges = %d\n",numEdges);
	printf("Loading vertices and polygons for sample_polygon.off... ");

	vdata=new GLfloat*[numVertices];
	for(int i=0;i<numVertices;i++)
		vdata[i]=new GLfloat[3];

	pdata=new GLuint*[numPolygons]; //array for storing polygon data (vertex indices)
	psize=new GLuint[numPolygons];  //array for storing polygon size

	resize = 0.0001;
	for (n = 0; n < numVertices; n++) { //read vertex data
		fscanf(fin,"%f %f %f\n",&x,&y,&z);
		vdata[n][0] = x;
		resize = max(resize,fabs(x));
		vdata[n][1] = y;
		resize = max(resize,fabs(y));
		vdata[n][2] = z;
		resize = max(resize,fabs(z));
	}

	for (n = 0; n < numVertices; n++) { //adjust vertex data
		vdata[n][0] = vdata[n][0]/resize;
		vdata[n][1] = vdata[n][1]/resize;
		vdata[n][2] = vdata[n][2]/resize;
	}

	for (n = 0; n < numPolygons; n++) {
		fscanf(fin,"%d",&a);
		psize[n] = a;  //store n-th polygon size in psize[n]
		pdata[n]=new GLuint[a];		
		for (j = 0; j < a; j++) { //read and save vertices of n-th polygon
			fscanf(fin,"%d",&b);
			pdata[n][j] = b;
		}
	}
	fclose(fin);
	printf("done.\n");

}

// Calculate Normals of vectors
void calculateNormal(){
	normals=new GLfloat*[numPolygons];
	for(int i=0;i<numPolygons;i++)
		normals[i]=new GLfloat[3];

	for(int i=0;i<numPolygons;i++){

		GLint t1=pdata[i][0],t2=pdata[i][1],t3=pdata[i][2];
		GLfloat v1[3]={vdata[t1][0],vdata[t1][1],vdata[t1][2]};
		GLfloat v2[3]={vdata[t2][0],vdata[t2][1],vdata[t2][2]};
		GLfloat v3[3]={vdata[t3][0],vdata[t3][1],vdata[t3][2]};

		GLfloat n1[3] = {v2[0] - v1[0],v2[1] - v1[1],v2[2] - v1[2]};
		GLfloat n2[3] = {v3[0] - v1[0],v3[1] - v1[1],v3[2] - v1[2]};

		float	normx = (n1[1] * n2[2]) - (n2[1] * n1[2]),
			normy = (n1[2] * n2[0]) - (n2[2] * n1[0]),
			normz = (n1[0] * n2[1]) - (n2[0] * n1[1]);

		float factor = sqrt(pow(normx,2) + pow(normy,2) + pow(normz,2));
		normx /= factor;
		normy /= factor;
		normz /= factor;
		normals[i][0]=normx;
		normals[i][1]=normy;
		normals[i][2]=normz;
	}
}

/* Not necessary, but useful to view polygons faces */
void setRandomColor(){
	cdata=new GLfloat*[numPolygons];
	for(int i=0;i<numPolygons;i++)
		cdata[i]=new GLfloat[4];

	for(int i = 0; i < numPolygons; i++){
		cdata[i][0] = (rand()*1.0)/RAND_MAX;
		cdata[i][1] = (rand()*1.0)/RAND_MAX;
		cdata[i][2] = (rand()*1.0)/RAND_MAX;
		cdata[i][3] = 1.0;
	}
}

// Animates lights using a cosine function
void animateLights(){
	if(tog1){
		curTimeX += delta; // Make it a function of time (or in this case, steps/frames)
	}
	
	if(tog2){
		curTimeY += delta; // Make it a function of time (or in this case, steps/frames)
	}

	lpos1[0] = 0.5* cos(curTimeX); // set lpos1 X position to cosine function
	lpos1[1] = 0.5 * sin(curTimeX);
	lpos2[1] = 0.75 * cos(curTimeY); // set lpos2 Y position to cosine function
}

// Draws the .off shape from read
void drawShape(){
	if(val == 8)
		glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
	else if(val == 9)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else if(val == 10)
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	for(int i = 0; i < numPolygons; i++){
		if(flag==1){
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cdata[i]);
		}
		glBegin(GL_POLYGON);
		glNormal3f(
				normals[i][0],
				normals[i][1],
				normals[i][2]
			);
		for(int j = 0; j < psize[i]; j++){
			glVertex3f(
				vdata[pdata[i][j]][0],
				vdata[pdata[i][j]][1],
				vdata[pdata[i][j]][2]
			);
		}
		glEnd();
	}
}

// Initiates some parameters and writes to console directions
bool init(){
	//glClearColor(0.93f, 0.93f, 0.93f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
	printf("********************************\n");
	printf("*    CS 459 - Final Project    *\n");
	printf("*    ------   Evan J Becker    *\n");
	printf("********************************\n");
	printf("*  R - Rotation  with   Mouse  *\n");
	printf("*  T - Translation with Mouse  *\n");
	printf("*  S - Scaling   with   Mouse  *\n");
	printf("********************************\n");
	printf("* Right click to change the    *\n");
	printf("* light's emission on the mat- *\n");
	printf("* erial as well as its diffuse *\n"); 
	printf("* color.                       *\n");
	printf("********************************\n");
	return true;
}

void display(){
	// Setup
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); // Necessary for drawShape();
	glLoadIdentity();

	// Camera
	gluLookAt(
		0.0f, 0.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	);

	// Animate the Lights
	animateLights();

	// Light 0 Settings
	glLightfv(GL_LIGHT0, GL_AMBIENT, light1_ambient);
	if(l1col == 0){ glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_diffuse); }
	else if(l1col == 1){ glLightfv(GL_LIGHT0, GL_DIFFUSE, blue); }
	glLightfv(GL_LIGHT0, GL_SPECULAR, light1_specular);
	glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5);
	glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos1);

	// Light 1 Settings
	glLightfv(GL_LIGHT1, GL_AMBIENT, light2_ambient);
	if(l2col == 0){ glLightfv(GL_LIGHT1, GL_DIFFUSE, light2_diffuse); }
	else if(l2col == 1){ glLightfv(GL_LIGHT1, GL_DIFFUSE, red); }
	glLightfv(GL_LIGHT1, GL_SPECULAR, light2_specular);
	glLightf (GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf (GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	glLightf (GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
	glLightfv(GL_LIGHT1, GL_POSITION, lpos2);

	// Draw Light 1 position
	glPushMatrix();
		glTranslatef(lpos1[0], lpos1[1], lpos1[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);
		glutSolidSphere(0.025, 10, 8);
	glPopMatrix();

	// Draw Light 2 position
	glPushMatrix();
		glTranslatef(lpos2[0],lpos2[1],lpos2[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);
		glutSolidSphere(0.025, 10, 8);
	glPopMatrix();

	// Translation Mode
	glTranslatef(xtrans, ytrans, 0.0f);

	// Rotate Mode
	glRotatef(xRotate, 1.0f, 0.0f, 0.0f);
	glRotatef(yRotate, 0.0f, 1.0f, 0.0f);

	// Scale Mode
	glScalef(scl, scl, scl);

	// DEBUG PURPOSES
	// printf("(X: %f, Y: %f)\n",xrot,yrot);

	if(eval==1){
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
	}
	else if(eval==2){
		glMaterialfv(GL_FRONT, GL_EMISSION, red);
	}
	else if(eval==3){
		glMaterialfv(GL_FRONT, GL_EMISSION, blue);
	}
	else if(eval==4){
		glMaterialfv(GL_FRONT, GL_EMISSION, green);
	}
	else if(eval==5){
		glMaterialfv(GL_FRONT, GL_EMISSION, yellow);
	}
	else if(eval==6){
		glMaterialfv(GL_FRONT, GL_EMISSION, purple);
	}
	else if(eval==7){
		glMaterialfv(GL_FRONT, GL_EMISSION, cyan);
	}
	else{
		glMaterialfv(GL_FRONT, GL_EMISSION, black);
	}

	// Change Ambient and Diffuse color of Shape
	if(flag==0){
		if(cval==1){
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
		}
		else if(cval==2){
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
		}
		else if(cval==3){
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
		}
		else if(cval==4){
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
		}
		else if(cval==5){
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, purple);
		}
		else if(cval==6){
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cyan);
		}
	}

	// Make the back blue
	glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, blue);

	// Redraw the frame and shape
	glutPostRedisplay();
	drawShape();

	// Flush and swap buffers
	glFlush();
	glutSwapBuffers();
}

// Resize Window Function
void resize(int w, int h){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, 1.0f * w / h, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard Fucntion
void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 27 : 
			exit(1); 
			break;
		case 'r' :
			mode = 0;
			printf("Mode: %d - Rotation\n", mode);
			break;
		case 't' :
			mode = 1;
			printf("Mode: %d - Translate\n", mode);
			break;
		case 's' :
			mode = 2;
			printf("Mode: %d - Scaling\n", mode);
			break;
		case 'f' :
			printf("- Recolor -\n");
			flag= 1-flag;
			glutPostRedisplay();
			break;
		case 'p' :
			printf("Switching model.. \n");
			p = 1-p;
			glutPostRedisplay();
	}
}

// For Fullscreen keyboard
void specialKeyboard(int key, int x, int y){
	if (key == GLUT_KEY_F1){
		fullscreen = !fullscreen;

		if (fullscreen)
			glutFullScreen();
		else{
			glutReshapeWindow(500, 500);
			glutPositionWindow(50, 50);
		}
	}
}

// For mice
void mouse(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		mouseDown = true;

		if(mode == 0){
			xdiff = x - yrot;
			ydiff = -y + xrot;
		}
		else if(mode == 1){
			xStart = x - xtrans;
			yStart = -y + ytrans;
		}
		else if(mode == 2){
			xSclSt = x - xloc;
			ySclSt = -y + yloc;
		}
	}
	else mouseDown = false;
}

void mouseMotion(int x, int y){
	if (mouseDown){
		if(mode == 0){
			yrot = x - xdiff;
			xrot = y + ydiff;
			yRotate = yrot;
			xRotate = xrot;
		}
		else if(mode == 1){
			xtrans = (x - xStart)/200.0;
			ytrans = -1 * (y + yStart)/200.0;
		}
		else if(mode == 2){
			xloc = (x - xSclSt)/200.0;
			yloc = -1 * (y + ySclSt)/200.0;
			scl = 1 + .5 * ((yloc * yloc) + (xloc * xloc));
		}

		glutPostRedisplay();
	}
}

// Creates Menu
void createMenu(void){
	menuA = glutCreateMenu(menu);
		// Add sub menu entry
		glutAddMenuEntry("White [Default]", 1);
		glutAddMenuEntry("Blue", 2);
		glutAddMenuEntry("Toggle Animation", 3);					 
	menuExit = glutCreateMenu(menu);

	menuB = glutCreateMenu(menu);
		// Add sub menu entry
		glutAddMenuEntry("White [Default]", 4);
		glutAddMenuEntry("Red", 5);
		glutAddMenuEntry("Toggle Animation", 6);				 
	menuExit = glutCreateMenu(menu);

	menuD = glutCreateMenu(menu);
		// Add sub menu entry
		glutAddMenuEntry("Red [Default]", 11);
		glutAddMenuEntry("Blue", 12);
		glutAddMenuEntry("Green", 13);
		glutAddMenuEntry("Yellow", 14);
		glutAddMenuEntry("Purple", 15);
		glutAddMenuEntry("Cyan", 16);
			 
	menuExit = glutCreateMenu(menu);

	menuE = glutCreateMenu(menu);
		// Add sub menu entry
		glutAddMenuEntry("Point", 8);
		glutAddMenuEntry("Line", 9);
		glutAddMenuEntry("Fill [Default]", 10);		 
	menuExit = glutCreateMenu(menu);

	menuF = glutCreateMenu(menu);
		// Add sub menu entry
		glutAddMenuEntry("Black [Default]", 21);
		glutAddMenuEntry("Red", 22);
		glutAddMenuEntry("Blue", 23);
		glutAddMenuEntry("Green", 24);
		glutAddMenuEntry("Yellow", 25);
		glutAddMenuEntry("Purple", 26);
		glutAddMenuEntry("Cyan", 27);		 
	menuExit = glutCreateMenu(menu);

	menuC = glutCreateMenu(menu);
		// Add sub menu entry
		glutAddSubMenu("Ambient and Diffuse Color", menuD);
		glutAddSubMenu("Emission Color", menuF);
		glutAddSubMenu("Geometry", menuE);
	menuExit = glutCreateMenu(menu);
	
	// Create an entry
	glutAddSubMenu("Light 1", menuA);
	glutAddSubMenu("Light 2", menuB);
	glutAddSubMenu("Model", menuC);
	glutAddMenuEntry("Toggle Random Poly Color Mode", 7);
	glutAddMenuEntry("Exit", 0);
	
	// Let the menu respond on the right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char *argv[]){
	glutInit(&argc, argv);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	win = glutCreateWindow("Evan Becker - CS 459 Final Project");

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	

	glEnable(GL_LIGHTING);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glEnable(GL_DEPTH_TEST);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	createMenu();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutReshapeFunc(resize);

	
	if (!init()) return 1;
	
	readOFF();
	calculateNormal();
	setRandomColor();
	glutMainLoop();
	return 0;
}

/**
 * Simply set val to the passed value. 
 * If 0, associate to closing the window.
 */
void menu(int value){
	if(value == 0){
		glutDestroyWindow(win);
		exit(0);
	}else if(value == 1){
		l1col = 0;
	}else if(value == 2){
		l1col = 1;
	}else if(value == 3){
		tog1 = !tog1;
	}else if(value == 4){
		l2col = 0;
	}else if(value == 5){
		l2col = 1;
	}else if(value == 6){
		tog2 = !tog2;
	}else if(value == 7){
		printf("- Recolor -\n");
		flag= 1-flag;
		glutPostRedisplay();
	}else if(value >= 11 && value <= 16){
		cval = value % 10;
	}else if(value >= 21 && value <= 27){
		eval = value % 10;
	}
	else{
		val=value;
	} glutPostRedisplay();
}