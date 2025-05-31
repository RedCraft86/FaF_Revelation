// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

namespace LatentID
{
	// Start at an offset in case we have other startup tasks on lower IDs
	static inline uint16 LatentID = 1000; 
	inline uint16 Get()
	{
		if (LatentID >= MAX_uint16)
			LatentID = 0;
		return LatentID++;
	}
}