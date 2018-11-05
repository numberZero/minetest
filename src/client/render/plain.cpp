/*
Minetest
Copyright (C) 2010–2013 celeron55, Perttu Ahola <celeron55@gmail.com>
Copyright (C) 2017–2018 numzero, Lobachevskiy Vitaliy <numzer0@yandex.ru>

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

#include "plain.h"
#include "client.h"
#include "shader.h"
#include "client/tile.h"
#include "clientmap.h"

RenderingCorePlain::RenderingCorePlain(
	IrrlichtDevice *_device, Client *_client, Hud *_hud)
	: RenderingCore(_device, _client, _hud)
{
	initMaterial();
}

void RenderingCorePlain::initMaterial()
{
	IShaderSource *s = client->getShaderSource();
	mat.UseMipMaps = false;
	mat.ZBuffer = false;
	mat.ZWriteEnable = false;
	u32 shader = s->getShader("render_fx", TILE_MATERIAL_BASIC, 0);
	mat.MaterialType = s->getShaderInfo(shader).material;
	for (int k = 0; k < 3; ++k) {
		mat.TextureLayer[k].AnisotropicFilter = false;
		mat.TextureLayer[k].BilinearFilter = false;
		mat.TextureLayer[k].TrilinearFilter = false;
		mat.TextureLayer[k].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
		mat.TextureLayer[k].TextureWrapV = video::ETC_CLAMP_TO_EDGE;
	}
}

void RenderingCorePlain::initTextures()
{
	solid = driver->addRenderTargetTexture(
			screensize, "render_fx_solid", video::ECF_A8R8G8B8);
// 	translucent = driver->addRenderTargetTexture(
// 			screensize, "render_fx_translucent", video::ECF_A16B16G16R16F);
	depth = driver->addRenderTargetTexture(
			screensize, "render_fx_depth", video::ECF_D16);
	rt = driver->addRenderTarget();
// 	rt->setTexture({solid, translucent}, depth);
	rt->setTexture(solid, depth);
	mat.TextureLayer[0].Texture = solid;
	mat.TextureLayer[1].Texture = translucent;
	mat.TextureLayer[2].Texture = depth;
}

void RenderingCorePlain::clearTextures()
{
	driver->removeTexture(solid);
// 	driver->removeTexture(translucent);
	driver->removeTexture(depth);
	driver->removeRenderTarget(rt);
}

void RenderingCorePlain::beforeDraw()
{
	driver->setRenderTargetEx(rt, video::ECBF_ALL, skycolor);
}

void RenderingCorePlain::drawAll()
{
	draw3D();
	merge();
	drawHUD();
}

void RenderingCorePlain::merge()
{
	video::SColorf fx = client->getEnv().getClientMap().getPostFxColor(camera->getCameraMode());
	video::SColor vcolor = video::SColorf(fx.a * fx.r, fx.a * fx.g, fx.a * fx.b, 1.0f - fx.a).toSColor();
	const video::S3DVertex vertices[4] = {
			video::S3DVertex( 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, vcolor, 1.0, 0.0),
			video::S3DVertex(-1.0, -1.0, 0.0, 0.0, 0.0, -1.0, vcolor, 0.0, 0.0),
			video::S3DVertex(-1.0, 1.0, 0.0, 0.0, 0.0, -1.0, vcolor, 0.0, 1.0),
			video::S3DVertex( 1.0, 1.0, 0.0, 0.0, 0.0, -1.0, vcolor, 1.0, 1.0),
	};
	static const u16 indices[6] = {0, 1, 2, 2, 3, 0};
	driver->setMaterial(mat);
	driver->setRenderTargetEx(nullptr, video::ECBF_ALL, skycolor);
	driver->drawVertexPrimitiveList(&vertices, 4, &indices, 2);
}
