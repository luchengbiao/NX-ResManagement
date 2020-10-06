/********************************************************************************
** Form generated from reading UI file 'ResManagementPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESMANAGEMENTPANEL_H
#define UI_RESMANAGEMENTPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ResManagementPanel
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *caption_widget;
    QHBoxLayout *horizontalLayout;
    QLabel *caption_label;
    QSpacerItem *horizontalSpacer;
    QToolButton *close_btn;
    QWidget *setting_wrapper_widget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *file_path_navi_widget;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *search_key_edit;
    QToolButton *search_setting_btn;
    QSlider *slider;
    QSplitter *main_splitter;
    QWidget *dir_tree_wrapper_widget;
    QVBoxLayout *verticalLayout_2;
    QTreeView *dir_tree_view;
    QWidget *file_list_wrapper_widget;
    QWidget *right_wrapper_widget;

    void setupUi(QWidget *ResManagementPanel)
    {
        if (ResManagementPanel->objectName().isEmpty())
            ResManagementPanel->setObjectName(QString::fromUtf8("ResManagementPanel"));
        ResManagementPanel->resize(1319, 428);
        verticalLayout = new QVBoxLayout(ResManagementPanel);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        caption_widget = new QWidget(ResManagementPanel);
        caption_widget->setObjectName(QString::fromUtf8("caption_widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(30);
        sizePolicy.setHeightForWidth(caption_widget->sizePolicy().hasHeightForWidth());
        caption_widget->setSizePolicy(sizePolicy);
        caption_widget->setMinimumSize(QSize(0, 30));
        caption_widget->setMaximumSize(QSize(16777215, 30));
        caption_widget->setBaseSize(QSize(0, 0));
        caption_widget->setStyleSheet(QString::fromUtf8(".QWidget{ background-color:LightSkyBlue }"));
        horizontalLayout = new QHBoxLayout(caption_widget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(8, 0, 8, 0);
        caption_label = new QLabel(caption_widget);
        caption_label->setObjectName(QString::fromUtf8("caption_label"));

        horizontalLayout->addWidget(caption_label);

        horizontalSpacer = new QSpacerItem(1209, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        close_btn = new QToolButton(caption_widget);
        close_btn->setObjectName(QString::fromUtf8("close_btn"));

        horizontalLayout->addWidget(close_btn);


        verticalLayout->addWidget(caption_widget);

        setting_wrapper_widget = new QWidget(ResManagementPanel);
        setting_wrapper_widget->setObjectName(QString::fromUtf8("setting_wrapper_widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(setting_wrapper_widget->sizePolicy().hasHeightForWidth());
        setting_wrapper_widget->setSizePolicy(sizePolicy1);
        setting_wrapper_widget->setMinimumSize(QSize(0, 40));
        setting_wrapper_widget->setMaximumSize(QSize(16777215, 40));
        setting_wrapper_widget->setStyleSheet(QString::fromUtf8(".QWidget{border-style: solid; border-color: gray; border-bottom-width: 1px;}"));
        horizontalLayout_2 = new QHBoxLayout(setting_wrapper_widget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(8, 0, 8, 0);
        file_path_navi_widget = new QWidget(setting_wrapper_widget);
        file_path_navi_widget->setObjectName(QString::fromUtf8("file_path_navi_widget"));
        horizontalLayout_3 = new QHBoxLayout(file_path_navi_widget);
        horizontalLayout_3->setSpacing(2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);

        horizontalLayout_2->addWidget(file_path_navi_widget);

        horizontalSpacer_2 = new QSpacerItem(815, 19, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        search_key_edit = new QLineEdit(setting_wrapper_widget);
        search_key_edit->setObjectName(QString::fromUtf8("search_key_edit"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(search_key_edit->sizePolicy().hasHeightForWidth());
        search_key_edit->setSizePolicy(sizePolicy2);
        search_key_edit->setMinimumSize(QSize(200, 30));
        search_key_edit->setMaximumSize(QSize(200, 30));

        horizontalLayout_2->addWidget(search_key_edit);

        search_setting_btn = new QToolButton(setting_wrapper_widget);
        search_setting_btn->setObjectName(QString::fromUtf8("search_setting_btn"));
        search_setting_btn->setMinimumSize(QSize(0, 30));
        search_setting_btn->setMaximumSize(QSize(16777215, 30));

        horizontalLayout_2->addWidget(search_setting_btn);

        slider = new QSlider(setting_wrapper_widget);
        slider->setObjectName(QString::fromUtf8("slider"));
        sizePolicy2.setHeightForWidth(slider->sizePolicy().hasHeightForWidth());
        slider->setSizePolicy(sizePolicy2);
        slider->setMinimumSize(QSize(200, 30));
        slider->setMaximumSize(QSize(200, 30));
        slider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(slider);


        verticalLayout->addWidget(setting_wrapper_widget);

        main_splitter = new QSplitter(ResManagementPanel);
        main_splitter->setObjectName(QString::fromUtf8("main_splitter"));
        main_splitter->setStyleSheet(QString::fromUtf8("QSplitter::handle { background-color:gray; }"));
        main_splitter->setOrientation(Qt::Horizontal);
        main_splitter->setOpaqueResize(true);
        main_splitter->setChildrenCollapsible(false);
        dir_tree_wrapper_widget = new QWidget(main_splitter);
        dir_tree_wrapper_widget->setObjectName(QString::fromUtf8("dir_tree_wrapper_widget"));
        verticalLayout_2 = new QVBoxLayout(dir_tree_wrapper_widget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        dir_tree_view = new QTreeView(dir_tree_wrapper_widget);
        dir_tree_view->setObjectName(QString::fromUtf8("dir_tree_view"));
        dir_tree_view->setFrameShape(QFrame::NoFrame);
        dir_tree_view->setFrameShadow(QFrame::Sunken);
        dir_tree_view->setLineWidth(0);
        dir_tree_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        dir_tree_view->setHeaderHidden(true);
        dir_tree_view->header()->setVisible(false);

        verticalLayout_2->addWidget(dir_tree_view);

        main_splitter->addWidget(dir_tree_wrapper_widget);
        file_list_wrapper_widget = new QWidget(main_splitter);
        file_list_wrapper_widget->setObjectName(QString::fromUtf8("file_list_wrapper_widget"));
        file_list_wrapper_widget->setStyleSheet(QString::fromUtf8(""));
        main_splitter->addWidget(file_list_wrapper_widget);
        right_wrapper_widget = new QWidget(main_splitter);
        right_wrapper_widget->setObjectName(QString::fromUtf8("right_wrapper_widget"));
        right_wrapper_widget->setStyleSheet(QString::fromUtf8(".QWidget{ background-color:LightSkyBlue; }"));
        main_splitter->addWidget(right_wrapper_widget);

        verticalLayout->addWidget(main_splitter);

        setting_wrapper_widget->raise();
        caption_widget->raise();
        main_splitter->raise();

        retranslateUi(ResManagementPanel);

        QMetaObject::connectSlotsByName(ResManagementPanel);
    } // setupUi

    void retranslateUi(QWidget *ResManagementPanel)
    {
        ResManagementPanel->setWindowTitle(QCoreApplication::translate("ResManagementPanel", "Form", nullptr));
        caption_label->setText(QCoreApplication::translate("ResManagementPanel", "\350\265\204\346\272\220\347\256\241\347\220\206", nullptr));
        close_btn->setText(QCoreApplication::translate("ResManagementPanel", "\345\205\263\351\227\255", nullptr));
        search_key_edit->setPlaceholderText(QCoreApplication::translate("ResManagementPanel", "\346\220\234\347\264\242\346\226\207\344\273\266\345\217\212\346\226\207\344\273\266\345\244\271", nullptr));
        search_setting_btn->setText(QCoreApplication::translate("ResManagementPanel", "\346\220\234\347\264\242\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResManagementPanel: public Ui_ResManagementPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESMANAGEMENTPANEL_H
