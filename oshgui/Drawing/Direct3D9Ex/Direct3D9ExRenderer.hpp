/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_DIRECT3D9EXRENDERER_HPP
#define OSHGUI_DRAWING_DIRECT3D9EXRENDERER_HPP

#include "../Direct3D9/Direct3D9Renderer.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class Direct3D9ExTexture;
		
		/**
		 * Die Direct3D9Ex Variante des Renderer.
		 */
		class OSHGUI_EXPORT Direct3D9ExRenderer : public Direct3D9Renderer
		{
		public:
			/**
			 * Konstruktor der Klasse.
			 *
			 * \param device Direct3D9 Device
			 */
			Direct3D9ExRenderer(LPDIRECT3DDEVICE9 device);

			virtual TexturePtr CreateTexture() override;
			virtual TexturePtr CreateTexture(const Misc::AnsiString &filename) override;
			virtual TexturePtr CreateTexture(const SizeF &size) override;
		};
	}
}

#endif
