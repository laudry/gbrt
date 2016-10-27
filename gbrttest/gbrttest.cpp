
// gbrttest.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "gbrttest.h"
#include "gbrttestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CgbrttestApp

BEGIN_MESSAGE_MAP(CgbrttestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CgbrttestApp ����

CgbrttestApp::CgbrttestApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CgbrttestApp ����

CgbrttestApp theApp;

GBEXTERN_C DWORD WINAPI MyThread(LPVOID lpParam)
{
	HANDLE hFSMAlloc = GBPTR2PTR(HANDLE, lpParam);
	LPVOID lplpFSM[100];
	for (int i = 0; i != 100; ++i)
	{
		for (int j = 0; j != 100; ++j)
			lplpFSM[j] = GBAllocFSM(hFSMAlloc);
		for (int j = 0; j != 100; ++j)
		{
			if (NULL != lplpFSM[j])
				GBFreeFSM(lplpFSM[j]);
		}
	}

	return 0;
}

GBEXTERN_C DWORD WINAPI MySafeThread(LPVOID lpParam)
{
	HANDLE hFSMAlloc = GBPTR2PTR(HANDLE, lpParam);
	LPVOID lplpFSM[100];
	for (int i = 0; i != 100; ++i)
	{
		for (int j = 0; j != 100; ++j)
			lplpFSM[j] = GBLockedAllocFSM(hFSMAlloc);
		for (int j = 0; j != 100; ++j)
		{
			if (NULL != lplpFSM[j])
				GBLockedFreeFSM(lplpFSM[j]);
		}
	}

	return 0;
}

// CgbrttestApp ��ʼ��

BOOL CgbrttestApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	{
		HANDLE hFSMAlloc = GBCreateFSMAlloc(16, 100);
		HANDLE lphThread[10];
		for (int i = 0; i != 10; ++i)
			lphThread[i] = ::CreateThread(NULL, 0, &MySafeThread, GBPTR2PTR(LPVOID, hFSMAlloc), 0, NULL);
		::WaitForMultipleObjects(10, lphThread, TRUE, INFINITE);
		for (int i = 0; i != 10; ++i)
			::CloseHandle(lphThread[i]);
		GBDestroyFSMAlloc(hFSMAlloc);
	}

	{
		HANDLE hObjLock = GBLockObj(this);

		GBLockObj(this);

		GBUnlockObj(this);

		GBUnlockObjByObjLock(hObjLock);
	}

	CgbrttestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

