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

#include "GOSoundBlockReader.h"
#include "GOrgueReleaseAlignTable.h"

bool   /** Returns true if the release_transient structure was initialised */
BlockReaderGetReleaseReader
	(const GOSoundBlockReader *reader_to_abort
	,bool                      use_release_alignment
	,GOSoundBlockReader       *release_transient
	,float                    *required_scaling
	)
{

	const AUDIO_SECTION* release_section = reader_to_abort->pipe_section->release_to;
	if (!release_section)
		return false;

	release_transient->pipe_section = release_section;
	 /* The following line requires that the aborting stream section and the
	  * playing back sample stream share the same sample rate. */
	release_transient->increment    = reader_to_abort->increment;
	release_transient->position     = 0;
	*required_scaling               = scalbnf(1.0f, -release_transient->pipe_section->sample_frac_bits);
	memcpy
		(release_transient->history
		,release_transient->pipe_section->history/* [start_position] FIXME!! */
		,sizeof(release_transient->history)
		);


	if (use_release_alignment && (release_section->release_aligner != NULL))
		release_section->release_aligner->SetupRelease(*release_transient, *reader_to_abort);

	return true;

}

