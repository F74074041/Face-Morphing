
// HW2Dlg.cpp : 實作檔
//

#include "stdafx.h"
#include "HW2.h"
#include "HW2Dlg.h"
#include "afxdialogex.h"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "iostream"
#include "cmath"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace cv;


// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
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


// CHW2Dlg 對話方塊



CHW2Dlg::CHW2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HW2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHW2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHW2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CHW2Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CHW2Dlg 訊息處理常式

BOOL CHW2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);


	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CHW2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CHW2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CHW2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

Mat img, img2,out,out2,out3;
int line1 = 0;
vector<Point2f> img1_start,img1_end,img2_start,img2_end; 
double alpha = 0, beta;
int alpha_slider, alpha_slider_max = 100;
void morphing(int, void*) {
	double weight, wsum,u,v;
	Point2f des,perpendicularQP,src,perpendicularQP2, psum;
	int i, j, k;
	Mat mapx, mapy,mapx1,mapy1;
	mapx.create(img.size(), CV_32FC1);
	mapy.create(img.size(), CV_32FC1);
	mapx1.create(img2.size(), CV_32FC1);
	mapy1.create(img.size(), CV_32FC1);
	alpha = (double)alpha_slider / alpha_slider_max;
	beta = 1.0 - alpha;
	//img1
	for (i = 0; i < 255; ++i) {
		for (j = 0; j < 189; ++j) {
			des = Point(i, j);
			psum = Point(0,0); weight = 0; u = 0; v = 0; wsum = 0;
			for (k = 0; k < img1_start.size(); k++) {
				perpendicularQP = Point((img2_end.at(k) - img2_start.at(k)).y, -((img2_end.at(k) - img2_start.at(k)).x));
				perpendicularQP2=Point((img1_end.at(k) - img1_start.at(k)).y, -((img1_end.at(k) - img1_start.at(k)).x));
				u = ((double)(img2_end.at(k) - img2_start.at(k)).x*(des - img2_start.at(k)).x + (img2_end.at(k) - img2_start.at(k)).y*(des - img2_start.at(k)).y) / (pow((double)(img2_end.at(k) - img2_start.at(k)).x, 2) + pow((double)(img2_end.at(k) - img2_start.at(k)).y, 2));
				v = ((double)(des - img2_start.at(k)).x*perpendicularQP.x + (des - img2_start.at(k)).y*perpendicularQP.y) / pow((pow((double)(img2_end.at(k) - img2_start.at(k)).x, 2) + pow((double)(img2_end.at(k) - img2_start.at(k)).y, 2)), 0.5);
				src=img1_start.at(k)+u*(img1_end.at(k)-img1_start.at(k))+v*perpendicularQP2/ pow((pow((double)(img1_end.at(k) - img1_start.at(k)).x, 2) + pow((double)(img1_end.at(k) - img1_start.at(k)).y, 2)), 0.5);
				weight =  pow(((double)pow((img1_end.at(k) - img1_start.at(k)).x,2)+ (double)pow((img1_end.at(k) - img1_start.at(k)).y, 2)), 0.25)/(1+abs(v));
				psum = psum + src*weight;
				wsum += weight;

			}
			mapx.at<float>(j, i) = ((psum / wsum-des )*beta+des).x;
			mapy.at<float>(j, i) = ((psum / wsum-des )*beta+des).y;
		}
	}
	remap(img, out, mapx, mapy, INTER_LINEAR);
	//img2
	for (i = 0; i < 255; ++i) {
		for (j = 0; j < 189; ++j) {
			des = Point(i, j);
			psum = Point(0, 0); weight = 0; u = 0; v = 0; wsum = 0;
			for (k = 0; k < img1_start.size(); k++) {
				perpendicularQP = Point((img1_end.at(k) - img1_start.at(k)).y, -((img1_end.at(k) - img1_start.at(k)).x));
				perpendicularQP2 = Point((img2_end.at(k) - img2_start.at(k)).y, -((img2_end.at(k) - img2_start.at(k)).x));
				u = ((double)(img1_end.at(k) - img1_start.at(k)).x*(des - img1_start.at(k)).x + (img1_end.at(k) - img1_start.at(k)).y*(des - img1_start.at(k)).y) / (pow((double)(img1_end.at(k) - img1_start.at(k)).x, 2) + pow((double)(img1_end.at(k) - img1_start.at(k)).y, 2));
				v = ((double)(des - img1_start.at(k)).x*perpendicularQP.x + (des - img1_start.at(k)).y*perpendicularQP.y) / pow((pow((double)(img1_end.at(k) - img1_start.at(k)).x, 2) + pow((double)(img1_end.at(k) - img1_start.at(k)).y, 2)), 0.5);
				src = img2_start.at(k) + u*(img2_end.at(k) - img2_start.at(k)) + v*perpendicularQP2 / pow((pow((double)(img2_end.at(k) - img2_start.at(k)).x, 2) + pow((double)(img2_end.at(k) - img2_start.at(k)).y, 2)), 0.5);
				weight =pow((double) pow((double)(pow(((double)pow((img2_end.at(k) - img2_start.at(k)).x, 2) + (double)pow((img2_end.at(k) - img2_start.at(k)).y, 2)), 0.25) / (1 + abs(v))),1),0.5);
				psum = psum + src*weight;
				wsum += weight;

			}
				mapx1.at<float>(j, i) = ((psum / wsum-des)*alpha+des).x;
				mapy1.at<float>(j, i) = ((psum / wsum-des)*alpha+des).y;
				
			
			
			
		}
	}
	remap(img2, out2, mapx1, mapy1, INTER_LINEAR);
	addWeighted(out, alpha, out2, beta, 0, out3);
	//imshow("wrap1", out);
	
	//arrowedLine(img, Point2f(138, 25), Point2f(163, 25), Scalar(255, 255, 255), 2);
	imshow("Blend", out3);
	//imshow("out", img);
}
void CHW2Dlg::OnBnClickedButton1()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	
	img = imread("./images/women.jpg");
	img2 = imread("./images/cheetah.jpg");
	//namedWindow("org", 1);

	img1_start = { Point2f(70,42),Point2f(63,47),Point2f(138,45),Point2f(138,49),Point2f(120,31),Point2f(97,97),Point2f(88,131),Point2f(44,25),Point2f(175,25) };
	img1_end = { Point2f(90,42),Point2f(90,47),Point2f(163,45),Point2f(161,49),Point2f(120,92),Point2f(130,97),Point2f(135,131),Point2f(66,167),Point2f(155,163) };
	img2_start = { Point2f(38,20),Point2f(38,28),Point2f(185,18),Point2f(185,28),Point2f(127,23),Point2f(99,134),Point2f(85,181),Point2f(0,26),Point2f(250,22) };
	img2_end = { Point2f(72,20),Point2f(75,28),Point2f(218,18),Point2f(218,28),Point2f(127,125),Point2f(159,134),Point2f(161,181),Point2f(44,190),Point2f(200,190) };
	namedWindow("Blend", 1);
	createTrackbar("Blend: ", "Blend", &alpha_slider, alpha_slider_max, morphing);
	morphing(alpha_slider, 0);
	//cout << img1_start;
	//imshow("org", img);
	//cout << img1_start.size() << "  " << img1_start.at(0).x;
	//FreeConsole();
	
}
