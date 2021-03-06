// MyLib.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <tchar.h>
#include "GameESP.h"

#include <TlHelp32.h>
#include <Psapi.h>
#pragma comment(lib,"Psapi.lib")
#include <math.h>
#include "instdrv.h"

const TCHAR *szCaption = _T("腾讯手游助手【极速傲引擎】");

int main()
{
	ProcManager::EnableDebugPriv();
	

	HANDLE hDevice = NULL;
	TCHAR szDrvPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szDrvPath, MAX_PATH);
	*_tcsrchr(szDrvPath, '\\') = '\0';
	_tcscat(szDrvPath, _T("\\wsex.sys"));
	BOOL bRet = scmLoadDeviceDriver(_T("CyberPeaceA"), szDrvPath, &hDevice);
	if (!bRet || hDevice == NULL)
	{
		printf("初始化失败!\n");
		system("pause");
		return 1;
	}

	dwGamePid = ProcManager::GetAowProcId();
	
	
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, dwGamePid);
	
	pMM = new MemoryManager(hProcess);
	
	CloseHandle(CreateThread(0, 0, ThreadUpdateData, 0, 0, 0));
	CloseHandle(CreateThread(0, 0, ThreadUpdateData2, 0, 0, 0));
	CloseHandle(CreateThread(0, 0, ThreadUpdateData3, 0, 0, 0));
		
	while (1)
	{
		pDxm = new DxManager();
		pDxm->SetupWindow(szCaption);

		MSG msg;
		while (1)
		{
			ZeroMemory(&msg, sizeof(MSG));
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			HWND hWnd = NULL;
			hWnd = FindWindow(0, szCaption);
			if (!hWnd)
			{
				//Shutdown();
				DestroyWindow(pDxm->over_hWnd);
				delete pDxm;
				break;
			}
			hWnd = ::GetWindow(hWnd, GW_CHILD);
			RECT rc;
			ZeroMemory(&rc, sizeof(RECT));
			
			GetWindowRect(hWnd, &rc);
			
			pDxm->s_width = rc.right - rc.left;
			pDxm->s_height = rc.bottom - rc.top;

			MoveWindow(pDxm->over_hWnd, rc.left, rc.top, pDxm->s_width, pDxm->s_height, true);
			
			//add
			HWND hwnd2 = GetForegroundWindow();
			HWND hwnd3 = GetWindow(hwnd2, GW_HWNDPREV);
			SetWindowPos(pDxm->over_hWnd, hwnd3, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			UpdateWindow(pDxm->over_hWnd);
			//render your esp
			pDxm->render();
		}
	}
	
}
