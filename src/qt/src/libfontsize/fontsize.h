
#ifndef QFONTSIZE_H
#define QFONTSIZE_H

class QFontDef;
class QPaintDevice;

class FLFontSize {

public:
  static double pointSize( const QFontDef &fd, QPaintDevice *paintdevice );
  static double pixelSize( const QFontDef &request, QPaintDevice *paintdevice );
};

#endif