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
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SearchSettingForm
{
public:

    void setupUi(QWidget *SearchSettingForm)
    {
        if (SearchSettingForm->objectName().isEmpty())
            SearchSettingForm->setObjectName(QString::fromUtf8("SearchSettingForm"));
        SearchSettingForm->resize(275, 300);

        retranslateUi(SearchSettingForm);

        QMetaObject::connectSlotsByName(SearchSettingForm);
    } // setupUi

    void retranslateUi(QWidget *SearchSettingForm)
    {
        SearchSettingForm->setWindowTitle(QCoreApplication::translate("SearchSettingForm", "\346\220\234\347\264\242\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SearchSettingForm: public Ui_SearchSettingForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHSETTINGFORM_H
