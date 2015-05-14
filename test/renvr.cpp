#include "include/Core.h"
using namespace std;

//	League of Legends rooom.nvr file spec
//	All data belongs to Riot Inc.
//	Reverse Engineered by Zihao Zhang
//	Only for educational purpose


struct nvr_header
{
#define MAGIC 5396046
	uint32_t magic; // char[4] = "NVR"
#define VERSION 65545 // new format
#define VERSION2 65544 // old format
	uint32_t version;
	
	uint32_t num_mats;
	uint32_t num_vertices;
	uint32_t num_indices;
	uint32_t num_models;
	uint32_t num_unknowns;
};

struct nvr_mat
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
};

#define DEBUG_OUT 0

int main()
{
#if DEBUG_OUT
	freopen("log","w",stdout);
#endif
	FILE* fp = fopen("bin/lol/levels/map1/scene/room.nvr","rb");
	nvr_header header;
	fread(&header,1,28,fp);
	cout << header.magic << endl;
	cout << header.version << endl;

	nvr_mat mat;
	vector<nvr_mat> materials;

	for(int i=0;i!=header.num_mats;i++)
	{
		fread(&mat,1,2988,fp);
		if(strstr(mat.textures[0].filename,"_floor"))
		{
			if(!mat.flag1)
				mat.flag2 |= 1;
		}
		materials.push_back(mat);

#if DEBUG_OUT
		cout << mat.name << endl;
		cout << mat.flag1 << " " << mat.flag2 << endl;
		for(int t=0;t!=8;t++)
		{
			for(int j=0;j!=4;j++)
				cout << hex << mat.textures[t].unknown[j] << " ";
			cout << endl;
			if(*mat.textures[t].filename)
				cout << mat.textures[t].filename << endl;
			for(int j=0;j!=4;j++)
			{
				for(int k=0;k!=4;k++)
					cout << hex << mat.textures[t].unknown2[j][k] << " ";
				cout << endl;
			}
			cout << endl;
		}
		cout << endl;
#endif

	}

	vector< vector<float> > vertices;
	for(int i=0;i!=header.num_vertices;i++)
	{
		uint32_t size;
		fread(&size,1,4,fp);
		vector<float> vf;
		vf.resize(size);
		fread(&vf[0],1,size,fp);
		vertices.push_back(vf);
	}

	vector< vector<float> > indices;
	for(int i=0;i!=header.num_indices;i++)
	{
		struct {
			uint32_t size;
			uint32_t flag; // 101->15 102->16
		} hd;
		fread(&hd,1,8,fp);
		vector<float> vf;
		vf.resize(hd.size);
		fread(&vf[0],1,hd.size,fp);
		indices.push_back(vf);
	}

	struct model
	{
		uint32_t data[25];
	};

	vector<model> models;
	models.resize(header.num_models);
	fread(&models[0],header.num_models,100,fp);

	for(int i=0;i!=header.num_models;i++)
	{
		cout << models[i].data[0] << endl;
	}

	struct unknown
	{
		uint32_t data[10];
	};

	vector<unknown> unknowns;
	unknowns.resize(header.num_unknowns);
	fread(&unknowns[0],header.num_unknowns,40,fp);
	
}