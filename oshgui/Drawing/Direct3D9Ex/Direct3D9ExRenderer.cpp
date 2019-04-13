#include "Direct3D9ExRenderer.hpp"
#include "Direct3D9ExTexture.hpp"
#include "../Direct3D9/Direct3D9GeometryBuffer.hpp"
#include "../Direct3D9/Direct3D9RenderTarget.hpp"
#include "../Direct3D9/Direct3D9ViewportTarget.hpp"
#include "../Direct3D9/Direct3D9TextureTarget.hpp"
#include "../../Misc/Exceptions.hpp"

#include <algorithm>

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Direct3D9ExRenderer::Direct3D9ExRenderer( LPDIRECT3DDEVICE9 device )
			: Direct3D9Renderer( device )
		{
			
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		TexturePtr Direct3D9ExRenderer::CreateTexture()
		{
			auto texture = std::shared_ptr< Direct3D9ExTexture >( new Direct3D9ExTexture( *this ) );
			RegisterTexture( texture );
			return texture;
		}
		//---------------------------------------------------------------------------
		TexturePtr Direct3D9ExRenderer::CreateTexture(const Misc::AnsiString &filename)
		{
			auto texture = std::shared_ptr< Direct3D9ExTexture >( new Direct3D9ExTexture( *this, filename ) );
			RegisterTexture( texture );
			return texture;
		}
		//---------------------------------------------------------------------------
		TexturePtr Direct3D9ExRenderer::CreateTexture(const SizeF &size)
		{
			auto texture = std::shared_ptr< Direct3D9ExTexture >( new Direct3D9ExTexture( *this, size ) );
			RegisterTexture( texture );
			return texture;
		}
		//---------------------------------------------------------------------------
	}
}
