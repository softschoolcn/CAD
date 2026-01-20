#include "StdAfx.h"
#include "resource.h"
#include "CmpReportDialog.h"
#include "CommCommand.h"
#include "GLockDocument.h"
COLORREF ColorNew = RGB(253,0,0);
COLORREF ColorDelete =RGB(254,0,0);
COLORREF ColorModify = RGB(255,0,0);
COLORREF ColorSame = RGB(0,0,0);//黑色
struct IndexPair
{
	int l;
	int r;
	IndexPair()
	{
		l=0;
		r=0;
	}
};
void RichEditCtrlSelect(CRichEditCtrl* pCtrl,int start, int length)
{
	if(length-1<0)
	{
		return;
	}
	pCtrl->SetSel(start,start+length);
}
void RichEditCtrlChangeTextColor(CRichEditCtrl* pCtrl,COLORREF& col)
{
	if(col==ColorDelete)
	{
		CHARFORMAT2 cf;
		pCtrl->GetSelectionCharFormat(cf);
		cf.crTextColor=col;
		cf.dwEffects&=~CFE_AUTOCOLOR;
		cf.dwMask|=CFM_COLOR;
		cf.dwEffects|=CFE_STRIKEOUT;
		pCtrl->SetSelectionCharFormat(cf);
	}
	else if(col==ColorModify)
	{
		CHARFORMAT2 cf;
		pCtrl->GetSelectionCharFormat(cf);
		cf.crTextColor=col;
		cf.dwEffects&=~CFE_AUTOCOLOR;
		cf.dwMask|=CFM_COLOR;
		cf.dwEffects&=~CFE_STRIKEOUT;
		pCtrl->SetSelectionCharFormat(cf);
	}
	else if(col==ColorNew)
	{
		CHARFORMAT2 cf;
		pCtrl->GetSelectionCharFormat(cf);
		cf.crTextColor=RGB(0,0,255);
		cf.dwEffects&=~CFE_AUTOCOLOR;
		cf.dwMask|=CFM_COLOR;
		cf.dwEffects&=~CFE_STRIKEOUT;
		pCtrl->SetSelectionCharFormat(cf);
	}
	else if(col==ColorSame)
	{
		CHARFORMAT2 cf;
		pCtrl->GetSelectionCharFormat(cf);
		cf.crTextColor=RGB(0,0,0);
		cf.dwEffects&=~CFE_AUTOCOLOR;
		cf.dwMask|=CFM_COLOR;
		cf.dwEffects&=~CFE_STRIKEOUT;
		pCtrl->SetSelectionCharFormat(cf);
	}
	
}
void CmpReportDialog::CompareText()
{
int nSelect = m_ListLeft.GetSelectionMark();
	if(nSelect < 0) 
		return;
	CmpDiffResult* pRC=(CmpDiffResult*)m_ListLeft.GetItemData(nSelect);
	nSelect = m_ListRight.GetSelectionMark();
	if(nSelect < 0) 
		return;
	CmpDiffItem* pDiffItem=(CmpDiffItem*)m_ListRight.GetItemData(nSelect);
	if(!pDiffItem->m_bOnlyOne)
	{
		m_ListSub.ShowWindow(SW_NORMAL);
		m_ListSub.DeleteAllItems();
		richTextBox1.ShowWindow(SW_HIDE);
		richTextBox2.ShowWindow(SW_HIDE);
		m_staticSourceText.ShowWindow(SW_HIDE);
		long lMax=max(pDiffItem->strOldValueArray.GetCount(),pDiffItem->strNewValueArray.GetCount());
		for(long i=0;i<lMax;i++)
		{
			CString strOldValue=i<=pDiffItem->strOldValueArray.GetCount()-1?pDiffItem->strOldValueArray.GetAt(i):_T("");
			CString strNewValue=i<=pDiffItem->strNewValueArray.GetCount()-1?pDiffItem->strNewValueArray.GetAt(i):_T("");
			if(strOldValue.Compare(strNewValue)!=0)
			{
				CString strId;
				strId.Format(_T("%d"),i+1);
				int nIndex=m_ListSub.InsertItem(m_ListSub.GetItemCount(),strId);
				m_ListSub.SetItemText(nIndex,1,strOldValue);
				m_ListSub.SetItemText(nIndex,2,strNewValue);
				m_ListSub.SetItemData(nIndex,(DWORD_PTR)pDiffItem->nDxfCode);
			}
		}
		return;
	}
	m_ListSub.ShowWindow(SW_HIDE);
		richTextBox1.ShowWindow(SW_NORMAL);
		richTextBox2.ShowWindow(SW_NORMAL);
		m_staticSourceText.ShowWindow(SW_NORMAL);

	CString leftText=pDiffItem->strOldValue;
	richTextBox1.SetWindowText(leftText);
	CString rightText=pDiffItem->strNewValue;
	richTextBox2.SetWindowText(rightText);

	WCHAR* left=new WCHAR[leftText.GetLength()+1];
	memset(left,0,(leftText.GetLength()+1)*sizeof(WCHAR));
#ifdef _UNICODE
	lstrcpy(left,leftText);
#else
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, leftText, leftText.GetLength(), left, leftText.GetLength() + 1);
#endif
	WCHAR* right=new WCHAR[rightText.GetLength()+1];
	memset(right,0,(rightText.GetLength()+1)*sizeof(WCHAR));
	#ifdef _UNICODE
	lstrcpy(right,rightText);
#else
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, rightText, rightText.GetLength(), right, rightText.GetLength() + 1);
#endif
	int leftLen = lstrlenW(left);
	int rightLen = lstrlenW(right);
	if(leftLen==0&&rightLen==0)
	{
		//都是空的内容
		delete[] left;
		delete[] right;
		return;
	}
	else if(leftLen==0&&rightLen>0)
	{
		//全部都是新增的
		RichEditCtrlSelect(&richTextBox2,0, rightLen);
		RichEditCtrlChangeTextColor(&richTextBox2,ColorNew);
		delete[] left;
		delete[] right;
		return;
	}
	else if(leftLen>0&&rightLen==0)
	{
		//全部都被删除了
		RichEditCtrlSelect(&richTextBox1,0, leftLen);
		RichEditCtrlChangeTextColor(&richTextBox1,ColorDelete);
		delete[] left;
		delete[] right;
		return;
	}
	bool** V=new bool*[leftLen];//申请一个动态的二维数组
	int l = 0;
	int r = 0;
	for(l=0;l<leftLen;l++)
	{
		V[l]=new bool[rightLen];//申请动态一维数组
	}
	for (l = 0; l < leftLen; l++)
	{
		WCHAR c1 = left[l];
		for (r = 0; r < rightLen; r++)
		{
			WCHAR c2 = right[r];;
			if (c1 == c2)
			{
				V[l][r] = true;
			}
			else
			{
				V[l][r] = false;
			}
		}
	}
	int nMax = 0;
	int** N = new int*[leftLen];//申请动态二维数组
	for(l=0;l<leftLen;l++)
	{
		N[l]=new int[rightLen];//申请动态一维数组
	}
	for (l = leftLen - 1; l >= 0; l--)
	{
		for (r = rightLen - 1; r >= 0; r--)
		{
			if (l == leftLen - 1 && r == rightLen - 1)
			{
				if (V[l][r])
				{
					N[l][r] = 1;
				}
				else
				{
					N[l][r] = 0;
				}
			}
			else if (l == leftLen - 1)
			{
				if (V[l][r])
				{
					N[l][r] = 1;
				}
				else
				{
					N[l][r] = N[l][r+1];
				}
			}
			else if (r == rightLen - 1)
			{
				if (V[l][r])
				{
					N[l][r] = 1;
				}
				else
				{
					N[l][r] = N[l+1][r];
				}
			}
			else
			{
				N[l][r] = max(max(N[l+1][r], N[l][r+1]), V[l][r] ? N[l+1][r+1] + 1 : N[l+1][r+1]);
			}
			if (N[l][r] > nMax)
			{
				nMax = N[l][r];
			}
		}
	}
	if (nMax == 0)
	{
		//未找到任何匹配
		ChangeColor(0, leftLen - 1, 0, rightLen - 1,false);
		//释放二维数组V
		for(l=0;l<leftLen;l++)
		{
			delete[] V[l];
		}
		delete[] V;
		//释放二维数组N
		for(l=0;l<leftLen;l++)
		{
			delete[] N[l];
		}
		delete[] N;
		delete[] left;
		delete[] right;
		return;
	}
	CArray<IndexPair> pairList;
	for (l = leftLen - 1; l >= 0; l--)
	{
		for (r = rightLen - 1; r >= 0; r--)
		{
			if (N[l][r] == nMax && V[l][r])
			{
				//起始点
				IndexPair pair;
				pair.l = l;
				pair.r = r;
				pairList.Add(pair);
			}
		}
	}
	IndexPair firstIndex;
	//寻找最优路径
	int nMin = -1;
	int** D = new int*[leftLen];//申请动态二维数组
	for(l=0;l<leftLen;l++)
	{
		D[l]=new int[rightLen];//申请动态一维数组
	}
	for (l = leftLen - 1; l >= 0; l--)
	{
		for (r = rightLen - 1; r >= 0; r--)
		{
			if (l == leftLen - 1 && r == rightLen - 1)
			{
				if (V[l][r])
				{
					D[l][r] = 1;
				}
				else
				{
					D[l][r] = 0;
				}
			}
			else if (l == leftLen - 1)
			{
				if (V[l][r])
				{
					D[l][r] = 1;
				}
				else
				{
					if (D[l][r + 1] > 0)
					{
						D[l][r] = D[l][r + 1] + 1;
					}
					else
					{
						D[l][r] = 0;
					}
				}
			}
			else if (r == rightLen - 1)
			{
				if (V[l][r])
				{
					D[l][r] = 1;
				}
				else
				{
					D[l][r] = D[l + 1][r];
				}
			}
			else
			{
				if (V[l][r])
				{
					D[l][r] = D[l + 1][r + 1] + 1;
				}
				else
				{
					if (N[l][r + 1] > N[l + 1][r])
					{
						D[l][r] = D[l][r + 1] + 1;
					}
					else
					{
						D[l][r] = D[l + 1][r];
					}
				}
			}

		}
	}
	for (int i = 0; i < pairList.GetCount(); i++)
	{
		IndexPair& pair = pairList.GetAt(i);
		if (D[pair.l][pair.r] < nMin || nMin < 0)
		{
			nMin = D[pair.l][pair.r];
			firstIndex = pair;
		}
	}
	pairList.RemoveAll();
	pairList.Add(firstIndex);
	while (true)
	{
		//寻找下一个合理的点
		IndexPair& lastIndex = pairList.GetAt(pairList.GetCount() - 1);
		if (lastIndex.l == leftLen - 1 || lastIndex.r == rightLen - 1)
		{
			//找到边缘了
			if (V[lastIndex.l][lastIndex.r])
			{
				IndexPair newPair;
				newPair.l = lastIndex.l;
				newPair.r = lastIndex.r;
				pairList.Add(newPair);
				break;
			}
			else
			{
				if (lastIndex.r != rightLen - 1)
				{
					for (r = lastIndex.r + 1; r <= rightLen - 1; r++)
					{
						if (V[lastIndex.l][r])
						{
							IndexPair newPair;
							newPair.l = lastIndex.l;
							newPair.r = r;
							pairList.Add(newPair);
							break;
						}
					}
				}
				else if (lastIndex.l != leftLen - 1)
				{
					for (l = lastIndex.l; l <= leftLen - 1; l++)
					{
						IndexPair newPair;
						newPair.l = l;
						newPair.r = lastIndex.r;
						pairList.Add(newPair);
						break;
					}
				}
				break;
			}

		}
		if (V[lastIndex.l][lastIndex.r])
		{
			IndexPair newPair;
			newPair.l = lastIndex.l + 1;
			newPair.r = lastIndex.r + 1;
			pairList.Add(newPair);
		}
		else
		{
			if (N[lastIndex.l][lastIndex.r + 1] > N[lastIndex.l + 1][lastIndex.r])
			{
				IndexPair newPair;
				newPair.l = lastIndex.l;
				newPair.r = lastIndex.r + 1;
				pairList.Add(newPair);
			}
			else
			{
				IndexPair newPair;
				newPair.l = lastIndex.l+1;
				newPair.r = lastIndex.r;
				pairList.Add(newPair);
			}
		}
	}
	for (int i = pairList.GetCount() - 1; i >= 0; i--)
	{
		IndexPair pair = pairList.GetAt(i);
		if (!V[pair.l][pair.r])
		{
			pairList.RemoveAt(i);
		}
	}
	for (int i = pairList.GetCount() - 2; i >= 0; i--)
	{
		IndexPair& pair1 = pairList.GetAt(i+1);
		IndexPair& pair2 = pairList.GetAt(i);
		for (int j = pair1.r - 1; j >= pair2.r + 1; j--)
		{
			if (right[j] == right[pair2.r])
			{
				pair2.r = j;
				pairList.SetAt(i,pair2);
				break;
			}
		}
	}
	int leftLast = 0;
	int rightLast = 0;
	IndexPair pair0 = pairList.GetAt(0);
	IndexPair pairTmp = pair0;
	//left的pair0.l和right的pair0.r匹配
	ChangeColor(0, pair0.l - 1, 0, pair0.r - 1, false);
	RichEditCtrlSelect(&richTextBox1,pair0.l, 1);
	RichEditCtrlChangeTextColor(&richTextBox1,ColorSame);
	RichEditCtrlSelect(&richTextBox2,pair0.r, 1);
	RichEditCtrlChangeTextColor(&richTextBox2,ColorSame);
	for (int i = 1; i < pairList.GetCount(); i++)
	{
		IndexPair pair1 = pairList.GetAt(i - 1);
		IndexPair pair2 = pairList.GetAt(i);
		if (pair2.l - pair1.l == 1 &&
			pair2.r - pair1.r == 1)
		{

		}
		else
		{
			ChangeColor(pair1.l + 1, pair2.l - 1, pair1.r + 1, pair2.r - 1, false);
		}
		RichEditCtrlSelect(&richTextBox1,pair2.l, 1);
		RichEditCtrlChangeTextColor(&richTextBox1,ColorSame);
		RichEditCtrlSelect(&richTextBox2,pair2.r, 1);
		RichEditCtrlChangeTextColor(&richTextBox2,ColorSame);

		if (i == pairList.GetCount() - 1)
		{
			ChangeColor(pair2.l + 1, leftLen - 1, pair2.r + 1, rightLen - 1, false);
		}
	}
	//释放二维数组V
	for(l=0;l<leftLen;l++)
	{
		delete[] V[l];
	}
	delete[] V;
	//释放二维数组N
	for(l=0;l<leftLen;l++)
	{
		delete[] N[l];
	}
	delete[] N;
	//释放二维数组D
	for(l=0;l<leftLen;l++)
	{
		delete[] D[l];
	}
	delete[] D;
	delete[] left;
		delete[] right;
}
void CmpReportDialog::ChangeColor(int leftStart, int leftEnd, int rightStart, int rightEnd,bool bSame)
{
	int leftLen = leftEnd - leftStart + 1;
	int rightLen = rightEnd - rightStart + 1;
	if(leftLen==0&&rightLen==0)
	{
		return;
	}
	if (leftLen > rightLen)
	{
		//删除了内容,长度为(leftLen - rightLen)
		RichEditCtrlSelect(&richTextBox1,leftStart, leftLen - rightLen);
		RichEditCtrlChangeTextColor(&richTextBox1,ColorDelete);
		if (rightLen > 0)
		{
			//修改了内容,长度为(rightLen)
			RichEditCtrlSelect(&richTextBox2,rightStart, rightLen);
			RichEditCtrlChangeTextColor(&richTextBox2,bSame ? ColorSame : ColorModify);

			RichEditCtrlSelect(&richTextBox1,leftStart + leftLen - rightLen - 1+1, rightLen);
			RichEditCtrlChangeTextColor(&richTextBox1,bSame ? ColorSame : ColorModify);
		}
	}
	else if (leftLen == rightLen)
	{
		//修改了内容,长度为(leftLen)
		RichEditCtrlSelect(&richTextBox2,rightStart, rightLen);
		RichEditCtrlChangeTextColor(&richTextBox2,bSame ? ColorSame : ColorModify);
		RichEditCtrlSelect(&richTextBox1,leftStart, leftLen);
		RichEditCtrlChangeTextColor(&richTextBox1,bSame ? ColorSame : ColorModify);

	}
	else
	{
		//增加了内容,长度为(rightLen-leftLen)
		RichEditCtrlSelect(&richTextBox2,rightStart, rightLen - leftLen);
		RichEditCtrlChangeTextColor(&richTextBox2,ColorNew);
		if (leftLen > 0)
		{
			//修改了内容，长度为(leftLen)
			RichEditCtrlSelect(&richTextBox1,leftStart, leftLen);
			RichEditCtrlChangeTextColor(&richTextBox1,bSame ? ColorSame : ColorModify);
			RichEditCtrlSelect(&richTextBox2,rightStart + rightLen - leftLen - 1+1, leftLen);
			RichEditCtrlChangeTextColor(&richTextBox2,bSame ? ColorSame : ColorModify);
		}
	}
}