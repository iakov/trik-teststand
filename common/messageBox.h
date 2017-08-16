#pragma once

#include <QtWidgets>

class MessageBox : public QMessageBox
{
	Q_OBJECT

public:
	explicit MessageBox(QString const &message = QString());
//	void setMessage(QString const &message)
};
