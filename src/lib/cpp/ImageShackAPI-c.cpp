#include "StdAfx.h"
#include "ImageShackAPI.h"
using namespace API::ImageShack;

extern "C" IMAGESHACK_API void ImageShackAPI_UploadFileA(LPCSTR pszDevKey, LPCSTR pszSourceFile)
{
    CStringW file(pszSourceFile);

    LPCWSTR files[1];
    files[0] = (LPCWSTR)file;
    ImageShackAPI::UploadFiles(pszDevKey, files, 1);
}

extern "C" IMAGESHACK_API void ImageShackAPI_UploadFileW(LPCSTR pszDevKey, LPCWSTR pszSourceFile)
{
    CStringW file(pszSourceFile);

    LPCWSTR files[1];
    files[0] = (LPCWSTR)file;
    ImageShackAPI::UploadFiles(pszDevKey, files, 1);
}
