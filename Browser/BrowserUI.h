#ifndef __BROWSERUI_H__
#define __BROWSERUI_H__
#pragma once
#include <list>
#include <set>
#undef GetFirstChild
#undef GetNextSibling
#include "include/cef_client.h"
#include "include/cef_app.h"
#include "include/base/cef_bind.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_v8.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_message_router.h"

class CBrowserHandler
	: public CefClient				// ������ͻ���
	, public CefContextMenuHandler	// ����������Ĳ˵�����
	, public CefDisplayHandler		// �������ʾ����
	, public CefDownloadHandler		// ��������ش���
	, public CefGeolocationHandler	// ���������λ����
	, public CefJSDialogHandler		// �����JS�Ի�����
	, public CefKeyboardHandler		// ��������̴���
	, public CefLifeSpanHandler		// ������������ڴ���
	, public CefLoadHandler			// ��������ش���
	, public CefRequestHandler		// �����������
	, public CefRequestContextHandler
{

public:
	// Interface for process message delegates. Do not perform work in the
	// RenderDelegate constructor.
	class ProcessMessageDelegate : public virtual CefBase
	{
	public:
		// Called when a process message is received. Return true if the message was
		// handled and should not be passed on to other handlers.
		// ProcessMessageDelegates should check for unique message names to avoid
		// interfering with each other.
		virtual bool OnProcessMessageReceived(
			CefRefPtr<CBrowserHandler> handler,
			CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message)
		{
			return false;
		}
	};
	typedef std::set<CefRefPtr<ProcessMessageDelegate> > ProcessMessageDelegateSet;

	// Interface for request handler delegates. Do not perform work in the
	// RequestDelegate constructor.
	class RequestDelegate : public virtual CefBase
	{
	public:
		// Called to retrieve a resource handler.
		virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
			CefRefPtr<CBrowserHandler> handler,
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request)
		{
			return NULL;
		}
	};
	typedef std::set<CefRefPtr<RequestDelegate> > RequestDelegateSet;

	typedef std::set<CefMessageRouterBrowserSide::Handler*> MessageHandlerSet;

	enum MENU_LIST
	{
		CLIENT_ID_REFRESH = MENU_ID_USER_FIRST,
		CLIENT_ID_SAMPLE,
	};

public:
	CBrowserHandler();
	~CBrowserHandler();

	//////////////////////////////////////////////////////////////////////////
	// CefClient methods
	virtual CefRefPtr<CefContextMenuHandler>	GetContextMenuHandler()	OVERRIDE { return this; }
	virtual CefRefPtr<CefDisplayHandler>		GetDisplayHandler()		OVERRIDE { return this; }
	virtual CefRefPtr<CefDownloadHandler>		GetDownloadHandler()	OVERRIDE { return this; }
	//	virtual CefRefPtr<CefFocusHandler>			GetFocusHandler()		OVERRIDE { return this; }
	//	virtual CefRefPtr<CefGeolocationHandler>	GetGeolocationHandler()	OVERRIDE { return this; }
	virtual CefRefPtr<CefJSDialogHandler>		GetJSDialogHandler()	OVERRIDE { return this; }
	//	virtual CefRefPtr<CefKeyboardHandler>		GetKeyboardHandler()	OVERRIDE { return this; }
	virtual CefRefPtr<CefLifeSpanHandler>		GetLifeSpanHandler()	OVERRIDE { return this; }
	virtual CefRefPtr<CefLoadHandler>			GetLoadHandler()		OVERRIDE { return this; }
	virtual CefRefPtr<CefRequestHandler>		GetRequestHandler()		OVERRIDE { return this; }

	virtual bool OnProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message)
		OVERRIDE;

	//////////////////////////////////////////////////////////////////////////
	// CefContextMenuHandler methods
	virtual void OnBeforeContextMenu(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		CefRefPtr<CefMenuModel> model) OVERRIDE;

	virtual bool OnContextMenuCommand(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		int command_id,
		EventFlags event_flags) OVERRIDE;

	//////////////////////////////////////////////////////////////////////////
	// CefDisplayHandler methods
	virtual void OnLoadingStateChange(
		CefRefPtr<CefBrowser> browser,
		bool isLoading,
		bool canGoBack,
		bool canGoForward) OVERRIDE;

	virtual void OnAddressChange(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& url) OVERRIDE;

	virtual void OnTitleChange(
		CefRefPtr<CefBrowser> browser,
		const CefString& title) OVERRIDE;

	virtual bool OnTooltip(
		CefRefPtr<CefBrowser> browser,
		CefString& text) OVERRIDE;

	virtual void OnStatusMessage(
		CefRefPtr<CefBrowser> browser,
		const CefString& value) OVERRIDE;

	virtual bool OnConsoleMessage(
		CefRefPtr<CefBrowser> browser,
		const CefString& message,
		const CefString& source,
		int line) OVERRIDE;


	//////////////////////////////////////////////////////////////////////////
	// CefDownloadHandler methods
	virtual void OnBeforeDownload(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDownloadItem> download_item,
		const CefString& suggested_name,
		CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;

	virtual void OnDownloadUpdated(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefDownloadItem> download_item,
		CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

	//////////////////////////////////////////////////////////////////////////
	// CefGeolocationHandler methods
	virtual bool OnRequestGeolocationPermission(
		CefRefPtr<CefBrowser> browser,
		const CefString& requesting_url,
		int request_id,
		CefRefPtr<CefGeolocationCallback> callback) OVERRIDE;

	//////////////////////////////////////////////////////////////////////////
	// CefKeyboardHandler methods
	virtual bool OnPreKeyEvent(
		CefRefPtr<CefBrowser> browser,
		const CefKeyEvent& event,
		CefEventHandle os_event,
		bool* is_keyboard_shortcut) OVERRIDE;

	//////////////////////////////////////////////////////////////////////////
	// CefLifeSpanHandler methods
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		cef_window_open_disposition_t target_disposition,
		bool user_gesture,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access) OVERRIDE;
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	//////////////////////////////////////////////////////////////////////////
	// CefLoadHandler methods
	virtual void OnLoadStart(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame/*,
		TransitionType transition_type*/) OVERRIDE;

	virtual void OnLoadEnd(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int httpStatusCode) OVERRIDE;

	virtual void OnLoadError(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) OVERRIDE;

	virtual void OnRenderProcessTerminated(
		CefRefPtr<CefBrowser> browser,
		TerminationStatus status) OVERRIDE;

	//////////////////////////////////////////////////////////////////////////
	// CefRequestHandler methods
	virtual ReturnValue OnBeforeResourceLoad(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		CefRefPtr<CefRequestCallback> callback) OVERRIDE;

	virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request) OVERRIDE;

	//virtual CefRefPtr<CefCookieManager> GetCookieManager() { return NULL; }
	virtual CefRefPtr<CefCookieManager> GetCookieManager() OVERRIDE {
		if (m_Cookie)
		{
			return m_Cookie;
		}
		m_Cookie = CefCookieManager::CreateManager("D:\\Users\\Downloads\\CefCookie", false, NULL);
		return m_Cookie;
	}

	virtual void OnProtocolExecution(
		CefRefPtr<CefBrowser> browser,
		const CefString& url,
		bool& allow_os_execution) OVERRIDE;

public:
	CefRefPtr<CefBrowser> GetBrowser() { return m_Browser; }
	CefRefPtr<CefFrame> GetMainFrame() { return m_Browser.get()?m_Browser->GetMainFrame():NULL; }
	HWND GetBrowserHandle() { return m_Browser.get()?m_Browser->GetHost()->GetWindowHandle():NULL; }
	void SetParentWindow(CBrowserDlg* pWindow) { m_pWindow = pWindow; }
	int GetBrowserId() { return m_BrowserId; }

	// Request that all existing browser windows close.
	void CloseAllBrowsers(bool force_close);

protected:
	// Create all of ProcessMessageDelegate objects.
	static void CreateProcessMessageDelegates(ProcessMessageDelegateSet& delegates);

	// Create all of RequestDelegateSet objects.
	static void CreateRequestDelegates(RequestDelegateSet& delegates);

	// The child browser window
	CefRefPtr<CefBrowser> m_Browser;
	CBrowserDlg* m_pWindow;

	// The child browser id
	int m_BrowserId;

	// Registered delegates.
	ProcessMessageDelegateSet m_process_message_delegates;
	RequestDelegateSet m_request_delegates;

  // The current number of browsers using this handler.
  int browser_count_;

  // Set of Handlers registered with the message router.
  MessageHandlerSet message_handler_set_;

  CefRefPtr<CefCookieManager> m_Cookie;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CBrowserHandler);
	// Include the default locking implementation.
	IMPLEMENT_LOCKING(CBrowserHandler);
};

class CBrowserV8Handler : public CefV8Handler
{
public:
	CBrowserV8Handler() : sParam("An initial string value.") {}
	virtual ~CBrowserV8Handler() {}

	virtual bool Execute(const CefString& name,
		CefRefPtr<CefV8Value> object,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception);
private:
	CefString sParam;
	IMPLEMENT_REFCOUNTING(CBrowserV8Handler);
};

class CBrowserApp
	: public CefApp
	, public CefBrowserProcessHandler
	, public CefRenderProcessHandler
{
public:
	CBrowserApp(){}

private:
	// CefApp methods.
	virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;
	virtual void OnRegisterCustomSchemes( CefRefPtr<CefSchemeRegistrar> registrar) OVERRIDE;
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE{ return this; }
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE{ return this; }

	// CefBrowserProcessHandler methods.
	virtual void OnContextInitialized() OVERRIDE;
	virtual void OnBeforeChildProcessLaunch(
		CefRefPtr<CefCommandLine> command_line) OVERRIDE;
	virtual void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;

	// CefRenderProcessHandler methods.
	virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;
	virtual void OnWebKitInitialized() OVERRIDE;
	virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;
	virtual bool OnBeforeNavigation(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		NavigationType navigation_type,
		bool is_redirect) OVERRIDE;
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
	virtual void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
	virtual void OnUncaughtException(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context,
		CefRefPtr<CefV8Exception> exception,
		CefRefPtr<CefV8StackTrace> stackTrace) OVERRIDE;
	virtual void OnFocusedNodeChanged(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefDOMNode> node) OVERRIDE;
	virtual bool OnProcessMessageReceived(
		CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) OVERRIDE;

private:
	std::vector<CefString> cookieable_schemes_;

	IMPLEMENT_REFCOUNTING(CBrowserApp);
};


class CBrowserUI : public CControlUI, public IMessageFilterUI
{
public:
	CBrowserUI();
	~CBrowserUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetPos(RECT rc, bool bNeedInvalidate = true);
	bool CreateBrowser(const CefString& sHomepage, CBrowserDlg* pWindow);
	void LoadURL(const CefString& url);
	
	CefRefPtr<CefBrowser> GetBrowser() { return m_Browser->GetBrowser(); }
	CefRefPtr<CefFrame> GetMainFrame() { return m_Browser->GetMainFrame(); }

	void CloseAllBrowsers();
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

protected:
	CefRefPtr<CBrowserHandler> m_Browser;
};

#endif
