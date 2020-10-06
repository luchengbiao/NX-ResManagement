#ifndef RESMANAGEMENTPANEFILEPATHNAVIHANDLER_H
#define RESMANAGEMENTPANEFILEPATHNAVIHANDLER_H
#include <QList>
#include "ResManagementPanelHandler.h"
#include "src/model/FileItem_Fwd.h"

class QWidget;

class ResManagementPanelFilePathNaviHandler : public ResManagementPanelHandlerWithType<ResManagementPanelHandlerType::FilePathNavi>
{
public:
    ResManagementPanelFilePathNaviHandler(ResManagementPanel* panel);

	void				Reset();
    void				ShowFilePath(const FileItem_SharedPtr& file_item);

private:
    virtual void		Init() override;
    virtual void		AfterInited() override;

private:
    QPointer<QWidget>	file_path_navi_widget_;
	QList<QWidget*>		file_path_widget_list_;

    FileItem_WeakPtr	current_file_item_;
};

#endif // RESMANAGEMENTPANELSELECTEDFILEPATHNAVIHANDLER_H
