#include "stdafx.h"
#include "SciEdit.h"

BOOL CSciEdit::SetLexer(DWORD lexer /* = SCLEX_CPP */)
{
	switch (lexer)
	{
	case SCLEX_CPP:
		{
			_SendSciMessage(SCI_STYLESETFONT,STYLE_DEFAULT,(DWORD)"Courier New");
			_SendSciMessage(SCI_STYLESETSIZE, STYLE_DEFAULT,10);
			_SendSciMessage(SCI_STYLECLEARALL);
			sptr_t temp = _SendSciMessage(SCI_GETLEXER,0,0);
			_SendSciMessage(SCI_SETLEXER,lexer);
			temp = _SendSciMessage(SCI_GETLEXER,0,0);
			CString strKeyWords;
			strKeyWords.LoadString(m_hinst,IDS_Sci_Lexer_Keywords_CPP);
			_SendSciMessage(SCI_SETKEYWORDS,0,(DWORD)(LPCSTR)CStringA(strKeyWords));
			strKeyWords.LoadString(m_hinst,IDS_Sci_Lexer_VariableTypes_CPP);
			_SendSciMessage(SCI_SETKEYWORDS,1,(DWORD)(LPCSTR)CStringA(strKeyWords));
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_WORD, 0x00FF0000);   //¹Ø¼ü×Ö
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_WORD2, 0x00800080);   //¹Ø¼ü×Ö
			_SendSciMessage(SCI_STYLESETBOLD, SCE_C_WORD2, TRUE);   //¹Ø¼ü×Ö
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_STRING, 0x001515A3); //×Ö·û´®
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_CHARACTER, 0x001515A3); //×Ö·û
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x00808080);//Ô¤±àÒë¿ª¹Ø
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_COMMENT, 0x00008000);//¿é×¢ÊÍ
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x00008000);//ÐÐ×¢ÊÍ
			_SendSciMessage(SCI_STYLESETFORE, SCE_C_COMMENTDOC, 0x00008000);//ÎÄµµ×¢ÊÍ£¨/**¿ªÍ·£©

			_SendSciMessage(SCI_SETCARETLINEVISIBLE, TRUE);
			_SendSciMessage(SCI_SETCARETLINEBACK, 0xb0ffff);
		}
		break;
	}
	return TRUE;
}

BOOL CSciEdit::_OnSciNotify( SCNotification* notify )
{
	switch(notify->nmhdr.code)
	{
	case SCN_PAINTED:
		_CalcLineNumberMarginWidth();
		break;
	case SCN_MARGINCLICK:
		{
			if (notify->nmhdr.idFrom = Scintilla_Margin_Folder)
			{
				const int lineNum = _SendSciMessage(SCI_LINEFROMPOSITION,notify->position);
				_SendSciMessage(SCI_TOGGLEFOLD,lineNum);
			}
		}
		break;
	}
	return FALSE;
}
