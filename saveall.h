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
#include <QToolBox>
#include <QScrollArea>
#include <QGroupBox>
#include <QTabWidget>
#include <QStackedWidget>

template<class ...Objects>
class SaveAll : public QSettings
{
public:
    SaveAll(const QString &fileName, Objects* ...object) :
        QSettings{fileName, Format::IniFormat}
    {
        (objects_.push_back(object),...);
    }
    void unbindObject(const QObject* object)
    {
        unbindedObjects_.push_back(object->objectName());
    }
    void save()
    {
        for (QObject *object : objects_) {
            saveProperties(object);
        }
    }
    void load()
    {
        for (QObject *object : objects_) {
            loadProperties(object);
        }
    }
private:
    void saveProperties(QObject* object)
    {
        QObjectList children = object->children();
        if (children.isEmpty()) return;
        beginGroup(object->objectName());
        for (QObject *child : children) {
            bool objectFound = unbindedObjects.contains(child->objectName());
            if (child->isWidgetType() && !objectFound) {
                saveProperty(child);
            }
        }
        endGroup();
    }
    void saveProperty(QObject* object)
    {
        QString className = object->metaObject()->className();
        if (className == "QLineEdit") {
            setValue(object->objectName(), static_cast<QLineEdit*>(object)->text());
        } else if ( className == "QPushButton") {
            QPushButton *pb = static_cast<QPushButton*>(object);
            if (pb->isCheckable()) setValue(object->objectName(), pb->isChecked());
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
        } else if (className == "QWidget") {
            saveProperties(object);
        } else if (className == "QStackedWidget") {
            setValue(object->objectName(), static_cast<QStackedWidget*>(object)->currentIndex());
            saveProperties(object);
        } else if (className == "QTabWidget") {
            setValue(object->objectName(), static_cast<QTabWidget*>(object)->currentIndex());
            saveProperties(object);
        } else if (className == "QToolBox") {
            setValue(object->objectName(), static_cast<QToolBox*>(object)->currentIndex());
            saveProperties(object);
        } else if (className == "QScrollArea") {
            saveProperties(object);
        } else if (className == "QGroupBox") {
            QGroupBox *gb = static_cast<QGroupBox*>(object);
            if (gb->isCheckable()) setValue(object->objectName(), gb->isChecked());
            saveProperties(gb);
        }
    }
    void loadProperties(QObject* object)
    {
        QObjectList children = object->children();
        if (children.isEmpty()) return;
        if (childGroups().isEmpty()) return;
        beginGroup(object->objectName());
        for (QObject *child : children) {
            bool objectFound = unbindedObjects.contains(child->objectName());
            if (child->isWidgetType() && !objectFound) {
                loadProperty(child);
            }
        }
        endGroup();
    }
    void loadProperty(QObject* object)
    {
        QString className = object->metaObject()->className();
        if (className == "QLineEdit") {
            static_cast<QLineEdit*>(object)->setText(value(object->objectName()).toString());
        } else if (className == "QPushButton") {
            QPushButton *pb = static_cast<QPushButton*>(object);
            if (pb->isCheckable()) pb->setChecked(value(object->objectName()).toBool());
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
        } else if (className == "QWidget") {
            loadProperties(object);
        } else if (className == "QStackedWidget") {
            static_cast<QStackedWidget*>(object)->setCurrentIndex(value(object->objectName()).toInt());
            loadProperties(object);
        }else if (className == "QTabWidget") {
            static_cast<QTabWidget*>(object)->setCurrentIndex(value(object->objectName()).toInt());
            loadProperties(object);
        } else if (className == "QToolBox") {
            static_cast<QToolBox*>(object)->setCurrentIndex(value(object->objectName()).toInt());
            loadProperties(object);
        } else if (className == "QScrollArea") {
            loadProperties(object);
        } else if (className == "QGroupBox") {
            QGroupBox *gb = static_cast<QGroupBox*>(object);
            if (gb->isCheckable()) gb->setChecked(value(object->objectName()).toBool());
            loadProperties(gb);
        }
    }
private:
    QVector<QString> unbindedObjects_;
    QVector<QObject*> objects_;
};

#endif // SAVEALL_H
