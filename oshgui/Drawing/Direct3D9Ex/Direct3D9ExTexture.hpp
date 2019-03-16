/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_DIRECT3D9EXTEXTURE_HPP
#define OSHGUI_DRAWING_DIRECT3D9EXTEXTURE_HPP

#include "../Direct3D9/Direct3D9Texture.hpp"
#include "Direct3D9ExRenderer.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Die Direct3D9Ex Variante der Texture.
		 */
		class OSHGUI_EXPORT Direct3D9ExTexture : public Direct3D9Texture
		{
		protected:
			// Friends (to allow construction and destruction)
			friend TexturePtr Direct3D9ExRenderer::CreateTexture();
			friend TexturePtr Direct3D9ExRenderer::CreateTexture(const Misc::AnsiString&);
			friend TexturePtr Direct3D9ExRenderer::CreateTexture(const SizeF&);

			/**
			 * Erstellt eine leere Textur.
			 *
			 * \param owner Renderer Objekt, das die Textur erstellt hat
			 */
			Direct3D9ExTexture(Direct3D9ExRenderer &owner);
			/**
			 * Erstellt eine Textur aus einer Datei.
			 *
			 * \param owner Renderer Objekt, das die Textur erstellt hat
			 * \param path Pfad zur Datei
			 */
			Direct3D9ExTexture(Direct3D9ExRenderer &owner, const Misc::AnsiString &path);
			/**
			 * Erstellt eine leere Textur mit der entsprechenden Größe.
			 *
			 * \param owner Renderer Objekt, das die Textur erstellt hat
			 * \param size Größe der Textur
			 */
			Direct3D9ExTexture(Direct3D9ExRenderer &owner, const SizeF &size);

			/**
			 * Erzeugt intern die verwendete Textur.
			 *
			 * \param size Größe der Textur
			 * \param format D3DFORMAT der Textur
			 */
			virtual void CreateDirect3D9Texture(const SizeF &size, D3DFORMAT format) override;
		};
	}
}

#endif
