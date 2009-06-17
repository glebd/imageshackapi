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
	CString code;
	CString message;

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
class IUploadInfo
{
public:
	virtual LPCWSTR GetFile() const = 0;
    virtual void SetFile(LPCWSTR pszValue) = 0;
	virtual LPCWSTR GetContentType() const = 0;
    virtual void SetContentType(LPCWSTR pszValue) = 0;
	virtual LPCWSTR GetThumbnail() const = 0;
    virtual void SetThumbnail(LPCWSTR pszValue) = 0;
    virtual LPCWSTR GetSize() const = 0;
    virtual void SetSize(LPCWSTR pszValue) = 0;
    virtual bool GetRemoveBar() const = 0;
    virtual void SetRemoveBar(bool bValue) = 0;
    virtual bool IsPublic() const = 0;
    virtual void SetPublic(bool bValue) = 0;
	virtual LPCWSTR GetTags() const = 0;
    virtual void SetTags(LPCWSTR pszValue) = 0;
};
// implementation
struct UploadInfo : public IUploadInfo
{
	CString file;
    CString content_type;
	CString thumbnail;
    CString size;
    bool    remove_bar;
    bool    is_public;
    CString tags;
    
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

    UploadInfo(const IUploadInfo* pInfo)
    {
        ATLASSERT(pInfo);
        file = pInfo->GetFile();
        content_type = pInfo->GetContentType();
        thumbnail = pInfo->GetThumbnail();
        size = pInfo->GetSize();
        remove_bar = pInfo->GetRemoveBar();
        is_public = pInfo->IsPublic();
        tags = pInfo->GetTags();
    }

	LPCWSTR GetFile() const { return file; }
    void SetFile(LPCWSTR pszValue) { file = pszValue; }
	LPCWSTR GetContentType() const { return content_type; }
    void SetContentType(LPCWSTR pszValue) { content_type = pszValue; }
	LPCWSTR GetThumbnail() const  { return thumbnail; }
    void SetThumbnail(LPCWSTR pszValue)  { thumbnail = pszValue; }
    LPCWSTR GetSize() const { return size; }
    void SetSize(LPCWSTR pszValue) { size = pszValue; }
    bool GetRemoveBar() const { return remove_bar; }
    void SetRemoveBar(bool bValue) { remove_bar = bValue; }
    bool IsPublic() const  { return is_public; }
    void SetPublic(bool bValue)  { is_public = bValue; }
	LPCWSTR GetTags() const  { return tags; }
    void SetTags(LPCWSTR pszValue) { tags = pszValue; }

    void SetSize(int width, int height)
    {
        size.Format( _T("%ix%i"), width, height );
    }

    operator const IUploadInfo* () const
    {
        return this;
    }

    operator IUploadInfo* ()
    {
        return this;
    }

    operator CString () const
    {
        return GetFileName();
    }

    CString GetFileName() const
    {
        int idx = file.ReverseFind('\\');
        if (-1 == idx) idx = file.ReverseFind('/');
        return idx == -1 ? file : file.Mid(idx+1);
    }

    // fields for internal use (any value will be reset)
	CString devkey;
	CString cookie;
	CString username;
	CString password;
};

/**
 * Upload Result
 */
struct UploadResult
{
    struct FileInfo
    {
        ULONGLONG size;
        CString content_type;
        CString file;

        FileInfo() : size(0) {}
    };

    struct Files
    {
        CString server;
        CString bucket;

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
        CString status;
        int duration;

        VideoInfo() : duration(0) {}
    };

    struct Links
    {
        CString image_link;
        CString image_html;
        CString image_bb;
        CString image_bb2;
        CString yfrog_link;
        CString yfrog_thumb;
        CString thumb_link;
        CString thumb_html;
        CString thumb_bb;
        CString thumb_bb2;
        CString ad_link;
        CString video_embed;
        CString done_page;
    };

    Files files;
    Resolution resolution;
    VideoInfo video_info;
    CString visibility;
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

	// Check login/password
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
    void UploadFiles(const IUploadInfo** pFiles, UINT nCount, const UploaderListenerSmartPtr &uploaderListener = UploaderListenerSmartPtr(), const ProgressListenerSmartPtr &progressListener = ProgressListenerSmartPtr());

    template <typename T>
    void UploadFiles(const CSimpleArray<T> &arFiles, const UploaderListenerSmartPtr &uploaderListener = UploaderListenerSmartPtr(), const ProgressListenerSmartPtr &progressListener = ProgressListenerSmartPtr())
    {
        CSimpleArray<const IUploadInfo*> arItems;
        for (int i = 0; i < arFiles.GetSize(); ++i)
        {
            arItems.Add((const IUploadInfo*)arFiles[i]);
        }
        UploadFiles(arItems.GetData(), arItems.GetSize(), uploaderListener, progressListener);
    }

	static void UploadFiles(const char* pszDevKey, LPCWSTR* pFiles, UINT nCount, const UploaderListenerSmartPtr &uploaderListener = UploaderListenerSmartPtr(), const ProgressListenerSmartPtr &progressListener = ProgressListenerSmartPtr());

    /**
     * Open My Images Page
     */
    void OpenMyImagesPage();

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
