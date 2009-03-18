#include "fontsize.h"

#include <windows.h>
#include <qpaintdevice.h>
#include <qmessagebox.h>
#include <qpaintdevicemetrics.h>

#include <private/qfontdata_p.h>

double FLFontSize::pixelSize( const QFontDef &request, QPaintDevice *paintdevice )
{
      int dpi = GetDeviceCaps( qt_display_dc(), LOGPIXELSY );
      double pSize;
      if ( request.pointSize != -1 ) {
          if ( paintdevice ) {
            pSize = request.pointSize * dpi / 72.;
            dpi = QPaintDeviceMetrics( paintdevice ).logicalDpiY();
            pSize = pSize * dpi / 72.;
          } else
            pSize = request.pointSize * dpi / 72.;
      } else
          pSize = request.pixelSize;
      return pSize / 10;
}


double FLFontSize::pointSize( const QFontDef &fd, QPaintDevice *paintdevice )
{
      int dpi = GetDeviceCaps( qt_display_dc(), LOGPIXELSY );
      double pSize;
      if ( paintdevice ) {
        dpi = QPaintDeviceMetrics( paintdevice ).logicalDpiY();
      }
      if ( fd.pointSize < 0 ) {
        if ( paintdevice ) {
            pSize = fd.pixelSize * 72. / ( ( float ) dpi );
            dpi = QPaintDeviceMetrics( paintdevice ).logicalDpiY();
            pSize = pSize * 72. / ( ( float ) dpi );
          } else
            pSize = fd.pixelSize * 72. / ( ( float ) dpi );
      } else
          pSize = fd.pointSize;
      return pSize / 10;
}
