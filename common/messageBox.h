#pragma once

#include <QtGui/QWidget>
#include <QtCore/QString>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtCore/QEventLoop>

class MessageBox : public QWidget
{
	Q_OBJECT

public:
	explicit MessageBox(QString const &message = QString());
	void exec();
	void setMessage(QString const &message);

protected:
	void keyPressEvent(QKeyEvent *);

private:
	QHBoxLayout layout;
	QLabel messageLabel;
	QEventLoop mEventLoop;
};
