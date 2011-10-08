/*
 * GrandOrgue - free pipe organ simulator based on MyOrgan
 *
 * MyOrgan 1.0.6 Codebase - Copyright 2006 Milan Digital Audio LLC
 * MyOrgan is a Trademark of Milan Digital Audio LLC
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#ifndef GOSOUNDBLOCKREADER_H_
#define GOSOUNDBLOCKREADER_H_

#include <math.h>
#include <wx/wx.h>
#include "GOSoundDefs.h"
#include "GOSoundAudioSection.h"

typedef struct
{
	int                        history[BLOCK_HISTORY][MAX_OUTPUT_CHANNELS];
	const AUDIO_SECTION       *pipe_section;
	float                      position;
	float                      increment;
} GOSoundBlockReader;

typedef wxInt16 steroSample[0][2];

/* The block decode functions should provide whatever the normal resolution of
 * the audio is. The fade engine should ensure that this data is always brought
 * into the correct range. */

static
inline
void stereoUncompressed
	(GOSoundBlockReader *sampler
	,float              *output
	)
{

	// "borrow" the output buffer to compute release alignment info
	steroSample& input = (steroSample&)*(wxInt16*)(sampler->pipe_section->data);

	// copy the sample buffer
	for (unsigned int i = 0; i < BLOCKS_PER_FRAME; sampler->position += sampler->increment, output += 2, i++)
	{
		unsigned pos = (unsigned)sampler->position;
		float fract = sampler->position - pos;
		output[0] = input[pos][0] * (1 - fract) + input[pos + 1][0] * fract;
		output[1] = input[pos][1] * (1 - fract) + input[pos + 1][1] * fract;
	}

	// update sample history (for release alignment / compression)
	unsigned pos = (unsigned)sampler->position;
	for (unsigned i = BLOCK_HISTORY; i > 0 && pos; i--, pos--)
	{
		sampler->history[i - 1][0] = input[pos][0];
		sampler->history[i - 1][1] = input[pos][1];
	}

}

static
inline
void monoUncompressed
	(GOSoundBlockReader *sampler
	,float              *output
	)
{

	// copy the sample buffer
	wxInt16* input = (wxInt16*)(sampler->pipe_section->data);
	for (unsigned int i = 0; i < BLOCKS_PER_FRAME; i++, sampler->position += sampler->increment, output += 2)
	{
		unsigned pos = (unsigned)sampler->position;
		float fract = sampler->position - pos;
		output[0] = input[pos] * (1 - fract) + input[pos + 1] * fract;
		output[1] = input[pos] * (1 - fract) + input[pos + 1] * fract;
	}

	// update sample history (for release alignment / compression)
	unsigned pos = (unsigned)sampler->position;
	for (unsigned i = BLOCK_HISTORY; i > 0 && pos; i--, pos--)
		sampler->history[i - 1][0] = input[pos];

}

static
inline
void monoCompressed
	(GOSoundBlockReader *sampler
	,float              *output
	)
{

	throw (wxString)_("unimplemented");

    /*short* v=(short*)&sampler->v;
    short* f=(short*)&sampler->f;
    wxByte* input=sampler->ptr+sampler->position;

    // we are in little endian, so the byte the most to the right is in input[0]
    // check at the end of the first 32bits
    if(input[0]&0x01)
        // an int is 32 bit so we will use int
    {
        int inputAsInt=*(int*)input;
        inputAsInt>>=1;
        v[0]+=((char*)(&inputAsInt))[0];
        v[1]+=((char*)(&inputAsInt))[0];
        v[2]+=((char*)(&inputAsInt))[1];
        v[3]+=((char*)(&inputAsInt))[1];
        sampler->position+=4;

    }
    else
    {
        int inputAsInt1=*(int*)input;
        inputAsInt1>>=1;

        v[0]+=(((char*)(&inputAsInt1))[0]<<8)|((short)(input[2]));
        v[1]+=(((char*)(&inputAsInt1))[0]<<8)|((short)(input[2]));
        v[2]+=(((char*)(&inputAsInt1))[1]<<8)|((short)(input[3]));
        v[3]+=(((char*)(&inputAsInt1))[1]<<8)|((short)(input[3]));
        sampler->position+=8;
    }

    f[0]+=v[0];
    f[1]+=v[1];
    f[2]+=v[2];
    f[3]+=v[3];


    output[0] = f[0];
    output[1] = f[1];
    output[2] = f[2];
    output[3] = f[3];*/

}

//if the data is compressed, 32 bits represents 4 data ((Right and Left) * 2)
// we know the data is compressed because these 32bits ends with 1.
//if the data is uncompressed, 64 bits represents 4 data
// the first 32bits end with a 0.
static
inline
void stereoCompressed
	(GOSoundBlockReader *sampler
	,float              *output
	)
{

	throw (wxString)_("unimplemented");

/*throw 0;

	short* v=(short*)&sampler->v;
	short* f=(short*)&sampler->f;
	wxByte* input=sampler->ptr+sampler->position;

	// we are in little endian, so the byte the most to the right is in input[0]
	// check at the end of the first 32bits
	if(input[0]&0x01)
	{
		// an int is 32 bit so we will use int
		int inputAsInt=*(int*)input;
		inputAsInt>>=1;
		v[0]+=((char*)(&inputAsInt))[0];
		v[1]+=((char*)(&inputAsInt))[1];
		v[2]+=((char*)(&inputAsInt))[2];
		v[3]+=((char*)(&inputAsInt))[3];
		sampler->position+=4;

	}
	else
	{
		int inputAsInt1=*(int*)input;
		inputAsInt1>>=1;

		v[0]+=(((char*)(&inputAsInt1))[0]<<8)|((short)(input[4]));
		v[1]+=(((char*)(&inputAsInt1))[1]<<8)|((short)(input[5]));
		v[2]+=(((char*)(&inputAsInt1))[2]<<8)|((short)(input[6]));
		v[3]+=(((char*)(&inputAsInt1))[3]<<8)|((short)(input[7]));
		sampler->position+=8;
	}

	f[0]+=v[0];
	f[1]+=v[1];
	f[2]+=v[2];
	f[3]+=v[3];


	output[0] = f[0];
    output[1] = f[1];
    output[2] = f[2];
    output[3] = f[3];
*/
}

static
inline
bool  /** Returns true if the method can continue to be called. */
GetNextFrame
	(GOSoundBlockReader *state
	,float              *buffer
	,unsigned            target_sample_rate
	)
{
	switch (state->pipe_section->type)
	{
		case AC_COMPRESSED_STEREO:
			stereoCompressed(state, buffer);
			break;
		case AC_UNCOMPRESSED_STEREO:
			stereoUncompressed(state, buffer);
			break;
		case AC_COMPRESSED_MONO:
			monoCompressed(state, buffer);
			break;
		case AC_UNCOMPRESSED_MONO:
			monoUncompressed(state, buffer);
			break;
		default:
			assert(0 && "broken sampler type");
	}

	unsigned currentBlockSize = state->pipe_section->sample_count;
	if (state->position >= currentBlockSize)
	{
		state->pipe_section = state->pipe_section->next;
		/* If the next section was null or the next section's data is NULL,
		 * this was a release (or a one-shot sample). */
		if ((!state->pipe_section) || (!state->pipe_section->data))
			return false;

		/* the pipe is not percussive (normal). The loop or
		 * attack segment has completed so we now (re)enter the
		 * loop. */
		state->position -= currentBlockSize;
		state->increment = state->pipe_section->sample_rate / (float)target_sample_rate;
	}

	return true;
}

static
inline
bool
IsFinalSection
	(const GOSoundBlockReader *state
	)
{
	return (state->pipe_section->next == NULL);
}

static
inline
void
InitBlockReader
	(GOSoundBlockReader  *state
	,float                start_position
	,unsigned             target_sample_rate
	,const AUDIO_SECTION *section
	,float               *required_scaling
	)
{
	state->pipe_section = section;
	state->increment    = state->pipe_section->sample_rate / (float)target_sample_rate;
	state->position     = start_position;
	*required_scaling   = scalbnf(1.0f, -state->pipe_section->sample_frac_bits);
	memcpy
		(state->history
		,state->pipe_section->history/* [start_position] FIXME!! */
		,sizeof(state->history)
		);
}

bool   /** Returns true if the release_transient structure was initialised */
BlockReaderGetReleaseReader
	(const GOSoundBlockReader *reader_to_abort
	,bool                      use_release_alignment
	,GOSoundBlockReader       *release_transient
	,float                    *required_scaling
	);

#endif /* GOSOUNDBLOCKREADER_H_ */
