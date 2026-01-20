/*
* Copyright (c) 2012,上海金曲信息技术有限公司
* All rights reserved.
* 
* 文件名称：GHttpService.h
* 摘    要：HTTP服务类
* 完成日期：2012年12月19日
*/
#pragma once
namespace GHttpService
{
	struct GDownProcessListener
	{
		virtual void SetTotal(long nTotal)=0;
		virtual void SetCur(long nCur)=0;
	};
	//功能:	下载文件
	//参数:	Url,下载的URL地址
	//    :save_as,本地文件路径
	//返回:	下载是否成功
	BOOL Download(const CString Url,const CString save_as,DWORD& dwStatusCode,GDownProcessListener* process_Listener=NULL);
	//功能:	根据HTTP状态码返回下载失败原因
	//参数:	strReason,返回下载失败原因
	//    :dwStatusCode,HTTP状态码
	//返回:	是否已知的状态码
	BOOL GetErrorReason(CString& strReason,const DWORD dwStatusCode);
};
