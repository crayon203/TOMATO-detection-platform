/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13OnnxInferenceE_t {};
} // unnamed namespace

template <> constexpr inline auto OnnxInference::qt_create_metaobjectdata<qt_meta_tag_ZN13OnnxInferenceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "OnnxInference",
        "logMessage",
        "",
        "msg"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'logMessage'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<OnnxInference, qt_meta_tag_ZN13OnnxInferenceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject OnnxInference::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13OnnxInferenceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13OnnxInferenceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13OnnxInferenceE_t>.metaTypes,
    nullptr
} };

void OnnxInference::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<OnnxInference *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->logMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (OnnxInference::*)(const QString & )>(_a, &OnnxInference::logMessage, 0))
            return;
    }
}

const QMetaObject *OnnxInference::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OnnxInference::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13OnnxInferenceE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int OnnxInference::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void OnnxInference::logMessage(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "onOpenImage",
        "",
        "onSaveYolo",
        "onClear",
        "onAddCategory",
        "onDelCategory",
        "onCategoryChanged",
        "index",
        "onDeleteSelectedBox",
        "onUndo",
        "onRedo",
        "onZoomIn",
        "onZoomOut",
        "onZoomFit",
        "onRotateLeft",
        "onRotateRight",
        "onRotationAngleChanged",
        "degrees",
        "onResetView",
        "onPanLeft",
        "onPanRight",
        "onPanUp",
        "onPanDown",
        "onLoadDataset",
        "onPrevImage",
        "onNextImage",
        "onSaveAllLabels",
        "updateDatasetNavState",
        "autoSaveCurrentLabels",
        "AnnotWidget*",
        "w",
        "syncAnnotationTable",
        "onAnnotationTableCellChanged",
        "row",
        "col",
        "onSelectDataset",
        "onStartTrain",
        "onStopTrain",
        "onSplitSliderChanged",
        "value",
        "onSaveTrainResult",
        "loadTrainingHistory",
        "onSelectModel",
        "onStartQuant",
        "onStopQuant",
        "onQuantOutput",
        "onQuantError",
        "onQuantFinished",
        "onSelectQuantModel",
        "onOpenInferImage",
        "onRunInfer",
        "onInferEngineChanged",
        "onBatchInfer",
        "onSaveInferResult",
        "loadInferenceHistory",
        "syncInferTable",
        "runCppInference",
        "onCppInferLog",
        "msg",
        "runPythonInference",
        "model",
        "finishInference",
        "QList<InferDetection>&",
        "detections",
        "modelPath",
        "imagePath",
        "onAbout",
        "updateStatusBar",
        "updateAnnotationStatus"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'onOpenImage'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSaveYolo'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onClear'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onAddCategory'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDelCategory'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCategoryChanged'
        QtMocHelpers::SlotData<void(int)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
        // Slot 'onDeleteSelectedBox'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onUndo'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRedo'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onZoomIn'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onZoomOut'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onZoomFit'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRotateLeft'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRotateRight'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRotationAngleChanged'
        QtMocHelpers::SlotData<void(double)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 18 },
        }}),
        // Slot 'onResetView'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPanLeft'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPanRight'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPanUp'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPanDown'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLoadDataset'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPrevImage'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onNextImage'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSaveAllLabels'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateDatasetNavState'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'autoSaveCurrentLabels'
        QtMocHelpers::SlotData<void(AnnotWidget *)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 30, 31 },
        }}),
        // Slot 'syncAnnotationTable'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onAnnotationTableCellChanged'
        QtMocHelpers::SlotData<void(int, int)>(33, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 34 }, { QMetaType::Int, 35 },
        }}),
        // Slot 'onSelectDataset'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onStartTrain'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onStopTrain'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSplitSliderChanged'
        QtMocHelpers::SlotData<void(int)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 40 },
        }}),
        // Slot 'onSaveTrainResult'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'loadTrainingHistory'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSelectModel'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onStartQuant'
        QtMocHelpers::SlotData<void()>(44, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onStopQuant'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onQuantOutput'
        QtMocHelpers::SlotData<void()>(46, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onQuantError'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onQuantFinished'
        QtMocHelpers::SlotData<void()>(48, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSelectQuantModel'
        QtMocHelpers::SlotData<void()>(49, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onOpenInferImage'
        QtMocHelpers::SlotData<void()>(50, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRunInfer'
        QtMocHelpers::SlotData<void()>(51, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onInferEngineChanged'
        QtMocHelpers::SlotData<void(int)>(52, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
        // Slot 'onBatchInfer'
        QtMocHelpers::SlotData<void()>(53, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSaveInferResult'
        QtMocHelpers::SlotData<void()>(54, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'loadInferenceHistory'
        QtMocHelpers::SlotData<void()>(55, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'syncInferTable'
        QtMocHelpers::SlotData<void()>(56, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'runCppInference'
        QtMocHelpers::SlotData<void()>(57, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCppInferLog'
        QtMocHelpers::SlotData<void(const QString &)>(58, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 59 },
        }}),
        // Slot 'runPythonInference'
        QtMocHelpers::SlotData<void(const QString &, AnnotWidget *)>(60, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 61 }, { 0x80000000 | 30, 31 },
        }}),
        // Slot 'finishInference'
        QtMocHelpers::SlotData<void(QVector<InferDetection> &, const QString &, const QString &)>(62, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 63, 64 }, { QMetaType::QString, 65 }, { QMetaType::QString, 66 },
        }}),
        // Slot 'onAbout'
        QtMocHelpers::SlotData<void()>(67, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateStatusBar'
        QtMocHelpers::SlotData<void(const QString &)>(68, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 59 },
        }}),
        // Slot 'updateAnnotationStatus'
        QtMocHelpers::SlotData<void()>(69, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onOpenImage(); break;
        case 1: _t->onSaveYolo(); break;
        case 2: _t->onClear(); break;
        case 3: _t->onAddCategory(); break;
        case 4: _t->onDelCategory(); break;
        case 5: _t->onCategoryChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->onDeleteSelectedBox(); break;
        case 7: _t->onUndo(); break;
        case 8: _t->onRedo(); break;
        case 9: _t->onZoomIn(); break;
        case 10: _t->onZoomOut(); break;
        case 11: _t->onZoomFit(); break;
        case 12: _t->onRotateLeft(); break;
        case 13: _t->onRotateRight(); break;
        case 14: _t->onRotationAngleChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 15: _t->onResetView(); break;
        case 16: _t->onPanLeft(); break;
        case 17: _t->onPanRight(); break;
        case 18: _t->onPanUp(); break;
        case 19: _t->onPanDown(); break;
        case 20: _t->onLoadDataset(); break;
        case 21: _t->onPrevImage(); break;
        case 22: _t->onNextImage(); break;
        case 23: _t->onSaveAllLabels(); break;
        case 24: _t->updateDatasetNavState(); break;
        case 25: _t->autoSaveCurrentLabels((*reinterpret_cast<std::add_pointer_t<AnnotWidget*>>(_a[1]))); break;
        case 26: _t->syncAnnotationTable(); break;
        case 27: _t->onAnnotationTableCellChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 28: _t->onSelectDataset(); break;
        case 29: _t->onStartTrain(); break;
        case 30: _t->onStopTrain(); break;
        case 31: _t->onSplitSliderChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 32: _t->onSaveTrainResult(); break;
        case 33: _t->loadTrainingHistory(); break;
        case 34: _t->onSelectModel(); break;
        case 35: _t->onStartQuant(); break;
        case 36: _t->onStopQuant(); break;
        case 37: _t->onQuantOutput(); break;
        case 38: _t->onQuantError(); break;
        case 39: _t->onQuantFinished(); break;
        case 40: _t->onSelectQuantModel(); break;
        case 41: _t->onOpenInferImage(); break;
        case 42: _t->onRunInfer(); break;
        case 43: _t->onInferEngineChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 44: _t->onBatchInfer(); break;
        case 45: _t->onSaveInferResult(); break;
        case 46: _t->loadInferenceHistory(); break;
        case 47: _t->syncInferTable(); break;
        case 48: _t->runCppInference(); break;
        case 49: _t->onCppInferLog((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 50: _t->runPythonInference((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<AnnotWidget*>>(_a[2]))); break;
        case 51: _t->finishInference((*reinterpret_cast<std::add_pointer_t<QList<InferDetection>&>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 52: _t->onAbout(); break;
        case 53: _t->updateStatusBar((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 54: _t->updateAnnotationStatus(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 55)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 55;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 55)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 55;
    }
    return _id;
}
QT_WARNING_POP
