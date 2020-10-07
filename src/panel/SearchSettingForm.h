#ifndef SEARCHSETTINGFORM_H
#define SEARCHSETTINGFORM_H
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class SearchSettingForm; }
QT_END_NAMESPACE

class SearchSettingForm : public QWidget
{
	Q_OBJECT

public:
    SearchSettingForm(QWidget* parent = nullptr);

private:
	QScopedPointer<Ui::SearchSettingForm> ui_;
};

#endif // SEARCHSETTINGFORM_H
