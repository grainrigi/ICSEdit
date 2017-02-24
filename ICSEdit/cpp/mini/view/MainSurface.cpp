#include "mini/view/MainSurface.h"
#include "graphics/Mesh2DRenderer.h"

using namespace ICSE::mini::view;
using namespace ICSE::graphics;

// Vertices and faces of a simple cube to demonstrate 3D render
// source: http://www.opengl.org/resources/code/samples/glut_examples/examples/cube.c
GLfloat v[8][3];
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
	{ 0, 1, 2, 3 },{ 3, 2, 6, 7 },{ 7, 6, 5, 4 },
	{ 4, 5, 1, 0 },{ 5, 6, 2, 1 },{ 7, 4, 0, 3 } };

MainSurface::MainSurface(wxFrame *parent, int* args)
	: wxGLCanvas(parent, wxID_ANY, args, wxPoint(50, 50), wxSize(200, 200))
	,m_context(nullptr)
	,m_mesh(nullptr)
{

		m_context = new wxGLContext(this);
		
	// prepare a simple cube to demonstrate 3D render
	// source: http://www.opengl.org/resources/code/samples/glut_examples/examples/cube.c
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

	// To avoid flashing on MSW
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

ICSE::mini::view::MainSurface::~MainSurface(void)
{
	if(m_context != nullptr)
	{
		delete m_context;
		m_context = nullptr;
	}
}

void ICSE::mini::view::MainSurface::render(wxPaintEvent & evt)
{
	if (!IsShown()) return;
	/*GLenum err = glewInit();
	if (err != GLEW_OK)
		throw std::runtime_error("GLEW Initialization failed.");
	Singleton<ICSE::graphics::Mesh2DRenderer>::create();

	this->initGeomerty();*/
	wxGLCanvas::SetCurrent(*m_context);
	this->initGeomerty();
	//wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
	//glViewport(0, 0, 200, 200);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	DrawEnv env;

	//m_mesh->draw(&env);
	
	m_mesh->draw(&env);

	glFlush();
	SwapBuffers();
}

void ICSE::mini::view::MainSurface::prepare3DViewport(int left, int top, int right, int bottom)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
	glClearDepth(1.0f);	// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST); // Enables Depth Testing
	glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_COLOR_MATERIAL);

	glViewport(left, top, right - left, bottom - top);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio_w_h = (float)(right - left) / (float)(bottom - top);
	gluPerspective(45 /*view angle*/, ratio_w_h, 0.1 /*clip close*/, 200 /*clip far*/);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void ICSE::mini::view::MainSurface::prepare2DViewport(int left, int top, int right, int bottom)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(left, top, right - left, bottom - top);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(left, right, bottom, top);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ICSE::mini::view::MainSurface::initGeomerty(void)
{
	float positions[] = {
		0.0f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};
	float colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
	};

	m_mesh.reset(new graphics::Mesh2D(3, Mesh2D::ATTR_POSITION | Mesh2D::ATTR_COLOR, GL_STATIC_DRAW));
	
	memcpy(&(*m_mesh->positions())[0], positions, sizeof(positions));
	memcpy(&(*m_mesh->colors())[0], colors, sizeof(colors));

	m_mesh->upload();
}

int MainSurface::getWidth(void)
{
	return GetSize().x;
}

int MainSurface::getHeight(void)
{
	return GetSize().y;
}

