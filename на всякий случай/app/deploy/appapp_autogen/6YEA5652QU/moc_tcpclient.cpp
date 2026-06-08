/****************************************************************************
** Meta object code from reading C++ file 'tcpclient.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../include/tcpclient.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpclient.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9TcpClientE_t {};
} // unnamed namespace

template <> constexpr inline auto TcpClient::qt_create_metaobjectdata<qt_meta_tag_ZN9TcpClientE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TcpClient",
        "errorOccurred",
        "",
        "message",
        "dataChanged",
        "responseReceived",
        "requestId",
        "success",
        "QVariantMap",
        "data",
        "ordersChanged",
        "checkUser",
        "login",
        "password",
        "getUserName",
        "getUserRole",
        "getUserId",
        "getFoods",
        "QVariantList",
        "getProducts",
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
        "foodId",
        "quantity",
        "price",
        "checkAndConsume",
        "returnIngredients",
        "getLowStockWarning",
        "getReportRevenue",
        "from",
        "to",
        "dishId",
        "waiterId",
        "getRevenueByDay",
        "getFoodPopularity",
        "getReportLowStock",
        "getDishesList",
        "getWaitersList",
        "getLocalIps",
        "pingServer",
        "host",
        "port",
        "reconnect",
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
        "syncOrderItems",
        "newItems",
        "getClosedOrders"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'dataChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'responseReceived'
        QtMocHelpers::SignalData<void(int, bool, const QVariantMap &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::Bool, 7 }, { 0x80000000 | 8, 9 },
        }}),
        // Signal 'ordersChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'checkUser'
        QtMocHelpers::MethodData<bool(const QString &, const QString &)>(11, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 12 }, { QMetaType::QString, 13 },
        }}),
        // Method 'getUserName'
        QtMocHelpers::MethodData<QString() const>(14, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'getUserRole'
        QtMocHelpers::MethodData<QString() const>(15, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'getUserId'
        QtMocHelpers::MethodData<int() const>(16, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'getFoods'
        QtMocHelpers::MethodData<QVariantList()>(17, 2, QMC::AccessPublic, 0x80000000 | 18),
        // Method 'getProducts'
        QtMocHelpers::MethodData<QVariantList()>(19, 2, QMC::AccessPublic, 0x80000000 | 18),
        // Method 'updateProductQuantity'
        QtMocHelpers::MethodData<bool(int, int)>(20, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 21 }, { QMetaType::Int, 22 },
        }}),
        // Method 'addProductQuantity'
        QtMocHelpers::MethodData<bool(int, int)>(23, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 21 }, { QMetaType::Int, 24 },
        }}),
        // Method 'createOrder'
        QtMocHelpers::MethodData<int(int, int, const QString &, double)>(25, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 26 }, { QMetaType::Int, 27 }, { QMetaType::QString, 28 }, { QMetaType::Double, 29 },
        }}),
        // Method 'addOrderItem'
        QtMocHelpers::MethodData<bool(int, int, int, double)>(30, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 31 }, { QMetaType::Int, 32 }, { QMetaType::Int, 33 }, { QMetaType::Double, 34 },
        }}),
        // Method 'checkAndConsume'
        QtMocHelpers::MethodData<bool(int, int)>(35, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 32 }, { QMetaType::Int, 33 },
        }}),
        // Method 'returnIngredients'
        QtMocHelpers::MethodData<bool(int, int)>(36, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 32 }, { QMetaType::Int, 33 },
        }}),
        // Method 'getLowStockWarning'
        QtMocHelpers::MethodData<QVariantList(int, int)>(37, 2, QMC::AccessPublic, 0x80000000 | 18, {{
            { QMetaType::Int, 32 }, { QMetaType::Int, 33 },
        }}),
        // Method 'getReportRevenue'
        QtMocHelpers::MethodData<QVariantMap(const QString &, const QString &, int, int, int)>(38, 2, QMC::AccessPublic, 0x80000000 | 8, {{
            { QMetaType::QString, 39 }, { QMetaType::QString, 40 }, { QMetaType::Int, 41 }, { QMetaType::Int, 26 },
            { QMetaType::Int, 42 },
        }}),
        // Method 'getRevenueByDay'
        QtMocHelpers::MethodData<QVariantList(const QString &, const QString &, int, int, int)>(43, 2, QMC::AccessPublic, 0x80000000 | 18, {{
            { QMetaType::QString, 39 }, { QMetaType::QString, 40 }, { QMetaType::Int, 41 }, { QMetaType::Int, 26 },
            { QMetaType::Int, 42 },
        }}),
        // Method 'getFoodPopularity'
        QtMocHelpers::MethodData<QVariantList(const QString &, const QString &)>(44, 2, QMC::AccessPublic, 0x80000000 | 18, {{
            { QMetaType::QString, 39 }, { QMetaType::QString, 40 },
        }}),
        // Method 'getReportLowStock'
        QtMocHelpers::MethodData<QVariantList()>(45, 2, QMC::AccessPublic, 0x80000000 | 18),
        // Method 'getDishesList'
        QtMocHelpers::MethodData<QVariantList()>(46, 2, QMC::AccessPublic, 0x80000000 | 18),
        // Method 'getWaitersList'
        QtMocHelpers::MethodData<QVariantList()>(47, 2, QMC::AccessPublic, 0x80000000 | 18),
        // Method 'getLocalIps'
        QtMocHelpers::MethodData<QStringList()>(48, 2, QMC::AccessPublic, QMetaType::QStringList),
        // Method 'pingServer'
        QtMocHelpers::MethodData<bool(const QString &, int)>(49, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 50 }, { QMetaType::Int, 51 },
        }}),
        // Method 'reconnect'
        QtMocHelpers::MethodData<bool(const QString &, quint16)>(52, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 50 }, { QMetaType::UShort, 51 },
        }}),
        // Method 'saveReportToCSV'
        QtMocHelpers::MethodData<bool(const QString &, const QString &, const QString &, int, int, int, const QString &)>(53, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 54 }, { QMetaType::QString, 39 }, { QMetaType::QString, 40 }, { QMetaType::Int, 41 },
            { QMetaType::Int, 26 }, { QMetaType::Int, 42 }, { QMetaType::QString, 55 },
        }}),
        // Method 'getOrders'
        QtMocHelpers::MethodData<QVariantList(const QString &)>(56, 2, QMC::AccessPublic, 0x80000000 | 18, {{
            { QMetaType::QString, 57 },
        }}),
        // Method 'getOrders'
        QtMocHelpers::MethodData<QVariantList()>(56, 2, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 18),
        // Method 'updateOrderStatus'
        QtMocHelpers::MethodData<bool(int, const QString &)>(58, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 31 }, { QMetaType::QString, 59 },
        }}),
        // Method 'getOrderItems'
        QtMocHelpers::MethodData<QVariantList(int)>(60, 2, QMC::AccessPublic, 0x80000000 | 18, {{
            { QMetaType::Int, 31 },
        }}),
        // Method 'closeOrder'
        QtMocHelpers::MethodData<bool(int, double, const QVariantList &)>(61, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 31 }, { QMetaType::Double, 62 }, { 0x80000000 | 18, 63 },
        }}),
        // Method 'getCheckByOrderId'
        QtMocHelpers::MethodData<QVariantMap(int)>(64, 2, QMC::AccessPublic, 0x80000000 | 8, {{
            { QMetaType::Int, 31 },
        }}),
        // Method 'syncOrderItems'
        QtMocHelpers::MethodData<bool(int, const QVariantList &)>(65, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 31 }, { 0x80000000 | 18, 66 },
        }}),
        // Method 'getClosedOrders'
        QtMocHelpers::MethodData<QVariantList()>(67, 2, QMC::AccessPublic, 0x80000000 | 18),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TcpClient, qt_meta_tag_ZN9TcpClientE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TcpClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9TcpClientE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9TcpClientE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9TcpClientE_t>.metaTypes,
    nullptr
} };

void TcpClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TcpClient *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->dataChanged(); break;
        case 2: _t->responseReceived((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[3]))); break;
        case 3: _t->ordersChanged(); break;
        case 4: { bool _r = _t->checkUser((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { QString _r = _t->getUserName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 6: { QString _r = _t->getUserRole();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 7: { int _r = _t->getUserId();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 8: { QVariantList _r = _t->getFoods();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 9: { QVariantList _r = _t->getProducts();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 10: { bool _r = _t->updateProductQuantity((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 11: { bool _r = _t->addProductQuantity((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->createOrder((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 13: { bool _r = _t->addOrderItem((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 14: { bool _r = _t->checkAndConsume((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 15: { bool _r = _t->returnIngredients((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 16: { QVariantList _r = _t->getLowStockWarning((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 17: { QVariantMap _r = _t->getReportRevenue((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[5])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 18: { QVariantList _r = _t->getRevenueByDay((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[5])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 19: { QVariantList _r = _t->getFoodPopularity((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 20: { QVariantList _r = _t->getReportLowStock();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 21: { QVariantList _r = _t->getDishesList();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 22: { QVariantList _r = _t->getWaitersList();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 23: { QStringList _r = _t->getLocalIps();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 24: { bool _r = _t->pingServer((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 25: { bool _r = _t->reconnect((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<quint16>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
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
        case 33: { bool _r = _t->syncOrderItems((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVariantList>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 34: { QVariantList _r = _t->getClosedOrders();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TcpClient::*)(const QString & )>(_a, &TcpClient::errorOccurred, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpClient::*)()>(_a, &TcpClient::dataChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpClient::*)(int , bool , const QVariantMap & )>(_a, &TcpClient::responseReceived, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpClient::*)()>(_a, &TcpClient::ordersChanged, 3))
            return;
    }
}

const QMetaObject *TcpClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9TcpClientE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TcpClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 35)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 35;
    }
    return _id;
}

// SIGNAL 0
void TcpClient::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void TcpClient::dataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TcpClient::responseReceived(int _t1, bool _t2, const QVariantMap & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3);
}

// SIGNAL 3
void TcpClient::ordersChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
