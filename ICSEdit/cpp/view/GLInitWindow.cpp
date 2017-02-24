#include "view/GLInitWindow.h"
#include "graphics/Mesh2DRenderer.h"

using namespace ICSE::view;

BEGIN_EVENT_TABLE(GLInitWindow, wxFrame)
EVT_PAINT(GLInitWindow::OnPaint)
END_EVENT_TABLE()

extern int *MainWindow_cpp_nolinkage_args;

ICSE::view::GLInitWindow::GLInitWindow(int width, int height, wxFrame *parent)
	: wxFrame(parent, -1, wxT("Initializing..."), wxDefaultPosition, wxSize(width, height))
	,m_sizer(new wxBoxSizer(wxHORIZONTAL))
{
	int options[] { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };
	this->m_child = new Surface(this, options);
	m_sizer->Add(m_child, 1, wxEXPAND);

	this->SetSizer(m_sizer.get());
	this->SetAutoLayout(true);
}

ICSE::view::GLInitWindow::~GLInitWindow(void)
{
	if(m_id != 0)
	{
		glutDestroyWindow(m_id);
		m_id = 0;
	}
}


void ICSE::view::GLInitWindow::SetCallback(std::function<void()>& func)
{
	m_initcb = func;
}

void ICSE::view::GLInitWindow::OnPaint(wxPaintEvent &evt)
{
	if (m_initialized)
		return;
	m_child->initcontext();
	GLenum err = glewInit();
	if (err != GLEW_OK)
		throw std::runtime_error("GLEW Initialization failed.");
	if (m_initcb)
		m_initcb();

	this->m_initialized = true;
	this->Hide();
}

ICSE::view::GLInitWindow::Surface::Surface(wxFrame * parent, int * args)
	: wxGLCanvas(parent, -1, args) 
{
}

void ICSE::view::GLInitWindow::Surface::initcontext(void)
{
	m_context = new wxGLContext(this);
	this->SetCurrent(*m_context);
}
