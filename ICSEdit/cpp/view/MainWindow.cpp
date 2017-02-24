#include "view/MainWindow.h"

using namespace ICSE::view;

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
EVT_PAINT(MainWindow::OnPaint)
EVT_WINDOW_DESTROY(MainWindow::OnDestroy)
END_EVENT_TABLE()

int MainWindow_cpp_nolinkage_args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 24, 0 };

ICSE::view::MainWindow::MainWindow(void)
	: m_mini(nullptr),
	m_sizer(new wxBoxSizer(wxHORIZONTAL)),
	wxFrame((wxFrame *)NULL, -1,  wxT("Hello GL World"), wxPoint(50,50), wxSize(400,200))
{
	this->SetAutoLayout(true);
	this->m_mini = new mini::view::MainSurface(this, MainWindow_cpp_nolinkage_args);
	m_sizer->Add(m_mini, 1, wxFIXED);

	this->SetSizer(m_sizer.get());
}

ICSE::view::MainWindow::~MainWindow(void)
{
	m_sizer.release();
	if(m_mini != nullptr)
	{
		delete m_mini;
		m_mini = nullptr;
	}
}

void ICSE::view::MainWindow::InitGL(void)
{
	
}

void ICSE::view::MainWindow::OnPaint(wxPaintEvent & evt)
{
	this->m_mini->render(evt);
}

void ICSE::view::MainWindow::OnDestroy(wxWindowDestroyEvent & evt)
{
	//wxExit();
}
