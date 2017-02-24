#pragma once
#include "graphics/Mesh2D.h"

namespace ICSE {
namespace view {

	class GLInitWindow : public wxFrame  {
		int m_id;
		std::function<void()> m_initcb;
		int m_width;
		int m_height;
		bool m_initialized = false;
		std::unique_ptr<wxBoxSizer> m_sizer;

		class Surface : public wxGLCanvas {
			wxGLContext * m_context;

		public:
			Surface(wxFrame *parent, int* args);
			void initcontext(void);
		};
		Surface *m_child;

	public:
		GLInitWindow(int width, int height, wxFrame *parent);
		~GLInitWindow(void);
		

		void SetCallback(std::function<void()> &func);

		void OnPaint(wxPaintEvent &evt);
	private:
		wxDECLARE_EVENT_TABLE();
	};

}
}
