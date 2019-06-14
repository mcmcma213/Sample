//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Scene.h
//!
//! @brief  3Dプログラム課題用シーンクラスのヘッダファイル
//!
//! @date   日付
//!
//! @author 制作者名
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 ==================================================
#pragma once




// ヘッダファイルの読み込み ================================================
#include <memory>

#include <Windows.h>
#include <wrl.h>

#include <d3d11.h>
#include <DirectXColors.h>
#include <Effects.h>
#include <GeometricPrimitive.h>
#include <PrimitiveBatch.h>
#include <SimpleMath.h>

#include "ImaseLib\DebugCamera.h"
#include "ImaseLib\Grid.h"
#include "ImaseLib\Texture.h"




// 型の宣言 ================================================================




// クラスの定義 ============================================================

// <シーン> ------------------------------------------------------------
class Scene
{
	// 静的データメンバの宣言
	public:
		static const wchar_t* TITLE;            // タイトル
		static const int      SCREEN_WIDTH;     // 画面幅
		static const int      SCREEN_HEIGHT;    // 画面高さ


    // データメンバの宣言
	private:
		GridFloor*   m_gridFloor;
		DebugCamera* m_camera;


		std::unique_ptr<DirectX::BasicEffect>							 m_basicEffect;
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalTexture>>m_primitiveBatch;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>						 m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>				 m_texture;

		float m_angle;
		bool flag = false;

	// メンバ関数の宣言
	public:
		// コンストラクタ
		Scene();

		// デストラクタ
		~Scene();

	public:
		// 操作
		void Initialize();
		void Update();
		void Render();
		void Finalize();

		void DrawCube(const DirectX::SimpleMath::Matrix& world);
};
