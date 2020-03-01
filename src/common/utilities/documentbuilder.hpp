#ifndef DOCUMENTBUILDER_HPP
#define DOCUMENTBUILDER_HPP

#include <QString>
#include <QSharedData>
#include <QSharedDataPointer>
#include "layerbuilder.hpp"

class DocumentBuilder
{
    struct DocumentBuilderData : QSharedData
    {
        QString filename;
        QList<LayerBuilder> layers;
    };
public:
    DocumentBuilder() { _data = new DocumentBuilderData; }
    DocumentBuilder(const DocumentBuilder& other) : _data(other._data) {}

    QString getFilename() { return _data->filename; }
    void setFilename(QString filename) { _data->filename = filename; }

    void addLayer(LayerBuilder& layer) { _data->layers.append(layer); }
    QList<LayerBuilder> getLayers() { return _data->layers; }

private:
    QSharedDataPointer<DocumentBuilderData> _data;
};

#endif // DOCUMENTBUILDER_HPP