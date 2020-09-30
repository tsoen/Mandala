#include "dimensionspinbox.h"

DimensionSpinBox::DimensionSpinBox(QWidget *parent) : QSpinBox (parent){}

int DimensionSpinBox::valueFromText(const QString &text) const
{
    static const QRegularExpression regExp(tr("(\\d+)(\\s*[xx]\\s*\\d+)?"));
    Q_ASSERT(regExp.isValid());

    const QRegularExpressionMatch match = regExp.match(text);
    if (match.isValid())
        return match.captured(1).toInt();
    return 0;
}

QString DimensionSpinBox::textFromValue(int value) const
{
    return tr("%1 x %1").arg(value);
}

