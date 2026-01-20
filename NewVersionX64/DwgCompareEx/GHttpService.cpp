/*
* Copyright (c) 2012,上海金曲信息技术有限公司
* All rights reserved.
* 
* 文件名称：GHttpService.cpp
* 摘    要：HTTP服务类
* 完成日期：2012年12月19日
*/
#include "StdAfx.h"
#include "GHttpService.h"
#define MAXBLOCKSIZE 1024
#include <wininet.h>
#pragma comment( lib, "wininet.lib" )
#include <afxinet.h>
namespace GHttpService
{
	//功能:	下载文件
	//参数:	Url,下载的URL地址
	//    :save_as,本地文件路径
	//返回:	下载是否成功
	BOOL Download(const CString Url,const CString save_as,DWORD& dwStatusCode,GDownProcessListener* process_Listener)
	{
		dwStatusCode=HTTP_STATUS_NOT_FOUND;
		CHttpFile			*fTargFile=NULL;
		try
		{
			char				filebuf[MAXBLOCKSIZE]={0};
			CInternetSession	netSession;

			int					outfs=0;
			fTargFile = (CHttpFile*)netSession.OpenURL(Url,1,INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD);
			if(fTargFile==NULL)
			{
				dwStatusCode=HTTP_STATUS_NOT_FOUND;
				return FALSE;
			}
			if(!fTargFile->QueryInfoStatusCode(dwStatusCode))
			{
				dwStatusCode=HTTP_STATUS_NOT_FOUND;
				fTargFile->Close();
				delete fTargFile;
				fTargFile=NULL;
				return FALSE;
			}
			if(dwStatusCode!=HTTP_STATUS_OK)
			{
				fTargFile->Close();
				delete fTargFile;
				fTargFile=NULL;
				return FALSE;
			}
			long nTotal=fTargFile->SeekToEnd();
			fTargFile->SeekToBegin();
			if(process_Listener!=NULL)
			{
				process_Listener->SetTotal(nTotal);
			}
			CFile file;
			int byteswrite;		// 写入文件的字节数
			long pos = 0;		// 当前进度条的位置
			if(!file.Open(save_as,CFile::modeWrite|CFile::modeCreate|CFile::typeBinary))
			{
				fTargFile->Close();
				delete fTargFile;
				fTargFile=NULL;
			}
			while (byteswrite = fTargFile->Read(filebuf, MAXBLOCKSIZE))
			{
				pos = pos + byteswrite;					// 设置新的进度条位置
				if(process_Listener!=NULL)
				{
					process_Listener->SetCur(pos);
				}
				file.Write(filebuf, byteswrite);	// 将实际数据写入文件
			}
			file.Close();
			fTargFile->Close();
			delete fTargFile;
			fTargFile=NULL;
			return TRUE;
		}


		catch(CInternetException *IE)
		{
			if(fTargFile!=NULL)
			{
				fTargFile->Close();
				delete fTargFile;
				fTargFile=NULL;
			}
			IE->Delete();					// 删除异常对象，以防止泄漏
			return FALSE;
		}
		
	}
	//功能:	根据HTTP状态码返回下载失败原因
	//参数:	strReason,返回下载失败原因
	//    :dwStatusCode,HTTP状态码
	//返回:	是否已知的状态码
	BOOL GetErrorReason(CString& strReason,const DWORD dwStatusCode)
	{
		if(dwStatusCode==HTTP_STATUS_OK)
		{
			strReason=_T("服务器成功返回网页");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_NOT_FOUND)
		{
			strReason=_T("请求的网页不存在");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_SERVICE_UNAVAIL)
		{
			strReason=_T("服务不可用");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_CONTINUE)
		{
			strReason=_T("临时响应并需要请求者继续执行操作的状态代码");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_SWITCH_PROTOCOLS)
		{
			strReason=_T("请求者已要求服务器切换协议，服务器已确认并准备切换");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_CREATED)
		{
			strReason=_T("请求成功并且服务器创建了新的资源");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_ACCEPTED)
		{
			strReason=_T("服务器已接受请求，但尚未处理");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_PARTIAL)
		{
			strReason=_T("服务器已成功处理了请求，但返回的信息可能来自另一来源");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_NO_CONTENT)
		{
			strReason=_T("服务器成功处理了请求，但没有返回任何内容");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_RESET_CONTENT)
		{
			strReason=_T("服务器成功处理了请求，但没有返回任何内容");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_PARTIAL_CONTENT)
		{
			strReason=_T("服务器成功处理了部分 GET 请求");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_BAD_REQUEST)
		{
			strReason=_T("服务器不理解请求的语法");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_DENIED)
		{
			strReason=_T("请求要求身份验证。 对于需要登录的网页，服务器可能返回此响应");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_PAYMENT_REQ)
		{
			//???不知道对不对
			strReason=_T("请求要求支付。 对于需要支付的网页，服务器可能返回此响应");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_FORBIDDEN)
		{
			strReason=_T("服务器拒绝请求");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_BAD_METHOD)
		{
			strReason=_T("禁用请求中指定的方法");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_NONE_ACCEPTABLE)
		{
			strReason=_T("无法使用请求的内容特性响应请求的网页");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_PROXY_AUTH_REQ)
		{
			strReason=_T("与 401（未授权）类似，但指定请求者应当授权使用代理");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_REQUEST_TIMEOUT)
		{
			strReason=_T("服务器等候请求时发生超时");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_CONFLICT)
		{
			strReason=_T("服务器在完成请求时发生冲突。 服务器必须在响应中包含有关冲突的信息");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_GONE)
		{
			strReason=_T("如果请求的资源已永久删除，服务器就会返回此响应");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_LENGTH_REQUIRED)
		{
			strReason=_T("服务器不接受不含有效内容长度标头字段的请求");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_PRECOND_FAILED)
		{
			strReason=_T("服务器未满足请求者在请求中设置的其中一个前提条件");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_REQUEST_TOO_LARGE)
		{
			strReason=_T("服务器无法处理请求，因为请求实体过大，超出服务器的处理能力");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_URI_TOO_LONG)
		{
			strReason=_T("（请求的 URI 过长） 请求的 URI（通常为网址）过长，服务器无法处理");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_UNSUPPORTED_MEDIA)
		{
			strReason=_T("请求的格式不受请求页面的支持");
			return TRUE;
		}

		else if(dwStatusCode==HTTP_STATUS_SERVER_ERROR)
		{
			strReason=_T("服务器遇到错误，无法完成请求");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_NOT_SUPPORTED)
		{
			strReason=_T("服务器不具备完成请求的功能。 例如，服务器无法识别请求方法时可能会返回此代码");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_BAD_GATEWAY)
		{
			strReason=_T("服务器作为网关或代理，从上游服务器收到无效响应");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_SERVICE_UNAVAIL)
		{
			strReason=_T("服务器目前无法使用（由于超载或停机维护）。 通常，这只是暂时状态");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_GATEWAY_TIMEOUT)
		{
			strReason=_T("服务器作为网关或代理，但是没有及时从上游服务器收到请求");
			return TRUE;
		}
		else if(dwStatusCode==HTTP_STATUS_VERSION_NOT_SUP)
		{
			strReason=_T("服务器不支持请求中所用的 HTTP 协议版本");
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

