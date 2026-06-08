/****************************************************************************
** Meta object code from reading C++ file 'dbworker.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../include/dbworker.h"
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
        "ordersChanged",
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
        "getLowStockWarning",
        "saveReportToCSV",
        "reportType",
        "fileName",
        "getOrders",
        "status",
        "updateOrderStatus",
        "newStatus",
        "getOrderItems",
        "closeOrder",
        "totalSum",
        "items",
        "getCheckByOrderId",
        "getAllOrdersForReport",
        "syncOrderItems",
        "newItems",
        "getClosedOrders",
        "updateConnectionParams",
        "getChecksByDateRange"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'foodAvailabilityChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'ordersChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'checkUser'
        QtMocHelpers::MethodData<bool(const QString &, const QString &)>(6, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 7 }, { QMetaType::QString, 8 },
        }}),
        // Method 'getUserName'
        QtMocHelpers::MethodData<QString() const>(9, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'getUserRole'
        QtMocHelpers::MethodData<QString() const>(10, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'getUserId'
        QtMocHelpers::MethodData<int() const>(11, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'checkAndConsume'
        QtMocHelpers::MethodData<bool(int, int)>(12, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 13 }, { QMetaType::Int, 14 },
        }}),
        // Method 'returnIngredients'
        QtMocHelpers::MethodData<bool(int, int)>(15, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 13 }, { QMetaType::Int, 14 },
        }}),
        // Method 'refreshAllFoodsAvailability'
        QtMocHelpers::MethodData<bool()>(16, 2, QMC::AccessPublic, QMetaType::Bool),
        // Method 'reconnect'
        QtMocHelpers::MethodData<bool(const QString &, int, const QString &, const QString &, const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 18 }, { QMetaType::Int, 19 }, { QMetaType::QString, 20 }, { QMetaType::QString, 21 },
            { QMetaType::QString, 8 },
        }}),
        // Method 'getLocalIps'
        QtMocHelpers::MethodData<QStringList()>(22, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'pingServer'
        QtMocHelpers::MethodData<bool(const QString &, int)>(23, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 18 }, { QMetaType::Int, 19 },
        }}),
        // Method 'getFoods'
        QtMocHelpers::MethodData<QVariantList()>(24, 2, QMC::AccessPublic, 0x80000000 | 25),
        // Method 'getProducts'
        QtMocHelpers::MethodData<QVariantList()>(26, 2, QMC::AccessPublic, 0x80000000 | 25),
        // Method 'getDishesList'
        QtMocHelpers::MethodData<QVariantList()>(27, 2, QMC::AccessPublic, 0x80000000 | 25),
        // Method 'getWaitersList'
        QtMocHelpers::MethodData<QVariantList()>(28, 2, QMC::AccessPublic, 0x80000000 | 25),
        // Method 'updateProductQuantity'
        QtMocHelpers::MethodData<bool(int, int)>(29, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 30 }, { QMetaType::Int, 31 },
        }}),
        // Method 'addProductQuantity'
        QtMocHelpers::MethodData<bool(int, int)>(32, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 30 }, { QMetaType::Int, 33 },
        }}),
        // Method 'createOrder'
        QtMocHelpers::MethodData<int(int, int, const QString &, double)>(34, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 35 }, { QMetaType::Int, 36 }, { QMetaType::QString, 37 }, { QMetaType::Double, 38 },
        }}),
        // Method 'addOrderItem'
        QtMocHelpers::MethodData<bool(int, int, int, double)>(39, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 40 }, { QMetaType::Int, 13 }, { QMetaType::Int, 14 }, { QMetaType::Double, 41 },
        }}),
        // Method 'getReportRevenue'
        QtMocHelpers::MethodData<QVariantMap(const QString &, const QString &, int, int, int)>(42, 2, QMC::AccessPublic, 0x80000000 | 43, {{
            { QMetaType::QString, 44 }, { QMetaType::QString, 45 }, { QMetaType::Int, 46 }, { QMetaType::Int, 35 },
            { QMetaType::Int, 47 },
        }}),
        // Method 'getRevenueByDay'
        QtMocHelpers::MethodData<QVariantList(const QString &, const QString &, int, int, int)>(48, 2, QMC::AccessPublic, 0x80000000 | 25, {{
            { QMetaType::QString, 44 }, { QMetaType::QString, 45 }, { QMetaType::Int, 46 }, { QMetaType::Int, 35 },
            { QMetaType::Int, 47 },
        }}),
        // Method 'getFoodPopularity'
        QtMocHelpers::MethodData<QVariantList(const QString &, const QString &)>(49, 2, QMC::AccessPublic, 0x80000000 | 25, {{
            { QMetaType::QString, 44 }, { QMetaType::QString, 45 },
        }}),
        // Method 'getReportLowStock'
        QtMocHelpers::MethodData<QVariantList()>(50, 2, QMC::AccessPublic, 0x80000000 | 25),
        // Method 'getLowStockWarning'
        QtMocHelpers::MethodData<QVariantList(int, int)>(51, 2, QMC::AccessPublic, 0x80000000 | 25, {{
            { QMetaType::Int, 13 }, { QMetaType::Int, 14 },
        }}),
        // Method 'saveReportToCSV'
        QtMocHelpers::MethodData<bool(const QString &, const QString &, const QString &, int, int, int, const QString &)>(52, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 53 }, { QMetaType::QString, 44 }, { QMetaType::QString, 45 }, { QMetaType::Int, 46 },
            { QMetaType::Int, 35 }, { QMetaType::Int, 47 }, { QMetaType::QString, 54 },
        }}),
        // Method 'getOrders'
        QtMocHelpers::MethodData<QVariantList(const QString &)>(55, 2, QMC::AccessPublic, 0x80000000 | 25, {{
            { QMetaType::QString, 56 },
        }}),
        // Method 'getOrders'
        QtMocHelpers::MethodData<QVariantList()>(55, 2, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 25),
        // Method 'updateOrderStatus'
        QtMocHelpers::MethodData<bool(int, const QString &)>(57, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 40 }, { QMetaType::QString, 58 },
        }}),
        // Method 'getOrderItems'
        QtMocHelpers::MethodData<QVariantList(int)>(59, 2, QMC::AccessPublic, 0x80000000 | 25, {{
            { QMetaType::Int, 40 },
        }}),
        // Method 'closeOrder'
        QtMocHelpers::MethodData<bool(int, double, const QVariantList &)>(60, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 40 }, { QMetaType::Double, 61 }, { 0x80000000 | 25, 62 },
        }}),
        // Method 'getCheckByOrderId'
        QtMocHelpers::MethodData<QVariantMap(int)>(63, 2, QMC::AccessPublic, 0x80000000 | 43, {{
            { QMetaType::Int, 40 },
        }}),
        // Method 'getAllOrdersForReport'
        QtMocHelpers::MethodData<QVariantList()>(64, 2, QMC::AccessPublic, 0x80000000 | 25),
        // Method 'syncOrderItems'
        QtMocHelpers::MethodData<bool(int, const QVariantList &)>(65, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 40 }, { 0x80000000 | 25, 66 },
        }}),
        // Method 'getClosedOrders'
        QtMocHelpers::MethodData<QVariantList()>(67, 2, QMC::AccessPublic, 0x80000000 | 25),
        // Method 'updateConnectionParams'
        QtMocHelpers::MethodData<void(const QString &, int, const QString &, const QString &, const QString &)>(68, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 18 }, { QMetaType::Int, 19 }, { QMetaType::QString, 20 }, { QMetaType::QString, 21 },
            { QMetaType::QString, 8 },
        }}),
        // Method 'getChecksByDateRange'
        QtMocHelpers::MethodData<QVariantList(const QString &, const QString &)>(69, 2, QMC::AccessPublic, 0x80000000 | 25, {{
            { QMetaType::QString, 44 }, { QMetaType::QString, 45 },
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
        case 2: _t->ordersChanged(); break;
        case 3: { bool _r = _t->checkUser((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { QString _r = _t->getUserName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 5: { QString _r = _t->getUserRole();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 6: { int _r = _t->getUserId();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->checkAndConsume((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { bool _r = _t->returnIngredients((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 9: { bool _r = _t->refreshAllFoodsAvailability();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { bool _r = _t->reconnect((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 11: { QStringList _r = _t->getLocalIps();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 12: { bool _r = _t->pingServer((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 13: { QVariantList _r = _t->getFoods();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 14: { QVariantList _r = _t->getProducts();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 15: { QVariantList _r = _t->getDishesList();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 16: { QVariantList _r = _t->getWaitersList();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 17: { bool _r = _t->updateProductQuantity((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 18: { bool _r = _t->addProductQuantity((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 19: { int _r = _t->createOrder((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 20: { bool _r = _t->addOrderItem((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 21: { QVariantMap _r = _t->getReportRevenue((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[5])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 22: { QVariantList _r = _t->getRevenueByDay((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[5])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 23: { QVariantList _r = _t->getFoodPopularity((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 24: { QVariantList _r = _t->getReportLowStock();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 25: { QVariantList _r = _t->getLowStockWarning((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 26: { bool _r = _t->saveReportToCSV((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[7])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 27: { QVariantList _r = _t->getOrders((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 28: { QVariantList _r = _t->getOrders();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 29: { bool _r = _t->updateOrderStatus((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 30: { QVariantList _r = _t->getOrderItems((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 31: { bool _r = _t->closeOrder((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QVariantList>>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 32: { QVariantMap _r = _t->getCheckByOrderId((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 33: { QVariantList _r = _t->getAllOrdersForReport();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 34: { bool _r = _t->syncOrderItems((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVariantList>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 35: { QVariantList _r = _t->getClosedOrders();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 36: _t->updateConnectionParams((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[5]))); break;
        case 37: { QVariantList _r = _t->getChecksByDateRange((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DBWorker::*)(const QString & )>(_a, &DBWorker::errorOccurred, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DBWorker::*)()>(_a, &DBWorker::foodAvailabilityChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (DBWorker::*)()>(_a, &DBWorker::ordersChanged, 2))
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
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 38)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 38;
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

// SIGNAL 2
void DBWorker::ordersChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
