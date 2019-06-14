//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Scene.cpp
//!
//! @brief  3D�v���O�����ۑ�p�V�[���N���X�̃\�[�X�t�@�C��
//!
//! @date   ���t
//!
//! @author ����Җ�
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "Scene.h"

#include <WICTextureLoader.h>

#include "ImaseLib\Direct3D.h"
#include "ImaseLib\DirectXTK.h"




// using �f�B���N�e�B�u ====================================================
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::SimpleMath;




// <Game�N���X> ****************************************************************
// �ÓI�f�[�^�����o�̒�` ==================================================
const wchar_t* Scene::TITLE = L"Sample Scene";    // �^�C�g��
const int      Scene::SCREEN_WIDTH = 800;                // ��ʕ�
const int      Scene::SCREEN_HEIGHT = 600;                // ��ʍ���




// �����o�֐��̒�` ========================================================
//------------------------------------------------------------------
//! @brief �f�t�H���g�R���X�g���N�^
//!
//! @param[in] �Ȃ�
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
//! @brief �f�X�g���N�^
//------------------------------------------------------------------
Scene::~Scene()
{
	delete m_gridFloor;
	delete m_camera;
}



//----------------------------------------------------------------------
//! @brief �Q�[���̏���������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Scene::Initialize()
{
	// �f�o�b�O�p�J�������쐬����
	m_camera = new DebugCamera(SCREEN_WIDTH, SCREEN_HEIGHT);

	// �O���b�h���I�u�W�F�N�g���쐬����
	m_gridFloor = new GridFloor(10.0f, 10);

	//�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<BasicEffect>(g_pd3dDevice.Get());


	//���̓��C�A�E�g���쐬����
	const void* shaderByteCode;
	size_t		byteCodeLength;

	m_basicEffect->SetLightingEnabled(false);			//���C�g(OFF)
	m_basicEffect->SetTextureEnabled(true);		//�e�N�X�`��(OFF)
	m_basicEffect->SetVertexColorEnabled(false);	//���_�J���[(OFF)
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);		//���_�V�F�[�_�[�̎擾

	g_pd3dDevice->CreateInputLayout(
		VertexPositionNormalTexture::InputElements,
		VertexPositionNormalTexture::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		m_inputLayout.GetAddressOf());


	//�v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionNormalTexture>>(g_pImmediateContext.Get());
	CreateWICTextureFromFile(g_pd3dDevice.Get(), L"Resources\\Textures\\Card.png", nullptr, &m_texture);


}



//----------------------------------------------------------------------
//! @brief �Q�[���̍X�V����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Scene::Update()
{
	// �L�[���͂�}�E�X�����X�V����
	DirectXTK_UpdateInputState();

	// �J�������X�V����
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
//! @brief �Q�[���̕`�揈��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Scene::Render()
{
	// �o�b�N�o�b�t�@���N���A����
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView.Get(), DirectX::Colors::MidnightBlue);

	// �[�x�o�b�t�@���N���A����
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	// �r���[�s��̍쐬
	Vector3 eye = Vector3(0.0f, 2.0f, 10.0f);
	Vector3 target = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	//	Matrix viewMatrix = Matrix::CreateLookAt(eye, target, up);
	Matrix viewMatrix = m_camera->GetCameraMatrix();


	// �ˉe�s��̐ݒ�
	float fieldOfView = XMConvertToRadians(45.0f);
	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;
	float nearPlane = 1.0f;
	float farPlane = 100.0f;
	Matrix projectionMatrix = Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);


	// �O���b�h����\������
	m_gridFloor->Render(viewMatrix, projectionMatrix);


	/////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ʂ̐ݒ�����ɂ܂Ƃ߂Đݒ肷��
	/////////////////////////////////////////////////////////////////////////////////////////////////
	g_pImmediateContext->OMSetDepthStencilState(g_state->DepthDefault(), 0);    // �[�x�o�b�t�@(ON)
	m_basicEffect->SetView(viewMatrix);					//�r���[�s��̐ݒ�
	m_basicEffect->SetProjection(projectionMatrix);		//�ˉe�s��̐ݒ�



	////////////////////////////////////////
	// ��������`�揈�����L�q���Ă������� //
	////////////////////////////////////////


	Matrix world = Matrix::Identity;

	world *= Matrix::CreateTranslation(0.5f, 0.0f, 0.0f);

	world *= Matrix::CreateRotationZ(XMConvertToRadians(m_angle));

	DrawCube(world);


	////////////////////////////////////////
	// �����܂�							  //
	////////////////////////////////////////
}



//----------------------------------------------------------------------
//! @brief �Q�[���̏I������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Scene::Finalize()
{

}
void Scene::DrawCube(const Matrix& world)
{
	//���f���f�[�^(���_���W�̂�)
	static const VertexPositionNormalTexture vertices[8] =
	{
		//����
		VertexPositionNormalTexture(Vector3(-0.5f, 0.0f, 0.8f),Vector3(),Vector2(0,0)),
		VertexPositionNormalTexture(Vector3(0.5f, 0.0f, 0.8f),Vector3(),Vector2(0.5f,0)),
		VertexPositionNormalTexture(Vector3(0.5f, 0.0f, -0.5f),Vector3(),Vector2(0.5f,1)),
		VertexPositionNormalTexture(Vector3(-0.5f, 0.0f, -0.5f),Vector3(),Vector2(0,1)),
		//����
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

	//������ݒ�
	m_basicEffect->SetLightingEnabled(false);		//���C�g(OFF)
	m_basicEffect->SetTextureEnabled(true);		//�e�N�X�`��(OFF)
	m_basicEffect->SetVertexColorEnabled(false);	//���_�J���[(OFF)

													//���[���h�s��̐ݒ�
	m_basicEffect->SetWorld(world);

	//�G�t�F�N�g�𔽉f
	m_basicEffect->Apply(g_pImmediateContext.Get());
	m_basicEffect->SetTexture(m_texture.Get());

	//���̓��C�A�E�g��ݒ�
	g_pImmediateContext->IASetInputLayout(m_inputLayout.Get());

	//�v���~�e�B�u�̕`��
	m_primitiveBatch->Begin();
	//	m_primitiveBatch->DrawTriangle(vertices[0], vertices[1], vertices[2]);

	m_primitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,		//���_�̐ڑ��`��
		indices,															//���_�C���f�b�N�X�̔z��
		(sizeof(indices) / sizeof(indices[0])),								//���_�̔z��̗v�f
		vertices,															//���_�f�[�^�̔z��
		(sizeof(vertices) / sizeof(vertices[0])));							//���_�f�[�^�̔z��̗v�f��

	m_primitiveBatch->End();
}
