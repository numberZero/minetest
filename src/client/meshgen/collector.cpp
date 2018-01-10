#include "collector.h"
#include <stdexcept>
#include "log.h"
#include "mesh.h"

void MeshCollector::append(const TileSpec &tile, const video::S3DVertex *vertices,
		u32 numVertices, const u16 *indices, u32 numIndices)
{
	for (int layernum = 0; layernum < MAX_TILE_LAYERS; layernum++) {
		const TileLayer *layer = &tile.layers[layernum];
		if (layer->texture_id == 0)
			continue;
		append(*layer, vertices, numVertices, indices, numIndices, layernum,
				tile.world_aligned);
	}
}

void MeshCollector::append(const TileLayer &layer, const video::S3DVertex *vertices,
		u32 numVertices, const u16 *indices, u32 numIndices, u8 layernum,
		bool use_scale)
{
	u16 buf_id;
	PreMeshBuffer &p = findBuffer(layer, layernum, numIndices, &buf_id);
	trackVertices(layer, layernum, numVertices, numIndices, buf_id);

	f32 scale = 1.0;
	if (use_scale)
		scale = 1.0 / layer.scale;

	u32 vertex_count = p.vertices.size();
	for (u32 i = 0; i < numVertices; i++) {
		video::S3DVertex vert(vertices[i].Pos, vertices[i].Normal,
				vertices[i].Color, scale * vertices[i].TCoords);
		p.vertices.push_back(vert);
	}

	for (u32 i = 0; i < numIndices; i++) {
		u32 j = indices[i] + vertex_count;
		p.indices.push_back(j);
	}
}

void MeshCollector::append(const TileSpec &tile, const video::S3DVertex *vertices,
		u32 numVertices, const u16 *indices, u32 numIndices, v3f pos,
		video::SColor c, u8 light_source)
{
	for (int layernum = 0; layernum < MAX_TILE_LAYERS; layernum++) {
		const TileLayer *layer = &tile.layers[layernum];
		if (layer->texture_id == 0)
			continue;
		append(*layer, vertices, numVertices, indices, numIndices, pos, c,
				light_source, layernum, tile.world_aligned);
	}
}

void MeshCollector::append(const TileLayer &layer, const video::S3DVertex *vertices,
		u32 numVertices, const u16 *indices, u32 numIndices, v3f pos,
		video::SColor c, u8 light_source, u8 layernum, bool use_scale)
{
	u16 buf_id;
	PreMeshBuffer &p = findBuffer(layer, layernum, numIndices, &buf_id);
	trackVertices(layer, layernum, numVertices, numIndices, buf_id);

	f32 scale = 1.0;
	if (use_scale)
		scale = 1.0 / layer.scale;

	video::SColor original_c = c;
	u32 vertex_count = p.vertices.size();
	for (u32 i = 0; i < numVertices; i++) {
		if (!light_source) {
			c = original_c;
			applyFacesShading(c, vertices[i].Normal);
		}
		video::S3DVertex vert(vertices[i].Pos + pos, vertices[i].Normal, c,
				scale * vertices[i].TCoords);
		p.vertices.push_back(vert);
	}

	for (u32 i = 0; i < numIndices; i++) {
		u32 j = indices[i] + vertex_count;
		p.indices.push_back(j);
	}
}

PreMeshBuffer &MeshCollector::findBuffer(const TileLayer& layer, u8 layernum,
		u32 numIndices, u16 *buf_id)
{
	if (numIndices > 65535) {
		errorstream << "FIXME: MeshCollector::append() called with numIndices="
			<< numIndices << " (limit 65535)" << std::endl;
		throw std::invalid_argument("MeshCollector does not support more than 65535 indices");
	}
	*buf_id = 0;
	std::vector<PreMeshBuffer> &buffers = prebuffers[layernum];
	for (PreMeshBuffer &p : buffers) {
		if (p.layer == layer && p.indices.size() + numIndices <= 65535)
			return p;
		++*buf_id;
	}
	PreMeshBuffer pp;
	pp.layer = layer;
	buffers.push_back(pp);
	return buffers.back();
}

void MeshCollector::trackVertices(const TileLayer& layer, u8 layernum,
		u32 numVertices, u32 numIndices, u16 buf_id)
{
	if (!current_tracker)
		return;
	if (layernum == 0) {
		current_tracker->layer0_vertex_count += numVertices;
		current_tracker->layer0_index_count += numIndices;
	}
	const PreMeshBuffer &prebuf = prebuffers[layernum][buf_id];
	u16 vertex_start = prebuf.vertices.size();
	u16 index_start = prebuf.indices.size();
	for (TrackData &track : current_tracker->faces) {
		if (track.layer != layernum)
			continue;
		if (track.buffer != buf_id)
			continue;
		if (track.first_index_index + track.index_count != index_start)
			continue;
		assert(track.first_vertex_index + track.vertex_count == vertex_start);
		track.vertex_count += numVertices;
		track.index_count += numIndices;
		return;
	}
	current_tracker->faces.push_back({
		layernum, buf_id, vertex_start, index_start, numVertices, numIndices
	});
}
