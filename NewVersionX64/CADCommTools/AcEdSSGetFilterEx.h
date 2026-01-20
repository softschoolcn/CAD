#pragma once

class AcEdSSGetFilterEx:public AcEdSSGetFilter
{
	virtual void
    ssgetAddFilter(
	    int ssgetFlags,
	    AcEdSelectionSetService &service,
	    const AcDbObjectIdArray& selectionSet,
	    const AcDbObjectIdArray& subSelectionSet);
	virtual void
    endSSGet(
	    Acad::PromptStatus returnStatus,
	    int ssgetFlags,
	    AcEdSelectionSetService &service,
	    const AcDbObjectIdArray& selectionSet);
    virtual void
    endEntsel(
        Acad::PromptStatus       returnStatus,
        const AcDbObjectId&      pickedEntity,
        const AcGePoint3d&       pickedPoint,
        AcEdSelectionSetService& service);
	//是否可以选中
	virtual bool CanBeSelect(AcDbObjectId id);
	//是否可以选中
	virtual bool CanBeSelect(AcDbEntity* pEnt);
};

class GLayerSSGetFilter:public AcEdSSGetFilterEx
{
public:
	GLayerSSGetFilter(){m_pClass=NULL;}
	virtual ~GLayerSSGetFilter(){}
	void Reset();
	AcDbObjectIdArray m_allowLayerArr;//允许的图层列表
	AcDbObjectIdArray m_notAllowLayerArr;//不允许的图层列表
	AcRxClass* m_pClass;
	//是否可以选中
	virtual bool CanBeSelect(AcDbEntity* pEnt);
};
