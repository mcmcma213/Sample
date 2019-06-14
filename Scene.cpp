//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Scene.cpp
//!
//! @brief  3Dプログラム課題用シーンクラスのソースファイル
//!
//! @date   日付
//!
//! @author 制作者名
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================
#include "Scene.h"

#include <WICTextureLoader.h>

#include "ImaseLib\Direct3D.h"
#include "ImaseLib\DirectXTK.h"




// using ディレクティブ ====================================================
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::SimpleMath;




// <Gameクラス> ****************************************************************
// 静的データメンバの定義 ==================================================
const wchar_t* Scene::TITLE = L"Sample Scene";    // タイトル
const int      Scene::SCREEN_WIDTH = 800;                // 画面幅
const int      Scene::SCREEN_HEIGHT = 600;                // 画面高さ




// メンバ関数の定義 ========================================================
//------------------------------------------------------------------
//! @brief デフォルトコンストラクタ
//!
//! @param[in] なし
//------------------------------------------------------------------
Scene::Scene()
	: m_camera(nullptr)
	, m_gridFloor(nullptr)
	, m_basicEffect(nullptr)
	, m_primitiveBatch(nullptr)
	, m_inputLayout(nullptr)
{

}



//------------------------------------------------------------------
//! @brief デストラクタ
//------------------------------------------------------------------
Scene::~Scene()
{
	delete m_gridFloor;
	delete m_camera;
}



//----------------------------------------------------------------------
//! @brief ゲームの初期化処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Scene::Initialize()
{
	// デバッグ用カメラを作成する
	m_camera = new DebugCamera(SCREEN_WIDTH, SCREEN_HEIGHT);

	// グリッド床オブジェクトを作成する
	m_gridFloor = new GridFloor(10.0f, 10);

	//エフェクトを作成する
	m_basicEffect = std::make_unique<BasicEffect>(g_pd3dDevice.Get());


	//入力レイアウトを作成する
	const void* shaderByteCode;
	size_t		byteCodeLength;

	m_basicEffect->SetLightingEnabled(false);			//ライト(OFF)
	m_basicEffect->SetTextureEnabled(true);		//テクスチャ(OFF)
	m_basicEffect->SetVertexColorEnabled(false);	//頂点カラー(OFF)
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);		//頂点シェーダーの取得

	g_pd3dDevice->CreateInputLayout(
		VertexPositionNormalTexture::InputElements,
		VertexPositionNormalTexture::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		m_inputLayout.GetAddressOf());


	//プリミティブバッチを作成する
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionNormalTexture>>(g_pImmediateContext.Get());
	CreateWICTextureFromFile(g_pd3dDevice.Get(), L"Resources\\Textures\\Card.png", nullptr, &m_texture);


}



//----------------------------------------------------------------------
//! @brief ゲームの更新処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Scene::Update()
{
	// キー入力やマウス情報を更新する
	DirectXTK_UpdateInputState();

	// カメラを更新する
	m_camera->Update();


	if (g_key.IsKeyDown(Keyboard::Space))
	{
		flag = true;
	}

	if (flag)
	{
		if (m_angle < 180.0f)
		{
			m_angle += 5.0f;
		}
	}
}



//----------------------------------------------------------------------
//! @brief ゲームの描画処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Scene::Render()
{
	// バックバッファをクリアする
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView.Get(), DirectX::Colors::MidnightBlue);

	// 深度バッファをクリアする
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	// ビュー行列の作成
	Vector3 eye = Vector3(0.0f, 2.0f, 10.0f);
	Vector3 target = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	//	Matrix viewMatrix = Matrix::CreateLookAt(eye, target, up);
	Matrix viewMatrix = m_camera->GetCameraMatrix();


	// 射影行列の設定
	float fieldOfView = XMConvertToRadians(45.0f);
	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;
	float nearPlane = 1.0f;
	float farPlane = 100.0f;
	Matrix projectionMatrix = Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);


	// グリッド床を表示する
	m_gridFloor->Render(viewMatrix, projectionMatrix);


	/////////////////////////////////////////////////////////////////////////////////////////////////
	// 共通の設定を一回にまとめて設定する
	/////////////////////////////////////////////////////////////////////////////////////////////////
	g_pImmediateContext->OMSetDepthStencilState(g_state->DepthDefault(), 0);    // 深度バッファ(ON)
	m_basicEffect->SetView(viewMatrix);					//ビュー行列の設定
	m_basicEffect->SetProjection(projectionMatrix);		//射影行列の設定



	////////////////////////////////////////
	// ここから描画処理を記述してください //
	////////////////////////////////////////


	Matrix world = Matrix::Identity;

	world *= Matrix::CreateTranslation(0.5f, 0.0f, 0.0f);

	world *= Matrix::CreateRotationZ(XMConvertToRadians(m_angle));

	DrawCube(world);


	////////////////////////////////////////
	// ここまで							  //
	////////////////////////////////////////
}



//----------------------------------------------------------------------
//! @brief ゲームの終了処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Scene::Finalize()
{

}
void Scene::DrawCube(const Matrix& world)
{
	//モデルデータ(頂点座標のみ)
	static const VertexPositionNormalTexture vertices[8] =
	{
		//正面
		VertexPositionNormalTexture(Vector3(-0.5f, 0.0f, 0.8f),Vector3(),Vector2(0,0)),
		VertexPositionNormalTexture(Vector3(0.5f, 0.0f, 0.8f),Vector3(),Vector2(0.5f,0)),
		VertexPositionNormalTexture(Vector3(0.5f, 0.0f, -0.5f),Vector3(),Vector2(0.5f,1)),
		VertexPositionNormalTexture(Vector3(-0.5f, 0.0f, -0.5f),Vector3(),Vector2(0,1)),
		//後ろ面
		VertexPositionNormalTexture(Vector3(-0.5f, 0.0f, 0.8f),Vector3(),Vector2(0.5f,0)),
		VertexPositionNormalTexture(Vector3(0.5f, 0.0f, 0.8f),Vector3(),Vector2(1,0)),
		VertexPositionNormalTexture(Vector3(0.5f, 0.0f, -0.5f),Vector3(),Vector2(1,1)),
		VertexPositionNormalTexture(Vector3(-0.5f, 0.0f, -0.5f),Vector3(),Vector2(0.5f,1)),
	};

	static const uint16_t indices[] = {
		0,1,2,
		0,2,3,
		5,4,6,
		4,7,6,
	};

	//属性を設定
	m_basicEffect->SetLightingEnabled(false);		//ライト(OFF)
	m_basicEffect->SetTextureEnabled(true);		//テクスチャ(OFF)
	m_basicEffect->SetVertexColorEnabled(false);	//頂点カラー(OFF)

													//ワールド行列の設定
	m_basicEffect->SetWorld(world);

	//エフェクトを反映
	m_basicEffect->Apply(g_pImmediateContext.Get());
	m_basicEffect->SetTexture(m_texture.Get());

	//入力レイアウトを設定
	g_pImmediateContext->IASetInputLayout(m_inputLayout.Get());

	//プリミティブの描画
	m_primitiveBatch->Begin();
	//	m_primitiveBatch->DrawTriangle(vertices[0], vertices[1], vertices[2]);

	m_primitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,		//頂点の接続形態
		indices,															//頂点インデックスの配列
		(sizeof(indices) / sizeof(indices[0])),								//頂点の配列の要素
		vertices,															//頂点データの配列
		(sizeof(vertices) / sizeof(vertices[0])));							//頂点データの配列の要素数

	m_primitiveBatch->End();
}
