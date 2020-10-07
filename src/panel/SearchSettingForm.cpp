#include "SearchSettingForm.h"
#include "ui_SearchSettingForm.h"

SearchSettingForm::SearchSettingForm(QWidget* parent)
	: QWidget(parent)
	, ui_(new Ui::SearchSettingForm)
{
	ui_->setupUi(this);
}
