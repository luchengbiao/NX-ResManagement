#include "SearchSettingForm.h"
#include "ui_SearchSettingForm.h"

SearchSettingForm::SearchSettingForm(QWidget* parent)
	: QWidget(parent)
	, ui_(new Ui::SearchSettingForm)
{
	ui_->setupUi(this);

	QPair<QCheckBox*, FileType> pairs[] = {
		{ ui_->scene_check_box,		FileType::Scene },
		{ ui_->light_check_box,		FileType::Light },
		{ ui_->model_check_box,		FileType::Model },
		{ ui_->animation_check_box, FileType::Animation },
		{ ui_->effect_check_box,	FileType::Effect },
		{ ui_->shader_check_box,	FileType::Shader },
		{ ui_->material_check_box,	FileType::Material },
		{ ui_->mapping_check_box,	FileType::Mapping },
		{ ui_->audio_check_box,		FileType::Audio },
		{ ui_->video_check_box,		FileType::Video },
		{ ui_->ui_check_box,		FileType::UI },
	};

	for (const auto& pair : pairs)
	{
		connect(pair.first, &QCheckBox::stateChanged, this, [this, file_type = pair.second](int state){
			state ? FileTypeChecked(file_type) : FileTypeUnchecked(file_type);
		});
	}
}

void SearchSettingForm::on_case_senditivity_check_box_stateChanged(int state)
{
	emit CaseSenditivityChanged(state? Qt::CaseSensitive : Qt::CaseInsensitive);
}

void SearchSettingForm::on_ok_btn_clicked()
{
	this->close();
}
