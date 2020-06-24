/****************************************************************************
** Meta object code from reading C++ file 'VideoWall.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../videowall - Copy/VideoWall.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoWall.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoWall_t {
    QByteArrayData data[14];
    char stringdata0[117];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoWall_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoWall_t qt_meta_stringdata_VideoWall = {
    {
QT_MOC_LITERAL(0, 0, 9), // "VideoWall"
QT_MOC_LITERAL(1, 10, 4), // "stop"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 13), // "openLocalFile"
QT_MOC_LITERAL(4, 30, 7), // "openUrl"
QT_MOC_LITERAL(5, 38, 5), // "about"
QT_MOC_LITERAL(6, 44, 5), // "pause"
QT_MOC_LITERAL(7, 50, 5), // "loade"
QT_MOC_LITERAL(8, 56, 11), // "onStartPlay"
QT_MOC_LITERAL(9, 68, 4), // "seek"
QT_MOC_LITERAL(10, 73, 14), // "onCaptureSaved"
QT_MOC_LITERAL(11, 88, 4), // "path"
QT_MOC_LITERAL(12, 93, 7), // "capture"
QT_MOC_LITERAL(13, 101, 15) // "changeClockType"

    },
    "VideoWall\0stop\0\0openLocalFile\0openUrl\0"
    "about\0pause\0loade\0onStartPlay\0seek\0"
    "onCaptureSaved\0path\0capture\0changeClockType"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoWall[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a /* Public */,
       3,    0,   75,    2, 0x0a /* Public */,
       4,    0,   76,    2, 0x0a /* Public */,
       5,    0,   77,    2, 0x0a /* Public */,
       6,    0,   78,    2, 0x0a /* Public */,
       7,    0,   79,    2, 0x0a /* Public */,
       8,    0,   80,    2, 0x0a /* Public */,
       9,    0,   81,    2, 0x0a /* Public */,
       9,    1,   82,    2, 0x0a /* Public */,
      10,    1,   85,    2, 0x0a /* Public */,
      12,    0,   88,    2, 0x0a /* Public */,
      13,    0,   89,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VideoWall::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoWall *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stop(); break;
        case 1: _t->openLocalFile(); break;
        case 2: _t->openUrl(); break;
        case 3: _t->about(); break;
        case 4: _t->pause(); break;
        case 5: _t->loade(); break;
        case 6: _t->onStartPlay(); break;
        case 7: _t->seek(); break;
        case 8: _t->seek((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->onCaptureSaved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->capture(); break;
        case 11: _t->changeClockType(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoWall::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_VideoWall.data,
    qt_meta_data_VideoWall,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoWall::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoWall::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoWall.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoWall::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
