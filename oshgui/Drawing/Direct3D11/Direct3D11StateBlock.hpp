#pragma once

/*
* OldSchoolHack GUI
*
* by KN4CK3R https://www.oldschoolhack.me/
*
* See license in OSHGui.hpp
*/

#include <d3d11.h>

namespace OSHGui {
	namespace Drawing {
		/**
		* Status-Speicher für D3D11
		*/
		class Direct3D11StateBlock {
		public:
			/**
			* Konstruktor der Klasse.
			*
			* \param context
			*/
			Direct3D11StateBlock( ID3D11DeviceContext *context );
			/**
			* Destruktor der Klasse.
			*/
			~Direct3D11StateBlock();

			/**
			* Speichert den Status des Device.
			*/
			void Capture();
			/**
			* Stellt den Status des Device wieder her.
			*/
			void Apply() const;
			/**
			* Gibt die angeforderten Ressourcen frei.
			*/
			void Release();

		private:
			ID3D11DeviceContext *context;

			D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
			ID3D11InputLayout *inputLayout;

			ID3D11BlendState *blendState;
			FLOAT blendFactor[4];
			UINT sampleMask;
			ID3D11DepthStencilState *depthStencilState;
			UINT stencilRef;

			ID3D11RasterizerState *rasterizerState;

			ID3D11VertexShader *vertexShader;
			ID3D11ClassInstance *vertexShaderInstances[256];
			UINT vertexShaderInstancesCount;
			ID3D11Buffer *vertexShaderBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];

			ID3D11PixelShader *pixelShader;
			ID3D11ClassInstance *pixelShaderInstances[256];
			UINT pixelShaderInstancesCount;
			ID3D11ShaderResourceView *pixelShaderResourceViews[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
			ID3D11SamplerState *samplerStates[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
			ID3D11Buffer *pixelShaderBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];

			ID3D11RenderTargetView *renderTargetViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
			ID3D11DepthStencilView *depthStencilView;

			UINT viewportCount;
			D3D11_VIEWPORT viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

			ID3D11GeometryShader *geometryShader;
			ID3D11ClassInstance *geometryShaderInstances[256];
			UINT geometryShaderInstancesCount;
			ID3D11Buffer *geometryShaderBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
			ID3D11ShaderResourceView *geometryShaderResourceView;

			ID3D11HullShader *hullShader;
			ID3D11ClassInstance *hullShaderInstances[256];
			UINT hullShaderInstancesCount;

			ID3D11DomainShader *domainShader;
			ID3D11ClassInstance *domainShaderInstances[256];
			UINT domainShaderInstancesCount;
			ID3D11SamplerState *domainShaderSamplerStates[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];

			ID3D11Buffer *vertexBuffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
			UINT vertexStrides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
			UINT vertexOffsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
			ID3D11Buffer *indexBuffer;
			DXGI_FORMAT indexFormat;
			UINT indexOffset;
		};
	}
}
