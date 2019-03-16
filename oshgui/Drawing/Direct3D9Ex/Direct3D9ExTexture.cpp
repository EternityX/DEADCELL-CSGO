#include "Direct3D9ExTexture.hpp"
#include "../../Misc/Exceptions.hpp"
#include <d3dx9.h>

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Direct3D9ExTexture::Direct3D9ExTexture(Direct3D9ExRenderer &owner)
			: Direct3D9Texture(owner)
		{

		}
		//---------------------------------------------------------------------------
		Direct3D9ExTexture::Direct3D9ExTexture(Direct3D9ExRenderer &owner, const Misc::AnsiString &filename)
			: Direct3D9Texture(owner) //base constructor because of inheritance
		{
			LoadFromFile(filename);
		}
		//---------------------------------------------------------------------------
		Direct3D9ExTexture::Direct3D9ExTexture(Direct3D9ExRenderer &owner, const SizeF &_size)
			: Direct3D9Texture(owner) //base constructor because of inheritance
		{
			CreateDirect3D9Texture(_size, D3DFMT_A8R8G8B8);
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void Direct3D9ExTexture::CreateDirect3D9Texture(const SizeF &size, D3DFORMAT format)
		{
			CleanupDirect3D9Texture();

			auto textureSize = owner.GetAdjustedSize(size);

			if (FAILED(D3DXCreateTexture(owner.GetDevice(), static_cast<UINT>(textureSize.Width), static_cast<UINT>(textureSize.Height), 1, D3DUSAGE_DYNAMIC, format, D3DPOOL_DEFAULT, &texture)))
			{
				throw Misc::Exception();
			}

			dataSize = size;
			UpdateTextureSize();
			UpdateCachedScaleValues();
		}
		//---------------------------------------------------------------------------
	}
}
