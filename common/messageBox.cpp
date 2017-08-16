#include <QtGui/QKeyEvent>

#include "messageBox.h"

MessageBox::MessageBox(QString const &message)
	: QMessageBox(QMessageBox::NoIcon, "", message, QMessageBox::NoButton)
{
	setWindowState(Qt::WindowFullScreen);
	setWindowModality(Qt::WindowModal);

//	auto btnOk = new QPushButton(OK");
//	btnOk->setAlignment(Qt::AlignRight | Qt::AlignBottom);
	addButton("OK (press Enter -> ->)", QMessageBox::AcceptRole);

}
