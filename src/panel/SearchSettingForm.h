#ifndef SEARCHSETTINGFORM_H
#define SEARCHSETTINGFORM_H
#include <QWidget>
#include "src/model/FileType.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SearchSettingForm; }
QT_END_NAMESPACE

class SearchSettingForm : public QWidget
{
	Q_OBJECT

public:
    SearchSettingForm(QWidget* parent = nullptr);

Q_SIGNALS:
	void CaseSenditivityChanged(Qt::CaseSensitivity);
	void FileTypeChecked(FileType);
	void FileTypeUnchecked(FileType);

private Q_SLOTS:
	void on_case_senditivity_check_box_stateChanged(int);
	void on_ok_btn_clicked();

private:
	QScopedPointer<Ui::SearchSettingForm> ui_;
};

#endif // SEARCHSETTINGFORM_H
