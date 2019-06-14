//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Scene.h
//!
//! @brief  3D�v���O�����ۑ�p�V�[���N���X�̃w�b�_�t�@�C��
//!
//! @date   ���t
//!
//! @author ����Җ�
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once




// �w�b�_�t�@�C���̓ǂݍ��� ================================================
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




// �^�̐錾 ================================================================




// �N���X�̒�` ============================================================

// <�V�[��> ------------------------------------------------------------
class Scene
{
	// �ÓI�f�[�^�����o�̐錾
	public:
		static const wchar_t* TITLE;            // �^�C�g��
		static const int      SCREEN_WIDTH;     // ��ʕ�
		static const int      SCREEN_HEIGHT;    // ��ʍ���


    // �f�[�^�����o�̐錾
	private:
		GridFloor*   m_gridFloor;
		DebugCamera* m_camera;


		std::unique_ptr<DirectX::BasicEffect>							 m_basicEffect;
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalTexture>>m_primitiveBatch;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>						 m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>				 m_texture;

		float m_angle;
		bool flag = false;

	// �����o�֐��̐錾
	public:
		// �R���X�g���N�^
		Scene();

		// �f�X�g���N�^
		~Scene();

	public:
		// ����
		void Initialize();
		void Update();
		void Render();
		void Finalize();

		void DrawCube(const DirectX::SimpleMath::Matrix& world);
};
