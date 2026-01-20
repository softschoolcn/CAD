#pragma once
//改块基点
void BlockBasePt();
//改块名称
void ModifyBlcokName();
//是否有效合理的块名称
BOOL IsValidBlockName(CString strName,CString& strError);
//快速定义块
void QuickBlockDefine();
//属性转文字
void AttrToTxt();
//120:块爆破
void BlastBlock();

/////////////////////////////////////////////////////////////////////////////////////

//将块中的属性转成文字
Acad::ErrorStatus BrefAttrToText(AcDbBlockReference* pRef);
//122:文字块
void TextBlock();
