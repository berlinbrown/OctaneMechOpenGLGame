/**
 * Copyright (c) 2006-2025 Berlin Brown.  All Rights Reserved
 *
 * http://www.opensource.org/licenses/bsd-license.php
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of the Botnode.com (Berlin Brown) nor
 * the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Date: 4/2025
 */

#ifndef _BOT_DRIVER_HPP_
#define _BOT_DRIVER_HPP_

// static bot
//
typedef struct tagStaticBot {

	int		list_id;
	float	position[3];
	float	rotation[3];
	float	size[3];
	float	color[3];
	float	state;

	float	food;

	bool	delete_flag;		// deleted from pheromone list

	float	heading;		// you can use rotation or heading
	float	linearv;

	//
	// for the collision
	//
	float	travel;		// distance traveled
	float	final_x;
	float	final_y;
	float	max_dist;

	
	//
	// 
	float	old_x;
	float	old_y;			// for drawing a line strip

	// used with bullets
	float	virt_x;
	float	virt_y;
	float	virt_heading;
	int		owner;			// owner of this bullet

	struct tagStaticBot *next;

} StaticBot, *StaticBotPtr;

// functions for static bot
typedef struct tagDriverSentinel {

	StaticBotPtr (*create)(int bot_id);
	void (*destroy)(StaticBotPtr b);
	void (*render)(StaticBotPtr boid);
	void (*process)(StaticBotPtr b);

	void (*generate)(void);
	void (*shutdown)(void);
	void (*drawall)(void);

	StaticBot	**objects;

	int		max_items;

} DriverSentinel, *DriverSentinelPtr;


typedef struct tagDriverBots {
		


} DriverBots, *DriverBotPtr;


/**
 * @class DriverBots
 * Driver bots class, contains position and speeds
 */ 
class DriverBots {
    public:
        DriverBots();
        
        DriverBotPtr createBot(int bot_id);

        void generateCommand(int cmd);
        void loadBotParms();

        void wanderCommand();
        void moveCommand();
        void attackCommand();

        int bruteCheckFood();
        void dropFood(int id, float food_rate);

        void destroyBot();
        void getAntFood();
        void resetBot();
        bool checkSight(DriverBotPtr nme);

        void PositionBot();

    private:
        float	x;
        float	y;
        float	linearv;	// speed
        float	size[3];	// scale 
        float	heading;	// direction
        float	target_dir;	// target heading
        float	color[3];
        int		id;
        int		alive;
        int		numSteps;	// how many steps ant has moved
        float	turning_speed;

        int		state;			// what is bot doing
        int		straightSteps;	// steps before changing direction
        float	food;
        float	foodstore;
        int		turn_rand;
        int		turn_direction;	

        bool	go_home;

        int		gun_reset;		// delay befor firing
        int		gun_index;

        bool	move_back;		// for move and turn


        // camera helper variables
        float	look_x;
        float	look_y;
        float	look_h;		// look height, actually the y
        float	cam_x;
        float	cam_y;
        int		view_mode;	// first or third person

        float	score;
        int		kills;

        //
        // the rectangle of the bot
        // Note: you have to add
        // the x,y to this value, but dont
        // do to this variable, use temps
        float	x_min;
        float	x_max;
        float	y_min;
        float	y_max;

        //
        // crosshair object 
        //
        int		crosshair_state;
        float	crosshair_scale;

        // Add the fire ant bullets
        //StaticBot	bullets[MAX_BULLETS];
        StaticBot	*bullets;

        // Command interface
        void (*run)(struct tagDriverBots *bot);

        int		last_command;
        int		command;
        float	attack_angle;
        int		target_moves;
        int		move_index;
        int		enemy_id;    

};



#endif
