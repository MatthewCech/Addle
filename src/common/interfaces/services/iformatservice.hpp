#ifndef IFORMATSERVICE_HPP
#define IFORMATSERVICE_HPP

#include <QObject>
#include <QIODevice>
#include <QString>
#include <QFileInfo>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

#include "iservice.hpp"
#include "common/interfaces/models/idocument.hpp"

#include "common/interfaces/format/iformatmodel.hpp"

#include "common/interfaces/tasks/itaskstatuscontroller.hpp"

#include "common/interfaces/traits/initialize_traits.hpp"
#include "common/utilities/data/importexportinfo.hpp"
#include "common/interfaces/servicelocator/imakeable.hpp"

/**
 * @class IFormatService
 * @brief fooooo
 * 
 * @ingroup interfaces
 */
class IFormatService : public virtual IService, public virtual IMakeable
{
public:
    //virtual IDocument* loadFile(QString filename) = 0;

    template<class FormatModel>
    FormatModel* importModel(QIODevice& device, const ImportExportInfo& info, ITaskStatusController* status = nullptr)
    {
        static_assert (
            std::is_base_of<IFormatModel, FormatModel>::value,
            "FormatModel must inherit IFormatModel"
        );

        return dynamic_cast<FormatModel*>(importModel_p(typeid(FormatModel), device, info, status));
    }

protected:
    virtual IFormatModel* importModel_p(const std::type_info& modelType, QIODevice& device, const ImportExportInfo& info, ITaskStatusController* status = nullptr) = 0;
};

#endif // IFORMATSERVICE_HPP