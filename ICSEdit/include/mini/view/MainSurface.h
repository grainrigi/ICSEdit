#pragma once
#include "graphics/Mesh2D.h"

namespace ICSE {
namespace mini {
namespace view {

	class MainSurface : public wxGLCanvas {		
		wxGLContext * m_context;
		std::unique_ptr<graphics::Mesh2D> m_mesh;
	public:
		MainSurface(wxFrame *parent, int* args);
		virtual ~MainSurface(void);

		int getWidth(void);
		int getHeight(void);

		void render(wxPaintEvent &evt);
		void prepare3DViewport(int left, int top, int right, int bottom);
		void prepare2DViewport(int left, int top, int right, int bottom);
		
	private:
		void initGeomerty(void);
	};

}
}
}
