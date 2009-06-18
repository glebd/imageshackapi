//////////////////////////////////////////////////////////////////////////
// ImageShackAPI.h - Declaration of ImageShackAPI classes
//     
// Requires:
//   ImageShackBase.h
//   uploader/Uploader.h
//   uploader/templates.h
//   uploader/stubs.h
//   uploader/safe.h
//   templates/SmartPtr.h
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlstr.h>
#include <atlcoll.h>
#include "ImageShackBase.h"

#include "uploader/Uploader.h"
#include "uploader/templates.h"

namespace API {
namespace ImageShack {

/**
 * ErrorResponse
 */
class IErrorResponse
{
public:
    virtual LPCWSTR GetCode() const = 0;
    virtual void SetCode(LPCWSTR pszCode) = 0;
    virtual LPCWSTR GetMessage() const = 0;
    virtual void SetMessage(LPCWSTR pszMessage) = 0;
};
// implementation
struct ErrorResponse : public IErrorResponse
{
	CStringW code;
	CStringW message;

    ErrorResponse() {}
    ErrorResponse(LPCWSTR pszMessage) : message(pszMessage) {}

    LPCWSTR GetCode() const { return code; }
    void SetCode(LPCWSTR pszCode) { code = pszCode; }
    LPCWSTR GetMessage() const { return message; }
    void SetMessage(LPCWSTR pszMessage) { message = pszMessage; }

    operator const IErrorResponse* () const
    {
        return this;
    }

    operator IErrorResponse* ()
    {
        return this;
    }
};

/**
 * Logged in User Info
 */
class IUserInfo
{
public:
    virtual LPCWSTR GetId() const = 0;
    virtual LPCWSTR GetName() const = 0;
    virtual LPCWSTR GetCookie() const = 0;
};

/**
 * Upload Info
 */
struct UploadInfo
{
	CStringW file; // it should be full path to file or HTTP URL
    CStringW content_type;
	CStringW thumbnail;
    CStringW size;
    bool    remove_bar;
    bool    is_public;
    CStringW tags;
    
    UploadInfo()
        : remove_bar(false)
        , is_public(false)
    {
    }

    UploadInfo(LPCWSTR pszFile)
        : file(pszFile)
        , remove_bar(false)
        , is_public(false)
    {
    }

    void SetSize(int width, int height)
    {
        size.Format(L"%ix%i", width, height );
    }

    operator CString () const
    {
        return (CString)GetFileName();
    }

    CStringW GetFileName() const
    {
        int idx = file.ReverseFind(L'\\');
        if (-1 == idx) idx = file.ReverseFind(L'/');
        return idx == -1 ? file : file.Mid(idx+1);
    }

    // fields for internal use (any value will be reset)
	CStringW devkey;
	CStringW cookie;
	CStringW username;
	CStringW password;
};

/**
 * Upload Result
 */
struct UploadResult
{
    struct FileInfo
    {
        ULONGLONG size;
        CStringW content_type;
        CStringW file;

        FileInfo() : size(0) {}
    };

    struct Files
    {
        CStringW server;
        CStringW bucket;

        FileInfo image;
        FileInfo thumbnail;
    };

    struct Resolution
    {
        int width;
        int height;

        Resolution() : width(0), height(0) {}
    };

    struct VideoInfo
    {
        CStringW status;
        int duration;

        VideoInfo() : duration(0) {}
    };

    struct Links
    {
        CStringW image_link;
        CStringW image_html;
        CStringW image_bb;
        CStringW image_bb2;
        CStringW yfrog_link;
        CStringW yfrog_thumb;
        CStringW thumb_link;
        CStringW thumb_html;
        CStringW thumb_bb;
        CStringW thumb_bb2;
        CStringW ad_link;
        CStringW video_embed;
        CStringW done_page;
    };

    Files files;
    Resolution resolution;
    VideoInfo video_info;
    CStringW visibility;
    Links links;
};

// Adopted external interfaces
typedef UPLOAD::IUniversalUploaderListener<UploadInfo, UploadResult>        UploaderListener;
typedef SmartReleasePtr<UploaderListener>                                   UploaderListenerSmartPtr;
typedef UPLOAD::IUniversalUploaderProgressListener                          ProgressListener;
typedef SmartReleasePtr<ProgressListener>                                   ProgressListenerSmartPtr;
typedef UPLOAD::IUniversalUploader<UploadInfo, UploadResult>                Uploader;
typedef SmartReleasePtr<Uploader>                                           UploaderSmartPtr;
typedef UPLOAD::IUniversalUploaderCancel                                    UploaderCancel;
typedef UPLOAD::UniversalUploaderException                                  UploaderException;
typedef UPLOAD::UniversalUploaderErrorAction                                UploaderErrorAction;
typedef UPLOAD::UniversalUploaderErrorInfo                                  UploaderErrorInfo;
typedef UPLOAD::TLIB::UniversalUploaderListenerComposite<UploadInfo, UploadResult>  UploaderListenerComposite;

// forward declaration
struct ImageShackAPIPrivate;

/**
 * ImageShack API.
 *
 * Remark: API use COM and it requires 'CoInitialize'd calling thread.
 *
 * @author Alexander Kozlov
 */
class IMAGESHACK_API ImageShackAPI
{
public:
    ImageShackAPI(const char* pszDevKey);
    ImageShackAPI(const ImageShackAPI& );
    ~ImageShackAPI();

	// Check login/password. After successful login use GetUserInfo() to get user information.
	bool Login(LPCWSTR pszUserName, LPCWSTR pszPassword, IErrorResponse* pError = NULL);
    // Open Login dialog.
	bool Login();

	// check whether user is logged in
	bool IsLoggedIn() const;

    // return id and cookie of the logged in user
    IUserInfo* GetUserInfo() const; 

    // reset username and cookie
    void Logout();

    // set cookie without login
    void SetCookie(LPCWSTR pszCookie);

    // set user/password without login
    void SetAuthInfo(LPCWSTR pszUserName, LPCWSTR pszPassword);

    // set plug-in name and version
    void SetPluginInfo(LPCWSTR pszPluginName, LPCWSTR pszPluginVersion);

    // show preview and start upload with logged in user (or show login dialog)
    void UploadFiles(const UploadInfo* pFiles, UINT nCount, const UploaderListenerSmartPtr &uploaderListener = UploaderListenerSmartPtr(), const ProgressListenerSmartPtr &progressListener = ProgressListenerSmartPtr());

    void UploadFiles(const CSimpleArray<UploadInfo> &arFiles, const UploaderListenerSmartPtr &uploaderListener = UploaderListenerSmartPtr(), const ProgressListenerSmartPtr &progressListener = ProgressListenerSmartPtr())
    {
        UploadFiles(arFiles.GetData(), arFiles.GetSize(), uploaderListener, progressListener);
    }

	static void UploadFiles(const char* pszDevKey, LPCWSTR* pFiles, UINT nCount, const UploaderListenerSmartPtr &uploaderListener = UploaderListenerSmartPtr(), const ProgressListenerSmartPtr &progressListener = ProgressListenerSmartPtr());

    // Open My Images Pages
    void OpenMyImagesPage();
    // Open My Videos Pages
    void OpenMyVideosPage();

private:
    ImageShackAPIPrivate* m_pPrivate;

    ImageShackAPI& operator = (const ImageShackAPI& );
};

} // namespace ImageShack
} // namespace API

// declare mapping for builder
HTTP::MultipartFormDataRequest& operator << (HTTP::MultipartFormDataRequest&, const API::ImageShack::UploadInfo &);
// declare mapping for parser
bool operator >> (const CStringA &, API::ImageShack::UploadResult &);
bool operator >> (const CStringA &, UPLOAD::UniversalUploaderErrorInfo &);
