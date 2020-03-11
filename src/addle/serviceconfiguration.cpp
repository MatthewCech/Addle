/**
 * Addle source code
 * Copyright 2019 Eleanor Hawk
 * Modification and distribution permitted under the terms of the MIT License.
 * See "LICENSE" for full details.
 */

#include "serviceconfiguration.hpp"

#include "utilities/configuration/tfactory.hpp"
#include "utilities/configuration/qobjectfactory.hpp"

#include "core/editing/brushrenderers/basicbrushrenderer.hpp"
#include "core/editing/operations/brushoperation.hpp"
#include "core/editing/surfaces/rastersurface.hpp"

#include "core/models/layer.hpp"
#include "core/models/document.hpp"
#include "core/models/brush.hpp"

#include "core/presenters/editorpresenter.hpp"
#include "core/presenters/canvaspresenter.hpp"
#include "core/presenters/viewportpresenter.hpp"
#include "core/presenters/errorpresenter.hpp"
#include "core/presenters/layerpresenter.hpp"
#include "core/presenters/assets/brushpresenter.hpp"
#include "core/presenters/toolpresenters/navigatetoolpresenter.hpp"
#include "core/presenters/toolpresenters/brushtoolpresenter.hpp"

#include "core/services/applicationservice.hpp"
#include "core/services/formatservice.hpp"
#include "core/services/taskservice.hpp"
#include "core/services/brushrepository.hpp"

#include "core/format/qtformatdrivers/qtpngformatdriver.hpp"
#include "core/format/qtformatdrivers/qtjpegformatdriver.hpp"

#include "core/tasks/taskcontroller.hpp"
#include "core/tasks/loaddocumentfiletask.hpp"

#include "widgetsgui/main/editorview.hpp"
#include "widgetsgui/gvfcanvas/viewportwidget.hpp"

void ServiceConfiguration::configure()
{
    // # Editing
    // ## Brush renderers
    REGISTER_TFACTORY(IBasicBrushRenderer, BasicBrushRenderer);

    // ## Operations
    REGISTER_QOBJECT_FACTORY(IBrushOperation, BrushOperation);

    // ## Surfaces
    REGISTER_QOBJECT_FACTORY(IRasterSurface, RasterSurface);

    // # Models
    REGISTER_TFACTORY(ILayer, Layer);
    REGISTER_TFACTORY(IDocument, Document);
    REGISTER_TFACTORY(IBrush, Brush);

    // # Presenters
    REGISTER_TFACTORY(IEditorPresenter, EditorPresenter);
    REGISTER_TFACTORY(ICanvasPresenter, CanvasPresenter);
    REGISTER_TFACTORY(IViewPortPresenter, ViewPortPresenter);
    REGISTER_TFACTORY(IErrorPresenter, ErrorPresenter);
    REGISTER_QOBJECT_FACTORY(ILayerPresenter, LayerPresenter);

    // ## Asset presenters
    REGISTER_QOBJECT_FACTORY(IBrushPresenter, BrushPresenter);

    // ## Tool presenters
    REGISTER_QOBJECT_FACTORY(INavigateToolPresenter, NavigateToolPresenter);
    REGISTER_QOBJECT_FACTORY(IBrushToolPresenter, BrushToolPresenter);

    // # Services
    REGISTER_QOBJECT_FACTORY(IApplicationService, ApplicationService);
    REGISTER_TFACTORY(IFormatService, FormatService);
    REGISTER_QOBJECT_FACTORY(ITaskService, TaskService);
    REGISTER_QOBJECT_FACTORY(IBrushRepository, BrushRepository);

    // # Tasks
    REGISTER_QOBJECT_FACTORY(ITaskController, TaskController);
    REGISTER_TFACTORY(ILoadDocumentFileTask, LoadDocumentFileTask);

    // # Formats
    REGISTER_TFACTORY(IPNGFormatDriver, QtPNGFormatDriver);
    REGISTER_TFACTORY(IJPEGFormatDriver, QtJPEGFormatDriver);

    // # Views
    REGISTER_QOBJECT_FACTORY(IEditorView, EditorView);
    REGISTER_QOBJECT_FACTORY(IViewPort, GVFCanvas::ViewPortWidget);
}
