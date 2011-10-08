/*
 * GOSoundSampler.h
 *
 *  Created on: Oct 8, 2011
 *      Author: nick
 */

#ifndef GOSOUNDSAMPLER_H_
#define GOSOUNDSAMPLER_H_

#include "GOSoundFader.h"
#include "GOSoundBlockReader.h"

class GOSoundProvider;

typedef struct GO_SAMPLER_T
{
	struct GO_SAMPLER_T   *next;
	const GOSoundProvider *pipe;
	int                    sampler_group_id;
	GOSoundFader           fader;
	GOSoundBlockReader     reader;
	unsigned               time;
	bool                   stop;
} GO_SAMPLER;

#endif /* GOSOUNDSAMPLER_H_ */
