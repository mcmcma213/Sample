//--------------------------------------------------------------------------------------
// File: Main.cpp
//
// ���C��
//
// ���l�F���̃v���O������DirectX11���g�p���邽�߂̍Œ�K�v�Ǝv���鏈����g�ݍ��񂾃T���v���ł�
//       �]���ȏ����͋L�q���Ă��܂���̂ŃI���W�i���t���[�����[�N�̃x�[�X�Ɏg�p���Ă�������
//
// Date: 2018.12.01
// Author: H.Imase
// Author: S.Takaki
//--------------------------------------------------------------------------------------
#include "ImaseLib\Direct3D.h"
#include "ImaseLib\DirectXTK.h"
#include "ImaseLib\FPSTimer.h"
#include "Resources\Icon\Icon.h"
#include "Scene.h"




//////////////////////////
// using �f�B���N�e�B�u //
//////////////////////////
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::SimpleMath;




//////////////
// �萔�錾 //
//////////////
// �E�C���h�E�X�^�C��
static const DWORD WINDOW_STYLE = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;



////////////////////////////
// �֐��̃v���g�^�C�v�錾 //
////////////////////////////
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



////////////////////
// �O���[�o���ϐ� //
////////////////////
HINSTANCE g_hInst = nullptr;    // �C���X�^���X�n���h��
HWND      g_hWnd  = nullptr;    // �E�C���h�E�n���h��




//--------------------------------------------------------------------------------------
// ���C��
//--------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ���������� -------------------------------------------------------------
	// �E�C���h�E���쐬����
	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return 0;

	// DirectX�f�o�C�X������������
	if (FAILED(Direct3D_InitDevice(g_hWnd)))
	{
		// �������Ɏ��s
		Direct3D_CleanupDevice();
		return 0;
	}

	// DirectXTK�֌W�̏�����
	DirectXTK_Initialize();


	// �K�v�ȃI�u�W�F�N�g�̐��� -----------------------------------------------
	// FPS�^�C�}�[���쐬����
	ImaseLib::FPSTimer fpsTimer(60);    // 1�b�Ԃ�60�t���[���ŌŒ肷��

	// �V�[�����쐬����
	Scene* scene = new Scene;
	scene->Initialize();
     

	// ���C�����[�v -----------------------------------------------------------
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		// ���b�Z�[�W�����Ă��邩���ׂ�
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// ���b�Z�[�W�����Ă���΃E�C���h�E�v���V�[�W���֑���
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// �V�[���̍X�V
			scene->Update();

			//---------- �����ŕ`��R�}���h�𔭍s����@---------//
			// �X�v���C�g�̕`��J�n
			g_spriteBatch->Begin();

			// �V�[���̕`��
			scene->Render();

			// FPS��\������
			int fps = fpsTimer.GetNowFPS();
			wchar_t buf[16];
			swprintf_s(buf, 16, L"fps = %d", fps);
			g_spriteFont->DrawString(g_spriteBatch.get(), buf, Vector2(0, 0));

			// �X�v���C�g�̕`��I��
			g_spriteBatch->End();
			//---------- �����܂łɕ`��R�}���h���I��点�� ---------//

			// ��ʍX�V��҂�
			fpsTimer.WaitFrame();

			// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���
			g_pSwapChain->Present(0, 0);
		}
	}


	// �I������ ---------------------------------------------------------------

	// �V�[���̏I������
	scene->Finalize();
	delete scene;

	// DirectX�f�o�C�X����̏I������
	Direct3D_CleanupDevice();

	return 0;
}



//--------------------------------------------------------------------------------------
// �E�C���h�E�̍쐬
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// �E�C���h�E�N���X��o�^����
	WNDCLASSEX wcex;
	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);
	wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName  = nullptr;
	wcex.lpszClassName = L"WindowClass";
	wcex.hIconSm       = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;


	// �E�C���h�E���쐬����
	g_hInst = hInstance;
	RECT rc = { 0, 0, Scene::SCREEN_WIDTH, Scene::SCREEN_HEIGHT };	// ���E�C���h�E�T�C�Y
	AdjustWindowRect(&rc, WINDOW_STYLE, FALSE);
	g_hWnd = CreateWindow(L"WindowClass", Scene::TITLE, WINDOW_STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		nullptr, nullptr, hInstance, nullptr);
	if (!g_hWnd)
		return E_FAIL;


	// �E�C���h�E��\������
	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;
}



//--------------------------------------------------------------------------------------
// �E�C���h�E�v���V�[�W���[
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:		// �E�C���h�E���j�����ꂽ���b�Z�[�W
			PostQuitMessage(0);	// WM_QUIT���b�Z�[�W�𑗏o����i���C�����[�v�𔲂���j
			break;

		case WM_ACTIVATEAPP:	// �E�C���h�E���A�N�e�B�u���A��A�N�e�B�u�����鎞�ɗ��郁�b�Z�[�W
			Keyboard::ProcessMessage(message, wParam, lParam);
			Mouse::ProcessMessage(message, wParam, lParam);
			break;

		case WM_KEYDOWN:		// �L�[���͊֌W�̃��b�Z�[�W
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard::ProcessMessage(message, wParam, lParam);
			break;

		case WM_INPUT:			// �}�E�X�֌W�̃��b�Z�[�W
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			Mouse::ProcessMessage(message, wParam, lParam);
			break;

		default:
			// �����ŏ������Ȃ����b�Z�[�W��DefWindowProc�֐��ɔC����
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

