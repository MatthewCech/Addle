/**
 * Addle source code
 * @file
 * @copyright Copyright 2020 Eleanor Hawk
 * @copyright Modification and distribution permitted under the terms of the
 * MIT License. See "LICENSE" for full details.
 */

#include "maineditorview.hpp"

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QCloseEvent>

#include "zoomrotatewidget.hpp"
#include "viewportscrollwidget.hpp"

#include "utils.hpp"

#include "interfaces/presenters/tools/itoolpresenter.hpp"
#include "interfaces/services/iapplicationsservice.hpp"

#include "layers/layersmanager.hpp"

#include "colorselector.hpp"
#include "viewport.hpp"

#include "utilities/qobject.hpp"
#include "utilities/presenter/propertybinding.hpp"
#include "utilities/widgetproperties.hpp"

#include "helpers/toolsetuphelper.hpp"

#include "tooloptionsbars/brushtooloptionsbar.hpp"
#include "tooloptionsbars/navigatetooloptionsbar.hpp"

using namespace Addle;

MainEditorWindow::MainEditorWindow(IMainEditorPresenter& presenter)
    : _presenter(presenter)
{
    connect_interface(&_presenter, SIGNAL(error(QSharedPointer<IErrorPresenter>)),
                              this, SLOT(onPresenterError(QSharedPointer<IErrorPresenter>)));
                            
    connect_interface(&_presenter, SIGNAL(undoStateChanged()),
                              this, SLOT(onUndoStateChanged()));

    connect_interface(&_presenter, SIGNAL(documentPresenterChanged(QSharedPointer<IDocumentPresenter>)),
                              this, SLOT(onDocumentChanged(QSharedPointer<IDocumentPresenter>)));
}

void MainEditorWindow::setupUi()
{
    _menuBar = new QMenuBar(this);
    QMainWindow::setMenuBar(_menuBar);

    _toolBar_documentActions = new QToolBar(this);
    _toolBar_documentActions->setWindowTitle(qtTrId("ui.document-actions.title"));
    QMainWindow::addToolBar(Qt::ToolBarArea::TopToolBarArea, _toolBar_documentActions);

    _toolBar_editorToolSelection = new QToolBar(this);
    _toolBar_editorToolSelection->setWindowTitle(qtTrId("ui.tool-selection.title"));
    QMainWindow::addToolBar(Qt::ToolBarArea::LeftToolBarArea, _toolBar_editorToolSelection);

    new PropertyBinding(
        _toolBar_editorToolSelection,
        WidgetProperties::enabled,
        qobject_interface_cast(&_presenter),
        IMainEditorPresenter::Meta::Properties::empty,
        PropertyBinding::ReadOnly,
        BindingConverter::negate()
    );

    _viewPort = new ViewPort(_presenter.viewPortPresenter());
    _viewPortScrollWidget = new ViewPortScrollWidget(_presenter.viewPortPresenter(), this);
    _viewPortScrollWidget->setViewPort(_viewPort);
    QMainWindow::setCentralWidget(_viewPortScrollWidget);
    _viewPort->setFocus();
    
    connect_interface(&_presenter, SIGNAL(isEmptyChanged(bool)), this, SLOT(onPresenterEmptyChanged(bool)));

    _statusBar = new QStatusBar(this);
    QMainWindow::setStatusBar(_statusBar);

    _zoomRotateWidget = new ZoomRotateWidget(_presenter.viewPortPresenter(), this);
    _statusBar->addPermanentWidget(_zoomRotateWidget);
    _zoomRotateWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    _action_open = new QAction(this);
    _action_open->setIcon(ADDLE_ICON("open"));
    _action_open->setToolTip(qtTrId("ui.open.description"));
    connect(_action_open, &QAction::triggered, this, &MainEditorWindow::onAction_open);

    _optionGroup_toolSelection = new OptionGroup(this);
    new PropertyBinding(
        _optionGroup_toolSelection,
        WidgetProperties::value,
        qobject_interface_cast(&_presenter),
        IMainEditorPresenter::Meta::Properties::currentTool
    );

    _action_new = new QAction(this);
    _action_new->setIcon(ADDLE_ICON("new"));
    _action_new->setToolTip(qtTrId("ui.new.description"));
    connect_interface(_action_new, SIGNAL(triggered()), &_presenter, SLOT(newDocument()));

    _action_undo = new QAction(this);
    _action_undo->setIcon(ADDLE_ICON("undo"));
    _action_undo->setToolTip(qtTrId("ui.undo.description"));
    _action_undo->setEnabled(_presenter.canUndo());
    connect_interface(_action_undo, SIGNAL(triggered()), &_presenter, SLOT(undo()));

    _action_redo = new QAction(this);
    _action_redo->setIcon(ADDLE_ICON("redo"));
    _action_redo->setToolTip(qtTrId("ui.redo.description"));
    _action_redo->setEnabled(_presenter.canRedo());
    connect_interface(_action_redo, SIGNAL(triggered()), &_presenter, SLOT(redo()));
    
    _toolBar_documentActions->addAction(_action_new);
    _toolBar_documentActions->addAction(_action_open);
    _toolBar_documentActions->addSeparator();
    _toolBar_documentActions->addAction(_action_undo);
    _toolBar_documentActions->addAction(_action_redo);

    ToolSetupHelper setupHelper(
        this,
        _presenter,
        _optionGroup_toolSelection
    );

    setupHelper.addTool(
        CoreTools::Brush,
        &_action_selectBrushTool,
        &_optionsToolBar_brush
    );

    setupHelper.addTool(
        CoreTools::Eraser,
        &_action_selectEraserTool,
        &_optionsToolBar_eraser
    );

    setupHelper.addTool(
        CoreTools::Navigate,
        &_action_selectNavigateTool,
        &_optionsToolBar_navigate
    );

    _toolBar_editorToolSelection->addAction(new QAction(ADDLE_ICON("select-tool"), "", this));

    _toolBar_editorToolSelection->addSeparator();

    _toolBar_editorToolSelection->addAction(_action_selectBrushTool);
    _toolBar_editorToolSelection->addAction(_action_selectEraserTool);
    _toolBar_editorToolSelection->addAction(new QAction(ADDLE_ICON("fill-tool"), "", this));

    _toolBar_editorToolSelection->addSeparator();

    _toolBar_editorToolSelection->addAction(new QAction(ADDLE_ICON("text-tool"), "", this));
    _toolBar_editorToolSelection->addAction(new QAction(ADDLE_ICON("shapes-tool"), "", this));
    _toolBar_editorToolSelection->addAction(new QAction(ADDLE_ICON("stickers-tool"), "", this));

    _toolBar_editorToolSelection->addSeparator();

    _toolBar_editorToolSelection->addAction(_action_selectNavigateTool);
    _toolBar_editorToolSelection->addAction(new QAction(ADDLE_ICON("eyedrop-tool"), "", this));
    _toolBar_editorToolSelection->addAction(new QAction(ADDLE_ICON("measure-tool"), "", this));

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    _layersManager = new LayersManager(this);
    addDockWidget(Qt::RightDockWidgetArea, _layersManager);

    _colorSelector = new ColorSelector(_presenter.colorSelection(), this);

    addDockWidget(Qt::BottomDockWidgetArea, _colorSelector);
}

void MainEditorWindow::onUndoStateChanged()
{
    //TODO: replace with PropertyBindings

    _action_undo->setEnabled(_presenter.canUndo());
    _action_redo->setEnabled(_presenter.canRedo());
}

void MainEditorWindow::onAction_open()
{
    QUrl file = QFileDialog::getOpenFileUrl(
        this, 
        qtTrId("ui.open-document.title"),
        QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation))
    );

    if (!file.isEmpty())
        _presenter.loadDocument(file);
}

void MainEditorWindow::onPresenterError(QSharedPointer<IErrorPresenter> error)
{
    QMessageBox* message = new QMessageBox(); 

    message->setWindowModality(Qt::WindowModal);
    message->setAttribute(Qt::WA_DeleteOnClose);

    message->setText(error->message());
    if (error->severity() == IErrorPresenter::Warning)
    {
        message->setIcon(QMessageBox::Icon::Warning);
    }

#ifdef ADDLE_DEBUG
    if (error->exception())
    {
        // thanks https://stackoverflow.com/a/38371503/2808947
        message->setStyleSheet(
        "QTextEdit {"
            "font-family: monospace;"
            "min-width: 720px;"
        "}");
        message->setDetailedText(error->exception()->what());
    }
#endif

    message->show();
}

void MainEditorWindow::onToolBarNeedsShown()
{
    QToolBar* toolBar = qobject_cast<QToolBar*>(sender());

    toolBar->show();
    addToolBar(toolBar);
}

void MainEditorWindow::onToolBarNeedsHidden()
{
    QToolBar* toolBar = qobject_cast<QToolBar*>(sender());

    toolBar->hide();
    removeToolBar(toolBar);
}

void MainEditorWindow::onPresenterEmptyChanged(bool empty)
{
    if (!empty && _viewPort)
        _viewPort->setFocus();
}

void MainEditorWindow::onDocumentChanged(QSharedPointer<IDocumentPresenter> document)
{
    _layersManager->setPresenter(document);
}

void MainEditorWindow::closeEvent(QCloseEvent* event)
{
    // the presenter will probably have something to say about this

    event->accept();
    closeEventAccepted();
}

void MainEditorView::initialize(IMainEditorPresenter& presenter)
{
    const Initializer init(_initHelper);

    _presenter = &presenter;

    _window = std::unique_ptr<MainEditorWindow>(new MainEditorWindow(presenter));

    connect(_window.get(), &MainEditorWindow::destroyed, this, &MainEditorView::deleteLater);
    connect(_window.get(), &MainEditorWindow::closeEventAccepted, this, &MainEditorView::closed);
}

void MainEditorView::show()
{
    if (!_uiIsSetup)
    {
        _window->setupUi();
        _uiIsSetup = true;
    }
    _window->show();
}

void MainEditorView::close()
{
    _window->close();
}