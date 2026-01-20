#pragma once
//文字统计
void TextStatistics();
struct CTextStatisticsResultItem
{
	CString strText;//文字内容
	long nNum;//文字数量
	CTextStatisticsResultItem()
	{
		nNum=0;
		strText=_T("");
	}
};
struct CTextStatisticsResult
{
	CArray<CTextStatisticsResultItem> m_ResultItems;
	void Add(const CString strText,const long nNum=1);
	void Add(const CTextStatisticsResultItem item);
	void Sort();
	CTextStatisticsResult(){};
	CTextStatisticsResult(const CTextStatisticsResult& result);
	CTextStatisticsResult& operator=(const CTextStatisticsResult& result);
};

