#include "zoomrotatewidget.hpp"

#include <QHBoxLayout>

#include <cmath>

#include "utilities/qt_extensions/qobject.hpp"
#include "utilities/propertybinding.hpp"
#include "widgetsgui/utilities/widgetproperties.hpp"

ZoomRotateWidget::ZoomRotateWidget(IViewPortPresenter& presenter, QWidget* parent)
    : QToolBar(parent),
    _presenter(presenter)
{
    new PropertyBinding(
        this,
        WidgetProperties::enabled,
        qobject_interface_cast(&_presenter),
        IViewPortPresenter::Meta::Properties::canNavigate
    );

    _button_zoomIn = new QToolButton(this);
    _action_zoomIn = new QAction(this);
    _action_zoomIn->setIcon(QIcon(":/icons/zoom_in.png"));
    _button_zoomIn->setDefaultAction(_action_zoomIn);
    connect_interface(_action_zoomIn, SIGNAL(triggered()), &_presenter, SLOT(zoomIn()));
    new PropertyBinding(
        _action_zoomIn,
        WidgetProperties::enabled,
        qobject_interface_cast(&_presenter), 
        IViewPortPresenter::Meta::Properties::canZoomIn,
        PropertyBinding::ReadOnly
    );

    _button_zoomOut = new QToolButton(this);
    _action_zoomOut = new QAction(this);
    _action_zoomOut->setIcon(QIcon(":/icons/zoom_out.png"));
    _button_zoomOut->setDefaultAction(_action_zoomOut);
    connect_interface(_action_zoomOut, SIGNAL(triggered()), &_presenter, SLOT(zoomOut()));
    new PropertyBinding(
        _action_zoomOut,
        WidgetProperties::enabled,
        qobject_interface_cast(&_presenter), 
        IViewPortPresenter::Meta::Properties::canZoomOut,
        PropertyBinding::ReadOnly
    );

    _button_turntableCW = new QToolButton(this);
    _action_turntableCW = new QAction(this);
    _action_turntableCW->setIcon(QIcon(":/icons/turntable_cw.png"));
    _button_turntableCW->setDefaultAction(_action_turntableCW);
    connect_interface(_action_turntableCW, SIGNAL(triggered()), &_presenter, SLOT(turntableCw()));

    _button_turntableCCW = new QToolButton(this);
    _action_turntableCCW = new QAction(this);
    _action_turntableCCW->setIcon(QIcon(":/icons/turntable_ccw.png"));
    _button_turntableCCW->setDefaultAction(_action_turntableCCW);
    connect_interface(_action_turntableCCW, SIGNAL(triggered()), &_presenter, SLOT(turntableCcw()));

    _button_reset = new QToolButton(this);
    _action_reset = new QAction(this);
    _action_reset->setIcon(QIcon(":/icons/reset_view.png"));
    _button_reset->setDefaultAction(_action_reset);
    connect_interface(_action_reset, SIGNAL(triggered()), &_presenter, SLOT(resetView()));

    _slider_zoom = new ZoomSlider(this);
    _slider_zoom->setMinZoom(_presenter.getMinZoomPresetValue());
    _slider_zoom->setMaxZoom(_presenter.getMaxZoomPresetValue());
    _slider_zoom->setOrientation(Qt::Orientation::Horizontal);
    _slider_zoom->setMinimumSize(QSize(150, 0));
    _slider_zoom->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    new PropertyBinding(
        _slider_zoom,
        IViewPortPresenter::Meta::Properties::zoom,
        qobject_interface_cast(&_presenter),
        ZoomSlider::ZOOM_PROPERTY_NAME
    );

    _spinBox_zoomPercent = new QDoubleSpinBox(this);
    _spinBox_zoomPercent->setMinimum(_presenter.getMinZoomPresetValue() * 100);
    _spinBox_zoomPercent->setMaximum(_presenter.getMaxZoomPresetValue() * 100);
    _spinBox_zoomPercent->setSingleStep(5.0);

    new PropertyBinding(
        _spinBox_zoomPercent,
        WidgetProperties::value,
        qobject_interface_cast(&_presenter),
        IViewPortPresenter::Meta::Properties::zoom,
        PropertyBinding::ReadWrite,
        BindingConverter::scale(100)
    );

    // QHBoxLayout* layout = new QHBoxLayout(this);
    // QWidget::setLayout(layout);

    // layout->addWidget(_button_turntableCCW);
    // layout->addWidget(_button_zoomOut);
    // layout->addWidget(_slider_zoom);
    // layout->addWidget(_spinBox_zoomPercent);
    // layout->addWidget(_button_zoomIn);
    // layout->addWidget(_button_turntableCW);
    // layout->addWidget(_button_reset);

    addWidget(_button_turntableCCW);
    addWidget(_button_zoomOut);
    addWidget(_slider_zoom);
    addWidget(_spinBox_zoomPercent);
    addWidget(_button_zoomIn);
    addWidget(_button_turntableCW);
    addWidget(_button_reset);
}