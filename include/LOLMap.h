#ifndef Z_LOLMAP_H_
#define Z_LOLMAP_H_

#include "Core.h"

using namespace std;

struct LOLMapMaterial
{
	char name[260]; // name
	uint32_t flag1;	// unknown flag
	uint32_t flag2;	// unknown flag 2
	struct
	{
		uint32_t unknown[4]; // idk what is this
		char filename[260]; // texture name
		uint32_t unknown2[4][4]; // seems like matrix
	} textures[8];

	void dump(ostream& out)
	{
		out << name << endl;
		out << flag1 << endl;
		out << flag2 << endl;
		for(int i=0;i!=8;i++)
		{
			if(textures[i].filename[0])
			{
				out << i << " " << textures[i].filename << endl;
			}
		}
		out << endl;
	}

};


struct LOLMapVertexList
{
	uint32_t size;
	float* vertices;
};

struct LOLMapIndexList
{
	uint32_t size;
	uint32_t d3dfmt;
	uint16_t* indices;
};

struct LOLMapModelData
{
	uint32_t vertex_index;
	uint32_t vertex_offset;
	uint32_t vertex_length;
	uint32_t index_index;
	uint32_t index_offset;
	uint32_t index_length;
};

struct LOLMapModel
{
	uint32_t flag_1;
	uint32_t flag_2;
	uint32_t b[10];
	uint32_t material;
	LOLMapModelData model[2];
};

struct LOLMapUnknown
{
	float unknown_1[6];
	int unknown_2[4];
};

struct LOLMap
{
	uint8_t magic[4];
	uint32_t version;
	uint32_t num_material;
	uint32_t num_vertex_list;
	uint32_t num_index_list;
	uint32_t num_model;
	uint32_t num_unknown;
	LOLMapMaterial * materials;
	LOLMapVertexList * vertex_lists;
	LOLMapIndexList * index_lists;
	LOLMapModel * models;
	LOLMapUnknown * unknowns;
};

LOLMap* read_map(const char* filename);

#endif