#ifndef RASTERBITHANDLES_HPP
#define RASTERBITHANDLES_HPP

#include <QRect>
#include <QImage>

class IRasterSurface;
class RasterBitReader
{
public:
    RasterBitReader(RasterBitReader&& other);
    ~RasterBitReader();

    inline QImage::Format format() { return _buffer.format(); }

    inline QPoint offset() const { return _area.topLeft() - _bufferOffset; }

    inline int pixelWidth() const { return _pixelWidth; }
    inline QRect area() const { return _area; }

    const uchar* scanLine(int line) const;

private:
    RasterBitReader(const IRasterSurface& surface,
        const QImage& buffer,
        QPoint bufferOffset,
        QRect area);

    const IRasterSurface& _surface;
    const QImage& _buffer;
    
    const QPoint _bufferOffset;
    const QRect _area;

    const int _pixelWidth;

    bool _final = true;

    friend class IRasterSurface;
};

class RasterBitWriter
{
public:
    RasterBitWriter(RasterBitWriter&& other);
    ~RasterBitWriter();

    inline QImage::Format format() { return _buffer.format(); }

    inline QRect area() const { return _area; }
    inline int pixelWidth() const { return _pixelWidth; }

    uchar* scanLine(int line) const;
private:
    RasterBitWriter(IRasterSurface& surface, QImage& buffer, QPoint bufferOffset, QRect area);

    IRasterSurface& _surface;
    QImage& _buffer;

    QPoint _bufferOffset;
    QRect _area;

    const int _pixelWidth;

    bool _final = true;

    friend class IRasterSurface;
};

#endif // RASTERBITHANDLES_HPP