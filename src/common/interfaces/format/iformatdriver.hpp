/**
 * Addle source code
 * @file
 * @copyright Copyright 2020 Eleanor Hawk
 * @copyright Modification and distribution permitted under the terms of the
 * MIT License. See "LICENSE" for full details.
 */

#ifndef IFORMATDRIVER_HPP
#define IFORMATDRIVER_HPP

#include <QString>
#include <QList>
#include <QByteArray>
#include <QIODevice>
#include <type_traits>

#include "interfaces/traits.hpp"

#include "utilities/format/importexportinfo.hpp"

#include "idtypes/formatid.hpp"

namespace Addle {

template<class ModelType>
class IFormatDriver
{
public:
    virtual ~IFormatDriver() = default;

    //virtual QString name() = 0;

    virtual bool supportsImport() const = 0;
    virtual bool supportsExport() const = 0;

    virtual FormatId<ModelType> id() const = 0;

    virtual ModelType* importModel(QIODevice& device, ImportExportInfo<ModelType> info) = 0;
    virtual void exportModel(ModelType* model, QIODevice& device, ImportExportInfo<ModelType> info) = 0;
};

typedef IFormatDriver<IDocument> IDocumentFormatDriver;

DECL_PERSISTENT_OBJECT_TYPE(IDocumentFormatDriver, DocumentFormatId);

} // namespace Addle
#endif // IFORMATDRIVER_HPP