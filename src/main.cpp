#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QMessageBox>
#include "panel/ResManagementPanel.h"
#include "src/utils/FunctionPerformer.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(ResManagement);

    QApplication app(argc, argv);

	FunctionPerformer::Init();

	QString project_res_dir = R"(C:\Users\luchengbiao\AppData\Local\neox-hub\demo_art_basic_tutorial\res)";
	QString builtin_res_dir = R"(C:\Users\luchengbiao\AppData\Local\neox-hub\NeoX-Engine_2020.09.01s1(Jun.Stable)\Core-Editor\win32_editor\packages\builtin\res)";

	// parse dirs from command line
	{
		QCommandLineOption opt_project_res_dir(QStringList() << "p" << "pro", "project_res_dir", "project");
		QCommandLineOption opt_builtin_res_dir(QStringList() << "b" << "blt", "builtin_res_dir", "builtin");

		QCommandLineParser parser;
		parser.setApplicationDescription("ResManagement");
		parser.addHelpOption();
		parser.addVersionOption();

		parser.addOption(opt_project_res_dir);
		parser.addOption(opt_builtin_res_dir);

		parser.process(app);

		if (parser.isSet(opt_project_res_dir))
		{
			project_res_dir = parser.value(opt_project_res_dir);
		}

		if (parser.isSet(opt_builtin_res_dir))
		{
			builtin_res_dir = parser.value(opt_builtin_res_dir);
		}
	}

	if (project_res_dir.isEmpty())
	{
		QMessageBox::warning(nullptr, "warning", "the project_res_dir is empty");

		return 0;
	}

	if (builtin_res_dir.isEmpty())
	{
		QMessageBox::warning(nullptr, "warning", "the builtin_res_dir is empty");

		return 0;
	}

	ResManagementPanel panel(project_res_dir, builtin_res_dir);
	panel.show();

    return app.exec();
}
