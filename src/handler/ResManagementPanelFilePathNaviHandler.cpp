#include "ResManagementPanelFilePathNaviHandler.h"
#include <QLabel>
#include <QPair>
#include <QPushButton>
#include "ResManagementPanelDirTreeHandler.h"
#include "src/model/FileItem.h"
#include "ui_ResManagementPanel.h"

ResManagementPanelFilePathNaviHandler::ResManagementPanelFilePathNaviHandler(ResManagementPanel* panel)
	: ResManagementPanelHandlerWithType(panel)
{}

void ResManagementPanelFilePathNaviHandler::Init()
{
	auto ui = UiOfPanel();
	if (ui == nullptr)
	{
		return;
	}

    file_path_navi_widget_ = ui->file_path_navi_widget;
}

void ResManagementPanelFilePathNaviHandler::AfterInited()
{}

void ResManagementPanelFilePathNaviHandler::Reset()
{
	current_file_item_.reset();

	QList<QWidget*> widget_list;
	file_path_widget_list_.swap(widget_list);

	for (auto widget : widget_list)
	{
		if (widget)
		{
			widget->deleteLater();
		}
	}
}

void ResManagementPanelFilePathNaviHandler::ShowFilePath(const FileItem_SharedPtr& file_item)
{
	auto current_file_item = current_file_item_.lock();
    if (file_item == current_file_item)
	{
		return;
	}

	this->Reset();
	if (file_item == nullptr)
	{
		return;
	}

	current_file_item_ = file_item;

	if (file_path_navi_widget_ == nullptr)
	{
		return;
	}

	QList<QPair<QPushButton*, FileItem_SharedPtr>> pair_list;
	FileItem_SharedPtr item = file_item;
	while (item != nullptr)
	{
		QString name = item->NameToDisplay();
		if (name.isEmpty() == false)
		{
			QPushButton* btn = new QPushButton(name, file_path_navi_widget_);
			pair_list.push_front(QPair(btn, item));
		}

		item = item->Parent();
	}

	QLayout* layout = file_path_navi_widget_->layout();
	int i = 0;
	const int back_index = pair_list.size() - 1;
	for (const auto& pair : pair_list)
	{
		file_path_widget_list_.push_back(pair.first);
		layout->addWidget(pair.first);

		pair.first->adjustSize();

		connect(pair.second.get(), &FileItem::FilePathRenamed, pair.first, [btn = pair.first, weak_file_item = FileItem_WeakPtr(pair.second)](const QString&, const QString&) {
			auto shared_file_item = weak_file_item.lock();
			if (shared_file_item)
			{
				btn->setText(shared_file_item->NameToDisplay());
			}
		});

		connect(pair.first, &QPushButton::clicked, this, [this, weak_file_item = FileItem_WeakPtr(pair.second)] {
			auto shared_file_item = weak_file_item.lock();
			if (shared_file_item 
				&& shared_file_item != current_file_item_.lock())
			{
				auto dir_tree_handler = SiblingHandler<ResManagementPanelDirTreeHandler>();
				if (dir_tree_handler)
				{
					dir_tree_handler->SelectFileItem(shared_file_item);
				}
			}
		});

		if (i < back_index)
		{
			QLabel* arrow = new QLabel(QStringLiteral(">"), file_path_navi_widget_);

			file_path_widget_list_.push_back(arrow);
			layout->addWidget(arrow);
		}

		++i;
	}
}
