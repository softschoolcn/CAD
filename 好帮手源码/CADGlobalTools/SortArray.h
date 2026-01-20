#pragma once
#include <afxtempl.h>
template<class TYPE, class ARG_TYPE,class TYPE_VALUE, class ARG_TYPE_VALUE>
//desc=true:降序排列
//desc=false:升序排列
void SortArray(CArray<TYPE,ARG_TYPE>& sourceArray,CArray<TYPE_VALUE,ARG_TYPE_VALUE>& valueArray,const bool desc=true)
{
	ASSERT(sourceArray.GetCount()==valueArray.GetCount());
	//排序增大方向(距离从小到大)
	for(long i=0;i<valueArray.GetCount()-1;i++)
	{
		TYPE_VALUE one_value=valueArray.GetAt(i);
		for(long j=i+1;j<valueArray.GetCount();j++)
		{
			TYPE_VALUE another_value=valueArray.GetAt(j);
			if((desc&&one_value<another_value)||//降序排列
				((!desc)&&one_value>another_value))//升序排列
			{
				TYPE swap=sourceArray.GetAt(i);
				sourceArray.SetAt(i,sourceArray.GetAt(j));
				sourceArray.SetAt(j,swap);

				TYPE_VALUE value_swap=valueArray.GetAt(i);
				valueArray.SetAt(i,valueArray.GetAt(j));
				valueArray.SetAt(j,value_swap);
				one_value=valueArray.GetAt(i);


			}
		}
	}
}
//desc=true:降序排列
//desc=false:升序排列
template<class TYPE, class ARG_TYPE,class TYPE1, class ARG_TYPE1,class TYPE_VALUE, class ARG_TYPE_VALUE>
void SortArray(CArray<TYPE,ARG_TYPE>& sourceArray,CArray<TYPE1,ARG_TYPE1>& sourceArray1,CArray<TYPE_VALUE,ARG_TYPE_VALUE>& valueArray,const bool desc=true)
{
	ASSERT(sourceArray.GetCount()==valueArray.GetCount());
	ASSERT(sourceArray1.GetCount()==valueArray.GetCount());
	//排序增大方向(距离从小到大)
	for(long i=0;i<valueArray.GetCount()-1;i++)
	{
		TYPE_VALUE one_value=valueArray.GetAt(i);
		for(long j=i+1;j<valueArray.GetCount();j++)
		{
			TYPE_VALUE another_value=valueArray.GetAt(j);
			if((desc&&one_value<another_value)||//降序排列
				((!desc)&&one_value>another_value))//升序排列
			{
				TYPE swap=sourceArray.GetAt(i);
				sourceArray.SetAt(i,sourceArray.GetAt(j));
				sourceArray.SetAt(j,swap);

				TYPE1 swap1=sourceArray1.GetAt(i);
				sourceArray1.SetAt(i,sourceArray1.GetAt(j));
				sourceArray1.SetAt(j,swap1);

				TYPE_VALUE value_swap=valueArray.GetAt(i);
				valueArray.SetAt(i,valueArray.GetAt(j));
				valueArray.SetAt(j,value_swap);
				one_value=valueArray.GetAt(i);

			}
		}
	}
}
