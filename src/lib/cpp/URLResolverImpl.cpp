#include "StdAfx.h"
#include "URLResolverImpl.h"

namespace ImageShack {

    using namespace API::ImageShack;

URLResolver::URLResolver()
	: m_image_resolver(IDS_UPLOAD_API_URL, IDS_IMAGESHACK_SERVERS, IDS_IMAGESHACK_SERVERS2, IDS_UPLOAD_SERVER_URL_FORMAT, IMAGESHACK_LOAD_COUNT)
	, m_video_resolver(IDS_RENDER_API_URL, IDS_IMAGESHACK_RENDER_SERVERS, IDS_IMAGESHACK_RENDER_SERVERS_FORMAT, IMAGESHACK_RENDER_COUNT)
{
    ATLTRACE( _T("ImageShack::URLResolver::URLResolver()\n") );
}

URLResolver::~URLResolver()
{
    ATLTRACE( _T("ImageShack::URLResolver::~URLResolver()\n") );
}

CString URLResolver::GetURL(const UploadInfo &item)
{
    CString strFileExt = ExtractFileExtension(item.file);

    if (IsHttpURL(item.file) || item.content_type.Find( _T("image/") ) != -1 || strFileExt == _T("swf"))
    {
        return m_image_resolver.GetURL();
    }
    else // it should be video
    {
        return m_video_resolver.GetURL();
    }
}

}//namespace ImageShack

