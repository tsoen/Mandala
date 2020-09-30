#ifndef DIMENSIONSPINBOX_H
#define DIMENSIONSPINBOX_H

#include <QSpinBox>
#include <QWidget>
#include <QString>
#include <QRegularExpression>

class DimensionSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    int valueFromText(const QString &text) const;
    QString textFromValue(int value) const;

public:
    explicit DimensionSpinBox(QWidget *parent = nullptr);
    using QSpinBox::QSpinBox;
};

#endif // DIMENSIONSPINBOX_H
