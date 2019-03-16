#include "Direct3D10StateBlock.hpp"

#include <cstring>

namespace OSHGui {
	namespace Drawing {
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Direct3D10StateBlock::Direct3D10StateBlock( ID3D10Device *device )
			: device( device ),
			  primitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED ),
			  inputLayout( nullptr ),
			  blendState( nullptr ),
			  sampleMask( 0 ),
			  depthStencilState( nullptr ),
			  stencilRef( 0 ),
			  rasterizerState( nullptr ),
			  vertexShader( nullptr ),
			  pixelShader( nullptr ),
			  depthStencilView( nullptr ),
			  viewportCount( D3D10_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE ),
			  geometryShader( nullptr ),
			  vertexBuffers( nullptr ),
			  vertexStride( 0 ),
			  vertexOffset( 0 ),
			  indexBuffer( nullptr ),
			  indexFormat( DXGI_FORMAT_UNKNOWN ),
			  indexOffset( 0 ) {
			device->AddRef();

			std::memset( blendFactor, 0, sizeof( blendFactor ) );
			std::memset( vertexShaderBuffers, 0, sizeof( vertexShaderBuffers ) );
			std::memset( geometryShaderBuffers, 0, sizeof( geometryShaderBuffers ) );
			std::memset( samplerStates, 0, sizeof( samplerStates ) );
			std::memset( pixelShaderResourceViews, 0, sizeof( pixelShaderResourceViews ) );
			std::memset( pixelShaderBuffers, 0, sizeof( pixelShaderBuffers ) );
			std::memset( renderTargetViews, 0, sizeof( renderTargetViews ) );
		}

		//---------------------------------------------------------------------------
		Direct3D10StateBlock::~Direct3D10StateBlock() {
			Release();

			device->Release();
		}

		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void Direct3D10StateBlock::Capture() {
			device->IAGetPrimitiveTopology( &primitiveTopology );
			device->IAGetInputLayout( &inputLayout );

			device->OMGetBlendState( &blendState, blendFactor, &sampleMask );
			device->OMGetDepthStencilState( &depthStencilState, &stencilRef );

			device->RSGetState( &rasterizerState );

			device->VSGetShader( &vertexShader );
			device->VSGetConstantBuffers( 0, 3, vertexShaderBuffers );

			device->PSGetShader( &pixelShader );
			device->PSGetShaderResources( 0, 9, pixelShaderResourceViews );
			device->PSGetSamplers( 0, 3, samplerStates );
			device->PSGetConstantBuffers( 0, 2, pixelShaderBuffers );

			device->OMGetRenderTargets( 1, renderTargetViews, &depthStencilView );

			device->RSGetViewports( &viewportCount, viewports );

			device->GSGetShader( &geometryShader );
			device->GSGetConstantBuffers( 0, 3, geometryShaderBuffers );

			device->IAGetVertexBuffers( 0, 1, &vertexBuffers, &vertexStride, &vertexOffset );
			device->IAGetIndexBuffer( &indexBuffer, &indexFormat, &indexOffset );
		}

		//---------------------------------------------------------------------------
		void Direct3D10StateBlock::Apply() const {
			device->IASetPrimitiveTopology( primitiveTopology );
			device->IASetInputLayout( inputLayout );

			device->RSSetState( rasterizerState );

			device->VSSetShader( vertexShader );
			device->VSSetConstantBuffers( 0, 3, vertexShaderBuffers );

			device->PSSetShader( pixelShader );
			device->PSSetShaderResources( 0, 9, pixelShaderResourceViews );
			device->PSSetSamplers( 0, 3, samplerStates );
			device->PSSetConstantBuffers( 0, 2, pixelShaderBuffers );

			device->OMSetRenderTargets( 1, renderTargetViews, depthStencilView );
			device->OMSetBlendState( blendState, blendFactor, sampleMask );
			device->OMSetDepthStencilState( depthStencilState, stencilRef );
			device->RSSetViewports( viewportCount, viewports );

			device->GSSetShader( geometryShader );
			device->GSSetConstantBuffers( 0, 3, geometryShaderBuffers );

			device->IASetVertexBuffers( 0, 1, &vertexBuffers, &vertexStride, &vertexOffset );
			device->IASetIndexBuffer( indexBuffer, indexFormat, indexOffset );
		}

		//---------------------------------------------------------------------------
		void Direct3D10StateBlock::Release() {
#define Release_(_1)\
			if (_1 != nullptr) { _1->Release(); _1 = nullptr; }
#define MultiRelease_(_1, count)\
			for (auto i = 0u; i < count; ++i)\
				Release_(_1[i]);

			Release_(inputLayout);

			Release_(rasterizerState);

			Release_(vertexShader);
			MultiRelease_(vertexShaderBuffers, 3);

			Release_(pixelShader);
			MultiRelease_(pixelShaderResourceViews, 9);
			MultiRelease_(samplerStates, 3);
			MultiRelease_(pixelShaderBuffers, 2);

			MultiRelease_(renderTargetViews, 2);
			Release_(depthStencilView);

			Release_(blendState);
			Release_(depthStencilState);

			Release_(geometryShader);
			MultiRelease_(geometryShaderBuffers, 3);

			Release_(vertexBuffers);
			Release_(indexBuffer);
		}

		//---------------------------------------------------------------------------
	}
}
