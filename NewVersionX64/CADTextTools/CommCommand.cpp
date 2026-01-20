#include "Stdafx.h"
#include "CommCommand.h"
#include "GTextEditorDialog.h"
#include "GFindDialog.h"
#include "GTextTools.h"
//批量修改
void BatchModify()
{
	if(!CheckReg())
	{
		return;
	}
	resbuf* rb = acutBuildList(RTDXF0, _T("TEXT,MTEXT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择文字:"),_T("\n移除文字:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	AcDbObjectIdArray ids;
	GCADUtils::SSToIds(ids,ssName);
	acedSSFree(ssName);
	if(ids.logicalLength()==0)
		return;
	CAcModuleResourceOverride res;
	GTextEditorDialog dlg(ids,CWnd::FromHandle(adsw_acadMainWnd()));
	GCADUtils::EnumTextStyles(dlg.mTextStyleIds,dlg.mTextStyleNames);
	dlg.DoModal();
}
//92:查找/替换
void FindEx()
{
	if(!CheckReg())
	{
		return;
	}
	CAcModuleResourceOverride res;
	GFindDialog dlg(CWnd::FromHandle(adsw_acadMainWnd()));
	dlg.DoModal();

}
//文字上下划线
//bAdd=true,增加
//bAdd=false,删除
//bUnder=true,下划线
//bUnder=false,上划线
void _TextUnderUpperline(bool bAdd,bool bUnder)
{
	resbuf* rb = acutBuildList(RTDXF0, _T("TEXT,MTEXT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择文字:"),_T("\n移除文字:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	AcDbObjectIdArray ids;
	GCADUtils::SSToIds(ids,ssName);
	acedSSFree(ssName);
	if(ids.logicalLength()==0)
		return;
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbObjectId id=ids.at(i);
		AcDbEntityPointer spEnt(id,AcDb::kForWrite);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if((!spEnt->isKindOf(AcDbText::desc()))&&(!spEnt->isKindOf(AcDbMText::desc())))
		{
			continue;
		}
		CString strText=GCADUtils::GetText(spEnt.object());
		if(spEnt->isKindOf(AcDbText::desc()))
		{
			if(bUnder)
			{
				if(bAdd)
				{
					GTextTools::AddTextUnderline(strText);
				}
				else
				{
					GTextTools::RemoveTextUnderline(strText);
				}
			}
			else
			{
				if(bAdd)
				{
					GTextTools::AddTextUpperline(strText);
				}
				else
				{
					GTextTools::RemoveTextUpperline(strText);
				}
			}
			AcDbText* pText=AcDbText::cast(spEnt.object());
			pText->setTextString(strText);
		}
		else
		{
			if(bUnder)
			{
				if(bAdd)
				{
					GTextTools::AddMTextUnderline(strText);
				}
				else
				{
					GTextTools::RemoveMTextUnderline(strText);
				}
			}
			else
			{
				if(bAdd)
				{
					GTextTools::AddMTextUpperline(strText);
				}
				else
				{
					GTextTools::RemoveMTextUpperline(strText);
				}
			}
			AcDbMText* pMText=AcDbMText::cast(spEnt.object());
			pMText->setContents(strText);
		}
	}
}
//95:下划线
void TextUnderline()
{
	_TextUnderUpperline(true,true);
}
//96:删除下划线
void RemoveTextUnderline()
{
	_TextUnderUpperline(false,true);
}
//97:上划线
void TextUpperline()
{
	_TextUnderUpperline(true,false);
}
//98:删除上划线
void RemoveTextUpperline()
{
	_TextUnderUpperline(false,false);
}
void _LowerText(CString& strText)
{
	strText.MakeLower();
}
void _UpperText(CString& strText)
{
	strText.MakeUpper();
}
void _UpperLowerText(TCHAR& c,int nMode)
{
	if(nMode==0)
	{
		//大写
		if(c>=_T('a')&&c<=_T('z'))
		{
			c=_T('A')-_T('a')+c;
		}
	}
	else
	{
		//小写
		if(c>=_T('A')&&c<=_T('Z'))
		{
			c=_T('a')-_T('A')+c;
		}
	}
}
void _UpperLowerText(CString& strText,int nMode)
{
	int nStartIndex=0;
	for(int i=0;i<strText.GetLength();i++)
	{
		TCHAR c=strText.GetAt(i);
		if(c==_T('%'))
		{
			if(i+1<strText.GetLength())
			{
				TCHAR c1=strText.GetAt(i+1);
				if(c1==_T('%'))
				{
					if(i+2<strText.GetLength())
					{
						TCHAR c2=strText.GetAt(i+2);
						if(c2==_T('o')||
							c2==_T('O')||
							c2==_T('u')||
							c2==_T('U')||
							c2==_T('d')||
							c2==_T('D')||
							c2==_T('p')||
							c2==_T('P')||
							c2==_T('c')||
							c2==_T('C')||
							c2==_T('%'))
						{
							//特殊符号
						}
						else
						{
							_UpperLowerText(c2,nMode);
							strText.SetAt(i+2,c2);
						}
						i=i+2;
					}
					else
					{
						//已经是最后一个了
						break;
					}
				}
				else
				{
					_UpperLowerText(c1,nMode);
			        strText.SetAt(i+1,c1);
					i=i+1;
				}
			}
			else
			{
				//最后一个是%号忽略
			}
		}
		else
		{
			_UpperLowerText(c,nMode);
			strText.SetAt(i,c);
		}
	}
}
void UpperLowerText(int nMode)
{
	resbuf* rb = acutBuildList(RTDXF0, _T("TEXT,MTEXT"), 0);
	ads_name ssName;
	TCHAR* promptPtrs[2]={_T("\n选择文字:"),_T("\n移除文字:")};
	int rc=acedSSGet(_T(":$"), promptPtrs,NULL,rb,ssName);
	if(rc!=RTNORM)
	{
		acutRelRb(rb);
		return;
	}
	acutRelRb(rb);
	AcDbObjectIdArray ids;
	GCADUtils::SSToIds(ids,ssName);
	acedSSFree(ssName);
	if(ids.logicalLength()==0)
		return;
	for(int i=0;i<ids.logicalLength();i++)
	{
		AcDbObjectId id=ids.at(i);
		AcDbEntityPointer spEnt(id,AcDb::kForWrite);
		if(spEnt.openStatus()!=Acad::eOk)
		{
			continue;
		}
		if((!spEnt->isKindOf(AcDbText::desc()))&&(!spEnt->isKindOf(AcDbMText::desc())))
		{
			continue;
		}
		CString strText=GCADUtils::GetText(spEnt.object());
		if(spEnt->isKindOf(AcDbText::desc()))
		{
			_UpperLowerText(strText,nMode);
			AcDbText::cast(spEnt.object())->setTextString(strText);
		}
		else
		{
			if(nMode==0)
			{
				GTextTools::EnumMText(strText,_UpperText);
			}
			else if(nMode==1)
			{
				GTextTools::EnumMText(strText,_LowerText);
			}
			AcDbMText* pMText=AcDbMText::cast(spEnt.object());
			pMText->setContents(strText);
		}
	}
}
//101:文字转换为大写
void UpperText()
{
	UpperLowerText(0);
}
//102:文字转换为小写
void LowerText()
{
	UpperLowerText(1);
}