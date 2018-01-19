// SenseAlignment.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SenseAlignment.h"

#include <stdio.h>
#include <commdlg.h>
#include <CommCtrl.h>


#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������




HWND hInstBase;                                // ��ǰ������ʵ��
HWND hInstDown;
HWND hInstSend;
HWND hInstResv;
HWND hInstProcess;
HWND hInstEditDst;
HWND hInstEditSrc;
HWND hwndPB;




// �˴���ģ���а����ĺ�����ǰ������: 
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

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SENSEALIGNMENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


	ShowWindow(hInstBase, SW_SHOW);
	UpdateWindow(hInstBase);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SENSEALIGNMENT));

    MSG msg;

    // ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   hInstBase = hWnd;
   hInstDown = CreateWindow(_T("BUTTON"), _T("����"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 580, 30, 100, 90, hInstBase, (HMENU)1001, NULL, NULL);
   hInstProcess = CreateWindow(_T("Static"), _T("0 %"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 580, 150, 100, 30, hInstBase, (HMENU)1002, NULL, NULL);
   hInstSend = CreateWindow(_T("BUTTON"), _T("�����ļ�"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 490, 30, 80, 30, hInstBase, (HMENU)1003, NULL, NULL);
   hInstResv = CreateWindow(_T("BUTTON"), _T("����·��"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 490, 90, 80, 30, hInstBase, (HMENU)1004, NULL, NULL);
   CreateWindow(_T("Static"), _T("������"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 30, 150, 80, 30, hInstBase, (HMENU)1005, NULL, NULL);
   hInstEditSrc = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 30, 30, 450, 30, hInstBase, (HMENU)1006, NULL, NULL);
   hInstEditDst = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 30, 90, 450, 30, hInstBase, (HMENU)1007, NULL, NULL);
   SetWindowText(hInstEditSrc, _T("SrcFilename:"));
   SetWindowText(hInstEditDst, _T("DstFilename:"));
   EnableWindow(hInstDown, FALSE);

   hwndPB = CreateWindowEx( //����������
	   0,
	   PROGRESS_CLASS,
	   NULL,
	   WS_CHILD | WS_VISIBLE,
	   120, 150, 450, 30,            //λ�úʹ�С��WM_SIZE������
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
	//������ start


	ShowWindow(hwndPB, SW_SHOW);        //��ʾ������

	ShowWindow(hInstProcess, SW_SHOW);  //��ʾ�ٷֱȿؼ�

	PBRANGE range;                        //�������ķ�Χ


	SendMessage(hwndPB, PBM_SETRANGE,    //���ý������ķ�Χ

		(WPARAM)0, (LPARAM)(MAKELPARAM(0, 100)));


	SendMessage(hwndPB, PBM_GETRANGE,    //��ȡ�������ķ�Χ

		(WPARAM)TRUE,                    //TRUE ��ʾ����ֵΪ��Χ����Сֵ,FALSE��ʾ�������ֵ

		(LPARAM)&range);
	//������ end


	//���ý�������ǰֵ
	TCHAR sumdata[64];

	for (size_t i = 1; i <= 100; i++)
	{
		SendMessage(hwndPB, PBM_SETPOS, (WPARAM)(i), (LPARAM)0);
		swprintf(sumdata, _T("%d %%"), i);
		SetDlgItemText(hInstBase, 1002, sumdata);
		Sleep(100);
	}

	SetDlgItemText(hInstBase, 1001, _T("����"));
	EnableWindow(hInstDown, TRUE);
	TCHAR buff[80] = _T("");

	GetWindowText(hInstEditSrc, buff, 80);

	return 0;
}

void OpenFile()
{
	OPENFILENAME opfn;
	opfn.hwndOwner = hInstBase;
	WCHAR strFilename[MAX_PATH];//����ļ���  
								//��ʼ��  
	ZeroMemory(&opfn, sizeof(OPENFILENAME));
	opfn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С  
											//���ù���  
	opfn.lpstrFilter = L"�ı��ļ�\0*.txt\0";
	//Ĭ�Ϲ�����������Ϊ1  
	opfn.nFilterIndex = 1;
	//�ļ������ֶα����Ȱѵ�һ���ַ���Ϊ \0  
	opfn.lpstrFile = strFilename;
	opfn.lpstrFile[0] = '\0';
	opfn.nMaxFile = sizeof(strFilename);
	//���ñ�־λ�����Ŀ¼���ļ��Ƿ����  
	opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	//opfn.lpstrInitialDir = NULL;  
	// ��ʾ�Ի������û�ѡ���ļ�  
	if (GetOpenFileName(&opfn))
	{
		//���ı�������ʾ�ļ�·��  
		SetWindowText(hInstEditSrc, opfn.lpstrFile);
	}
	EnableWindow(hInstBase, TRUE);
}






//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
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
					SetDlgItemText(hInstBase, 1001, _T("���ڷ���..."));
					EnableWindow(hInstDown, FALSE);
					//�����߳�
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

			
            // �����˵�ѡ��: 
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
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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

// �����ڡ������Ϣ�������
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
