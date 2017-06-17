#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdio>

#include "gl/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "Math/Vector3.h"
#include "Math/Matrix4.h"

#if defined(_WIN32) || defined(_WIN64)
	#undef main
	#include "gl/glu.h"
#elif defined(__APPLE__)
	#include "OpenGL/glu.h"
#endif

#if 0
#include "Cg/cg.h"
#include "Cg/cgGL.h"
#endif

#include "LOLMap.h"

#include "Timer.h"
#include "Renderer.h"
#include "Program.h"
#include "Shader.h"
#include "Texture.h"
#include "Window.h"
using namespace std;

const int WIDTH = 1280;
const int HEIGHT = 720;

GLuint glbuffer (GLenum target,const void *data, GLsizei size)
{
	GLuint buffer;
	glGenBuffers(1,&buffer);
	glBindBuffer(target,buffer);
	glBufferData(target,size,data,GL_STATIC_DRAW);
	return buffer;
}

double r()
{
	return ((double)rand()/(RAND_MAX));
}

GLuint gdtexture(const char* file)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	SDL_Surface* sf = IMG_Load(file);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
	glTexImage2D(
		GL_TEXTURE_2D, 0,           /* target, level of detail */
		GL_RGBA,                    /* internal format */
		sf->w, sf->h, 0,           /* width, height, border */
		GL_RGBA, GL_UNSIGNED_BYTE,   /* external format, type */
		sf->pixels                      /* pixels */
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(sf);
	return texture;

}

SDL_Surface* flipVert(SDL_Surface* sfc)
{
	SDL_Surface* result = SDL_CreateRGBSurface(sfc->flags, sfc->w, sfc->h,
		sfc->format->BytesPerPixel * 8, sfc->format->Rmask, sfc->format->Gmask,
		sfc->format->Bmask, sfc->format->Amask);
	if (result == NULL) return NULL;

	uint8_t* pixels = (uint8_t*) sfc->pixels;
	uint8_t* rpixels = (uint8_t*) result->pixels;

	uint32_t pitch = sfc->pitch;
	uint32_t pxlength = pitch*sfc->h;

	for(int line = 0; line < sfc->h; ++line) {
		uint32_t pos = line * pitch;
		memcpy(&rpixels[pos], &pixels[(pxlength-pos)-pitch], pitch);
	}

	return result;
}

void screenshot()
{
	SDL_Surface * sf = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);

	glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, sf->pixels);

	SDL_Surface * flip = flipVert(sf);
	SDL_SaveBMP(flip,"screenshot.bmp");

	SDL_FreeSurface(sf);
	SDL_FreeSurface(flip);
}

Program* map_default;
Program* map_four_blend;

GLint fmvp;
GLint ftex0;
GLint ftex1;
GLint ftex2;
GLint ftex3;
GLint ftex4;

GLint dtex;
GLint dmvp;

Program* split_1;
GLint s1mvp;
GLint s1tex0;
GLint s1tex1;
GLint s1mode;

class RiotMap
{
public:
	string folder;
	LOLMap* map;
	vector<vector<Texture> > texs;
	vector<GLuint> vbufs;
	vector<GLuint> ebufs;

	RiotMap(string folder)
	{
		this->folder = folder;
		map = read_map((folder + "Scene/room.nvr").c_str());

		for(int i=0;i!=map->num_material;i++)
		{
			vector<Texture> vt;
			for(int j=0;j!=8;j++)
			{
				if(map->materials[i].textures[j].filename[0])
				{
					string name = map->materials[i].textures[j].filename;
					name = folder + "Scene/Textures/" +
						name.substr(0,name.size()-3) + "png";
					if(map->materials[i].flag1!=1)
					{
						vt.push_back(Texture::CreateTextureFromFile(name.c_str()));
					} else {
						SDL_Surface* sf = IMG_Load(name.c_str());
						if(!sf)
							continue;
						GLenum format = GL_RGBA;
						if(sf->format->BytesPerPixel == 4)
						{
							if(sf->format->Rmask != 0x000000ff)
								format = GL_BGRA;
						} else {
							if(sf->format->Rmask == 0x000000ff)
								format = GL_RGB;
							else
								format = GL_BGR;
						}
						GLuint texture_;
						glGenTextures(1,&texture_);

						glBindTexture(GL_TEXTURE_2D,texture_);

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP);

						glTexImage2D(
							GL_TEXTURE_2D, 0,           /* target, level of detail */
							GL_RGBA,                    /* internal format */
							sf->w, sf->h, 0,           /* width, height, border */
							format, GL_UNSIGNED_BYTE,   /* external format, type */
							sf->pixels                   /* pixels */
						);
					 	glGenerateMipmap(GL_TEXTURE_2D);
						vt.push_back(Texture(texture_));
						SDL_FreeSurface(sf);
					}

				} else {
					vt.push_back(Texture());
				}
			}
			texs.push_back(vt);
		}
		for(int m=0;m!=map->num_vertex_list;m++)
		{
			vbufs.push_back(
				glbuffer(GL_ARRAY_BUFFER,
					map->vertex_lists[m].vertices,
					map->vertex_lists[m].size
				)
			);
			ebufs.push_back(
				glbuffer(GL_ELEMENT_ARRAY_BUFFER,
					map->index_lists[m].indices,
					map->index_lists[m].size
				)
			);
		}
	}

	void render(Matrix4f mvp)
	{
		for(int m=0;m!=map->num_model;m++)
		{
			if(map->materials[map->models[m].material].flag1 == 3 )
			{
				//continue;
				// Four Blend
				map_four_blend->Use();
				glUniformMatrix4fv(fmvp, 1, GL_TRUE, mvp._m);

				glUniform1i(ftex0,0);
				glUniform1i(ftex1,1);
				glUniform1i(ftex2,2);
				glUniform1i(ftex3,3);
				glUniform1i(ftex4,4);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D,texs[map->models[m].material][1].GetTexture());
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D,texs[map->models[m].material][0].GetTexture());
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D,texs[map->models[m].material][2].GetTexture());
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D,texs[map->models[m].material][4].GetTexture());
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D,texs[map->models[m].material][6].GetTexture());

				glBindBuffer(GL_ARRAY_BUFFER,
					vbufs[map->models[m].model[0].vertex_index]);

				glVertexAttribPointer(
					Program::POSITION,
					3,
					GL_FLOAT,
					GL_FALSE,
					sizeof(GLfloat)*11,
					(const GLvoid*)(0)
				);

				glVertexAttribPointer(
					Program::UV0,
					2,
					GL_FLOAT,
					GL_FALSE,
					sizeof(GLfloat)*11,
					(const GLvoid*)(sizeof(GLfloat)*6)
				);

				glVertexAttribPointer(
					Program::UV1,
					2,
					GL_FLOAT,
					GL_FALSE,
					sizeof(GLfloat)*11,
					(const GLvoid*)(sizeof(GLfloat)*8)
				);


				glEnableVertexAttribArray(Program::POSITION);
				glEnableVertexAttribArray(Program::UV0);
				glEnableVertexAttribArray(Program::UV1);


				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
					ebufs[map->models[m].model[0].index_index]);
				glDrawElements(
					GL_TRIANGLES,
					map->models[m].model[0].index_length,
					GL_UNSIGNED_SHORT,
					(const GLvoid*)(sizeof(GLushort)*map->models[m].model[0].index_offset)
				);
				glDisableVertexAttribArray(Program::POSITION);
				glDisableVertexAttribArray(Program::UV0);
				glDisableVertexAttribArray(Program::UV1);

			}
		/*
		}

		for(int m=0;m!=map->num_model;m++)
		{
			*/
			if(map->materials[map->models[m].material].flag1 == 1) {
				//continue;
				map_default->Use();

				glUniformMatrix4fv(dmvp, 1, GL_TRUE, mvp._m);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D,texs[map->models[m].material][0].GetTexture());
				glUniform1i(dtex,0);

				glBindBuffer(GL_ARRAY_BUFFER,
					vbufs[map->models[m].model[0].vertex_index]);

				static int strides[] = {10,9,9,11};

				int stride = strides[map->materials[map->models[m].material].flag1];
				int uvoff = 6;

				//if(map->materials[map->models[m].material].flag1 ==1  ) continue;
				//if(map->materials[map->models[m].material].flag2 !=17 ) continue;

				//if(map->materials[map->models[m].material].flag1 ==3 ) uvoff = 6;

				glVertexAttribPointer(
					Program::POSITION,
					3,
					GL_FLOAT,
					GL_FALSE,
					sizeof(GLfloat)*stride,
					(const GLvoid*)(0)
				);

				glVertexAttribPointer(
					Program::UV0,
					2,
					GL_FLOAT,
					GL_FALSE,
					sizeof(GLfloat)*stride,
					(const GLvoid*)(sizeof(GLfloat)*uvoff)
				);


				glEnableVertexAttribArray(Program::POSITION);
				glEnableVertexAttribArray(Program::UV0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
					ebufs[map->models[m].model[0].index_index]);
				glDrawElements(
					GL_TRIANGLES,
					map->models[m].model[0].index_length,
					GL_UNSIGNED_SHORT,
					(const GLvoid*)(sizeof(GLushort)*map->models[m].model[0].index_offset)
				);
				glDisableVertexAttribArray(Program::POSITION);
				glDisableVertexAttribArray(Program::UV0);
			}
		/*
		}

		for(int m=0;m!=map->num_model;m++)
		{
			*/

			if(map->materials[map->models[m].material].flag1 == 0) {
				//continue;
				map_default->Use();

				glUniformMatrix4fv(dmvp, 1, GL_TRUE, mvp._m);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D,texs[map->models[m].material][0].GetTexture());
				glUniform1i(dtex,0);

				glBindBuffer(GL_ARRAY_BUFFER,
					vbufs[map->models[m].model[0].vertex_index]);

				int stride;
				if(map->materials[map->models[m].material].flag2 == 0)
				{
					stride = 9;
				} else {
					stride = 10;
				}

				//if(map->materials[map->models[m].material].flag1 ==1  ) continue;
				//if(map->materials[map->models[m].material].flag2 !=17 ) continue;

				//if(map->materials[map->models[m].material].flag1 ==3 ) uvoff = 6;

				glVertexAttribPointer(
					Program::POSITION,
					3,
					GL_FLOAT,
					GL_FALSE,
					sizeof(GLfloat)*stride,
					(const GLvoid*)(0)
				);

				glVertexAttribPointer(
					Program::UV0,
					2,
					GL_FLOAT,
					GL_FALSE,
					sizeof(GLfloat)*stride,
					(const GLvoid*)(sizeof(GLfloat)*6)
				);


				glEnableVertexAttribArray(Program::POSITION);
				glEnableVertexAttribArray(Program::UV0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
					ebufs[map->models[m].model[0].index_index]);
				glDrawElements(
					GL_TRIANGLES,
					map->models[m].model[0].index_length,
					GL_UNSIGNED_SHORT,
					(const GLvoid*)(sizeof(GLushort)*map->models[m].model[0].index_offset)
				);

				glDisableVertexAttribArray(Program::POSITION);
				glDisableVertexAttribArray(Program::UV0);
			}
			/*
		}
		for(int m=0;m!=map->num_model;m++)
		{
			*/
			if(map->materials[map->models[m].material].flag1 == 2) {
				//continue;
				map_default->Use();

				glUniformMatrix4fv(dmvp, 1, GL_TRUE, mvp._m);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D,texs[map->models[m].material][0].GetTexture());
				glUniform1i(dtex,0);

				glBindBuffer(GL_ARRAY_BUFFER,
					vbufs[map->models[m].model[0].vertex_index]);

				int stride = 9;

				//(map->materials[map->models[m].material].flag1 ==1  ) continue;
				//if(map->materials[map->models[m].material].flag2 !=17 ) continue;

				//if(map->materials[map->models[m].material].flag1 ==3 ) uvoff = 6;

				glVertexAttribPointer(
					Program::POSITION,
					3,
					GL_FLOAT,
					GL_FALSE,
					sizeof(GLfloat)*stride,
					(const GLvoid*)(0)
				);

				glVertexAttribPointer(
					Program::UV0,
					2,
					GL_FLOAT,
					GL_FALSE,
					sizeof(GLfloat)*stride,
					(const GLvoid*)(sizeof(GLfloat)*6)
				);


				glEnableVertexAttribArray(Program::POSITION);
				glEnableVertexAttribArray(Program::UV0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
					ebufs[map->models[m].model[0].index_index]);
				glDrawElements(
					GL_TRIANGLES,
					map->models[m].model[0].index_length,
					GL_UNSIGNED_SHORT,
					(const GLvoid*)(sizeof(GLushort)*map->models[m].model[0].index_offset)
				);
				glDisableVertexAttribArray(Program::POSITION);
				glDisableVertexAttribArray(Program::UV0);
			}
			/*
		}


		for(int m=0;m!=map->num_model;m++)
		{
			*/
			if(map->materials[map->models[m].material].flag1 > 3)
			{
				//cout << "fk" << endl;
			}
		}
	}
};

class FrameBuffer
{
public:
	GLuint textureId;
	GLuint rboId;
	GLuint fboId;

	FrameBuffer()
	{

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0,
		             GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);


		glGenRenderbuffers(1, &rboId);
		glBindRenderbuffer(GL_RENDERBUFFER, rboId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		                      WIDTH, HEIGHT);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);

		// attach the texture to FBO color attachment point
		glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
		                       GL_COLOR_ATTACHMENT0,  // 2. attachment point
		                       GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
		                       textureId,             // 4. tex ID
		                       0);                    // 5. mipmap level: 0(base)

		// attach the renderbuffer to depth attachment point
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
		                          GL_DEPTH_ATTACHMENT, // 2. attachment point
		                          GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
		                          rboId);              // 4. rbo ID

		// check FBO status
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE)
		    cerr << "WTF" << endl;

		// switch back to window-system-provided framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	}

	void unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};
static SDL_Color SDL_RED	 = {255, 0, 0, 0};
static SDL_Color SDL_GREEN	 = {0, 255, 0, 0};
static SDL_Color SDL_BLUE	 = {0, 0, 255, 0};
static SDL_Color SDL_WHITE	 = {255, 255, 255, 0};
static SDL_Color SDL_BLACK	 = {0,0,0,0};

class TextRenderer
{
public:
	TextRenderer()
	{}

	void Render(string text, SDL_Color color, int x,int y, int size)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		gluOrtho2D(0, WIDTH, HEIGHT, 0); // m_Width and m_Height is the resolution of window
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		TTF_Font * font = TTF_OpenFont("Arial.ttf", size);
		SDL_Surface * sFont = TTF_RenderText_Blended(font, text.c_str(), color);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, sFont->pixels);

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0,0); glVertex2f(x, y);
			glTexCoord2f(1,0); glVertex2f(x + sFont->w, y);
			glTexCoord2f(1,1); glVertex2f(x + sFont->w, y + sFont->h);
			glTexCoord2f(0,1); glVertex2f(x, y + sFont->h);
		}
		glEnd();

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glDeleteTextures(1, &texture);
		TTF_CloseFont(font);
		SDL_FreeSurface(sFont);
	}
};

#define RENDERMAP 1

float quad[] = {
	0,0,
	1,0,
	1,1,
	0,0,
	1,1,
	0,1
};

int splitmode = 0;

GLuint quadbuf;

const char* cmd = "ffmpeg -r 20 -f rawvideo -pix_fmt rgba -s 1280x720 -i - "
	                "-threads 0 -preset fast -y -crf 21 -vf vflip output2.mp4";

int main (int argc, char* argv[])
{
	TTF_Init();
	TextRenderer* textrender = new TextRenderer;
	// open pipe to ffmpeg's stdin in binary write mode
	// FILE* ffmpeg = _popen(cmd, "wb");

	int* buffer = new int[WIDTH*HEIGHT];

	//freopen("err.log","w",stderr);
	//freopen("out.log","w",stdout);

	Window* window = new Window(WIDTH,HEIGHT,"Nothing v0.0.4");
	Renderer* renderer = new Renderer(window);

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

#if RENDERMAP
	RiotMap map1("lol/LEVELS/Map1/");
	RiotMap map11("lol/lolpbe/LEVELS/Map11/");
	//RiotMap map12("lol/LEVELS/Map12/");
#endif

	FrameBuffer map1frame;
	FrameBuffer map11frame;

	quadbuf = glbuffer(GL_ARRAY_BUFFER, quad, sizeof(quad));

#if 0
	CGcontext cgContext = cgCreateContext();
	cgGLEnableProfile(CG_PROFILE_GLSLV);
	cgGLEnableProfile(CG_PROFILE_GLSLF);

	ifstream ifs("Shaders/LIT_VS.vs_2_0",ios::binary);
	ifs.seekg(0,ios::end);
	size_t size = ifs.tellg();
	ifs.seekg(0,ios::beg);
	char* vs = new char[size+1];
	ifs.read(vs,size);
	vs[size]=0;

	CGprogram vsp = cgCreateProgram(cgContext,CG_SOURCE,vs,CG_PROFILE_GLSLV,"main",NULL);
	const char* wtf = cgGetProgramString(vsp,CG_COMPILED_PROGRAM);

	ofstream ofs("vs.out");
	ofs << wtf << endl;
#endif

	map_default = new Program();

	map_default->AttachShader(Shader::CreateShaderFromFile(GL_VERTEX_SHADER, "Shaders/MAP_DEFAULT.vert"));
	map_default->AttachShader(Shader::CreateShaderFromFile(GL_FRAGMENT_SHADER, "Shaders/MAP_DEFAULT.frag"));
	map_default->Link();

	dtex = map_default->GetUniformLocation("Z_TEX0");
	dmvp = map_default->GetUniformLocation("Z_MODEL_VIEW_PROJECTION");

	map_four_blend = new Program();

	map_four_blend->AttachShader(Shader::CreateShaderFromFile(GL_VERTEX_SHADER, "Shaders/MAP_FOUR_BLEND.vert"));
	map_four_blend->AttachShader(Shader::CreateShaderFromFile(GL_FRAGMENT_SHADER, "Shaders/MAP_FOUR_BLEND.frag"));
	map_four_blend->Link();

	fmvp = map_four_blend->GetUniformLocation("Z_MODEL_VIEW_PROJECTION");
	ftex0 = map_four_blend->GetUniformLocation("Z_TEX0");
	ftex1 = map_four_blend->GetUniformLocation("Z_TEX1");
	ftex2 = map_four_blend->GetUniformLocation("Z_TEX2");
	ftex3 = map_four_blend->GetUniformLocation("Z_TEX3");
	ftex4 = map_four_blend->GetUniformLocation("Z_TEX4");


	split_1 = new Program();

	split_1->AttachShader(Shader::CreateShaderFromFile(GL_VERTEX_SHADER, "Shaders/SPLIT_1.vert"));
	split_1->AttachShader(Shader::CreateShaderFromFile(GL_FRAGMENT_SHADER, "Shaders/SPLIT_1.frag"));
	split_1->Link();

	s1mvp = split_1->GetUniformLocation("Z_MODEL_VIEW_PROJECTION");
	s1tex0 = split_1->GetUniformLocation("Z_TEX0");
	s1tex1 = split_1->GetUniformLocation("Z_TEX1");
	s1mode = split_1->GetUniformLocation("MODE");

	Matrix4f projection,view,model,mvp;

	projection = Matrix4f::CreatePerspective(45.0f, WIDTH/(float)HEIGHT , 1, 1e6);
	model = Matrix4f::IDENTITY;

	Matrix4f mvps = Matrix4f::CreateOrthographic(0,1,0,1,-1,1);

	window->SetRelativeMouseMode(true);

	Vector3f position(0,0,5);
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;
	float FoV = 45.0f;
	float moveSpeed = 3.0f;
	float mouseSpeed = 0.0005f;
	float zoomSpeed = 2.0f;

	float lastSec = Timer::GetTimeInSeconds();

#if 0
	ofstream dump("model.txt");
	//int list[] = {1,11,13};
	//for(int i=0;i!= sizeof(list)/sizeof(list[0]);i++)
	//int m = 2;
	//{
	//	int m = list[i];
	for(int m=0;m!=map->num_model;m++)
	{
		if(map->materials[map->models[m].material].flag1!=1)continue;
		dump << "Model: " << m << endl;
		dump << map->models[m].flag_1 << endl;
		dump << map->models[m].flag_2 << endl;
		for(int j=0;j!=10;j++)
			dump << map->models[m].b[j] << " ";
		dump << endl;
		dump << "\t" <<
			map->models[m].model[0].vertex_index << " " <<
			map->models[m].model[0].vertex_offset << " " <<
			map->models[m].model[0].vertex_length << " " <<
			map->models[m].model[0].index_index << " " <<
			map->models[m].model[0].index_offset << " " <<
			map->models[m].model[0].index_length << " " <<  endl << "\t" <<
			map->models[m].model[1].vertex_index << " " <<
			map->models[m].model[1].vertex_offset << " " <<
			map->models[m].model[1].vertex_length << " " <<
			map->models[m].model[1].index_index << " " <<
			map->models[m].model[1].index_offset << " " <<
			map->models[m].model[1].index_length << " " <<  endl;

		map->materials[map->models[m].material].dump(dump);

		int s = 9;

		for(int j=0;j!=3;j++)
		{
			for(int i=0;i!=9;i++)
			{
				int idx = map->index_lists[map->models[m].model[0].index_index].indices[map->models[m].model[0].index_offset+j];

				if(i==10)
				{
					union
					{
						float f;
						uint32_t_t i;
					}u;
					u.f = map->vertex_lists[map->models[m].model[0].vertex_index].vertices[idx*s+i];
					dump << hex << u.i << endl;
				} else {
					dump << map->vertex_lists[map->models[m].model[0].vertex_index].vertices[idx*s+i] << " ";
				}
			}
			dump << endl;
		}


		dump << endl;
	}
	dump.close();
#endif

	int Inter = 1;
	int Model = 10;
	int zz = 10;
	int qq = 10;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	SDL_Event e;
	bool running = true;
	while(running)
	{
		while(window->PollEvent(e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				if(e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					running = false;
				else if(e.key.keysym.sym == SDLK_F12)
				{
					screenshot();
				}
				else if(e.key.keysym.scancode == SDL_SCANCODE_H)
				{
					//cout << zz << endl;
					zz++;
				}
				else if(e.key.keysym.scancode == SDL_SCANCODE_J)
					zz--;
				else if(e.key.keysym.scancode == SDL_SCANCODE_K) {
					//cout << qq << endl;
					qq++;
				}
				else if(e.key.keysym.scancode == SDL_SCANCODE_L)
					qq--;
				else if(e.key.keysym.scancode == SDL_SCANCODE_M)
					splitmode = (splitmode + 1)%5;
				else if(e.key.keysym.scancode == SDL_SCANCODE_1)
					splitmode = 0;
				else if(e.key.keysym.scancode == SDL_SCANCODE_2)
					splitmode = 1;
				else if(e.key.keysym.scancode == SDL_SCANCODE_3)
					splitmode = 2;
				else if(e.key.keysym.scancode == SDL_SCANCODE_4)
					splitmode = 3;
				else if(e.key.keysym.scancode == SDL_SCANCODE_5)
					splitmode = 4;
				break;
			case SDL_MOUSEWHEEL:
				FoV -= zoomSpeed * e.wheel.y;
				break;
			case SDL_MOUSEMOTION:
				horizontalAngle -= mouseSpeed * e.motion.xrel;
				verticalAngle -= mouseSpeed * e.motion.yrel;
				break;
			case SDL_WINDOWEVENT:
				switch(e.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					renderer->Resize(e.window.data1,e.window.data2);
					break;
				}
				break;
			}
		}

		float curSec = Timer::GetTimeInSeconds();
		float deltaTime = curSec - lastSec;
		lastSec = curSec;

		Vector3f direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		Vector3f right(
			sin(horizontalAngle - 3.14f/2.0f),
			0,
			cos(horizontalAngle - 3.14f/2.0f)
		);

		Vector3f up = right.Cross(direction);

		const uint8_t* keyboard = window->GetKeyboardState(0);

		if(keyboard[SDL_SCANCODE_Z])
		{
			moveSpeed = 2000;
		} else {
			moveSpeed = 500;
		}

		if(keyboard[SDL_SCANCODE_W])
		{
			position += direction * deltaTime * moveSpeed;
		}
		if(keyboard[SDL_SCANCODE_S])
		{
			position -= direction * deltaTime * moveSpeed;
		}
		if(keyboard[SDL_SCANCODE_D])
		{
			position += right * deltaTime * moveSpeed;
		}
		if(keyboard[SDL_SCANCODE_A])
		{
			position -= right * deltaTime * moveSpeed;
		}
		if(keyboard[SDL_SCANCODE_SPACE])
		{
			position += up * deltaTime * moveSpeed;
		}
		if(keyboard[SDL_SCANCODE_LSHIFT])
		{
			position -= up * deltaTime * moveSpeed;
		}


		view = Matrix4f::CreateLookAt(
			position,
			position+direction,
			up
		);


		Matrix4f mvp = projection * view * model;

		mvp.m02 = -mvp.m02;
		mvp.m12 = -mvp.m12;
		mvp.m22 = -mvp.m22;
		mvp.m32 = -mvp.m32;

		//glDepthMask(true);

		float t0 = Timer::GetTimeInSeconds();

#if RENDERMAP
		map1frame.bind();
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		map1.render(mvp);
		map1frame.unbind();
#endif
		float t1 = Timer::GetTimeInSeconds();
#if RENDERMAP
		map11frame.bind();
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		map11.render(mvp);
		map11frame.unbind();
#endif
		float t2 = Timer::GetTimeInSeconds();


		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		split_1->Use();

		glUniformMatrix4fv(s1mvp, 1, GL_TRUE, mvps._m);

		glUniform1i(s1mode,splitmode);

		glUniform1i(s1tex0,0);
		glUniform1i(s1tex1,1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,map1frame.textureId);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,map11frame.textureId);

		glBindBuffer(GL_ARRAY_BUFFER, quadbuf);

		glVertexAttribPointer(
			Program::POSITION,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(const GLvoid*)(0)
		);

		glVertexAttribPointer(
			Program::UV0,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(const GLvoid*)(0)
		);

		glEnableVertexAttribArray(Program::POSITION);
		glEnableVertexAttribArray(Program::UV0);

		glDrawArrays(GL_TRIANGLES,0,6);

		glDisableVertexAttribArray(Program::POSITION);
		glDisableVertexAttribArray(Program::UV0);

		glUseProgram(0);

		textrender->Render("CATT",SDL_BLUE,100,100,50);

		window->SwapBuffers();

		//glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		//fwrite(buffer, sizeof(int)*WIDTH*HEIGHT, 1, ffmpeg);
	}
	// _pclose(ffmpeg);
	return 0;
}
