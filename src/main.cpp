#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <gl/glut.h>
#include <glm/glm.hpp>

#include "particle.h"
#include "util.h"

using namespace glm;

void over_sea_level(Particle* p)
{
	vec2& pos = p->get_pos();
	vec2& speed = p->get_speed();
	const vec2& acc = p->cget_acc();
	if (pos.y > -FLT_EPSILON)
	{
		pos += speed;
		pos.y = max(0.f, pos.y);
		speed += acc;
	}
	else
	{
		speed.y = 0;
	}
}

Particle generate_firework()
{
	vec2 size = uniform_random_vec2() * 0.2f;
	vec3 color(uniform_random(), uniform_random(), uniform_random());

	vec2 pos = sample_circle();
	float maxspeed = 0.2;
	float maxacc = 0.001;
	vec2 speed = maxspeed * (2.0f * uniform_random_vec2() - vec2(1, 1));
	vec2 acc = maxacc * (2.0f * uniform_random_vec2() - vec2(1, 1));

	unsigned int max_life = 100;

	Particle p(pos, size, speed, acc, uniform_random() * max_life, color);
	return p;
}

Particle generate_fountain()
{
	vec2 size = uniform_random_vec2() * 0.5f;
	float r = uniform_random() * 0.5f;
	float g = uniform_random() * 0.5f;
	float b = 0.8f;
	vec3 color(r, g, b);

	vec2 pos(0, 0);
	float maxspeed = 1;
	float speed_y =  maxspeed * (uniform_random() + 1);
	float speed_x = (2 * uniform_random() - 1) * 0.2f;
	vec2 speed(speed_x, speed_y);
	vec2 acc(0, -0.1f);

	unsigned int max_life = 50;

	Particle p(pos, size, speed, acc, uniform_random() * max_life, color, over_sea_level, false);
	return p;
}

ParticleSystem firework(vec2(0, 0), 1000, 200, generate_firework, false);
//ParticleSystem firework2(vec2(-10, -10), 1000, 200, generate_firework, false);

ParticleSystem fountain(vec2(-10, -10), 220, 20, generate_fountain, 20, true);

list<ParticleSystem*> fireworks;

//viewport
float ratio;
int window_height = 0;
int window_width = 0;

//camera position
float look_at[] = { 0, 0, 0 };
float camera_pos[] = { 0, 0, 10 };

clock_t clock_time;

void updateView(int height, int width) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ratio = (GLfloat)width / (GLfloat)height;
	glOrtho(-40, 40, -40, 40, -100, 100);
	glMatrixMode(GL_MODELVIEW);
}

void reshape(int width, int height) {
	if (height == 0) {
		height = 1;
	}
	window_height = height;
	window_width = width;
	updateView(window_height, window_width);
}

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera_pos[0], camera_pos[1], camera_pos[2], look_at[0], look_at[1], look_at[2], 0, 1, 0);
	
	firework.render();
	fountain.render();

	for (auto fireworkp : fireworks)
		fireworkp->render();

	clock_t now = clock();
	if (now - clock_time > 10)
	{
		clock_time = now;
		fountain.update();
		firework.update();

		for (auto fireworkp : fireworks)
			fireworkp->update();
	}
	//firework2.render();
	//firework2.update();

	glutSwapBuffers();
}

void init(void) {
	//srand(unsigned(time(nullptr)));
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glColor4f(1.0, 1.0, 1.0, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);

	firework.init();
	//firework2.init();
	fountain.init();
}

void idle() {
	glutPostRedisplay();
}

void pressMouse(int button, int state, int _x, int _y) {
	if (button == GLUT_LEFT_BUTTON) 
	{
		if (state == GLUT_DOWN) 
		{
			float x = _x * 1.0 / window_width * 80 - 40;
			float y = _y * 1.0 / window_height * 80 - 40;
			ParticleSystem *firework = new ParticleSystem(vec2(x, -y), 1000, 200, generate_firework, false);
			firework->init();
			fireworks.push_back(firework);
		}
	}
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	int windowHandle = glutCreateWindow("ParticleSystem");
	//glutKeyboardFunc(pressKey);
	glutMouseFunc(pressMouse);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	init();
	clock_time = clock();
	glutMainLoop();
	return 0;
}