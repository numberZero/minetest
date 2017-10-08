/*
Minetest
Copyright (C) 2010-2013 celeron55, Perttu Ahola <celeron55@gmail.com>
Copyright (C) 2017 numzer0, Lobachevskiy Vitaliy <numzer0@yandex.ru>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

// #include "mapblock_mesh.h"
// #include "light.h"
// #include "mapblock.h"
// #include "map.h"
// #include "profiler.h"
// #include "nodedef.h"
// #include "mesh.h"
// #include "minimap.h"
// #include "content_mapblock.h"
// #include "noise.h"
// #include "shader.h"
// #include "settings.h"
#include <vector>
#include "client/tile.h"
#include "constants.h"
#include "irrlichttypes.h"
#include "util/directiontables.h"
// #include <IMeshManipulator.h>

/*
	vertex_dirs: v3s16[4]
*/
static void getNodeVertexDirs(v3s16 dir, v3s16 *vertex_dirs)
{
	/*
		If looked from outside the node towards the face, the corners are:
		0: bottom-right
		1: bottom-left
		2: top-left
		3: top-right
	*/
	if(dir == v3s16(0,0,1))
	{
		// If looking towards z+, this is the face that is behind
		// the center point, facing towards z+.
		vertex_dirs[0] = v3s16(-1,-1, 1);
		vertex_dirs[1] = v3s16( 1,-1, 1);
		vertex_dirs[2] = v3s16( 1, 1, 1);
		vertex_dirs[3] = v3s16(-1, 1, 1);
	}
	else if(dir == v3s16(0,0,-1))
	{
		// faces towards Z-
		vertex_dirs[0] = v3s16( 1,-1,-1);
		vertex_dirs[1] = v3s16(-1,-1,-1);
		vertex_dirs[2] = v3s16(-1, 1,-1);
		vertex_dirs[3] = v3s16( 1, 1,-1);
	}
	else if(dir == v3s16(1,0,0))
	{
		// faces towards X+
		vertex_dirs[0] = v3s16( 1,-1, 1);
		vertex_dirs[1] = v3s16( 1,-1,-1);
		vertex_dirs[2] = v3s16( 1, 1,-1);
		vertex_dirs[3] = v3s16( 1, 1, 1);
	}
	else if(dir == v3s16(-1,0,0))
	{
		// faces towards X-
		vertex_dirs[0] = v3s16(-1,-1,-1);
		vertex_dirs[1] = v3s16(-1,-1, 1);
		vertex_dirs[2] = v3s16(-1, 1, 1);
		vertex_dirs[3] = v3s16(-1, 1,-1);
	}
	else if(dir == v3s16(0,1,0))
	{
		// faces towards Y+ (assume Z- as "down" in texture)
		vertex_dirs[0] = v3s16( 1, 1,-1);
		vertex_dirs[1] = v3s16(-1, 1,-1);
		vertex_dirs[2] = v3s16(-1, 1, 1);
		vertex_dirs[3] = v3s16( 1, 1, 1);
	}
	else if(dir == v3s16(0,-1,0))
	{
		// faces towards Y- (assume Z+ as "down" in texture)
		vertex_dirs[0] = v3s16( 1,-1, 1);
		vertex_dirs[1] = v3s16(-1,-1, 1);
		vertex_dirs[2] = v3s16(-1,-1,-1);
		vertex_dirs[3] = v3s16( 1,-1,-1);
	}
}

struct FastFace
{
	TileSpec tile;
	video::S3DVertex vertices[4]; // Precalculated vertices
};

static void makeFastFace(TileSpec tile, u16 li0, u16 li1, u16 li2, u16 li3,
		v3f p, v3s16 dir, v3f scale, std::vector<FastFace> &dest)
{
	// Position is at the center of the cube.
	v3f pos = p * BS;

	float x0 = 0.0;
	float y0 = 0.0;
	float w = 1.0;
	float h = 1.0;

	v3f vertex_pos[4];
	v3s16 vertex_dirs[4];
	getNodeVertexDirs(dir, vertex_dirs);

	v3s16 t;
	u16 t1;
	switch (tile.rotation)
	{
	case 0:
		break;
	case 1: //R90
		t = vertex_dirs[0];
		vertex_dirs[0] = vertex_dirs[3];
		vertex_dirs[3] = vertex_dirs[2];
		vertex_dirs[2] = vertex_dirs[1];
		vertex_dirs[1] = t;
		t1=li0;
		li0=li3;
		li3=li2;
		li2=li1;
		li1=t1;
		break;
	case 2: //R180
		t = vertex_dirs[0];
		vertex_dirs[0] = vertex_dirs[2];
		vertex_dirs[2] = t;
		t = vertex_dirs[1];
		vertex_dirs[1] = vertex_dirs[3];
		vertex_dirs[3] = t;
		t1  = li0;
		li0 = li2;
		li2 = t1;
		t1  = li1;
		li1 = li3;
		li3 = t1;
		break;
	case 3: //R270
		t = vertex_dirs[0];
		vertex_dirs[0] = vertex_dirs[1];
		vertex_dirs[1] = vertex_dirs[2];
		vertex_dirs[2] = vertex_dirs[3];
		vertex_dirs[3] = t;
		t1  = li0;
		li0 = li1;
		li1 = li2;
		li2 = li3;
		li3 = t1;
		break;
	case 4: //FXR90
		t = vertex_dirs[0];
		vertex_dirs[0] = vertex_dirs[3];
		vertex_dirs[3] = vertex_dirs[2];
		vertex_dirs[2] = vertex_dirs[1];
		vertex_dirs[1] = t;
		t1  = li0;
		li0 = li3;
		li3 = li2;
		li2 = li1;
		li1 = t1;
		y0 += h;
		h *= -1;
		break;
	case 5: //FXR270
		t = vertex_dirs[0];
		vertex_dirs[0] = vertex_dirs[1];
		vertex_dirs[1] = vertex_dirs[2];
		vertex_dirs[2] = vertex_dirs[3];
		vertex_dirs[3] = t;
		t1  = li0;
		li0 = li1;
		li1 = li2;
		li2 = li3;
		li3 = t1;
		y0 += h;
		h *= -1;
		break;
	case 6: //FYR90
		t = vertex_dirs[0];
		vertex_dirs[0] = vertex_dirs[3];
		vertex_dirs[3] = vertex_dirs[2];
		vertex_dirs[2] = vertex_dirs[1];
		vertex_dirs[1] = t;
		t1  = li0;
		li0 = li3;
		li3 = li2;
		li2 = li1;
		li1 = t1;
		x0 += w;
		w *= -1;
		break;
	case 7: //FYR270
		t = vertex_dirs[0];
		vertex_dirs[0] = vertex_dirs[1];
		vertex_dirs[1] = vertex_dirs[2];
		vertex_dirs[2] = vertex_dirs[3];
		vertex_dirs[3] = t;
		t1  = li0;
		li0 = li1;
		li1 = li2;
		li2 = li3;
		li3 = t1;
		x0 += w;
		w *= -1;
		break;
	case 8: //FX
		y0 += h;
		h *= -1;
		break;
	case 9: //FY
		x0 += w;
		w *= -1;
		break;
	default:
		break;
	}

	for(u16 i=0; i<4; i++)
	{
		vertex_pos[i] = v3f(
				BS/2*vertex_dirs[i].X,
				BS/2*vertex_dirs[i].Y,
				BS/2*vertex_dirs[i].Z
		);
	}

	for(u16 i=0; i<4; i++)
	{
		vertex_pos[i].X *= scale.X;
		vertex_pos[i].Y *= scale.Y;
		vertex_pos[i].Z *= scale.Z;
		vertex_pos[i] += pos;
	}

	f32 abs_scale = 1.0;
	if     (scale.X < 0.999 || scale.X > 1.001) abs_scale = scale.X;
	else if(scale.Y < 0.999 || scale.Y > 1.001) abs_scale = scale.Y;
	else if(scale.Z < 0.999 || scale.Z > 1.001) abs_scale = scale.Z;

	v3f normal(dir.X, dir.Y, dir.Z);

	dest.push_back(FastFace());

	FastFace& face = *dest.rbegin();

	u16 li[4] = { li0, li1, li2, li3 };
	v2f32 f[4] = {
		core::vector2d<f32>(x0 + w * abs_scale, y0 + h),
		core::vector2d<f32>(x0, y0 + h),
		core::vector2d<f32>(x0, y0),
		core::vector2d<f32>(x0 + w * abs_scale, y0) };

	for (u8 i = 0; i < 4; i++) {
		video::SColor c = encode_light_and_color(li[i], tile.color,
			tile.emissive_light);
		if (!tile.emissive_light)
			applyFacesShading(c, normal);

		face.vertices[i] = video::S3DVertex(vertex_pos[i], normal, c, f[i]);
	}

	face.tile = tile;
}

/*
	Nodes make a face if contents differ and solidness differs.
	Return value:
		0: No face
		1: Face uses m1's content
		2: Face uses m2's content
	equivalent: Whether the blocks share the same face (eg. water and glass)

	TODO: Add 3: Both faces drawn with backface culling, remove equivalent
*/
static u8 face_contents(content_t m1, content_t m2, bool *equivalent,
		INodeDefManager *ndef)
{
	*equivalent = false;

	if(m1 == CONTENT_IGNORE || m2 == CONTENT_IGNORE)
		return 0;

	bool contents_differ = (m1 != m2);

	const ContentFeatures &f1 = ndef->get(m1);
	const ContentFeatures &f2 = ndef->get(m2);

	// Contents don't differ for different forms of same liquid
	if(f1.sameLiquid(f2))
		contents_differ = false;

	u8 c1 = f1.solidness;
	u8 c2 = f2.solidness;

	bool solidness_differs = (c1 != c2);
	bool makes_face = contents_differ && solidness_differs;

	if(makes_face == false)
		return 0;

	if(c1 == 0)
		c1 = f1.visual_solidness;
	if(c2 == 0)
		c2 = f2.visual_solidness;

	if(c1 == c2){
		*equivalent = true;
		// If same solidness, liquid takes precense
		if(f1.isLiquid())
			return 1;
		if(f2.isLiquid())
			return 2;
	}

	if(c1 > c2)
		return 1;
	else
		return 2;
}
