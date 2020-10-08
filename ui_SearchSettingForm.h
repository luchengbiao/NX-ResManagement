/********************************************************************************
** Form generated from reading UI file 'SearchSettingForm.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHSETTINGFORM_H
#define UI_SEARCHSETTINGFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SearchSettingForm
{
public:
    QVBoxLayout *verticalLayout_3;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *exact_match_radio_btn;
    QRadioButton *fuzzy_match_radio_btn;
    QCheckBox *case_senditivity_check_box;
    QGroupBox *file_type_group_box;
    QVBoxLayout *verticalLayout;
    QCheckBox *scene_check_box;
    QCheckBox *light_check_box;
    QCheckBox *model_check_box;
    QCheckBox *animation_check_box;
    QCheckBox *effect_check_box;
    QCheckBox *shader_check_box;
    QCheckBox *material_check_box;
    QCheckBox *mapping_check_box;
    QCheckBox *audio_check_box;
    QCheckBox *video_check_box;
    QCheckBox *ui_check_box;
    QSpacerItem *verticalSpacer;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *ok_btn;
    QSpacerItem *horizontalSpacer_2;
    QButtonGroup *match_btn_group;

    void setupUi(QWidget *SearchSettingForm)
    {
        if (SearchSettingForm->objectName().isEmpty())
            SearchSettingForm->setObjectName(QString::fromUtf8("SearchSettingForm"));
        SearchSettingForm->resize(283, 434);
        verticalLayout_3 = new QVBoxLayout(SearchSettingForm);
        verticalLayout_3->setSpacing(16);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 16, 0, 8);
        widget = new QWidget(SearchSettingForm);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setSpacing(8);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(8, 0, 0, 0);
        exact_match_radio_btn = new QRadioButton(widget);
        match_btn_group = new QButtonGroup(SearchSettingForm);
        match_btn_group->setObjectName(QString::fromUtf8("match_btn_group"));
        match_btn_group->addButton(exact_match_radio_btn);
        exact_match_radio_btn->setObjectName(QString::fromUtf8("exact_match_radio_btn"));
        exact_match_radio_btn->setChecked(true);

        verticalLayout_2->addWidget(exact_match_radio_btn);

        fuzzy_match_radio_btn = new QRadioButton(widget);
        match_btn_group->addButton(fuzzy_match_radio_btn);
        fuzzy_match_radio_btn->setObjectName(QString::fromUtf8("fuzzy_match_radio_btn"));

        verticalLayout_2->addWidget(fuzzy_match_radio_btn);

        case_senditivity_check_box = new QCheckBox(widget);
        case_senditivity_check_box->setObjectName(QString::fromUtf8("case_senditivity_check_box"));

        verticalLayout_2->addWidget(case_senditivity_check_box);


        verticalLayout_3->addWidget(widget);

        file_type_group_box = new QGroupBox(SearchSettingForm);
        file_type_group_box->setObjectName(QString::fromUtf8("file_type_group_box"));
        verticalLayout = new QVBoxLayout(file_type_group_box);
        verticalLayout->setSpacing(8);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(16, 8, 8, 0);
        scene_check_box = new QCheckBox(file_type_group_box);
        scene_check_box->setObjectName(QString::fromUtf8("scene_check_box"));

        verticalLayout->addWidget(scene_check_box);

        light_check_box = new QCheckBox(file_type_group_box);
        light_check_box->setObjectName(QString::fromUtf8("light_check_box"));

        verticalLayout->addWidget(light_check_box);

        model_check_box = new QCheckBox(file_type_group_box);
        model_check_box->setObjectName(QString::fromUtf8("model_check_box"));

        verticalLayout->addWidget(model_check_box);

        animation_check_box = new QCheckBox(file_type_group_box);
        animation_check_box->setObjectName(QString::fromUtf8("animation_check_box"));

        verticalLayout->addWidget(animation_check_box);

        effect_check_box = new QCheckBox(file_type_group_box);
        effect_check_box->setObjectName(QString::fromUtf8("effect_check_box"));

        verticalLayout->addWidget(effect_check_box);

        shader_check_box = new QCheckBox(file_type_group_box);
        shader_check_box->setObjectName(QString::fromUtf8("shader_check_box"));

        verticalLayout->addWidget(shader_check_box);

        material_check_box = new QCheckBox(file_type_group_box);
        material_check_box->setObjectName(QString::fromUtf8("material_check_box"));

        verticalLayout->addWidget(material_check_box);

        mapping_check_box = new QCheckBox(file_type_group_box);
        mapping_check_box->setObjectName(QString::fromUtf8("mapping_check_box"));

        verticalLayout->addWidget(mapping_check_box);

        audio_check_box = new QCheckBox(file_type_group_box);
        audio_check_box->setObjectName(QString::fromUtf8("audio_check_box"));

        verticalLayout->addWidget(audio_check_box);

        video_check_box = new QCheckBox(file_type_group_box);
        video_check_box->setObjectName(QString::fromUtf8("video_check_box"));

        verticalLayout->addWidget(video_check_box);

        ui_check_box = new QCheckBox(file_type_group_box);
        ui_check_box->setObjectName(QString::fromUtf8("ui_check_box"));

        verticalLayout->addWidget(ui_check_box);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        verticalLayout_3->addWidget(file_type_group_box);

        widget_2 = new QWidget(SearchSettingForm);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ok_btn = new QPushButton(widget_2);
        ok_btn->setObjectName(QString::fromUtf8("ok_btn"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ok_btn->sizePolicy().hasHeightForWidth());
        ok_btn->setSizePolicy(sizePolicy);
        ok_btn->setLayoutDirection(Qt::LeftToRight);
        ok_btn->setCheckable(false);

        horizontalLayout->addWidget(ok_btn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_3->addWidget(widget_2);


        retranslateUi(SearchSettingForm);

        QMetaObject::connectSlotsByName(SearchSettingForm);
    } // setupUi

    void retranslateUi(QWidget *SearchSettingForm)
    {
        SearchSettingForm->setWindowTitle(QCoreApplication::translate("SearchSettingForm", "\346\220\234\347\264\242\350\256\276\347\275\256", nullptr));
        exact_match_radio_btn->setText(QCoreApplication::translate("SearchSettingForm", "\347\262\276\347\241\256\345\214\271\351\205\215", nullptr));
        fuzzy_match_radio_btn->setText(QCoreApplication::translate("SearchSettingForm", "\346\250\241\347\263\212\345\214\271\351\205\215 ?", nullptr));
        case_senditivity_check_box->setText(QCoreApplication::translate("SearchSettingForm", "\345\244\247\345\260\217\345\206\231\346\225\217\346\204\237", nullptr));
        file_type_group_box->setTitle(QCoreApplication::translate("SearchSettingForm", "\345\217\252\346\230\276\347\244\272\344\273\245\344\270\213\347\261\273\345\236\213\346\226\207\344\273\266:", nullptr));
        scene_check_box->setText(QCoreApplication::translate("SearchSettingForm", "\345\234\272\346\231\257", nullptr));
        light_check_box->setText(QCoreApplication::translate("SearchSettingForm", "\347\201\257\345\205\211", nullptr));
        model_check_box->setText(QCoreApplication::translate("SearchSettingForm", "\346\250\241\345\236\213", nullptr));
        animation_check_box->setText(QCoreApplication::translate("SearchSettingForm", "\345\212\250\347\224\273", nullptr));
        effect_check_box->setText(QCoreApplication::translate("SearchSettingForm", "\347\211\271\346\225\210", nullptr));
        shader_check_box->setText(QCoreApplication::translate("SearchSettingForm", "Shader", nullptr));
        material_check_box->setText(QCoreApplication::translate("SearchSettingForm", "\346\235\220\350\264\250", nullptr));
        mapping_check_box->setText(QCoreApplication::translate("SearchSettingForm", "\350\264\264\345\233\276", nullptr));
        audio_check_box->setText(QCoreApplication::translate("SearchSettingForm", "\351\237\263\346\225\210", nullptr));
        video_check_box->setText(QCoreApplication::translate("SearchSettingForm", "\350\247\206\351\242\221", nullptr));
        ui_check_box->setText(QCoreApplication::translate("SearchSettingForm", "UI", nullptr));
        ok_btn->setText(QCoreApplication::translate("SearchSettingForm", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SearchSettingForm: public Ui_SearchSettingForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHSETTINGFORM_H
