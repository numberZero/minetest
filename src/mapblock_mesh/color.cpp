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

#include "color.h"
#include "light.h"
// #include "mapblock_mesh.h"
// #include "mapblock.h"
// #include "map.h"
// #include "profiler.h"
#include "nodedef.h"
// #include "mesh.h"
// #include "minimap.h"
// #include "content_mapblock.h"
// #include "noise.h"
// #include "shader.h"
#include "settings.h"
// #include "util/directiontables.h"
// #include <IMeshManipulator.h>

/*
	Calculate non-smooth lighting at interior of node.
	Single light bank.
*/
static u8 getInteriorLight(enum LightBank bank, MapNode n, s32 increment,
		INodeDefManager *ndef)
{
	u8 light = n.getLight(bank, ndef);

	while(increment > 0)
	{
		light = undiminish_light(light);
		--increment;
	}
	while(increment < 0)
	{
		light = diminish_light(light);
		++increment;
	}

	return decode_light(light);
}

/*
	Calculate non-smooth lighting at interior of node.
	Both light banks.
*/
u16 getInteriorLight(MapNode n, s32 increment, INodeDefManager *ndef)
{
	u16 day = getInteriorLight(LIGHTBANK_DAY, n, increment, ndef);
	u16 night = getInteriorLight(LIGHTBANK_NIGHT, n, increment, ndef);
	return day | (night << 8);
}

/*
	Calculate non-smooth lighting at face of node.
	Single light bank.
*/
static u8 getFaceLight(enum LightBank bank, MapNode n, MapNode n2,
		v3s16 face_dir, INodeDefManager *ndef)
{
	u8 light;
	u8 l1 = n.getLight(bank, ndef);
	u8 l2 = n2.getLight(bank, ndef);
	if(l1 > l2)
		light = l1;
	else
		light = l2;

	// Boost light level for light sources
	u8 light_source = MYMAX(ndef->get(n).light_source,
			ndef->get(n2).light_source);
	if(light_source > light)
		light = light_source;

	return decode_light(light);
}

/*
	Calculate non-smooth lighting at face of node.
	Both light banks.
*/
u16 getFaceLight(MapNode n, MapNode n2, v3s16 face_dir, INodeDefManager *ndef)
{
	u16 day = getFaceLight(LIGHTBANK_DAY, n, n2, face_dir, ndef);
	u16 night = getFaceLight(LIGHTBANK_NIGHT, n, n2, face_dir, ndef);
	return day | (night << 8);
}

/*
	Calculate smooth lighting at the XYZ- corner of p.
	Both light banks
*/
static u16 getSmoothLightCombined(v3s16 p, MeshMakeData *data)
{
	static const v3s16 dirs8[8] = {
		v3s16(0,0,0),
		v3s16(0,0,1),
		v3s16(0,1,0),
		v3s16(0,1,1),
		v3s16(1,0,0),
		v3s16(1,1,0),
		v3s16(1,0,1),
		v3s16(1,1,1),
	};

	INodeDefManager *ndef = data->m_client->ndef();

	u16 ambient_occlusion = 0;
	u16 light_count = 0;
	u8 light_source_max = 0;
	u16 light_day = 0;
	u16 light_night = 0;

	for (u32 i = 0; i < 8; i++)
	{
		MapNode n = data->m_vmanip.getNodeNoExNoEmerge(p - dirs8[i]);

		// if it's CONTENT_IGNORE we can't do any light calculations
		if (n.getContent() == CONTENT_IGNORE) {
			continue;
		}

		const ContentFeatures &f = ndef->get(n);
		if (f.light_source > light_source_max)
			light_source_max = f.light_source;
		// Check f.solidness because fast-style leaves look better this way
		if (f.param_type == CPT_LIGHT && f.solidness != 2) {
			light_day += decode_light(n.getLightNoChecks(LIGHTBANK_DAY, &f));
			light_night += decode_light(n.getLightNoChecks(LIGHTBANK_NIGHT, &f));
			light_count++;
		} else {
			ambient_occlusion++;
		}
	}

	if(light_count == 0)
		return 0xffff;

	light_day /= light_count;
	light_night /= light_count;

	// Boost brightness around light sources
	bool skip_ambient_occlusion_day = false;
	if(decode_light(light_source_max) >= light_day) {
		light_day = decode_light(light_source_max);
		skip_ambient_occlusion_day = true;
	}

	bool skip_ambient_occlusion_night = false;
	if(decode_light(light_source_max) >= light_night) {
		light_night = decode_light(light_source_max);
		skip_ambient_occlusion_night = true;
	}

	if (ambient_occlusion > 4)
	{
		static const float ao_gamma = rangelim(
			g_settings->getFloat("ambient_occlusion_gamma"), 0.25, 4.0);

		// Table of gamma space multiply factors.
		static const float light_amount[3] = {
			powf(0.75, 1.0 / ao_gamma),
			powf(0.5,  1.0 / ao_gamma),
			powf(0.25, 1.0 / ao_gamma)
		};

		//calculate table index for gamma space multiplier
		ambient_occlusion -= 5;

		if (!skip_ambient_occlusion_day)
			light_day = rangelim(core::round32(light_day*light_amount[ambient_occlusion]), 0, 255);
		if (!skip_ambient_occlusion_night)
			light_night = rangelim(core::round32(light_night*light_amount[ambient_occlusion]), 0, 255);
	}

	return light_day | (light_night << 8);
}

/*
	Calculate smooth lighting at the given corner of p.
	Both light banks.
*/
u16 getSmoothLight(v3s16 p, v3s16 corner, MeshMakeData *data)
{
	if(corner.X == 1) p.X += 1;
	// else corner.X == -1
	if(corner.Y == 1) p.Y += 1;
	// else corner.Y == -1
	if(corner.Z == 1) p.Z += 1;
	// else corner.Z == -1

	return getSmoothLightCombined(p, data);
}

void get_sunlight_color(video::SColorf *sunlight, u32 daynight_ratio){
	f32 rg = daynight_ratio / 1000.0f - 0.04f;
	f32 b = (0.98f * daynight_ratio) / 1000.0f + 0.078f;
	sunlight->r = rg;
	sunlight->g = rg;
	sunlight->b = b;
}

void final_color_blend(video::SColor *result,
		u16 light, u32 daynight_ratio)
{
	video::SColorf dayLight;
	get_sunlight_color(&dayLight, daynight_ratio);
	final_color_blend(result,
		encode_light_and_color(light, video::SColor(0xFFFFFFFF), 0), dayLight);
}

void final_color_blend(video::SColor *result,
		const video::SColor &data, const video::SColorf &dayLight)
{
	static const video::SColorf artificialColor(1.04f, 1.04f, 1.04f);

	video::SColorf c(data);
	f32 n = 1 - c.a;

	f32 r = c.r * (c.a * dayLight.r + n * artificialColor.r) * 2.0f;
	f32 g = c.g * (c.a * dayLight.g + n * artificialColor.g) * 2.0f;
	f32 b = c.b * (c.a * dayLight.b + n * artificialColor.b) * 2.0f;

	// Emphase blue a bit in darker places
	// Each entry of this array represents a range of 8 blue levels
	static const u8 emphase_blue_when_dark[32] = {
		1, 4, 6, 6, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	b += emphase_blue_when_dark[irr::core::clamp((s32) ((r + g + b) / 3 * 255),
		0, 255) / 8] / 255.0f;

	result->setRed(core::clamp((s32) (r * 255.0f), 0, 255));
	result->setGreen(core::clamp((s32) (g * 255.0f), 0, 255));
	result->setBlue(core::clamp((s32) (b * 255.0f), 0, 255));
}

video::SColor encode_light_and_color(u16 light, const video::SColor &color,
	u8 emissive_light)
{
	// Get components
	f32 day = (light & 0xff) / 255.0f;
	f32 night = (light >> 8) / 255.0f;
	// Add emissive light
	night += emissive_light * 0.01f;
	if (night > 255)
		night = 255;
	// Since we don't know if the day light is sunlight or
	// artificial light, assume it is artificial when the night
	// light bank is also lit.
	if (day < night)
		day = 0;
	else
		day = day - night;
	f32 sum = day + night;
	// Ratio of sunlight:
	float r;
	if (sum > 0)
		r = day / sum;
	else
		r = 0;
	// Average light:
	float b = (day + night) / 2;
	return video::SColor(r * 255, b * color.getRed(), b * color.getGreen(),
		b * color.getBlue());
}
