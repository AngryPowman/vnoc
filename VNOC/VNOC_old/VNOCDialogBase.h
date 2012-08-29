#pragma once

class CVNOCDialogBase
{
public:
	virtual BOOL Refresh() = 0;
	virtual BOOL LanguageChanged() = 0;	// 语言改变，通知刷新
};

