#ifndef RESMANAGEMENTPANELHANDLER_H
#define RESMANAGEMENTPANELHANDLER_H
#include <QPointer>
#include <QObject>
#include "ResManagementPanelHandlerTypes.h"
#include "src/panel/ResManagementPanel.h"

class ResManagementPanelHandler : public QObject
{
	friend class ResManagementPanel;

public:
	explicit ResManagementPanelHandler(ResManagementPanel* panel)
		: panel_(panel)
	{}

	Ui::ResManagementPanel* UiOfPanel() const
	{
		return panel_ ? panel_->UiOfPanel() : nullptr;
	}

	template<typename HandlerT>
	HandlerT* SiblingHandler() const;

protected:
	virtual void Init() {}
	virtual void AfterInited() {}

protected:
	QPointer<ResManagementPanel> panel_;
};

template<typename HandlerT>
HandlerT* ResManagementPanelHandler::SiblingHandler() const
{
	return panel_ ? panel_->SubHandler<HandlerT>() : nullptr;
}

template<ResManagementPanelHandlerType type>
class ResManagementPanelHandlerWithType : public ResManagementPanelHandler
{
public:
	static constexpr ResManagementPanelHandlerType Type = type;

public:
	explicit ResManagementPanelHandlerWithType(ResManagementPanel* panel)
		: ResManagementPanelHandler(panel)
	{}
};


#endif // RESMANAGEMENTPANELHANDLER_H