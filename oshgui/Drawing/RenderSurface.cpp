#include "RenderSurface.hpp"
#include "RenderTarget.hpp"

namespace OSHGui {
	namespace Drawing {
		RenderSurface::RenderSurface( RenderTarget &_target )
			: target( _target ) {
			queue.resize( 3 );
		}

		void RenderSurface::AddGeometry( RenderQueueType type, const GeometryBufferPtr &geometry ) {
			queue[ static_cast< int >(type) ].AddGeometryBuffer( geometry );
		}

		void RenderSurface::RemoveGeometry( RenderQueueType type, const GeometryBufferPtr &geometry ) {
			queue[ static_cast< int >(type) ].RemoveGeometryBuffer( geometry );
		}

		void RenderSurface::ResetQueue( RenderQueueType type ) {
			queue[ static_cast< int >(type) ].Reset();
		}

		void RenderSurface::Reset() {
			for( auto &q : queue ) {
				q.Reset();
			}
		}

		void RenderSurface::Draw() {
			target.Activate();

			for( auto &q : queue ) {
				target.Draw( q );
			}

			target.Deactivate();
		}
	}
}
