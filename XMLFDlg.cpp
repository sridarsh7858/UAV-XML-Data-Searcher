// XMLFDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "XMLF.h"
#include "XMLFDlg.h"
#include "afxdialogex.h"
#include <afxdlgs.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <windows.h>
#include <comdef.h>
#include <msxml6.h>
#include <algorithm>
#pragma comment(lib, "msxml6.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CXMLFDlg dialog
CXMLFDlg::CXMLFDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_XMLF_DIALOG, pParent)
    , m_TN(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXMLFDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO1, m_P);
    DDX_Control(pDX, IDC_COMBO2, m_TT);
    DDX_Control(pDX, IDC_COMBO3, m_TSD);
    DDX_Control(pDX, IDC_COMBO5, m_E);
    DDX_Control(pDX, IDC_COMBO4, m_A);
    DDX_Control(pDX, IDC_COMBO6, m_C);
    DDX_Text(pDX, IDC_EDIT1, m_TN);
}

BEGIN_MESSAGE_MAP(CXMLFDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CXMLFDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CXMLFDlg message handlers
BOOL CXMLFDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Add "About..." menu item to system menu.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);            // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // Populate combo boxes
    m_P.AddString(_T("EO"));
    m_P.AddString(_T("SAR"));
    m_P.AddString(_T("MPAR"));
    m_P.AddString(_T("ELINT"));
    m_P.AddString(_T("COMINT"));

    m_TT.AddString(_T("Building"));
    m_TT.AddString(_T("Bridge"));
    m_TT.AddString(_T("Sub Station"));
    m_TT.AddString(_T("Petrol Bunk"));
    m_TT.AddString(_T("Runway"));
    m_TT.AddString(_T("Road Junction"));
    m_TT.AddString(_T("Water Body"));
    m_TT.AddString(_T("Vehicle"));
    m_TT.AddString(_T("People"));
    m_TT.AddString(_T("Animal"));

    m_TSD.AddString(_T("Static"));
    m_TSD.AddString(_T("Dynamic"));

    m_E.AddString(_T("Laser Firing"));
    m_E.AddString(_T("Vehicle Tracking"));
    m_E.AddString(_T("Take Off"));
    m_E.AddString(_T("Landing"));
    m_E.AddString(_T("Hovering"));
    m_E.AddString(_T("Rolling"));
    m_E.AddString(_T("Climbing"));

    m_A.AddString(_T("Roll"));
    m_A.AddString(_T("Pitch"));
    m_A.AddString(_T("Hdg"));
    m_A.AddString(_T("Alt"));

    m_C.AddString(_T("Azimuth"));
    m_C.AddString(_T("Elevation"));
    m_C.AddString(_T("FOV"));

    return TRUE;
}

void CXMLFDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CXMLFDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CXMLFDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CXMLFDlg::OnBnClickedButton1()
{
    UpdateData(TRUE); // Sync variables with UI controls

    // Retrieve input parameters
    CString payload, targetType, targetSD, event, aircraft, camera, targetName;
    m_P.GetWindowText(payload);
    m_TT.GetWindowText(targetType);
    m_TSD.GetWindowText(targetSD);
    m_E.GetWindowText(event);
    m_A.GetWindowText(aircraft);
    m_C.GetWindowText(camera);
    targetName = m_TN;

    // Convert CString to std::wstring for processing
    std::vector<std::wstring> searchParams;
    if (!payload.IsEmpty()) searchParams.push_back((LPCTSTR)payload);
    if (!targetType.IsEmpty()) searchParams.push_back((LPCTSTR)targetType);
    if (!targetSD.IsEmpty()) searchParams.push_back((LPCTSTR)targetSD);
    if (!event.IsEmpty()) searchParams.push_back((LPCTSTR)event);
    if (!aircraft.IsEmpty()) searchParams.push_back((LPCTSTR)aircraft);
    if (!camera.IsEmpty()) searchParams.push_back((LPCTSTR)camera);
    if (!targetName.IsEmpty()) searchParams.push_back((LPCTSTR)targetName);

    if (searchParams.empty())
    {
        AfxMessageBox(_T("Please enter at least one search parameter."));
        return;
    }

    // Prepare results file
    std::ofstream resultsFile("results.txt");
    if (!resultsFile)
    {
        AfxMessageBox(_T("Unable to create results.txt."));
        return;
    }

    // Get current directory
    TCHAR currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    // Store matched files and their start times
    struct FileData {
        CString filePath;
        std::wstring startTime;
    };
    std::vector<FileData> matchedFiles;

    // Traverse XML files in the directory
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(_T("*.xml"), &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        AfxMessageBox(_T("No XML files found in the directory."));
        return;
    }

    do
    {
        CString filePath;
        filePath.Format(_T("%s\\%s"), currentDir, findFileData.cFileName);

        CComPtr<IXMLDOMDocument> pXMLDoc;
        HRESULT hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pXMLDoc));
        if (FAILED(hr))
        {
            AfxMessageBox(_T("Failed to initialize MSXML parser."));
            return;
        }

        VARIANT_BOOL isSuccess;
        pXMLDoc->put_async(VARIANT_FALSE);
        hr = pXMLDoc->load(_variant_t(filePath), &isSuccess);
        if (isSuccess == VARIANT_TRUE)
        {
            bool allParamsMatch = true;

            for (const auto& param : searchParams)
            {
                CComPtr<IXMLDOMNode> pNode;
                CString xpath;
                xpath.Format(_T("//*[text()='%s']"), param.c_str());
                hr = pXMLDoc->selectSingleNode(_bstr_t(xpath), &pNode);

                if (!pNode)
                {
                    allParamsMatch = false;
                    break;
                }
            }

            if (allParamsMatch)
            {
                CComPtr<IXMLDOMNode> pStartTimeNode;
                hr = pXMLDoc->selectSingleNode(_bstr_t(L"//Target/Start_Time"), &pStartTimeNode);

                if (pStartTimeNode)
                {
                    CComBSTR bstrStartTime;
                    pStartTimeNode->get_text(&bstrStartTime);
                    matchedFiles.push_back({ filePath, std::wstring(bstrStartTime) });
                }
            }
        }

    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    // Sort matched files by Start_Time
    std::sort(matchedFiles.begin(), matchedFiles.end(), [](const FileData& a, const FileData& b) {
        return a.startTime < b.startTime;
        });

    // Generate results
    for (const auto& fileData : matchedFiles)
    {
        resultsFile << "Matched file: " << CT2A(fileData.filePath) << "\n";

        CComPtr<IXMLDOMDocument> pXMLDoc;
        HRESULT hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pXMLDoc));
        if (SUCCEEDED(hr))
        {
            VARIANT_BOOL isSuccess;
            pXMLDoc->put_async(VARIANT_FALSE);
            hr = pXMLDoc->load(_variant_t(fileData.filePath), &isSuccess);
            if (isSuccess == VARIANT_TRUE)
            {
                // Extract and write sibling nodes
                CComPtr<IXMLDOMNode> firstMatchedNode;
                hr = pXMLDoc->selectSingleNode(_bstr_t(L"//Target/Start_Time"), &firstMatchedNode);

                if (firstMatchedNode)
                {
                    CComPtr<IXMLDOMNode> pParentNode;
                    firstMatchedNode->get_parentNode(&pParentNode);

                    if (pParentNode)
                    {
                        CComPtr<IXMLDOMNodeList> siblingNodes;
                        pParentNode->get_childNodes(&siblingNodes);

                        long siblingCount = 0;
                        siblingNodes->get_length(&siblingCount);

                        for (long i = 0; i < siblingCount; ++i)
                        {
                            CComPtr<IXMLDOMNode> siblingNode;
                            siblingNodes->get_item(i, &siblingNode);

                            if (siblingNode)
                            {
                                CComBSTR siblingTagName, siblingValue;
                                siblingNode->get_nodeName(&siblingTagName);
                                siblingNode->get_text(&siblingValue);

                                // Skip redundant Event node
                                if (wcscmp(siblingTagName, L"Event") != 0)
                                {
                                    resultsFile << "  " << CW2A(siblingTagName) << " - " << CW2A(siblingValue) << "\n";
                                }
                            }
                        }
                    }
                }

                // Add Event contents before Description
                CComPtr<IXMLDOMNode> pEventNode;
                hr = pXMLDoc->selectSingleNode(_bstr_t(L"//Event"), &pEventNode);

                if (pEventNode)
                {
                    resultsFile << "  Event Contents:\n";

                    CComPtr<IXMLDOMNodeList> childNodes;
                    pEventNode->get_childNodes(&childNodes);

                    long childCount = 0;
                    childNodes->get_length(&childCount);

                    for (long i = 0; i < childCount; ++i)
                    {
                        CComPtr<IXMLDOMNode> childNode;
                        childNodes->get_item(i, &childNode);

                        if (childNode)
                        {
                            CComBSTR childTagName, childValue;
                            childNode->get_nodeName(&childTagName);
                            childNode->get_text(&childValue);

                            resultsFile << "    " << CW2A(childTagName) << " - " << CW2A(childValue) << "\n";
                        }
                    }
                }
            }
        }

        resultsFile << "\n"; // Add spacing between files
    }

    resultsFile.close();

    if (!matchedFiles.empty())
    {
        ShellExecute(0, 0, _T("results.txt"), 0, 0, SW_SHOW);
    }
    else
    {
        AfxMessageBox(_T("No files matched the entered parameters."));
    }
}
