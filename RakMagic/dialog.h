#ifndef DIALOG_H
#define DIALOG_H

struct stSAMPDialog {
	BYTE bDialogStyle;
	WORD wDialogID;
	BYTE bTitleLength;
	char szTitle[257];
	BYTE bButton1Len;
	char szButton1[257];
	BYTE bButton2Len;
	char szButton2[257];
	char szInfo[257];
};

enum {
	DIALOG_STYLE_MSGBOX,
	DIALOG_STYLE_INPUT,
	DIALOG_STYLE_LIST,
	DIALOG_STYLE_PASSWORD,
	DIALOG_STYLE_TABLIST,
	DIALOG_STYLE_TABLIST_HEADERS
};

namespace Interface {
	namespace Dialog {

		extern bool bEnabled;
		extern bool bShowDialog;
		extern stSAMPDialog dialog;
		extern int iSelectedItem;

		void Render();
	}
}

#endif