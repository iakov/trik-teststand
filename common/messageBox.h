#pragma once

#include <QtGui/QWidget>
#include <QtCore/QString>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
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
