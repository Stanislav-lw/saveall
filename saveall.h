#ifndef SAVEALL_H
#define SAVEALL_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QFontComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QSlider>
#include <QGroupBox>
#include <QTabWidget>

template<class ...Objects>
class SaveAll : public QSettings
{
public:
    SaveAll(const QString &fileName, Objects* ...object) :
        QSettings{fileName, QSettings::Format::IniFormat}
    {
        (objects.push_back(object),...);
    }
    void save()
    {
        for (QObject *object : objects) {
            QObjectList children = object->children();
            if (children.isEmpty()) return;
            for (QObject *child : children) {
                if (child->isWidgetType()) {
                    saveProperty(child);
                }
            }
        }
    }
    void load()
    {
        for (QObject *object : objects) {
            QObjectList children = object->children();
            if (children.isEmpty()) return;
            for (QObject *child : children) {
                if (child->isWidgetType()) {
                    loadProperty(child);
                }
            }
        }
    }
private:
    void saveProperty(QObject *object)
    {
        beginGroup(object->parent()->objectName());
        QString className = object->metaObject()->className();
        if (className == "QLineEdit") {
            setValue(object->objectName(), static_cast<QLineEdit*>(object)->text());
        } else if (QPushButton *pb = static_cast<QPushButton*>(object); className == "QPushButton" && pb->isCheckable()) {
            setValue(object->objectName(), pb->isChecked());
        } else if (className == "QDoubleSpinBox") {
            setValue(object->objectName(), static_cast<QDoubleSpinBox*>(object)->value());
        } else if (className == "QSpinBox") {
            setValue(object->objectName(), static_cast<QSpinBox*>(object)->value());
        } else if (className == "QCheckBox") {
            setValue(object->objectName(), static_cast<QCheckBox*>(object)->isChecked());
        } else if (className == "QRadioButton"){
            setValue(object->objectName(), static_cast<QRadioButton*>(object)->isChecked());
        } else if (className == "QComboBox" || className == "QFontComboBox") {
            setValue(object->objectName(), static_cast<QComboBox*>(object)->currentIndex());
        } else if (className == "QSlider") {
            setValue(object->objectName(), static_cast<QSlider*>(object)->value());
        } else if (className == "QTabWidget") {
            setValue(object->objectName(), static_cast<QTabWidget*>(object)->currentIndex());
        } else if (QGroupBox *gb = static_cast<QGroupBox*>(object); className == "QGroupBox" && gb->isCheckable()) {
            setValue(object->objectName(), gb->isChecked());
        }
        endGroup();
    }
    void loadProperty(QObject *object)
    {
        beginGroup(object->parent()->objectName());
        QString className = object->metaObject()->className();
        if (className == "QLineEdit"){
            static_cast<QLineEdit*>(object)->setText(value(object->objectName()).toString());
        } else if (QPushButton *pb = static_cast<QPushButton*>(object); className == "QPushButton" && pb->isCheckable()) {
            pb->setChecked(value(object->objectName()).toBool());
        } else if (className == "QDoubleSpinBox") {
            static_cast<QDoubleSpinBox*>(object)->setValue(value(object->objectName()).toDouble());
        } else if (className == "QSpinBox") {
            static_cast<QSpinBox*>(object)->setValue(value(object->objectName()).toInt());
        } else if (className == "QCheckBox") {
            static_cast<QCheckBox*>(object)->setChecked(value(object->objectName()).toBool());
        } else if (className == "QRadioButton") {
            static_cast<QRadioButton*>(object)->setChecked(value(object->objectName()).toBool());
        } else if (className == "QComboBox" || className == "QFontComboBox") {
            static_cast<QComboBox*>(object)->setCurrentIndex(value(object->objectName()).toInt());
        } else if (className == "QSlider") {
            static_cast<QSlider*>(object)->setValue(value(object->objectName()).toInt());
        } else if (className == "QTabWidget") {
            static_cast<QTabWidget*>(object)->setCurrentIndex(value(object->objectName()).toInt());
        } else if (QGroupBox *gb = static_cast<QGroupBox*>(object); className == "QGroupBox" && gb->isCheckable()) {
            gb->setChecked(value(object->objectName()).toBool());
        }
        endGroup();
    }
private:
    QVector<QObject*> objects;
};

#endif // SAVEALL_H
