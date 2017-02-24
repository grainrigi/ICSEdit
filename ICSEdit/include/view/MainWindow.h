#pragma once
#include "mini/view/MainSurface.h"

namespace ICSE {
namespace view {

	class MainWindow : public wxFrame {		
		mini::view::MainSurface * m_mini;
		std::unique_ptr<wxBoxSizer> m_sizer;
	 public:
		MainWindow(void);
		virtual ~MainWindow(void);

		void InitGL(void);

		virtual void OnPaint(wxPaintEvent &evt);
		void OnDestroy(wxWindowDestroyEvent &evt);

	 private:
		
		 wxDECLARE_EVENT_TABLE();
	};

}
}
