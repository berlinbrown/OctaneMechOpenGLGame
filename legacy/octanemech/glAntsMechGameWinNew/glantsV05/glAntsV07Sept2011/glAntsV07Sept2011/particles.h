//
// particles.h
//
#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#define MAX_PARTICLES		90

#define MAX_PARTICLE_LIFE	70
#define MAX_PARTICLE_SIZE	1.0f

// max number of particles
// on the floor at one time
#define MAX_PARTICLE_SET	60

#define PARTICLE_SPEED		0.3f

//
// particles will be made up
// of debris and particles
//
typedef struct tagParticle {

	int					p_id;

	float				p_pos[3];
	float				p_speed[3];
	float				p_color[3];

	int					p_state;

} Particle, *ParticlePtr;

//
// Particle List
//
typedef struct tagParticleList {

	Particle	particles[MAX_PARTICLES];

	float		x;
	float		y;

	int			life;	
	int			state;

} ParticleList;


void SetExplosion(float x, float y);
void AnimateExplosions(void);
void DrawExplosions(void);

void Build_ParticleSet(void);
void Destroy_ParticleSet(void);

#endif