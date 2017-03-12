/*
(c) 2016,2017 Grain

This file is part of ICSEdit.

ICSEdit is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ICSEdit is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ICSEdit.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "wnd/AWCanvasControl.h"
#include "graphics/MemCanvasRenderTexturePool.h"
#include "graphics/gl/GLVBO.h"
#include "graphics/gl/GLShaderSet.h"
#include "graphics/Geometry.h"
#include "graphics/drawenv.h"

namespace ICSE {
namespace wnd {
	class AWCanvasControl;
	class ControlRenderer;

	class ControlObserver
	{
		ControlRenderer *m_renderer;
	public:
		ControlObserver(ControlRenderer *renderer);
		void NotifyResize(const AWCanvasControl *control);
		void NotifyMove(const AWCanvasControl *control);
		void NotifyDeletion(uint32_t txunitid, uint32_t control_id);

		void Invalidate(void);
	};

	class ControlRenderer {
	public:
		
	private:
		
		struct Vertex
		{
			graphics::Position pos;
			graphics::Coordinate coord;
		};

		class TxPoolHook : public graphics::MemCanvasRenderTexturePool {
		private:
			ControlRenderer *m_parent;
		public:
			TxPoolHook(ControlRenderer *parent);
			virtual graphics::MemTexturedCanvasRGBA8 ObtainCanvas(int width, int height) override;
		};

		TxPoolHook m_pool;

		struct DrawInfo{
			static constexpr uint32_t FLAG_DIRTY = 0x00000001;
			static constexpr uint32_t FLAG_UNUSED = 0x00000002;

			uint32_t control_id;
			graphics::MemCanvasRenderTexturePool::SpaceUnit space;
			BoundingBox bbox;
			float texwidth;
			float texheight;
			uint32_t attribute;
			int next_free;

			DrawInfo(void) : attribute{ FLAG_DIRTY }, next_free{ -1 } {}
			
			bool hasAttribute(uint32_t attrib) { return (attribute & attrib) != 0; }
			void setAttribute(uint32_t attrib) { attribute |= attrib; }
			void unsetAttribute(uint32_t attrib) { attribute &= (~attrib); }

			void unuse(void);
		};

		class DrawUnit {
			ControlRenderer *m_parent;
			graphics::gl::GLVBO m_vbuf;
			graphics::gl::GLVBO m_ibuf;
			std::vector<DrawInfo> m_infos;
			std::unordered_map<uint32_t, int> m_infomap;
			uint32_t m_txunitid;
			int first_free;
		public:
			DrawUnit(void) {};
			DrawUnit(uint32_t txunitid, ControlRenderer *parent);
			~DrawUnit(void) {};

			//DrawUnit(const DrawUnit&) = default;
			DrawUnit(DrawUnit &&) = default;

			void AddInfo(DrawInfo info);
			void RemoveDrawInfo(uint32_t control_id);
			void RenderAll(graphics::gl::GLShaderSet &shader);
			DrawInfo &GetDrawInfo(uint32_t control_id);
			bool scavengeGC(void);
			void fullGC(void);
		private:
			void updateInfo(int index);
			
			void resizeVBO(int size);
			
		};

		std::unordered_map<uint32_t, DrawUnit> m_drawunits;
		std::shared_ptr<ControlObserver> m_observer;

		graphics::gl::GLShaderSet m_shader;
		GLuint m_unif_mat_loc;

	public:
		ControlRenderer(void);

		void LateInitialize(void);

		void AddCanvasControl(const std::shared_ptr<AWCanvasControl> control);
		void RemoveControl(uint32_t txunitid, uint32_t control_id);

		void RenderAll(graphics::DrawEnv *env);

		void OnResize(const AWCanvasControl *control);
		void OnMove(const AWCanvasControl *control);

		graphics::MemCanvasRenderTexturePool *GetPool(void);
		
	private:
		void createDrawUnit(uint32_t txunitid);
		void scavengeGC(void);
		void fullGC(void);

		void initShader(void);
	};

}
}
