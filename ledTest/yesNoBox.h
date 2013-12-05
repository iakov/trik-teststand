#pragma once

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtCore/QEventLoop>
#include <QtCore/QString>

class YesNoBox : public QWidget
{
	Q_OBJECT

public:
	explicit YesNoBox(QString const &question = QString());
	void setQuestion(QString const &question);

	enum Answer
	{
		yes
		, no
	};

	Answer exec();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	QVBoxLayout mainLayout;
	QHBoxLayout answersLayout;
	QLabel questionLabel, yesLabel, noLabel;

	QEventLoop mEventLoop;
};
