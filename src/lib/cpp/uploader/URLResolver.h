#pragma once

#include "Uploader.h"

namespace UPLOAD {

/**
 * URL Resolver implementation.
 *
 * @author Alexander Kozlov
 */
template <typename T>
class URLResolver : public IUniversalUploaderURLResolver<T>
{
protected:
    CString m_strURL;

protected:
    URLResolver(LPCTSTR pszURL)
        : m_strURL(pszURL)
    {
        ATLTRACE( _T("URLResolver: %s\n"), m_strURL );
    }

    ~URLResolver()
    {
        ATLTRACE( _T("URLResolver::~URLResolver()\n") );
    }

public:
    static SmartReleasePtr<IUniversalUploaderURLResolver<T> > NewInstance(LPCTSTR pszURL)
    {
        return SmartReleasePtr<IUniversalUploaderURLResolver<T> >(new URLResolver(pszURL));
    }

public:
    /**
     * Must return URL for POST data request
     */
    virtual CString GetURL(const T &item)
    {
        return m_strURL;
    }

	/**
	 * Called to destroy
	 */
	virtual void Release()
    {
        delete this;
    }
};

} // namespace UPLOAD
