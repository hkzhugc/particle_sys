#include "particle.h"
#include <gl/glut.h>
#include "util.h"
void Particle::update()
{
	if (update_condition) // user update
	{
		update_condition(this);
	}
	else // simple update
	{
		pos += speed;
		speed += acc;
	}

	if (!is_permanent && life)
	{
		life--;
	}
}

void Particle::render()
{
	glPushMatrix();
	glColor3f(color[0], color[1], color[2]);
	glTranslatef(pos.x, pos.y, 0);
	glScalef(size.x, size.y, 0);
	render_quad();
	glPopMatrix();
	glColor3f(1, 1, 1);
}

void Particle::render_quad()
{
	glBegin(GL_QUADS);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			glVertex2f(i ? 0.5 : -0.5, j ? 0.5 : -0.5);
		}
	}
	glEnd();
}

void ParticleSystem::init()
{
	particle_list.clear();
	int init_num = mean_particle_num + (2 * uniform_random() - 1) * dev_particle_num; // random init num
	generate_some_particles(init_num);
}

void ParticleSystem::update()
{
	//update the state for all particle
	for (auto& p : particle_list)
	{
		p.update();
	}

	//check if some particle is dead, erase those particle
	for (auto pit = particle_list.begin(); pit != particle_list.end();)
	{
		auto t_pit = pit;
		pit++;//move to next element before erase
		if (!t_pit->exist())
		{
			particle_list.erase(t_pit);
		}
	}

	//printf("particle_list.size = %d\n", particle_list.size());

	//printf("%d particle exists\n", particle_list.size());
	//if particle is too little, generate some
	if (is_permanent)
	{
		generate_some_particles(update_rate);
	}
}

void ParticleSystem::render()
{
	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	for (auto p : particle_list)
	{
		p.render();
	}
	glPopMatrix();
}

void ParticleSystem::generate_some_particles(int generate_num)
{
	for (int i = 0; i < generate_num; i++)
	{
		particle_list.push_back(generate_particle());
	}
}
