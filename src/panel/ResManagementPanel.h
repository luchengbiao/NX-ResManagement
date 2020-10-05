#ifndef RESMANAGEMENTPANEL_H
#define RESMANAGEMENTPANEL_H
#include <unordered_map>
#include <QWidget>
#include "src/handler/ResManagementPanelHandlerTypes.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ResManagementPanel; }
QT_END_NAMESPACE

class ResManagementPanelHandler;

class ResManagementPanel : public QWidget
{
	Q_OBJECT

	using SubHandlerMap = std::unordered_map<ResManagementPanelHandlerType, ResManagementPanelHandler*>;

public:
    explicit ResManagementPanel(const QString& project_res_dir, 
								const QString& builtin_res_dir,
		                        QWidget* parent = nullptr);
	~ResManagementPanel();

	Ui::ResManagementPanel*					UiOfPanel() const;

	QString									ProjectResDir() const;
	QString									BuiltinResDir() const;

	template<typename HandlerT>
	HandlerT*								SubHandler() const;

private:
	void									Init();

	void									InitSubHandlers();
	void									DeleteSubHandlers();

private Q_SLOTS:
	void									on_close_btn_clicked();

private:
    QScopedPointer<Ui::ResManagementPanel>	ui_;
	const QString							project_res_dir_;
	const QString							builtin_res_dir_;

	SubHandlerMap							sub_handlers_;
};

template<typename HandlerT>
HandlerT* ResManagementPanel::SubHandler() const
{
	auto it = sub_handlers_.find(HandlerT::Type);

	return it != sub_handlers_.cend() ? dynamic_cast<HandlerT*>(it->second) : nullptr;
}

#endif // RESMANAGEMENTPANEL_H
