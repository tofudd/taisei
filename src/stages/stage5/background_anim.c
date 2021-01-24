/*
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 * ---
 * Copyright (c) 2011-2019, Lukas Weber <laochailan@web.de>.
 * Copyright (c) 2012-2019, Andrei Alexeyev <akari@taisei-project.org>.
 */

#include "taisei.h"

#include "background_anim.h"
#include "draw.h"

#include "global.h"
#include "stageutils.h"

TASK(stage5_bg_update, NO_ARGS) {
	Camera3D *cam = &stage_3d_context.cam;
	float r = 3.7;
	float vel = -0.01/r;
	for(int i = 0;; i++) {
		stage3d_update(&stage_3d_context);
		cam->rot.v[2] = 180/M_PI*vel*i-210;
		cam->pos[0] = r*cos(vel*i);
		cam->pos[1] = r*sin(vel*i);

		cam->pos[2] = 2.6 - 11.2/M_TAU*vel*i;

		
		YIELD;
	}
}

void stage5_bg_init_fullstage(void) {
	Camera3D *cam = &stage_3d_context.cam;
	cam->pos[0] = 0;
	cam->pos[1] = 2.5;
	//cam->pos[2] = 0.1;
	//cam->vel[2] = 0.01;
	cam->rot.v[2] = 180;
	cam->rot.v[0] = 80;
	cam->fovy = STAGE3D_DEFAULT_FOVY*1.5;
	INVOKE_TASK(stage5_bg_update);
}

void stage5_bg_init_spellpractice(void) {
	stage5_bg_init_fullstage();
}

void stage5_update(void) {
	Stage5DrawData *stage5_draw_data = stage5_get_draw_data();

	TIMER(&global.timer);
	float w = 0.005;

	stage5_draw_data->stairs.rotshift += stage5_draw_data->stairs.omega;
	stage_3d_context.crot[0] += stage5_draw_data->stairs.omega*0.5;
	stage5_draw_data->stairs.rad += stage5_draw_data->stairs.omega*20;

	int rot_time = 6350;

	FROM_TO(rot_time, rot_time+50, 1) {
		stage5_draw_data->stairs.omega -= 0.005;
	}

	FROM_TO(rot_time+200, rot_time+250, 1) {
		stage5_draw_data->stairs.omega += 0.005;
	}

	stage_3d_context.cx[0] = stage5_draw_data->stairs.rad*cos(-w*global.frames);
	stage_3d_context.cx[1] = stage5_draw_data->stairs.rad*sin(-w*global.frames);
	stage_3d_context.cx[2] = -1700+w*3000/M_PI*global.frames;

	stage_3d_context.crot[2] = stage5_draw_data->stairs.rotshift-180/M_PI*w*global.frames;

	stage5_draw_data->stairs.light_strength *= 0.98;

	if (frand() < 0.01) {
		stage5_draw_data->stairs.light_strength = 5+5*frand();
	}

	stage3d_update(&stage_3d_context);
}
