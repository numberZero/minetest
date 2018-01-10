#pragma once
#include <vector>
#include "irrlichttypes.h"

struct TrackData
{
	u8 layer;
	u16 buffer;
	u16 first_vertex_index;
	u16 first_index_index;
	u16 vertex_count;
	u16 index_count;
};

struct MeshTracker
{
	std::vector<TrackData> faces;
	u16 layer0_vertex_count = 0;
	u16 layer0_index_count = 0;
};
