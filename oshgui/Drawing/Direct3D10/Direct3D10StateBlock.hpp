#pragma once

/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R https://www.oldschoolhack.me/
 *
 * See license in OSHGui.hpp
 */

#include <d3d10.h>

namespace OSHGui {
	namespace Drawing {
		/**
		 * Status-Speicher für D3D10
		 */
		class Direct3D10StateBlock {
		public:
			/**
			 * Konstruktor der Klasse.
			 *
			 * \param context
			 */
			Direct3D10StateBlock( ID3D10Device *device );
			/**
			 * Destruktor der Klasse.
			 */
			~Direct3D10StateBlock();

			/**
			 * Speichert den Status des Device.
			 */
			void Capture();
			/**
			 * Stellt den Status des Device wieder her.
			 */
			void Apply() const;

			void Release();

		private:
			ID3D10Device *device;

			D3D10_PRIMITIVE_TOPOLOGY primitiveTopology;
			ID3D10InputLayout *inputLayout;

			ID3D10BlendState *blendState;
			FLOAT blendFactor[4];
			UINT sampleMask;
			ID3D10DepthStencilState *depthStencilState;
			UINT stencilRef;

			ID3D10RasterizerState *rasterizerState;

			ID3D10VertexShader *vertexShader;
			ID3D10Buffer *vertexShaderBuffers[D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];

			ID3D10PixelShader *pixelShader;
			ID3D10ShaderResourceView *pixelShaderResourceViews[D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
			ID3D10SamplerState *samplerStates[D3D10_COMMONSHADER_SAMPLER_SLOT_COUNT];
			ID3D10Buffer *pixelShaderBuffers[D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];

			ID3D10RenderTargetView *renderTargetViews[D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT];
			ID3D10DepthStencilView *depthStencilView;

			UINT viewportCount;
			D3D10_VIEWPORT viewports[D3D10_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

			ID3D10GeometryShader *geometryShader;
			ID3D10Buffer *geometryShaderBuffers[D3D10_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];

			ID3D10Buffer *vertexBuffers;
			UINT vertexStride;
			UINT vertexOffset;
			ID3D10Buffer *indexBuffer;
			DXGI_FORMAT indexFormat;
			UINT indexOffset;
		};
	}
}
