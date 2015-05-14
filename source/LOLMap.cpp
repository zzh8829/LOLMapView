#include "LOLMap.h"
#include <fstream>
using namespace std;

template<typename T>
void Read(T& data,istream& in)
{
	in.read((char*)&data,sizeof(T));
}

template<typename T>
void Read(T& data,size_t num,istream& in)
{
	in.read((char*)&data,num);
}

LOLMap* read_map(const char* filename)
{
	cout << "Reading " << filename << endl;
	ifstream nvr(filename,ios::binary);

	LOLMap *map = new LOLMap();

	Read(map->magic,nvr);
	Read(map->version,nvr);
	Read(map->num_material,nvr);
	Read(map->num_vertex_list,nvr);
	Read(map->num_index_list,nvr);
	Read(map->num_model,nvr);
	Read(map->num_unknown,nvr);
	
#if 1
	cout << "magic " << map->magic << endl;
	cout << "version " << map->version << endl;
	cout << "num_material " << map->num_material << endl;
	cout << "num_vertex_list " << map->num_vertex_list << endl;
	cout << "num_index_list " << map->num_index_list << endl;
	cout << "num_model " << map->num_model << endl;
	cout << "num_unknown " << map->num_unknown << endl;
#endif

	map->materials = new LOLMapMaterial[map->num_material];
	for(int i=0;i!=map->num_material;i++)
	{
		Read(map->materials[i],nvr);
		if(strstr(map->materials[i].textures[0].filename,"_floor") ||
			strstr(map->materials[i].textures[0].filename,"_dirt") ||
			strstr(map->materials[i].textures[0].filename,"grass") ||
			strstr(map->materials[i].textures[0].filename,"RiverBed") ||
			strstr(map->materials[i].textures[0].filename,"_project"))
		{
			if(!map->materials[i].flag1)
				map->materials[i].flag2 |= 1;
		}

#if 0
		map->materials[i].dump(cout);
#endif
	}

	map->vertex_lists = new LOLMapVertexList[map->num_vertex_list];
	for(int i=0;i!=map->num_vertex_list;i++)
	{
		Read(map->vertex_lists[i].size,nvr);
		map->vertex_lists[i].vertices = new float[map->vertex_lists[i].size/4];
		Read(map->vertex_lists[i].vertices[0],map->vertex_lists[i].size,nvr);

		/*
		cout << "Vertex List: " << 
			map->vertex_lists[i].size << endl;
		*/

		//int a = map->vertex_lists[i].size;
		//cerr << a << " " << a%3 <<" " << a%9 << " " << a%11 << endl;
			/*
		if(i==69)
		{
			for(int j=0;j!=map->vertex_lists[i].size/4;j++)
			{
				if (j==119) cerr << "!!!" << endl;
				cerr << map->vertex_lists[i].vertices[j] << " ";
			}
			cerr << endl;
		}
		*/
		
	}

	map->index_lists = new LOLMapIndexList[map->num_index_list];
	for(int i=0;i!=map->num_index_list;i++)
	{
		Read(map->index_lists[i].size,nvr);
		Read(map->index_lists[i].d3dfmt,nvr);
		map->index_lists[i].indices = new uint16_t[map->index_lists[i].size/2];
		Read(map->index_lists[i].indices[0],map->index_lists[i].size,nvr);

		/*
		cout << "Index List: " << 
			map->index_lists[i].size << " " << 
			map->index_lists[i].d3dfmt << endl;
		*/
	}

	map->models = new LOLMapModel[map->num_model];
	for(int i=0;i!=map->num_model;i++)
	{
		Read(map->models[i],nvr);

#if 0
		cout << "Model: " << endl << "\t" << 
			//map->models[i].flag_1 << " " <<
			//map->models[i].flag_2 << " " << 
			map->models[i].model[0].vertex_index << " " << 
			map->models[i].model[0].vertex_offset << " " << 
			map->models[i].model[0].vertex_length << " " << 
			map->models[i].model[0].index_index << " " << 
			map->models[i].model[0].index_offset << " " << 
			map->models[i].model[0].index_length << " " <<  endl << "\t" << 
			map->models[i].model[1].vertex_index << " " << 
			map->models[i].model[1].vertex_offset << " " << 
			map->models[i].model[1].vertex_length << " " << 
			map->models[i].model[1].index_index << " " << 
			map->models[i].model[1].index_offset << " " << 
			map->models[i].model[1].index_length << " " <<  endl << "\t" << 
			map->materials[map->models[i].material].name << " " << endl;
#endif
	}

	/*
	map->unknowns = new LOLMapUnknown[map->num_unknown];

	for(int i=0;i!=map->num_unknown;i++)
	{
		Read(map->unknowns[i],nvr);
	}
	*/

	nvr.close();

	return map;
}