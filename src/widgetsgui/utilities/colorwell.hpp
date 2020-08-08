#ifndef COLORWELL_HPP
#define COLORWELL_HPP

#include <QFrame>
#include "utilities/model/colorinfo.hpp"

class ColorWell : public QFrame 
{
    Q_OBJECT
    Q_PROPERTY(
        ColorInfo info
        READ info 
        WRITE setInfo
    )
    Q_PROPERTY(
        bool isHighlighted 
        READ isHighlighted 
        WRITE setHighlighted
    )
public:
    ColorWell(QWidget* parent = nullptr);
    virtual ~ColorWell() = default; 

    QSize sizeHint() const override;

    ColorInfo info() const { return _info; }
    void setInfo(ColorInfo info);

    QColor color() const { return _info.color(); }

    void setSize(QSize size);

    bool isEmpty() const { return _isEmpty; }
    bool isHighlighted() const { return _isHighlighted; }

    void setHighlighted(bool value);

signals:
    void clicked();
    void doubleClicked();

protected:
    void paintEvent(QPaintEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseDoubleClickEvent(QMouseEvent* e) override;

private: 
    bool _isEmpty = true;
    bool _isHighlighted = false;

    ColorInfo _info;

    QSize _size;

    QPixmap _checkerTexture;
};

#endif //COLORWELL_HPP