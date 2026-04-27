/****************************************************************************
** Meta object code from reading C++ file 'dbworker.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../dbworker.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dbworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
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
struct qt_meta_tag_ZN8DBWorkerE_t {};
} // unnamed namespace

template <> constexpr inline auto DBWorker::qt_create_metaobjectdata<qt_meta_tag_ZN8DBWorkerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DBWorker",
        "errorOccurred",
        "",
        "message",
        "foodAvailabilityChanged",
        "checkUser",
        "login",
        "password",
        "getUserName",
        "getUserRole",
        "getUserId",
        "checkAndConsume",
        "foodId",
        "quantity",
        "returnIngredients",
        "refreshAllFoodsAvailability",
        "reconnect",
        "host",
        "port",
        "dbName",
        "user",
        "getLocalIps",
        "pingServer",
        "getFoods",
        "QVariantList",
        "getProducts",
        "getDishesList",
        "getWaitersList",
        "updateProductQuantity",
        "productId",
        "newQuantity",
        "addProductQuantity",
        "delta",
        "createOrder",
        "tableNumber",
        "userId",
        "specialRequests",
        "totalCost",
        "addOrderItem",
        "orderId",
        "price",
        "getReportRevenue",
        "QVariantMap",
        "from",
        "to",
        "dishId",
        "waiterId",
        "getRevenueByDay",
        "getFoodPopularity",
        "getReportLowStock",
        "getLowStockWarning"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'foodAvailabilityChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'checkUser'
        QtMocHelpers::MethodData<bool(const QString &, const QString &)>(5, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 6 }, { QMetaType::QString, 7 },
        }}),
        // Method 'getUserName'
        QtMocHelpers::MethodData<QString() const>(8, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'getUserRole'
        QtMocHelpers::MethodData<QString() const>(9, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'getUserId'
        QtMocHelpers::MethodData<int() const>(10, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'checkAndConsume'
        QtMocHelpers::MethodData<bool(int, int)>(11, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 12 }, { QMetaType::Int, 13 },
        }}),
        // Method 'returnIngredients'
        QtMocHelpers::MethodData<bool(int, int)>(14, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 12 }, { QMetaType::Int, 13 },
        }}),
        // Method 'refreshAllFoodsAvailability'
        QtMocHelpers::MethodData<bool()>(15, 2, QMC::AccessPublic, QMetaType::Bool),
        // Method 'reconnect'
        QtMocHelpers::MethodData<bool(const QString &, int, const QString &, const QString &, const QString &)>(16, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 17 }, { QMetaType::Int, 18 }, { QMetaType::QString, 19 }, { QMetaType::QString, 20 },
            { QMetaType::QString, 7 },
        }}),
        // Method 'getLocalIps'
        QtMocHelpers::MethodData<QStringList()>(21, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'pingServer'
        QtMocHelpers::MethodData<bool(const QString &, int)>(22, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 17 }, { QMetaType::Int, 18 },
        }}),
        // Method 'getFoods'
        QtMocHelpers::MethodData<QVariantList()>(23, 2, QMC::AccessPublic, 0x80000000 | 24),
        // Method 'getProducts'
        QtMocHelpers::MethodData<QVariantList()>(25, 2, QMC::AccessPublic, 0x80000000 | 24),
        // Method 'getDishesList'
        QtMocHelpers::MethodData<QVariantList()>(26, 2, QMC::AccessPublic, 0x80000000 | 24),
        // Method 'getWaitersList'
        QtMocHelpers::MethodData<QVariantList()>(27, 2, QMC::AccessPublic, 0x80000000 | 24),
        // Method 'updateProductQuantity'
        QtMocHelpers::MethodData<bool(int, int)>(28, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 29 }, { QMetaType::Int, 30 },
        }}),
        // Method 'addProductQuantity'
        QtMocHelpers::MethodData<bool(int, int)>(31, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 29 }, { QMetaType::Int, 32 },
        }}),
        // Method 'createOrder'
        QtMocHelpers::MethodData<int(int, int, const QString &, double)>(33, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 34 }, { QMetaType::Int, 35 }, { QMetaType::QString, 36 }, { QMetaType::Double, 37 },
        }}),
        // Method 'addOrderItem'
        QtMocHelpers::MethodData<bool(int, int, int, double)>(38, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 39 }, { QMetaType::Int, 12 }, { QMetaType::Int, 13 }, { QMetaType::Double, 40 },
        }}),
        // Method 'getReportRevenue'
        QtMocHelpers::MethodData<QVariantMap(const QString &, const QString &, int, int, int)>(41, 2, QMC::AccessPublic, 0x80000000 | 42, {{
            { QMetaType::QString, 43 }, { QMetaType::QString, 44 }, { QMetaType::Int, 45 }, { QMetaType::Int, 34 },
            { QMetaType::Int, 46 },
        }}),
        // Method 'getRevenueByDay'
        QtMocHelpers::MethodData<QVariantList(const QString &, const QString &, int, int, int)>(47, 2, QMC::AccessPublic, 0x80000000 | 24, {{
            { QMetaType::QString, 43 }, { QMetaType::QString, 44 }, { QMetaType::Int, 45 }, { QMetaType::Int, 34 },
            { QMetaType::Int, 46 },
        }}),
        // Method 'getFoodPopularity'
        QtMocHelpers::MethodData<QVariantList(const QString &, const QString &)>(48, 2, QMC::AccessPublic, 0x80000000 | 24, {{
            { QMetaType::QString, 43 }, { QMetaType::QString, 44 },
        }}),
        // Method 'getReportLowStock'
        QtMocHelpers::MethodData<QVariantList()>(49, 2, QMC::AccessPublic, 0x80000000 | 24),
        // Method 'getLowStockWarning'
        QtMocHelpers::MethodData<QVariantList(int, int)>(50, 2, QMC::AccessPublic, 0x80000000 | 24, {{
            { QMetaType::Int, 12 }, { QMetaType::Int, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DBWorker, qt_meta_tag_ZN8DBWorkerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DBWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8DBWorkerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8DBWorkerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8DBWorkerE_t>.metaTypes,
    nullptr
} };

void DBWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DBWorker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->foodAvailabilityChanged(); break;
        case 2: { bool _r = _t->checkUser((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->getUserName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { QString _r = _t->getUserRole();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 5: { int _r = _t->getUserId();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->checkAndConsume((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->returnIngredients((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { bool _r = _t->refreshAllFoodsAvailability();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 9: { bool _r = _t->reconnect((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { QStringList _r = _t->getLocalIps();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 11: { bool _r = _t->pingServer((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 12: { QVariantList _r = _t->getFoods();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 13: { QVariantList _r = _t->getProducts();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 14: { QVariantList _r = _t->getDishesList();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 15: { QVariantList _r = _t->getWaitersList();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 16: { bool _r = _t->updateProductQuantity((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 17: { bool _r = _t->addProductQuantity((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 18: { int _r = _t->createOrder((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 19: { bool _r = _t->addOrderItem((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 20: { QVariantMap _r = _t->getReportRevenue((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[5])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 21: { QVariantList _r = _t->getRevenueByDay((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[5])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 22: { QVariantList _r = _t->getFoodPopularity((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 23: { QVariantList _r = _t->getReportLowStock();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 24: { QVariantList _r = _t->getLowStockWarning((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DBWorker::*)(const QString & )>(_a, &DBWorker::errorOccurred, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DBWorker::*)()>(_a, &DBWorker::foodAvailabilityChanged, 1))
            return;
    }
}

const QMetaObject *DBWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DBWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8DBWorkerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DBWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void DBWorker::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void DBWorker::foodAvailabilityChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
