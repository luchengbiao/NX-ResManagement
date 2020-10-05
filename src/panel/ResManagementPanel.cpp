#include "ResManagementPanel.h"
#include "src/handler/ResManagementPanelDirTreeHandler.h"
#include "src/handler/ResManagementPanelFileListHandler.h"
#include "ui_ResManagementPanel.h"

ResManagementPanel::ResManagementPanel(const QString& project_res_dir,
								       const QString& builtin_res_dir,
	                                   QWidget* parent)
    : QWidget(parent)
    , ui_(new Ui::ResManagementPanel)
	, project_res_dir_(project_res_dir)
	, builtin_res_dir_(builtin_res_dir)
{
	ui_->setupUi(this);

	this->Init();
}

ResManagementPanel::~ResManagementPanel()
{
	this->DeleteSubHandlers();
}

void ResManagementPanel::Init()
{
	QSplitter* main_splitter = ui_->main_splitter;
	main_splitter->setStretchFactor(0, 1);
	main_splitter->setStretchFactor(1, 1);
	main_splitter->setStretchFactor(2, 1);

	this->InitSubHandlers();
}

void ResManagementPanel::InitSubHandlers()
{
#define TYPE_2_HANDLER(HandlerT) { HandlerT::Type, new HandlerT(this) }

    sub_handlers_ = {
		TYPE_2_HANDLER(ResManagementPanelDirTreeHandler),
		TYPE_2_HANDLER(ResManagementPanelFileListHandler),
	};

#undef TYPE_2_HANDLER

	for (const auto& pair : sub_handlers_)
	{
		if (pair.second)
		{
			pair.second->Init();
		}
	}

	for (const auto& pair : sub_handlers_)
	{
		if (pair.second)
		{
			pair.second->AfterInited();
		}
	}
}

void ResManagementPanel::DeleteSubHandlers()
{
	SubHandlerMap sub_handlers;
	sub_handlers.swap(sub_handlers_);

	for (const auto& pair : sub_handlers)
	{
		delete pair.second;
	}
}

Ui::ResManagementPanel* ResManagementPanel::UiOfPanel() const
{
	return ui_.get();
}

QString ResManagementPanel::ProjectResDir() const
{
	return project_res_dir_;
}

QString ResManagementPanel::BuiltinResDir() const
{
	return builtin_res_dir_;
}

void ResManagementPanel::on_close_btn_clicked()
{
	this->close();
}
