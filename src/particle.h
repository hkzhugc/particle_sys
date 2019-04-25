#pragma once
#include <glm/glm.hpp>
#include <list>

using namespace std;
using namespace glm;
class Particle;

typedef void (*Particle_Update_Condition) (Particle*);

class Particle
{
public:
	Particle() {};
	Particle(vec2 _pos, vec2 _size, vec2 _speed, vec2 _acc, unsigned int _life, vec3 _color, Particle_Update_Condition _update_condition = NULL, bool _is_permanent = false)
		: pos(_pos), size(_size), speed(_speed), acc(_acc), life(_life), color(_color), is_permanent(_is_permanent), update_condition(_update_condition)
	{};
	Particle(vec2 _pos, vec2 _size, vec2 _speed, vec2 _acc, unsigned int _life, Particle_Update_Condition _update_condition = NULL, bool _is_permanent = false)
		: pos(_pos), size(_size), speed(_speed), acc(_acc), life(_life), color(vec3(1, 1, 1)), is_permanent(_is_permanent), update_condition(_update_condition)
	{};

	~Particle() {};

	void update();
	void render();

	inline const vec2& cget_pos() { return pos; }
	inline const vec2& cget_speed() { return speed; }
	inline const vec2& cget_acc() { return acc; }

	inline vec2& get_pos() { return pos; }
	inline vec2& get_speed() { return speed; }
	inline vec2& get_acc() { return acc; }

	inline bool exist()
	{
		return (is_permanent || life);
	}
protected:
	Particle_Update_Condition update_condition;
	vec2 pos; // local location
	vec2 size;
	vec2 speed;
	vec2 acc;
	bool is_permanent;
	unsigned int life;
	vec3 color;

private:
	void render_quad();
};

typedef Particle(*GenerateParticle) (void);

class ParticleSystem
{
public:
	ParticleSystem(vec2 _pos, int _mean_particle_num, int _dev_particle_num, GenerateParticle _generate_particle, int _update_rate = 0, bool _is_permanent = false)
		: pos(_pos), mean_particle_num(_mean_particle_num), dev_particle_num(_dev_particle_num), generate_particle(_generate_particle), is_permanent(_is_permanent)
	{
		update_rate = _update_rate;
	};
	~ParticleSystem() {};

	void init();
	void update();
	void render();
	inline bool exist()
	{
		return particle_list.size() > 0;
	}

private:
	list<Particle> particle_list;

	vec2 pos;// location of the system

	int mean_particle_num;
	int dev_particle_num;
	bool is_permanent; // if it is true, will regenerate particle to make the num of particles be the same for a time

	int update_rate;

	GenerateParticle generate_particle;

	void generate_some_particles(int generate_num);
};

