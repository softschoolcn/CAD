#pragma once
//高亮辅助类
//注意:高亮必须和取消高亮成对调用
//因此设计此高亮辅助类
//类析构的时候会自动取消所有高亮
class GHighlightHelper
{
public:
	GHighlightHelper(void);
public:
	~GHighlightHelper(void);
public:
	Acad::ErrorStatus Highlight(AcDbObjectId objId);
	Acad::ErrorStatus UnHighlight(AcDbObjectId objId);
	Acad::ErrorStatus Highlight(AcDbObjectIdArray& ids);
	Acad::ErrorStatus UnHighlight(AcDbObjectIdArray& ids);
	bool IsHighlight(AcDbObjectId objId);
private:
	//已经被高亮的实体
	AcDbObjectIdArray highlightIds;
	Acad::ErrorStatus _Highlight(AcDbObjectId objId,bool update);
	Acad::ErrorStatus _UnHighlight(AcDbObjectId objId,bool update);
};
