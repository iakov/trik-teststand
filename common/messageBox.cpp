#include <QtGui/QKeyEvent>

#include "messageBox.h"

MessageBox::MessageBox(QString const &message)
	: QMessageBox(QMessageBox::NoIcon
		, tr("Информация перед стартом")
		, message, QMessageBox::NoButton)
{
	setWindowState(Qt::WindowFullScreen);
	setWindowModality(Qt::WindowModal);
	addButton("OK", QMessageBox::AcceptRole);
}
