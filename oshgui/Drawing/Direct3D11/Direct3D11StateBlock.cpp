#include "Direct3D11StateBlock.hpp"

#include <cstring>

namespace OSHGui {
	namespace Drawing {
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Direct3D11StateBlock::Direct3D11StateBlock( ID3D11DeviceContext *context )
			: context( context ),
			  inputLayout( nullptr ),
			  blendState( nullptr ),
			  depthStencilState( nullptr ),
			  rasterizerState( nullptr ),
			  vertexShader( nullptr ),
			  vertexShaderInstancesCount( 0 ),
			  geometryShader( nullptr ),
			  geometryShaderInstancesCount( 0 ),
			  geometryShaderResourceView( nullptr ),
			  pixelShader( nullptr ),
			  pixelShaderInstancesCount( 0 ),
			  hullShader( nullptr ),
			  hullShaderInstancesCount( 0 ),
			  domainShader( nullptr ),
			  domainShaderInstancesCount( 0 ),
			  indexBuffer( nullptr ),
			  indexFormat( DXGI_FORMAT_UNKNOWN ),
			  depthStencilView( nullptr ),
			  viewportCount( D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE ) {
			context->AddRef();

			std::memset( blendFactor, 0, sizeof( blendFactor ) );
			std::memset( vertexShaderBuffers, 0, sizeof( vertexShaderBuffers ) );
			std::memset( geometryShaderBuffers, 0, sizeof( geometryShaderBuffers ) );
			std::memset( samplerStates, 0, sizeof( samplerStates ) );
			std::memset( pixelShaderResourceViews, 0, sizeof( pixelShaderResourceViews ) );
			std::memset( pixelShaderBuffers, 0, sizeof( pixelShaderBuffers ) );
			std::memset( renderTargetViews, 0, sizeof( renderTargetViews ) );
			std::memset( domainShaderSamplerStates, 0, sizeof( domainShaderSamplerStates ) );
			std::memset( vertexBuffers, 0, sizeof( vertexBuffers ) );
			std::memset( vertexStrides, 0, sizeof( vertexStrides ) );
			std::memset( vertexOffsets, 0, sizeof( vertexOffsets ) );
		}

		//---------------------------------------------------------------------------
		Direct3D11StateBlock::~Direct3D11StateBlock() {
			Release();

			context->Release();
		}

		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void Direct3D11StateBlock::Capture() {
			context->IAGetPrimitiveTopology( &primitiveTopology );
			context->IAGetInputLayout( &inputLayout );

			context->OMGetBlendState( &blendState, blendFactor, &sampleMask );
			context->OMGetDepthStencilState( &depthStencilState, &stencilRef );

			context->RSGetState( &rasterizerState );

			vertexShaderInstancesCount = 256;
			context->VSGetShader( &vertexShader, vertexShaderInstances, &vertexShaderInstancesCount );
			context->VSGetConstantBuffers( 0, 4, vertexShaderBuffers );

			pixelShaderInstancesCount = 256;
			context->PSGetShader( &pixelShader, pixelShaderInstances, &pixelShaderInstancesCount );
			context->PSGetShaderResources( 0, 9, pixelShaderResourceViews );
			context->PSGetSamplers( 0, 10, samplerStates );
			context->PSGetConstantBuffers( 0, 3, pixelShaderBuffers );

			context->OMGetRenderTargetsAndUnorderedAccessViews( 2, renderTargetViews, &depthStencilView, 0, 0, nullptr );

			context->RSGetViewports( &viewportCount, viewports );

			geometryShaderInstancesCount = 256;
			context->GSGetShader( &geometryShader, geometryShaderInstances, &geometryShaderInstancesCount );
			context->GSGetConstantBuffers( 0, 2, geometryShaderBuffers );
			context->GSGetShaderResources( 0, 1, &geometryShaderResourceView );

			hullShaderInstancesCount = 256;
			context->HSGetShader( &hullShader, hullShaderInstances, &hullShaderInstancesCount );

			domainShaderInstancesCount = 256;
			context->DSGetShader( &domainShader, domainShaderInstances, &domainShaderInstancesCount );
			context->DSGetSamplers( 0, 3, domainShaderSamplerStates );

			context->IAGetVertexBuffers( 0, 3, vertexBuffers, vertexStrides, vertexOffsets );
			context->IAGetIndexBuffer( &indexBuffer, &indexFormat, &indexOffset );
		}

		//---------------------------------------------------------------------------
		void Direct3D11StateBlock::Apply() const {
			context->IASetPrimitiveTopology( primitiveTopology );
			context->IASetInputLayout( inputLayout );

			context->RSSetState( rasterizerState );

			context->VSSetShader( vertexShader, vertexShaderInstances, vertexShaderInstancesCount );
			context->VSSetConstantBuffers( 0, 4, vertexShaderBuffers );

			context->PSSetShader( pixelShader, pixelShaderInstances, pixelShaderInstancesCount );
			context->PSSetShaderResources( 0, 9, pixelShaderResourceViews );
			context->PSSetSamplers( 0, 10, samplerStates );
			context->PSSetConstantBuffers( 0, 3, pixelShaderBuffers );
			context->OMSetRenderTargets( 2, renderTargetViews, depthStencilView );

			context->OMSetBlendState( blendState, blendFactor, sampleMask );
			context->OMSetDepthStencilState( depthStencilState, stencilRef );
			context->RSSetViewports( viewportCount, viewports );

			context->GSSetShader( geometryShader, geometryShaderInstances, geometryShaderInstancesCount );
			context->GSSetConstantBuffers( 0, 2, geometryShaderBuffers );
			context->GSSetShaderResources( 0, 1, &geometryShaderResourceView );

			context->HSSetShader( hullShader, hullShaderInstances, hullShaderInstancesCount );

			context->DSSetShader( domainShader, domainShaderInstances, domainShaderInstancesCount );
			context->DSSetSamplers( 0, 3, domainShaderSamplerStates );

			context->IASetVertexBuffers( 0, 3, vertexBuffers, vertexStrides, vertexOffsets );
			context->IASetIndexBuffer( indexBuffer, indexFormat, indexOffset );
		}

		//---------------------------------------------------------------------------
		void Direct3D11StateBlock::Release() {
#define Release_(_1)\
			if (_1 != nullptr) { _1->Release(); _1 = nullptr; }
#define MultiRelease_(_1, count)\
			for (auto i = 0u; i < count; ++i)\
				Release_(_1[i]);

			Release_(inputLayout);

			Release_(blendState);
			Release_(depthStencilState);

			Release_(rasterizerState);

			Release_(vertexShader);
			MultiRelease_(vertexShaderInstances, vertexShaderInstancesCount);
			MultiRelease_(vertexShaderBuffers, 4);

			Release_(pixelShader);
			MultiRelease_(pixelShaderInstances, pixelShaderInstancesCount);
			MultiRelease_(pixelShaderResourceViews, 9);
			MultiRelease_(samplerStates, 10);
			MultiRelease_(pixelShaderBuffers, 3);

			MultiRelease_(renderTargetViews, 2);
			Release_(depthStencilView);

			Release_(geometryShader);
			MultiRelease_(geometryShaderInstances, geometryShaderInstancesCount);
			MultiRelease_(geometryShaderBuffers, 2);
			Release_(geometryShaderResourceView);

			Release_(hullShader);
			MultiRelease_(hullShaderInstances, hullShaderInstancesCount);

			Release_(domainShader);
			MultiRelease_(domainShaderInstances, domainShaderInstancesCount);
			MultiRelease_(domainShaderSamplerStates, 3);

			MultiRelease_(vertexBuffers, 3);
			Release_(indexBuffer);
		}

		//---------------------------------------------------------------------------
	}
}
