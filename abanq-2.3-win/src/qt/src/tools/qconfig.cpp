/* Install paths from configure */

static const char QT_INSTALL_PREFIX      [267] = "qt_nstpath=/home/falbujer/svn.infosial-dmz.lan/lite/linux/tronco/lite-instalacion";
static const char QT_INSTALL_BINS        [267] = "qt_binpath=/home/falbujer/svn.infosial-dmz.lan/lite/linux/tronco/lite-instalacion/bin";
static const char QT_INSTALL_DOCS        [267] = "qt_docpath=/home/falbujer/svn.infosial-dmz.lan/lite/linux/tronco/lite-instalacion/doc";
static const char QT_INSTALL_HEADERS     [267] = "qt_hdrpath=/home/falbujer/svn.infosial-dmz.lan/lite/linux/tronco/lite-instalacion/include";
static const char QT_INSTALL_LIBS        [267] = "qt_libpath=/home/falbujer/svn.infosial-dmz.lan/lite/linux/tronco/lite-instalacion/lib";
static const char QT_INSTALL_PLUGINS     [267] = "qt_plgpath=/home/falbujer/svn.infosial-dmz.lan/lite/linux/tronco/lite-instalacion/plugins";
static const char QT_INSTALL_DATA        [267] = "qt_datpath=/home/falbujer/svn.infosial-dmz.lan/lite/linux/tronco/lite-instalacion";
static const char QT_INSTALL_TRANSLATIONS[267] = "qt_trnpath=/home/falbujer/svn.infosial-dmz.lan/lite/linux/tronco/lite-instalacion/translations";
static const char QT_INSTALL_SYSCONF     [267] = "qt_cnfpath=/home/falbujer/svn.infosial-dmz.lan/lite/linux/tronco/lite-instalacion/etc/settings";

/* strlen( "qt_xxxpath=" ) == 11 */
const char *qInstallPath()             { return QT_INSTALL_PREFIX       + 11; }
const char *qInstallPathDocs()         { return QT_INSTALL_DOCS         + 11; }
const char *qInstallPathHeaders()      { return QT_INSTALL_HEADERS      + 11; }
const char *qInstallPathLibs()         { return QT_INSTALL_LIBS         + 11; }
const char *qInstallPathBins()         { return QT_INSTALL_BINS         + 11; }
const char *qInstallPathPlugins()      { return QT_INSTALL_PLUGINS      + 11; }
const char *qInstallPathData()         { return QT_INSTALL_DATA         + 11; }
const char *qInstallPathTranslations() { return QT_INSTALL_TRANSLATIONS + 11; }
const char *qInstallPathSysconf()      { return QT_INSTALL_SYSCONF      + 11; }
