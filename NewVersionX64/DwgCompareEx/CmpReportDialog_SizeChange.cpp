#include "StdAfx.h"
#include "resource.h"
#include "CmpReportDialog.h"
#include "CommCommand.h"
void CmpReportDialog::OnSizeChanged()
{
	if(m_bEx)
	{
		CRect rc,rc1;
		GetClientRect(rc);
		CRect rc2;
		rc2.top=rc.top+5;
		rc2.bottom=rc.bottom -5;
		rc2.left=rc.left+5;
		rc2.right=m_nMode==2?rc.right-5:rc.left+max((rc.right-rc.left)*0.21,(rc.right-rc.left)*0.25-10);
		GetDlgItem(IDC_STATIC_FILES)->MoveWindow(rc2);

		CRect rc_files=rc2;
		rc_files.top+=15;
		rc_files.left+=5;
		rc_files.bottom-=5;
		rc_files.right-=5;
		m_listFiles.MoveWindow(rc_files);
		if(m_nMode==2)
		{
			m_listFiles.SetColumnWidth(0,rc_files.Width()*1/2-2);
		m_listFiles.SetColumnWidth(1,rc_files.Width()*1/2-2);
		m_listFiles.SetColumnWidth(2,0);
		}
		else
		{
		m_listFiles.SetColumnWidth(0,rc_files.Width()*2/3-2);
		m_listFiles.SetColumnWidth(1,0);
		m_listFiles.SetColumnWidth(2,rc_files.Width()/3-2);
		}

		rc1.top = rc.top+5; 
		rc1.left = rc.left+0.25*(rc.right-rc.left); 
		rc1.right = rc.right-5; 
		rc1.bottom = rc.bottom -5;

		CRect rc_left;
		rc_left.top=rc1.top;
		rc_left.bottom=rc1.bottom;
		rc_left.left=rc1.left;
		rc_left.right=rc1.left+max((rc1.right-rc1.left)*0.35,(rc1.right-rc1.left)*0.4-10);
		GetDlgItem(IDC_GROUP_LEFT)->MoveWindow(rc_left);

		//实体，左->右
		CRect rc_EntLToR;
		m_btnEntLToR.GetClientRect(rc_EntLToR);
		CRect rc_ent_modify=rc_left;
		rc_ent_modify.right=rc_ent_modify.left+rc_EntLToR.Width();
		rc_ent_modify.bottom=rc_left.bottom-5;
		rc_ent_modify.top=rc_ent_modify.bottom-rc_EntLToR.Height();

		CRect rc_ent_modify_lr=rc_ent_modify;
		rc_ent_modify_lr.MoveToX(rc_left.left+(rc_left.Width()/2-rc_EntLToR.Width())/2);
		m_btnEntLToR.MoveWindow(rc_ent_modify_lr);
		rc_ent_modify_lr.MoveToX(rc_left.left+rc_left.Width()/2+(rc_left.Width()/2-rc_EntLToR.Width())/2);
		m_btnEntRToL.MoveWindow(rc_ent_modify_lr);

		rc_left.top+=15;
		rc_left.left+=5;
		rc_left.bottom=rc_ent_modify.top-5;
		rc_left.right-=5;
		m_ListLeft.MoveWindow(rc_left);
		m_ListLeft.SetColumnWidth(0,rc_left.Width()/2-1);
		m_ListLeft.SetColumnWidth(1,rc_left.Width()/3-1);
		m_ListLeft.SetColumnWidth(2,rc_left.Width()/6-1);
		m_ListLeft.SetColumnWidth(3,0);

		CRect rc_right;
		rc_right.top=rc1.top;
		rc_right.bottom=rc1.bottom;
		rc_right.left=rc1.left+(rc1.right-rc1.left)*0.4;
		rc_right.right=rc1.right;
		GetDlgItem(IDC_GROUP_RIGHT)->MoveWindow(rc_right);

		CRect rc_right_top=rc_right;
		//DXF同步，从左边同步到右边
		CRect rc_DxfLToR;
		m_btnDxfLToR.GetClientRect(rc_DxfLToR);
		CRect rc_dxf_modify=rc_right;
		rc_dxf_modify.right=rc_dxf_modify.left+rc_DxfLToR.Width();
		rc_dxf_modify.bottom-=5+(rc_right.bottom-5-(rc_right.top))*0.55;
		rc_dxf_modify.top=rc_dxf_modify.bottom-rc_DxfLToR.Height();

		CRect rc_dxf_modify_lr=rc_dxf_modify;
		rc_dxf_modify_lr.MoveToX(rc_right_top.left+rc_right.Width()*1/5*0.95);
		m_btnDxfLToR.MoveWindow(rc_dxf_modify_lr);

		CRect rc_dxf_modify_rl=rc_dxf_modify;
		rc_dxf_modify_rl.MoveToX(rc_right_top.left+rc_right.Width()*4/5*0.95);
		m_btnDxfRToL.MoveWindow(rc_dxf_modify_rl);

		CRect rc_Color;
		m_btnColor.GetClientRect(rc_Color);

		int x=(rc_dxf_modify_lr.right+rc_dxf_modify_rl.left)/2;
		int y=(rc_dxf_modify.top+rc_dxf_modify.bottom)/2;
		rc_Color.MoveToXY(x-rc_Color.Width()/2,y-rc_Color.Height()/2);
		m_btnColor.MoveWindow(rc_Color);

		rc_right_top=rc_right;
		rc_right_top.top+=15;
		rc_right_top.left+=5;
		rc_right_top.bottom=rc_dxf_modify.top-5;
		rc_right_top.right-=5;
		m_ListRight.MoveWindow(rc_right_top);
		m_ListRight.SetColumnWidth(0,rc_right.Width()/5*0.95);
		m_ListRight.SetColumnWidth(1,rc_right.Width()*2/5*0.95);
		m_ListRight.SetColumnWidth(2,rc_right.Width()*2/5*0.95);
		rc_right_top.bottom=rc_dxf_modify.bottom;

		CRect rc_list_sub=rc_right_top;
		rc_list_sub.top=rc_right_top.bottom+5;
		rc_list_sub.bottom=rc_right.bottom-5;
		m_ListSub.MoveWindow(rc_list_sub);
		m_ListSub.SetColumnWidth(0,rc_list_sub.Width()/5*0.95);
		m_ListSub.SetColumnWidth(1,rc_list_sub.Width()*2/5*0.95);
		m_ListSub.SetColumnWidth(2,rc_list_sub.Width()*2/5*0.95);


		CRect rc_right_bottom=rc_right_top;
		rc_right_bottom.top=rc_right_top.bottom+5;
		rc_right_bottom.bottom=rc_right.bottom-5;

		CRect rc_sourceText;
		m_staticSourceText.GetClientRect(&rc_sourceText);

		CRect rc_right_bottom_left_text;
		rc_right_bottom_left_text.left=rc_right_bottom.left;
		rc_right_bottom_left_text.top=rc_right_bottom.top;
		rc_right_bottom_left_text.bottom=rc_right_bottom_left_text.top+(rc_right_bottom.Height()-rc_sourceText.Height())/2-5;
		rc_right_bottom_left_text.right=rc_right.right-5;
		richTextBox1.MoveWindow(rc_right_bottom_left_text);

		CRect rc_right_bottom_left;
		rc_right_bottom_left.left=rc_right_top.left;
		rc_right_bottom_left.top=rc_right_bottom_left_text.bottom+5;
		rc_right_bottom_left.bottom=rc_sourceText.Height()+rc_right_bottom_left.top;
		rc_right_bottom_left.right=rc_sourceText.Width()+rc_right_bottom_left.left;
		m_staticSourceText.MoveWindow(rc_right_bottom_left);

		

		CRect rc_right_bottom_right_text;
		rc_right_bottom_right_text.left=rc_right_bottom_left.left;
		rc_right_bottom_right_text.right=rc_right_top.right;
		rc_right_bottom_right_text.top=rc_right_bottom_left.bottom+5;
		rc_right_bottom_right_text.bottom=rc_right.bottom-5;
		richTextBox2.MoveWindow(rc_right_bottom_right_text);

		m_staticMldbck.ShowWindow(SW_HIDE);
			m_staticDbwj1.ShowWindow(SW_HIDE);
			m_staticDbwj2.ShowWindow(SW_HIDE);
		if(m_nMode==2)
		{
			GetDlgItem(IDC_GROUP_LEFT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_GROUP_RIGHT)->ShowWindow(SW_HIDE);
			m_ListLeft.ShowWindow(SW_HIDE);
			m_ListRight.ShowWindow(SW_HIDE);

			m_listFiles.ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_STATIC_FILES)->ShowWindow(SW_NORMAL);

			m_staticSourceText.ShowWindow(SW_HIDE);
			richTextBox1.ShowWindow(SW_HIDE);
			richTextBox2.ShowWindow(SW_HIDE);
			
			m_btnEntLToR.ShowWindow(SW_HIDE);
			m_btnEntRToL.ShowWindow(SW_HIDE);
			m_btnDxfLToR.ShowWindow(SW_HIDE);
			m_btnDxfRToL.ShowWindow(SW_HIDE);
			m_btnColor.ShowWindow(SW_HIDE);

		}
		else
		{
			GetDlgItem(IDC_GROUP_LEFT)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_GROUP_RIGHT)->ShowWindow(SW_NORMAL);
			m_ListLeft.ShowWindow(SW_NORMAL);
			m_ListRight.ShowWindow(SW_NORMAL);

			m_staticSourceText.ShowWindow(SW_NORMAL);
			richTextBox1.ShowWindow(SW_NORMAL);
			richTextBox2.ShowWindow(SW_NORMAL);

		m_listFiles.ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_STATIC_FILES)->ShowWindow(SW_NORMAL);

		m_btnEntLToR.ShowWindow(SW_NORMAL);
			m_btnEntRToL.ShowWindow(SW_NORMAL);
		m_btnDxfLToR.ShowWindow(SW_NORMAL);
			m_btnDxfRToL.ShowWindow(SW_NORMAL);
			m_btnColor.ShowWindow(SW_NORMAL);
		
		}
	}
	else
	{
		CRect rc,rc1;
		GetClientRect(rc);
		rc1.top = rc.top+5; 
		rc1.left = rc.left+5; 
		rc1.right = rc.right-5; 
		rc1.bottom = rc.bottom -5;

		CRect rc_left;
		rc_left.top=rc1.top;
		rc_left.bottom=rc1.bottom;
		rc_left.left=rc1.left;
		rc_left.right=rc1.left+max((rc1.right-rc1.left)*0.25,(rc1.right-rc1.left)*0.3-10);
		GetDlgItem(IDC_GROUP_LEFT)->MoveWindow(rc_left);

		//实体，左->右
		CRect rc_EntLToR;
		m_btnEntLToR.GetClientRect(rc_EntLToR);
		CRect rc_ent_modify=rc_left;
		rc_ent_modify.right=rc_ent_modify.left+rc_EntLToR.Width();
		rc_ent_modify.bottom=rc_left.bottom-5;
		rc_ent_modify.top=rc_ent_modify.bottom-rc_EntLToR.Height();

		CRect rc_ent_modify_lr=rc_ent_modify;
		rc_ent_modify_lr.MoveToX(rc_left.left+(rc_left.Width()/2-rc_EntLToR.Width())/2);
		m_btnEntLToR.MoveWindow(rc_ent_modify_lr);
		rc_ent_modify_lr.MoveToX(rc_left.left+rc_left.Width()/2+(rc_left.Width()/2-rc_EntLToR.Width())/2);
		m_btnEntRToL.MoveWindow(rc_ent_modify_lr);

		rc_left.top+=15;
		rc_left.left+=5;
		rc_left.bottom=rc_ent_modify.top-5;
		rc_left.right-=5;
		m_ListLeft.MoveWindow(rc_left);
		m_ListLeft.SetColumnWidth(0,rc_left.Width()/2-1);
		m_ListLeft.SetColumnWidth(1,rc_left.Width()/3-1);
		m_ListLeft.SetColumnWidth(2,rc_left.Width()/6-1);
		m_ListLeft.SetColumnWidth(3,0);

		CRect rc_right;
		rc_right.top=rc1.top;
		rc_right.bottom=rc1.bottom;
		rc_right.left=rc1.left+(rc1.right-rc1.left)*0.3;
		rc_right.right=rc1.right;
		GetDlgItem(IDC_GROUP_RIGHT)->MoveWindow(rc_right);


		CRect rc_right_top=rc_right;
		//DXF同步，从左边同步到右边
		CRect rc_DxfLToR;
		m_btnDxfLToR.GetClientRect(rc_DxfLToR);
		CRect rc_dxf_modify=rc_right;
		rc_dxf_modify.right=rc_dxf_modify.left+rc_DxfLToR.Width();
		rc_dxf_modify.bottom-=5+(rc_right.bottom-5-(rc_right.top))*0.55;
		rc_dxf_modify.top=rc_dxf_modify.bottom-rc_DxfLToR.Height();

		CRect rc_dxf_modify_lr=rc_dxf_modify;
		rc_dxf_modify_lr.MoveToX(rc_right_top.left+rc_right.Width()*2/5*0.95);
		m_btnDxfLToR.MoveWindow(rc_dxf_modify_lr);

		CRect rc_dxf_modify_rl=rc_dxf_modify;
		rc_dxf_modify_rl.MoveToX(rc_right_top.left+rc_right.Width()*4/5*0.95);
		m_btnDxfRToL.MoveWindow(rc_dxf_modify_rl);

		CRect rc_Color;
		m_btnColor.GetClientRect(rc_Color);

		int x=(rc_dxf_modify_lr.right+rc_dxf_modify_rl.left)/2;
		int y=(rc_dxf_modify.top+rc_dxf_modify.bottom)/2;
		rc_Color.MoveToXY(x-rc_Color.Width()/2,y-rc_Color.Height()/2);
		m_btnColor.MoveWindow(rc_Color);


		rc_right_top.top+=15;
		rc_right_top.left+=5;
		rc_right_top.bottom=rc_dxf_modify.top-5;
		rc_right_top.right-=5;
		m_ListRight.MoveWindow(rc_right_top);
		m_ListRight.SetColumnWidth(0,rc_right.Width()/5*0.95);
		m_ListRight.SetColumnWidth(1,rc_right.Width()*2/5*0.95);
		m_ListRight.SetColumnWidth(2,rc_right.Width()*2/5*0.95);
		rc_right_top.bottom=rc_dxf_modify.bottom;

		CRect rc_list_sub=rc_right_top;
		rc_list_sub.top=rc_right_top.bottom+5;
		rc_list_sub.bottom=rc_right.bottom-5;

		m_ListSub.MoveWindow(rc_list_sub);
		m_ListSub.SetColumnWidth(0,rc_list_sub.Width()/5*0.95);
		m_ListSub.SetColumnWidth(1,rc_list_sub.Width()*2/5*0.95);
		m_ListSub.SetColumnWidth(2,rc_list_sub.Width()*2/5*0.95);



		CRect rc_sourceText;
		m_staticSourceText.GetClientRect(&rc_sourceText);

		CRect rc_right_bottom=rc_right_top;
		rc_right_bottom.top=rc_right_top.bottom+5;
		rc_right_bottom.bottom=rc_right.bottom-5;

		CRect rc_right_bottom_left_text;
		rc_right_bottom_left_text.left=rc_right_bottom.left;
		rc_right_bottom_left_text.top=rc_right_bottom.top;
		rc_right_bottom_left_text.bottom=rc_right_bottom_left_text.top+(rc_right_bottom.Height()-rc_sourceText.Height())/2-5;
		rc_right_bottom_left_text.right=rc_right.right-5;
		richTextBox1.MoveWindow(rc_right_bottom_left_text);

		CRect rc_right_bottom_left;
		rc_right_bottom_left.left=rc_right_top.left;
		rc_right_bottom_left.top=rc_right_bottom_left_text.bottom+5;
		rc_right_bottom_left.bottom=rc_sourceText.Height()+rc_right_bottom_left.top;
		rc_right_bottom_left.right=rc_sourceText.Width()+rc_right_bottom_left.left;
		m_staticSourceText.MoveWindow(rc_right_bottom_left);

		

		CRect rc_right_bottom_right_text;
		rc_right_bottom_right_text.left=rc_right_bottom_left.left;
		rc_right_bottom_right_text.right=rc_right_top.right;
		rc_right_bottom_right_text.top=rc_right_bottom_left.bottom+5;
		rc_right_bottom_right_text.bottom=rc_right.bottom-5;
		richTextBox2.MoveWindow(rc_right_bottom_right_text);

		if(m_nMode==2)
		{
			m_staticMldbck.ShowWindow(SW_NORMAL);
			m_staticDbwj1.ShowWindow(SW_NORMAL);
			m_staticDbwj2.ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_GROUP_LEFT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_GROUP_RIGHT)->ShowWindow(SW_HIDE);
			m_ListLeft.ShowWindow(SW_HIDE);
			m_ListRight.ShowWindow(SW_HIDE);
			m_listFiles.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_FILES)->ShowWindow(SW_HIDE);
			
			m_staticSourceText.ShowWindow(SW_HIDE);
			richTextBox1.ShowWindow(SW_HIDE);
			richTextBox2.ShowWindow(SW_HIDE);


		}
		else
		{
			m_staticMldbck.ShowWindow(SW_HIDE);
			m_staticDbwj1.ShowWindow(SW_HIDE);
			m_staticDbwj2.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_GROUP_LEFT)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_GROUP_RIGHT)->ShowWindow(SW_NORMAL);
			m_ListLeft.ShowWindow(SW_NORMAL);
			m_ListRight.ShowWindow(SW_NORMAL);

			m_staticSourceText.ShowWindow(SW_NORMAL);
			richTextBox1.ShowWindow(SW_NORMAL);
			richTextBox2.ShowWindow(SW_NORMAL);

		m_listFiles.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FILES)->ShowWindow(SW_HIDE);
		
		}
		
	}
}