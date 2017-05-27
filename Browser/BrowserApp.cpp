#include "BrowserApp.h"
#include "JSHandler.h"

namespace Browser
{
	//////////////////////////////////////////////////////////////////////////////////////////
	// CefApp methods.
	void CBrowserApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
	{
		if (process_type.empty()) 
		{
			// �����ӽ���·���ܹؼ�����������ã����ܻᴥ��cef��һ��bug
			// cef��LoadUrl������Ⱦ����ʱ��������ӽ��̵�·�������ܻ�����һ��bug����IDE��debug״̬ʱ����
			// ����ָ�����ӽ���·���Ϳ�����

			// ����ʹ��sandbox�Ļ���������ʹ��������ӽ���;��ʹ��sandbox�Ļ�����һ�μ���flash���ʱ�ᵯ����������ʾ�У�����cef��bug��flash���ӽ��̶��߲��ɼ��
			//command_line->AppendSwitchWithValue("browser-subprocess-path", "render.exe");

			command_line->AppendSwitchWithValue("ppapi-flash-version", "20.0.0.228");
			command_line->AppendSwitchWithValue("ppapi-flash-path", "PepperFlash\\pepflashplayer.dll");

			//ͬһ�����µ�ʹ��ͬһ����Ⱦ����
			command_line->AppendSwitch("process-per-site");
			command_line->AppendSwitch("enable-caret-browsing");
			command_line->AppendSwitch("auto-positioned-ime-window");

			//����һЩѡ�����cef����
			command_line->AppendSwitch("disable-surfaces");
			command_line->AppendSwitch("disable-gpu");
			command_line->AppendSwitch("disable-gpu-compositing");

			//����������Ⱦ
			//command_line->AppendSwitch("enable-begin-frame-scheduling");
		}
	}

	void CBrowserApp::OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar)
	{
		// Default schemes that support cookies.
		cookieable_schemes_.push_back("http");
		cookieable_schemes_.push_back("https");
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// CefBrowserProcessHandler methods.
	void CBrowserApp::OnContextInitialized() 
	{
		// Register cookieable schemes with the global cookie manager.
		CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(NULL);
		manager->SetSupportedSchemes(cookieable_schemes_, NULL);

		// ɾ�������Cooies��Ϣ
		// manager->DeleteCookies(L"", L"", nullptr);
	}

	void CBrowserApp::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
	{
	}

	void CBrowserApp::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) 
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// CefRenderProcessHandler methods.
	void CBrowserApp::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) 
	{
	}

	void CBrowserApp::OnWebKitInitialized() 
	{
		//JS��չ����
		std::string extensionCode =
			"var Client;"
			"if (!Client)"
			"  Client = {};"
			"(function() {"
			"  Client.GetComputerName = function() {"
			"    native function GetComputerName();"
			"      return GetComputerName();"
			"  };"
			"})();";
		CefRegisterExtension( "v8/Client", extensionCode, new CJSHandler() );
	}

	void CBrowserApp::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
	{
	}

	void CBrowserApp::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) 
	{
	}

	CefRefPtr<CefLoadHandler> CBrowserApp::GetLoadHandler()
	{
		return NULL;
	}

	bool CBrowserApp::OnBeforeNavigation(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		NavigationType navigation_type,
		bool is_redirect) 
	{
		return false;
	}

	void CBrowserApp::OnContextCreated(CefRefPtr<CefBrowser> browser,	CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) 
	{
	}

	void CBrowserApp::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,	CefRefPtr<CefV8Context> context) 
	{
	}

	void CBrowserApp::OnUncaughtException
		(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context,
		CefRefPtr<CefV8Exception> exception,
		CefRefPtr<CefV8StackTrace> stackTrace) 
	{
	}

	void CBrowserApp::OnFocusedNodeChanged(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefDOMNode> node) 
	{
	}

	bool CBrowserApp::OnProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) 
	{
		return false;
	}
}
