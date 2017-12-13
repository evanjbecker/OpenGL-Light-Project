
#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <iostream>

// Menu Functions
void create_menu(void);
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

// Lighting Animation Control
static float curTimeX = 1.0;
static float curTimeY = 1.0;
static float delta = .005;

// Light 1
static float lpos1[] = { .5, 0.0, 1.0, 1.0 };
static float light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
static float light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static float light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };

// Light 2
static float lpos2[] = { -.5, 0.0, 1.0, 1.0 };
static float light2_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
static float light2_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // Diffuse Color is blue
static float light2_specular[] = { 1.0, 1.0, 1.0, 1.0 };

// Defined Colors
static float white[] = { 1.0, 1.0, 1.0, 1.0 };
static float black[] = { 0.0, 0.0, 0.0, 1.0 };
static float red[] = { 1.0, 0.0, 0.0, 1.0 };
static float blue[] = { 0.0, 0.0, 1.0, 1.0 };
static float green[] = { 0.0, 1.0, 0.0, 1.0 };
static float yellow[] = { 1.0, 1.0, 0.0, 1.0 };
static float purple[] = { 1.0, 0.0, 1.0, 1.0 };
static float cyan[] = { 0.0, 1.0, 1.0, 1.0 };

// Boolean flags
static int flag = 0;//used for color change
bool fullscreen = false;
bool mouseDown = false;
bool tog1 = false;
bool tog2 = false;

// Transformations
float x_rot = 0.0f;
float y_rot = 0.0f;
float x_diff = 0.0f;
float y_diff = 0.0f;
float x_trans = 0.0f;
float y_trans = 0.0f;
float x_rotate = 0.0f;
float y_rotate = 0.0f;
float x_scl_st = 0.0f;
float y_scl_st = 0.0f;
float x_loc = 0.0f;
float y_loc = 0.0f;
float scale = 1.0f;
float x_start = 0.0f;
float y_start = 0.0f;
int p = 0;

// .Off File Table Information
static int num_vertices, num_polygons, num_edges;
static float ** v_data;   //array for vertex data
static unsigned ** p_data;    //array for polygon data (vertex indices)
static unsigned * p_size;     //array for polygon size
static float ** normals; //array for polygon normals
static float ** cdata;//array for polygon color

// Min and max Function
#define min(x,y) ((x)<(y) ? (x) : (y))
#define max(x,y) ((x)>(y) ? (x) : (y))

// Read an off File (in this case, sample_polygon.off
void read_off() {
	int n, j;
	int a, b;
	float x, y, z;
	char line[256];

	FILE* fin = nullptr;

	while (fin == nullptr) {
		printf("Opening sample_polygon.off...\n");
		fin = fopen("sample_polygon.off", "rb");
	};

	/* OFF format */
	while (fgets(line, 256, fin) != nullptr) {
		if (line[0] == 'O' && line[1] == 'F' && line[2] == 'F')
			break;
	}

	fscanf(fin, "%d %d %d\n", &num_vertices, &num_polygons, &num_edges);
	printf("Number of vertices  = %d\n", num_vertices);
	printf("Number of polygons = %d\n", num_polygons);
	printf("Number of edges = %d\n", num_edges);
	printf("Loading vertices and polygons for sample_polygon.off... ");

	v_data = new float*[num_vertices];
	for (int i = 0; i < num_vertices; i++)
		v_data[i] = new float[3];

	p_data = new unsigned*[num_polygons]; //array for storing polygon data (vertex indices)
	p_size = new unsigned[num_polygons];  //array for storing polygon size

	float resize = 0.0001;
	for (n = 0; n < num_vertices; n++) { //read vertex data
		fscanf(fin, "%f %f %f\n", &x, &y, &z);
		v_data[n][0] = x;
		resize = max(resize, fabs(x));
		v_data[n][1] = y;
		resize = max(resize, fabs(y));
		v_data[n][2] = z;
		resize = max(resize, fabs(z));
	}

	for (n = 0; n < num_vertices; n++) { //adjust vertex data
		v_data[n][0] = v_data[n][0] / resize;
		v_data[n][1] = v_data[n][1] / resize;
		v_data[n][2] = v_data[n][2] / resize;
	}

	for (n = 0; n < num_polygons; n++) {
		fscanf(fin, "%d", &a);
		p_size[n] = a;  //store n-th polygon size in p_size[n]
		p_data[n] = new unsigned[a];
		for (j = 0; j < a; j++) { //read and save vertices of n-th polygon
			fscanf(fin, "%d", &b);
			p_data[n][j] = b;
		}
	}
	fclose(fin);
	printf("done.\n");

}

// Calculate Normals of vectors
void calculate_normal() {
	normals = new float*[num_polygons];
	for (auto i = 0; i < num_polygons; i++)
		normals[i] = new float[3];

	for (auto i = 0; i < num_polygons; i++) {

		const int t1 = p_data[i][0], t2 = p_data[i][1], t3 = p_data[i][2];
		float v1[3] = { v_data[t1][0],v_data[t1][1],v_data[t1][2] };
		float v2[3] = { v_data[t2][0],v_data[t2][1],v_data[t2][2] };
		float v3[3] = { v_data[t3][0],v_data[t3][1],v_data[t3][2] };

		float n1[3] = { v2[0] - v1[0],v2[1] - v1[1],v2[2] - v1[2] };
		float n2[3] = { v3[0] - v1[0],v3[1] - v1[1],v3[2] - v1[2] };

		auto norm_x = n1[1] * n2[2] - n2[1] * n1[2],
			norm_y = n1[2] * n2[0] - n2[2] * n1[0],
			norm_z = n1[0] * n2[1] - n2[0] * n1[1];

		const auto factor = sqrt(pow(norm_x, 2) + pow(norm_y, 2) + pow(norm_z, 2));
		norm_x /= factor;
		norm_y /= factor;
		norm_z /= factor;
		normals[i][0] = norm_x;
		normals[i][1] = norm_y;
		normals[i][2] = norm_z;
	}
}

/* Not necessary, but useful to view polygons faces */
void set_random_color() {
	cdata = new float*[num_polygons];
	for (auto i = 0; i < num_polygons; i++)
		cdata[i] = new float[4];

	for (auto i = 0; i < num_polygons; i++) {
		cdata[i][0] = rand()*1.0 / RAND_MAX;
		cdata[i][1] = rand()*1.0 / RAND_MAX;
		cdata[i][2] = rand()*1.0 / RAND_MAX;
		cdata[i][3] = 1.0;
	}
}

// Animates lights using a cosine function
void animate_lights() {
	if (tog1)
		curTimeX += delta; // Make it a function of time (or in this case, steps/frames)
	else if (tog2)
		curTimeY += delta; // Make it a function of time (or in this case, steps/frames)

	lpos1[0] = 0.5* cos(curTimeX); // set lpos1 X position to cosine function
	lpos1[1] = 0.5 * sin(curTimeX);
	lpos2[1] = 0.75 * cos(curTimeY); // set lpos2 Y position to cosine function
}

// Draws the .off shape from read
void draw_shape() {
	if (val == 8)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else if (val == 9)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if (val == 10)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < num_polygons; i++) {
		if (flag == 1) {
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cdata[i]);
		}
		glBegin(GL_POLYGON);
		glNormal3f(
			normals[i][0],
			normals[i][1],
			normals[i][2]
		);
		for (auto j = 0; j < p_size[i]; j++) {
			glVertex3f(
				v_data[p_data[i][j]][0],
				v_data[p_data[i][j]][1],
				v_data[p_data[i][j]][2]
			);
		}
		glEnd();
	}
}

// Initiates some parameters and writes to console directions
bool init() {
	//glClearColor(0.93f, 0.93f, 0.93f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
	printf("********************************\n");
	printf("*             Evan J Becker    *\n");
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

void display() {
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
	animate_lights();

	// Light 0 Settings
	glLightfv(GL_LIGHT0, GL_AMBIENT, light1_ambient);
	if (l1col == 0) { glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_diffuse); }
	else if (l1col == 1) { glLightfv(GL_LIGHT0, GL_DIFFUSE, blue); }
	glLightfv(GL_LIGHT0, GL_SPECULAR, light1_specular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos1);

	// Light 1 Settings
	glLightfv(GL_LIGHT1, GL_AMBIENT, light2_ambient);
	if (l2col == 0) { glLightfv(GL_LIGHT1, GL_DIFFUSE, light2_diffuse); }
	else if (l2col == 1) { glLightfv(GL_LIGHT1, GL_DIFFUSE, red); }
	glLightfv(GL_LIGHT1, GL_SPECULAR, light2_specular);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
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
	glTranslatef(lpos2[0], lpos2[1], lpos2[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, white);
	glutSolidSphere(0.025, 10, 8);
	glPopMatrix();

	// Translation Mode
	glTranslatef(x_trans, y_trans, 0.0f);

	// Rotate Mode
	glRotatef(x_rotate, 1.0f, 0.0f, 0.0f);
	glRotatef(y_rotate, 0.0f, 1.0f, 0.0f);

	// Scale Mode
	glScalef(scale, scale, scale);

	switch(eval)
	{
		case 1:
			glMaterialfv(GL_FRONT, GL_EMISSION, black);
			break;
		case 2:
			glMaterialfv(GL_FRONT, GL_EMISSION, red);
			break;
		case 3:
			glMaterialfv(GL_FRONT, GL_EMISSION, blue);
			break;
		case 4:
			glMaterialfv(GL_FRONT, GL_EMISSION, green);
			break;
		case 5:
			glMaterialfv(GL_FRONT, GL_EMISSION, yellow);
			break;
		case 6:
			glMaterialfv(GL_FRONT, GL_EMISSION, purple);
			break;
		case 7:
			glMaterialfv(GL_FRONT, GL_EMISSION, cyan);
			break;
		default:
			glMaterialfv(GL_FRONT, GL_EMISSION, black);
			break;
	}

	// Change Ambient and Diffuse color of Shape
	if (flag == 0) {
		switch(cval)
		{
			case 1:
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
				break;
			case 2:
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
				break;
			case 3:
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
				break;
			case 4:
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
				break;
			case 5:
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, purple);
				break;
			case 6:
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cyan);
				break;
			default: ;
		}
	}

	// Make the back blue
	glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, blue);

	// Redraw the frame and shape
	glutPostRedisplay();
	draw_shape();

	// Flush and swap buffers
	glFlush();
	glutSwapBuffers();
}

// Resize Window Function
void resize(const int w, const int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, 1.0f * w / h, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard Function
void keyboard(const unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			exit(1);
	case 'r':
			mode = 0;
			printf("Mode: %d - Rotation\n", mode);
			break;
		case 't':
			mode = 1;
			printf("Mode: %d - Translate\n", mode);
			break;
		case 's':
			mode = 2;
			printf("Mode: %d - Scaling\n", mode);
			break;
		case 'f':
			printf("- Recolor -\n");
			flag = 1 - flag;
			glutPostRedisplay();
			break;
		case 'p':
			printf("Switching model.. \n");
			p = 1 - p;
			glutPostRedisplay();
			break;
		default: ;
	}
}

// For Fullscreen keyboard
void special_keyboard(const int key, int x, int y) {
	if (key == GLUT_KEY_F1) {
		fullscreen = !fullscreen;

		if (fullscreen)
			glutFullScreen();
		else {
			glutReshapeWindow(500, 500);
			glutPositionWindow(50, 50);
		}
	}
}

// For mice
void mouse(const int button, const int state, const int x, const int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouseDown = true;

		switch(mode)
		{
			case 0:
				x_diff = x - y_rot;
				y_diff = -y + x_rot;
				break;
			case 1:
				x_start = x - x_trans;
				y_start = -y + y_trans;
				break;
			case 2:
				x_scl_st = x - x_loc;
				y_scl_st = -y + y_loc;
				break;
			default: ;
		}
	}
	else mouseDown = false;
}

void mouse_motion(const int x, const int y) {
	if (mouseDown) {
		if (mode == 0) {
			y_rot = x - x_diff;
			x_rot = y + y_diff;
			y_rotate = y_rot;
			x_rotate = x_rot;
		}
		else if (mode == 1) {
			x_trans = (x - x_start) / 200.0;
			y_trans = -1 * (y + y_start) / 200.0;
		}
		else if (mode == 2) {
			x_loc = (x - x_scl_st) / 200.0;
			y_loc = -1 * (y + y_scl_st) / 200.0;
			scale = 1 + .5 * (y_loc * y_loc + x_loc * x_loc);
		}

		glutPostRedisplay();
	}
}

// Creates Menu
void create_menu(void) {
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

int main(int argc, char *argv[]) {
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

	create_menu();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_motion);
	glutReshapeFunc(resize);

	if (!init()) return 1;

	read_off();
	calculate_normal();
	set_random_color();
	glutMainLoop();
	return 0;
}

/**
 * Simply set val to the passed value.
 * If 0, associate to closing the window.
 */
void menu(const int value) {
	switch (value) {
		case 0:
			glutDestroyWindow(win);
			exit(0);
	case 1:
			l1col = 0;
			break;
		case 2:
			l1col = 1;
			break;
		case 3:
			tog1 = !tog1;
			break;
		case 4:
			l2col = 0;
			break;
		case 5:
			l2col = 1;
			break;
		case 6:
			tog2 = !tog2;
			break;
		case 7:
			printf("- Recolor -\n");
			flag = 1 - flag;
			glutPostRedisplay();
			break;
		default:
			if (value >= 11 && value <= 16)
				cval = value % 10;
			else if (value >= 21 && value <= 27)
				eval = value % 10;
			else
				val = value;
			break;
	}
	glutPostRedisplay();
}