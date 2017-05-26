#include "StdAfx.h"
#include <sstream>
#include <string>
#include <ShellAPI.h>
#include <iphlpapi.h>
#include "Browser.h"
#include "BrowserUI.h"
#pragma comment(lib,"Iphlpapi.lib")


CBrowserHandler::CBrowserHandler()
	: m_Browser(NULL)
	, m_pWindow(NULL)
	, m_BrowserId(0)
	, m_Cookie(NULL)
{
	CreateProcessMessageDelegates(m_process_message_delegates);
	CreateRequestDelegates(m_request_delegates);
}

CBrowserHandler::~CBrowserHandler()
{
}

bool CBrowserHandler::OnProcessMessageReceived(
	CefRefPtr<CefBrowser> browser,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message)
{
	bool handled = false;

	// Execute delegate callbacks.
	ProcessMessageDelegateSet::iterator it = m_process_message_delegates.begin();
	for (; it != m_process_message_delegates.end() && !handled; ++it)
	{
		handled = (*it)->OnProcessMessageReceived(this, browser, source_process, message);
	}

	return handled;
}

void CBrowserHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	CefRefPtr<CefMenuModel> model)
{
	//model->Clear();//清空上下文菜单项

	//if(model->IsVisible(MENU_ID_PRINT))
	//	model->Remove(MENU_ID_PRINT);//删除打印菜单

	//if(model->IsVisible(MENU_ID_VIEW_SOURCE))
	//	model->Remove(MENU_ID_VIEW_SOURCE);//删除查看源码菜单

	//model->AddSeparator();//增加分隔符

	model->AddItem(CLIENT_ID_REFRESH,	CefString(L"刷新(&R)"));
	//model->AddItem(CLIENT_ID_SAMPLE,	CefString(L"计算机名"));
}

bool CBrowserHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	int command_id,
	EventFlags event_flags)
{
	switch (command_id)
	{
	case CLIENT_ID_REFRESH:
		browser->Reload();
		return true;
	case CLIENT_ID_SAMPLE:
		{
			std::string html =  
				"<html><body>"
				"<script language=\"JavaScript\">"
				"document.writeln('ComputerName:');"
				"document.writeln(Client.GetComputerName());"
				"</script>"
				"</body></html>";  
			frame->LoadString(html, "about:blank");
		}
		return true;
	default:
		return false;
	}
}

void CBrowserHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
	bool isLoading,
	bool canGoBack,
	bool canGoForward)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_BrowserId == browser->GetIdentifier() && m_pWindow != NULL)
	{
		m_pWindow->SetLoadingState(isLoading, canGoBack, canGoForward);
	}
}

void CBrowserHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& url)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_BrowserId == browser->GetIdentifier()
		/*&& frame->IsMain()*/ && m_pWindow != NULL)
	{
		m_pWindow->SetAddress(std::wstring(url).c_str());
	}
}

void CBrowserHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_BrowserId == browser->GetIdentifier() && m_pWindow != NULL)
	{
		m_pWindow->SetTitle(std::wstring(title).c_str());
	}
}


bool CBrowserHandler::OnTooltip(CefRefPtr<CefBrowser> browser,
	CefString& text)
{
	CEF_REQUIRE_UI_THREAD();

	return false;
}

void CBrowserHandler::OnStatusMessage(CefRefPtr<CefBrowser> browser,
	const CefString& value)
{
	CEF_REQUIRE_UI_THREAD();
}

bool CBrowserHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
	const CefString& message,
	const CefString& source,
	int line)
{
	CEF_REQUIRE_UI_THREAD();

	return false;
}

void CBrowserHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	const CefString& suggested_name,
	CefRefPtr<CefBeforeDownloadCallback> callback)
{
	CEF_REQUIRE_UI_THREAD();
	// Continue the download and show the "Save As" dialog.
	//callback->Continue(GetDownloadPath(suggested_name), true);
}



void CBrowserHandler::OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDownloadItem> download_item,
	CefRefPtr<CefDownloadItemCallback> callback)
{
	CEF_REQUIRE_UI_THREAD();
	//if (download_item->IsComplete())
	//{
	//	SetLastDownloadFile(download_item->GetFullPath());
	//	SendNotification(NOTIFY_DOWNLOAD_COMPLETE);
	//}
}


bool CBrowserHandler::OnRequestGeolocationPermission(CefRefPtr<CefBrowser> browser,
	const CefString& requesting_url,
	int request_id,
	CefRefPtr<CefGeolocationCallback> callback)
{
	// 允许来自所有网站的地理位置访问.
	callback->Continue(true);
	return true;
}

bool CBrowserHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
	const CefKeyEvent& event,
	CefEventHandle os_event,
	bool* is_keyboard_shortcut)
{
	//if (!event.focus_on_editable_field && event.windows_key_code == 0x20)
	//{
	//	if (event.type == KEYEVENT_RAWKEYDOWN)
	//	{
	//		browser->GetMainFrame()->ExecuteJavaScript("alert('You pressed the space bar!');", "", 0);
	//	}
	//	return true;
	//}

	return false;
}

bool CBrowserHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& target_url,
	const CefString& target_frame_name,
	cef_window_open_disposition_t target_disposition,
	bool user_gesture,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings,
	bool* no_javascript_access)
{
	//m_pWindow->Popup(std::wstring(target_url).c_str());
	//return true;
	return false;
}

void CBrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	AutoLock lock_scope(this);
	if (!m_Browser.get())
	{
		// We need to keep the main child window, but not popup windows
		m_Browser = browser;
		m_BrowserId = browser->GetIdentifier();
	}
}

bool CBrowserHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	if (m_BrowserId == browser->GetIdentifier())
	{
		if(m_pWindow != NULL)
			m_pWindow->Close();
		return true;
	}

	// Allow the close. For windowed browsers this will result in the OS close
	// event being sent.
	return false;
}

void CBrowserHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_BrowserId == browser->GetIdentifier())
	{
		// Free the browser pointer so that the browser can be destroyed
		m_Browser = NULL;
	}
}

void CBrowserHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame/*,TransitionType transition_type*/)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_BrowserId == browser->GetIdentifier() && frame->IsMain())
	{
		// We've just started loading a page
		//SetLoading(true);
		//Invoke_LoadStart(browser, frame);
	}
}

void CBrowserHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	int httpStatusCode)
{
	CEF_REQUIRE_UI_THREAD();

	if (m_BrowserId == browser->GetIdentifier() && frame->IsMain())
	{
		//frame->ExecuteJavaScript("alert('ExecuteJavaScript works!');", frame->GetURL(), 0);
		//CefRefPtr<CefV8Context> v8 = frame->GetV8Context();
	}
}

void CBrowserHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	ErrorCode errorCode,
	const CefString& errorText,
	const CefString& failedUrl)
{
	CEF_REQUIRE_UI_THREAD();

	// Don't display an error for downloaded files.
	if (errorCode == ERR_ABORTED)
		return;

	// Display a load error message.
	std::stringstream ss;
	ss << "<html><body><h2>Failed to load URL " << std::string(failedUrl) <<
		" with error " << std::string(errorText) << " (" << errorCode <<
		").</h2></body></html>";
	frame->LoadString(ss.str(), failedUrl);
}

void CBrowserHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
	TerminationStatus status)
{
}

CefRequestHandler::ReturnValue CBrowserHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	CefRefPtr<CefRequestCallback> callback)
{
	return RV_CONTINUE;
}

CefRefPtr<CefResourceHandler> CBrowserHandler::GetResourceHandler(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request)
{
	CefRefPtr<CefResourceHandler> handler;

	// Execute delegate callbacks.
	RequestDelegateSet::iterator it = m_request_delegates.begin();
	for (; it != m_request_delegates.end() && !handler.get(); ++it)
		handler = (*it)->GetResourceHandler(this, browser, frame, request);

	return handler;
}

void CBrowserHandler::OnProtocolExecution(CefRefPtr<CefBrowser> browser,
	const CefString& url,
	bool& allow_os_execution)
{
	std::string urlStr = url;

	// Allow OS execution of Spotify URIs.
	if (urlStr.find("spotify:") == 0)
		allow_os_execution = true;
}

void CBrowserHandler::CreateProcessMessageDelegates(ProcessMessageDelegateSet& delegates)
{
	// Create the binding test delegates.
	//client_extension::CreateProcessMessageDelegates(delegates);
	//client_extension::CreateProcessMessageDelegates(delegates);
}

void CBrowserHandler::CreateRequestDelegates(RequestDelegateSet& delegates)
{
	// Create the binding test delegates.
	//client_extension::CreateRequestDelegates(delegates);
}

void CBrowserHandler::CloseAllBrowsers(bool force_close)
{
	if (!CefCurrentlyOn(TID_UI)) {
		// Execute on the UI thread.
		CefPostTask(TID_UI, base::Bind(&CBrowserHandler::CloseAllBrowsers, this, force_close));
		return;
	}
}

bool CBrowserV8Handler::Execute(const CefString& name,
	CefRefPtr<CefV8Value> object,
	const CefV8ValueList& arguments,
	CefRefPtr<CefV8Value>& retval,
	CefString& exception)
{
	if (name == _T("GetComputerName")) {
		TCHAR szComputerName[MAX_COMPUTERNAME_LENGTH+1];
		memset(szComputerName,0,sizeof(szComputerName));
		DWORD dwNameLen = MAX_COMPUTERNAME_LENGTH;  
		GetComputerName(szComputerName, &dwNameLen);
		retval = CefV8Value::CreateString(szComputerName);
		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////////////////////
// CefApp methods.
void CBrowserApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
	if (process_type.empty()) 
	{
		// 设置子进程路径很关键，如果不设置，可能会触发cef的一个bug
		// cef在LoadUrl建立渲染进程时，会查找子进程的路径，可能会引发一个bug导致IDE在debug状态时卡死
		// 这里指定好子进程路径就可以了

		// 但是使用sandbox的话，不允许使用另外的子进程;不使用sandbox的话，第一次加载flash插件时会弹出个命令提示行，这是cef的bug。flash与子进程二者不可兼得
		//command_line->AppendSwitchWithValue("browser-subprocess-path", "render.exe");

		command_line->AppendSwitchWithValue("ppapi-flash-version", "20.0.0.228");
		command_line->AppendSwitchWithValue("ppapi-flash-path", "PepperFlash\\pepflashplayer.dll");

		//同一个域下的使用同一个渲染进程
		command_line->AppendSwitch("process-per-site");
		command_line->AppendSwitch("enable-caret-browsing");
		command_line->AppendSwitch("auto-positioned-ime-window");
		
		//禁用一些选项，提升cef性能
		command_line->AppendSwitch("disable-surfaces");
		command_line->AppendSwitch("disable-gpu");
		command_line->AppendSwitch("disable-gpu-compositing");

		//开启离屏渲染
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
	ASSERT(manager.get());
	manager->SetSupportedSchemes(cookieable_schemes_, NULL);

	// 删除保存的Cooies信息
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
	//JS扩展代码
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
	CefRegisterExtension( "v8/Client", extensionCode, new CBrowserV8Handler() );
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
	ASSERT(source_process == PID_BROWSER);

	return false;
}

CBrowserUI::CBrowserUI()
	: m_Browser(NULL)
{
	SetBkColor(0xFFFFFFFF);
}

CBrowserUI::~CBrowserUI()
{
}

LPCTSTR CBrowserUI::GetClass() const
{
	return _T("BrowserUI");
}

LPVOID CBrowserUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, _T("BrowserUI")) == 0 ) return static_cast<CBrowserUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

bool CBrowserUI::CreateBrowser(const CefString& sHomepage, CBrowserDlg* pWindow)
{
	CefWindowInfo info;
	if(m_Browser == NULL)
		m_Browser = new CBrowserHandler();
	if(m_Browser != NULL){
		m_Browser->SetParentWindow(pWindow);
		info.SetAsChild(GetManager()->GetPaintWindow(), GetPos());
		CefBrowserSettings browser_settings;
		return CefBrowserHost::CreateBrowser(info, m_Browser.get(), sHomepage, browser_settings, NULL);
	}
	return false;
}

void CBrowserUI::LoadURL(const CefString& url)
{
	if (m_Browser.get())
	{
		CefRefPtr<CefFrame> mainfram = m_Browser->GetMainFrame();
		if (mainfram)
			mainfram->LoadURL(url);
	}
}

void CBrowserUI::CloseAllBrowsers()
{
	if (m_Browser.get())
		m_Browser->CloseAllBrowsers(true);
}

void CBrowserUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	CControlUI::SetPos(rc, bNeedInvalidate);
	if (m_Browser.get())
	{
		CefRefPtr<CefBrowser> browser = m_Browser->GetBrowser();
		DuiLib::CDuiRect rc = GetPos();
		if (browser && !rc.IsNull())
		{
			::SetWindowPos(browser->GetHost()->GetWindowHandle(),
				NULL, rc.left, rc.top, rc.right - rc.left,
				rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}
}

LRESULT CBrowserUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	LRESULT lResult = 0;
	if( m_Browser == NULL ) return 0;
	if( !m_Browser.get() ) return 0;
	bool bWasHandled = true;
	switch( uMsg ) {
	case WM_HELP:
	case WM_CONTEXTMENU:
		bWasHandled = false;
		break;
	default:
		return 0;
	}
	bHandled = bWasHandled;
	return lResult;
}
