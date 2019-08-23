#include "SpriteWindow.h"
#include "SpriteWndProc.h"


SpriteWindow::SpriteWindow(LPCSTR className, HINSTANCE hInstance, LPCSTR wndTitle, SIZE wndsize)
	:windowClassName(className), instance(hInstance), windowTitle(wndTitle), windowSize(wndsize) {
}

SpriteWindow::~SpriteWindow()
{
}
//
HWND SpriteWindow::CreateIrrWindow(POINT point) {

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = SpriteWndProc;
	wndClass.hInstance = this->instance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = this->windowClassName;

	// 注册
	RegisterClassEx(&wndClass);
	// 创建
	this->window = CreateWindowEx(
		// 窗口扩展样式
		WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED,
		// 窗体类名
		this->windowClassName,
		// 窗口标题
		this->windowTitle,
		// 窗口样式
		WS_POPUP | WS_VISIBLE,
		// 窗口初始屏幕X坐标
		point.x,
		// 窗口初始屏幕Y坐标
		point.y,
		// 窗口初始宽度
		this->windowSize.cx,
		// 窗口初始高度
		this->windowSize.cx,
		// 父窗体句柄
		NULL,
		// 菜单的句柄或是子窗口的标识符
		NULL,
		// 应用程序实例的句柄
		this->instance,
		// 指向窗口的创建数据
		this
	);

	/// 初始化hdc  创建兼容dc >>>
	RECT clientRect = { 0,0,this->windowSize.cx,this->windowSize.cy };
	this->hdcDst = GetDC(this->window);
	this->hdcSrc = CreateCompatibleDC(this->hdcDst);
	HBITMAP memBitmap = ::CreateCompatibleBitmap(hdcDst, this->windowSize.cx, this->windowSize.cy);
	::SelectObject(this->hdcSrc, memBitmap);

	return this->window;
}

void SpriteWindow::RedrawWndFrame()
{
	POINT ptSrc = { 0 };
	BLENDFUNCTION bf;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = 255;
	::UpdateLayeredWindow(this->window, this->hdcDst, NULL, &this->windowSize, this->hdcSrc, &ptSrc, NULL, &bf, ULW_ALPHA);
}
