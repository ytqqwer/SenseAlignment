// SenseAlignment.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SenseAlignment.h"

#include <stdio.h>
#include <commdlg.h>
#include <CommCtrl.h>

//// 导入静态库  
//#pragma comment(lib, "Comctl32.lib")  
//// 开启视觉效果 Copy from MSDN  
//#pragma comment(linker,"\"/manifestdependency:type='win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

HWND hSearchEdit;							// 搜索框句柄
HWND hClassComboBox;						// 类别下拉列表句柄
HWND hSearchButton;							// 搜索按钮句柄

HWND hDictionaryOneListView;					// 词典1列表视图句柄
HWND hDictionaryTwoListView;					// 词典2列表视图句柄

HWND hDictionaryOneDetailsListView;				// 词典1更详细列表视图句柄
HWND hDictionaryTwoDetailsListView;				// 词典2更详细列表视图句柄

HWND hSimilarityText;						// 相似度
HWND hRelationshipText;						// 对应关心
HWND hNewRelationshipText;					// 新对应关系

HWND hRelationEqualButton;					// 相等按钮
HWND hRelationNotEqualButton;				// 不相等按钮
HWND hRelationUnsureButton;					// 不确定按钮
HWND hRelationBelongButton;					// 属于按钮

//旧搜索编辑框处理过程
WNDPROC oldEditSearchProc;

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//搜索编辑框处理过程
LRESULT CALLBACK	subEditSearchProc(HWND, UINT, WPARAM, LPARAM);

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
	
	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SENSEALIGNMENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 0);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SENSEALIGNMENT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
		CW_USEDEFAULT, 0, 800, 450, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	//初始化搜索编辑框
	hSearchEdit = CreateWindow(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_LEFT,
		70, 30, 200, 30, hWnd, (HMENU)ID_SEARCH_EDIT, hInst, NULL);
	oldEditSearchProc = (WNDPROC)SetWindowLongPtr(hSearchEdit, GWLP_WNDPROC, (LONG_PTR)subEditSearchProc);

	//初始化搜索类别下拉列表
	hClassComboBox = CreateWindow(WC_COMBOBOX, _T(""), CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE ,
		310, 30, 100, 500, hWnd, (HMENU)ID_SEARCH_COMBOBOX, hInst, NULL);
	
	// load the combobox with item list. Send a CB_ADDSTRING message to load each item
	TCHAR temp[100];

	LoadStringW(hInstance, ID_PART_OF_SPEECH_N, temp, MAX_LOADSTRING);
	SendMessage(hClassComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)temp);
	
	LoadStringW(hInstance, ID_PART_OF_SPEECH_V, temp, MAX_LOADSTRING);
	SendMessage(hClassComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)temp);

	LoadStringW(hInstance, ID_PART_OF_SPEECH_A, temp, MAX_LOADSTRING);
	SendMessage(hClassComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)temp);

	LoadStringW(hInstance, ID_PART_OF_SPEECH_M, temp, MAX_LOADSTRING);
	SendMessage(hClassComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)temp);

	LoadStringW(hInstance, ID_PART_OF_SPEECH_Q, temp, MAX_LOADSTRING);
	SendMessage(hClassComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)temp);

	LoadStringW(hInstance, ID_PART_OF_SPEECH_R, temp, MAX_LOADSTRING);
	SendMessage(hClassComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)temp);

	LoadStringW(hInstance, ID_PART_OF_SPEECH_D, temp, MAX_LOADSTRING);
	SendMessage(hClassComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)temp);
	
	LoadStringW(hInstance, ID_PART_OF_SPEECH_P, temp, MAX_LOADSTRING);
	SendMessage(hClassComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)temp);

	LoadStringW(hInstance, ID_PART_OF_SPEECH_C, temp, MAX_LOADSTRING);
	SendMessage(hClassComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)temp);

	LoadStringW(hInstance, ID_PART_OF_SPEECH_U, temp, MAX_LOADSTRING);
	SendMessage(hClassComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)temp);

	LoadStringW(hInstance, ID_PART_OF_SPEECH_E, temp, MAX_LOADSTRING);
	SendMessage(hClassComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)temp);

	LoadStringW(hInstance, ID_PART_OF_SPEECH_O, temp, MAX_LOADSTRING);
	SendMessage(hClassComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)temp);
	
	// Send the CB_SETCURSEL message to display an initial item in the selection field  
	SendMessage(hClassComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	
	//初始化搜索按钮
	hSearchButton = CreateWindow(_T("BUTTON"), _T("搜索"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		430, 30, 100, 30, hWnd, (HMENU)ID_SEARCH_BUTTON, hInst, NULL);
	
	//初始化文本
	CreateWindow(_T("static"), _T("词语"), WS_CHILD | WS_VISIBLE | SS_LEFT, 30, 30, 30, 30, hWnd,
		NULL, hInst, NULL);
	CreateWindow(_T("static"), _T("词类"), WS_CHILD | WS_VISIBLE | SS_LEFT, 270, 30, 30, 30, hWnd,
		NULL, hInst, NULL);

	CreateWindow(_T("static"), _T("词典1"), WS_CHILD | WS_VISIBLE | SS_LEFT, 30, 110, 40, 30, hWnd,
		NULL, hInst, NULL);
	CreateWindow(_T("static"), _T("词典2"), WS_CHILD | WS_VISIBLE | SS_LEFT, 30, 210, 40, 30, hWnd,
		NULL, hInst, NULL);

	LoadStringW(hInstance, ID_TEXT_STATIC_SIMILARITY, temp, MAX_LOADSTRING);
	HWND hSimilarityStaticText = CreateWindow(_T("static"), temp, WS_CHILD | WS_VISIBLE | SS_LEFT, 550, 10, 100, 30, hWnd,
		(HMENU)ID_TEXT_STATIC_SIMILARITY, hInst, NULL);
	hSimilarityText = CreateWindow(_T("static"), _T("test"), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT, 650, 10, 90, 30, hWnd,
		(HMENU)ID_TEXT_SIMILARITY, hInst, NULL);
	
	LoadStringW(hInstance, ID_TEXT_STATIC_RELATIONSHIP, temp, MAX_LOADSTRING);
	HWND hRelationshipStaticText = CreateWindow(_T("static"), temp, WS_CHILD | WS_VISIBLE | SS_LEFT, 550, 40, 100, 30, hWnd,
		(HMENU)ID_TEXT_STATIC_RELATIONSHIP, hInst, NULL);
	hRelationshipText = CreateWindow(_T("static"), _T("test"), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT, 650, 40, 90, 30, hWnd,
		(HMENU)ID_TEXT_RELATIONSHIP, hInst, NULL);

	LoadStringW(hInstance, ID_TEXT_STATIC_NEW_RELATIONSHIP, temp, MAX_LOADSTRING);
	HWND hNewRelationshipStaticText = CreateWindow(_T("static"), temp, WS_CHILD | WS_VISIBLE | SS_LEFT, 550, 70, 100, 30, hWnd,
		(HMENU)ID_TEXT_STATIC_NEW_RELATIONSHIP, hInst, NULL);
	hNewRelationshipText = CreateWindow(_T("static"), _T("test"), WS_CHILD | WS_VISIBLE| WS_BORDER | SS_LEFT, 650, 70, 90, 30, hWnd,
		(HMENU)ID_TEXT_NEW_RELATIONSHIP, hInst, NULL);

	//初始化词典1的列表视图
	hDictionaryOneListView = CreateWindow(WC_LISTVIEW, L"",	WS_CHILD | WS_VISIBLE | LVS_REPORT | WS_BORDER,
		30, 140, 600,45, hWnd,	(HMENU)ID_DICTIONARY_ONE_LISTVIEW,	hInst,	NULL);
	
	WCHAR szText[256];     // Temporary buffer.
	LVCOLUMN lvc;
	int iCol;
	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	// Add the columns.
	for (iCol = 0; iCol < 4; iCol++)
	{
		lvc.iSubItem = iCol;
		lvc.pszText = szText;
		lvc.fmt = LVCFMT_LEFT;		// Left-aligned column.

		if (iCol < 2)				// Width of column in pixels.
			lvc.cx = 100;             
		else
			lvc.cx = 200;
									// Load the names of the column headings from the string resources.
		LoadString(hInst, ID_DICTIONARY_COLUMN_WORDS + iCol, szText, sizeof(szText) / sizeof(szText[0]));

		// Insert the columns into the list view.
		if (ListView_InsertColumn(hDictionaryOneListView, iCol, &lvc) == -1)
			return FALSE;
	}
	
	//初始化词典2的列表视图
	hDictionaryTwoListView = CreateWindow(WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | LVS_REPORT | WS_BORDER,
		30, 240, 600, 45, hWnd, (HMENU)ID_DICTIONARY_ONE_LISTVIEW, hInst, NULL);

	// Add the columns.
	for (iCol = 0; iCol < 4; iCol++)
	{
		lvc.iSubItem = iCol;
		lvc.pszText = szText;
		lvc.fmt = LVCFMT_LEFT;		// Left-aligned column.

		if (iCol < 2)				// Width of column in pixels.
			lvc.cx = 100;
		else
			lvc.cx = 200;

		// Load the names of the column headings from the string resources.
		LoadString(hInst, ID_DICTIONARY_COLUMN_WORDS + iCol, szText, sizeof(szText) / sizeof(szText[0]));

		// Insert the columns into the list view.
		if (ListView_InsertColumn(hDictionaryTwoListView, iCol, &lvc) == -1)
			return FALSE;
	}
	
	//初始化对应关系按钮
	hRelationEqualButton = CreateWindow(_T("BUTTON"), _T("相等"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		650, 120, 100, 30, hWnd, (HMENU)ID_EQUAL_BUTTON, hInst, NULL);

	hRelationNotEqualButton = CreateWindow(_T("BUTTON"), _T("不相等"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		650, 170, 100, 30, hWnd, (HMENU)ID_NOT_EQUAL_BUTTON, hInst, NULL);

	hRelationUnsureButton = CreateWindow(_T("BUTTON"), _T("不确定"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		650, 220, 100, 30, hWnd, (HMENU)ID_UNSURE_BUTTON, hInst, NULL);
	
	hRelationBelongButton = CreateWindow(_T("BUTTON"), _T("属于"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		650, 270, 100, 30, hWnd, (HMENU)ID_BELONG_BUTTON, hInst, NULL);
	
	//下一个按钮

	hRelationBelongButton = CreateWindow(_T("BUTTON"), _T("下一词义"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		200, 330, 100, 30, hWnd, (HMENU)ID_NEXT_SENSE_BUTTON, hInst, NULL);
	
	hRelationBelongButton = CreateWindow(_T("BUTTON"), _T("下一词语"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		350, 330, 100, 30, hWnd, (HMENU)ID_NEXT_WORD_BUTTON, hInst, NULL);



	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


// 搜索编辑框 的消息处理函数
LRESULT CALLBACK subEditSearchProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_RETURN:
			//Do your stuff

			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);

			break;  //or return 0; if you don't want to pass it further to def proc
					//If not your key, skip to default:
		}
	default:
		return CallWindowProc(oldEditSearchProc, hWnd, msg, wParam, lParam);
	}
	return 0;
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
		// 分析wParam全部: 
		//switch (wParam)
		//{
		//case CBN_SELCHANGE:
		//{
		//
		//}
		//break;
		//default:
		//	//继续处理
		//	break;
		//}

		// 分析wParam高位: 
		switch (HIWORD(wParam))
		{
		case CBN_SELCHANGE: 
		{
			// If the user makes a selection from the list:
			//   Send CB_GETCURSEL message to get the index of the selected list item.
			//   Send CB_GETLBTEXT message to get the item.
			//   Display the item in a messagebox.

			int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
				(WPARAM)0, (LPARAM)0);
			TCHAR  ListItem[256];
			(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
				(WPARAM)ItemIndex, (LPARAM)ListItem);
			MessageBox(hWnd, (LPCWSTR)ListItem, TEXT("Item Selected"), MB_OK);

		}
			break;
		default:
			//继续处理
			break;
		}
		
		// 分析wParam低位: 
		switch (LOWORD(wParam))
		{
		case ID_SEARCH_BUTTON:
			MessageBox(hWnd, L"您点击了一个按钮。", L"提示", MB_OK);
			break;
		case ID_EQUAL_BUTTON:
			MessageBox(hWnd, L"点击了一个按钮。", L"提示", MB_OKCANCEL);
			break;
		case ID_NOT_EQUAL_BUTTON:
			MessageBox(hWnd, L"一个按钮。", L"提示", MB_OK);
			break;
		case ID_UNSURE_BUTTON:
			MessageBox(hWnd, L"点击按钮。", L"提示", MB_OKCANCEL);
			break;
		case ID_BELONG_BUTTON:
			MessageBox(hWnd, L"点了个按钮。", L"提示", MB_OK);
			break;
		case ID_NEXT_SENSE_BUTTON:
			MessageBox(hWnd, L"按钮。", L"提示", MB_OKCANCEL);
			break;
		case ID_NEXT_WORD_BUTTON:
			MessageBox(hWnd, L"点按钮。", L"Tip", MB_OK);
			break;


		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		//// 分析lParam全部: 
		//switch (lParam)
		//{
		//case ID_SEARCH_BUTTON:
		//{
		//	MessageBox(hWnd, L"您点击了一个按钮。", L"提示", MB_OK);
		//}
		//break;
		//default:
		//	return DefWindowProc(hWnd, message, wParam, lParam);
		//}


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
