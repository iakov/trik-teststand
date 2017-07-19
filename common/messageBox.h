#pragma once

#include <QtWidgets>
#include <QtCore/QString>
#include <QtCore/QEventLoop>

class MessageBox : public QWidget
{
	Q_OBJECT

public:
	explicit MessageBox(QString const &message = QString());
	void exec();
	void setMessage(QString const &message);

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	QVBoxLayout layout;
	QLabel messageLabel;
	QLabel okLabel;
	QEventLoop mEventLoop;
};
