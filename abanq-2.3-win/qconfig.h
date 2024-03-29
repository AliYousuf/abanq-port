/* License information */
#define QT_PRODUCT_LICENSEE "Free"
#define QT_PRODUCT_LICENSE  "qt-free"

/* Build key */
//#define QT_BUILD_KEY "@BKEY@"


/* Machine byte-order */
#define Q_BIG_ENDIAN 4321
#define Q_LITTLE_ENDIAN 1234
#define Q_BYTE_ORDER Q_LITTLE_ENDIAN

/* Compile time features */
#define QT_POINTER_SIZE 4
#define QT_LARGEFILE_SUPPORT 64
//#ifndef QT_NO_IMAGEIO_GIF
//# define QT_NO_IMAGEIO_GIF
//#endif

#ifndef QT_NO_CHECK
# define QT_NO_CHECK
#endif

#ifndef QT_NO_IPV6
# define QT_NO_IPV6
#endif

#ifndef QT_QWINEXPORT
#define QT_QWINEXPORT
#endif
#ifndef QT_DLL
#define QT_DLL
#endif

/* Build key  on windows*/
