// SenseAlignment.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SenseAlignment.h"

#include <stdio.h>
#include <commdlg.h>
#include <CommCtrl.h>


#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名




HWND hInstBase;                                // 当前主窗口实例
HWND hInstDown;
HWND hInstSend;
HWND hInstResv;
HWND hInstProcess;
HWND hInstEditDst;
HWND hInstEditSrc;
HWND hwndPB;




// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SENSEALIGNMENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


	ShowWindow(hInstBase, SW_SHOW);
	UpdateWindow(hInstBase);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SENSEALIGNMENT));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SENSEALIGNMENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SENSEALIGNMENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   hInstBase = hWnd;
   hInstDown = CreateWindow(_T("BUTTON"), _T("发送"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 580, 30, 100, 90, hInstBase, (HMENU)1001, NULL, NULL);
   hInstProcess = CreateWindow(_T("Static"), _T("0 %"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 580, 150, 100, 30, hInstBase, (HMENU)1002, NULL, NULL);
   hInstSend = CreateWindow(_T("BUTTON"), _T("发送文件"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 490, 30, 80, 30, hInstBase, (HMENU)1003, NULL, NULL);
   hInstResv = CreateWindow(_T("BUTTON"), _T("接收路径"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 490, 90, 80, 30, hInstBase, (HMENU)1004, NULL, NULL);
   CreateWindow(_T("Static"), _T("进度条"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 30, 150, 80, 30, hInstBase, (HMENU)1005, NULL, NULL);
   hInstEditSrc = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 30, 30, 450, 30, hInstBase, (HMENU)1006, NULL, NULL);
   hInstEditDst = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 30, 90, 450, 30, hInstBase, (HMENU)1007, NULL, NULL);
   SetWindowText(hInstEditSrc, _T("SrcFilename:"));
   SetWindowText(hInstEditDst, _T("DstFilename:"));
   EnableWindow(hInstDown, FALSE);

   hwndPB = CreateWindowEx( //创建进度条
	   0,
	   PROGRESS_CLASS,
	   NULL,
	   WS_CHILD | WS_VISIBLE,
	   120, 150, 450, 30,            //位置和大小在WM_SIZE中设置
	   hInstBase,
	   (HMENU)0,
	   NULL,
	   NULL);
   SendMessage(hWnd, WM_CREATE, NULL, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

DWORD WINAPI PBThreadProc(LPVOID lpParameter)
{
	//进度条 start


	ShowWindow(hwndPB, SW_SHOW);        //显示进度条

	ShowWindow(hInstProcess, SW_SHOW);  //显示百分比控件

	PBRANGE range;                        //进度条的范围


	SendMessage(hwndPB, PBM_SETRANGE,    //设置进度条的范围

		(WPARAM)0, (LPARAM)(MAKELPARAM(0, 100)));


	SendMessage(hwndPB, PBM_GETRANGE,    //获取进度条的范围

		(WPARAM)TRUE,                    //TRUE 表示返回值为范围的最小值,FALSE表示返回最大值

		(LPARAM)&range);
	//进度条 end


	//设置进度条当前值
	TCHAR sumdata[64];

	for (size_t i = 1; i <= 100; i++)
	{
		SendMessage(hwndPB, PBM_SETPOS, (WPARAM)(i), (LPARAM)0);
		swprintf(sumdata, _T("%d %%"), i);
		SetDlgItemText(hInstBase, 1002, sumdata);
		Sleep(100);
	}

	SetDlgItemText(hInstBase, 1001, _T("发送"));
	EnableWindow(hInstDown, TRUE);
	TCHAR buff[80] = _T("");

	GetWindowText(hInstEditSrc, buff, 80);

	return 0;
}

void OpenFile()
{
	OPENFILENAME opfn;
	opfn.hwndOwner = hInstBase;
	WCHAR strFilename[MAX_PATH];//存放文件名  
								//初始化  
	ZeroMemory(&opfn, sizeof(OPENFILENAME));
	opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小  
											//设置过滤  
	opfn.lpstrFilter = L"文本文件\0*.txt\0";
	//默认过滤器索引设为1  
	opfn.nFilterIndex = 1;
	//文件名的字段必须先把第一个字符设为 \0  
	opfn.lpstrFile = strFilename;
	opfn.lpstrFile[0] = '\0';
	opfn.nMaxFile = sizeof(strFilename);
	//设置标志位，检查目录或文件是否存在  
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	//opfn.lpstrInitialDir = NULL;  
	// 显示对话框让用户选择文件  
	if (GetOpenFileName(&opfn))
	{
		//在文本框中显示文件路径  
		SetWindowText(hInstEditSrc, opfn.lpstrFile);
	}
	EnableWindow(hInstBase, TRUE);
}






//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);


			switch (wmEvent)
			{
			case BN_CLICKED:
			{
				if (1001 == wmId)
				{
					SetDlgItemText(hInstBase, 1001, _T("正在发送..."));
					EnableWindow(hInstDown, FALSE);
					//创建线程
					CreateThread(
						NULL,
						0,
						(LPTHREAD_START_ROUTINE)PBThreadProc,
						NULL,
						0,
						0
					);
				}
				else if (1003 == wmId)
				{
					EnableWindow(hInstBase, FALSE);
					OpenFile();
				}
			}
			break;
			default:
				break;
			}

			
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
