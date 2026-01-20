#pragma once
//单行文本转换为多行文本
void TxtToMText();
//多行文字转换为单行文字
void MTextToTxt();
//文字边框
void TextBorder();
//文字重叠
void TextCross();
//文字转线
void TextToPline();
//文字递增复制
void TextBatchCopy();
Adesk::Boolean createBatchCopyDialog(AcDbObjectId id,CString strLeft,CString strMid,CString strRight);
void showDlgBatchCopy(AcDbObjectId id,CString strLeft,CString strMid,CString strRight);
void destroyDlgBatchCopy();
//文字刷
void TextBrush();
//属性转文字
void AttrToTxt();
//批量修改
void BatchModify();
//连续修改
void ContinueModify();
//日期时间
void AssDateTime();
//清除格式
void ClearFormat();
//89:拷贝文字内容
void CopyText();
