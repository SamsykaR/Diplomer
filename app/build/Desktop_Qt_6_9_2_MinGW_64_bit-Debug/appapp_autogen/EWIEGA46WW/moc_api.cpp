/****************************************************************************
** Meta object code from reading C++ file 'api.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../api.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'api.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN3ApiE_t {};
} // unnamed namespace

template <> constexpr inline auto Api::qt_create_metaobjectdata<qt_meta_tag_ZN3ApiE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Api",
        "response",
        "",
        "requestId",
        "success",
        "QVariantMap",
        "data",
        "dataRefreshRequested",
        "onResponse",
        "onDataChanged",
        "checkUser",
        "login",
        "password",
        "getFoods",
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
        "QVariantList",
        "items",
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
        "getUserName",
        "getUserRole",
        "getUserId"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'response'
        QtMocHelpers::SignalData<void(int, bool, const QVariantMap &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Bool, 4 }, { 0x80000000 | 5, 6 },
        }}),
        // Signal 'dataRefreshRequested'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onResponse'
        QtMocHelpers::SlotData<void(int, bool, const QVariantMap &)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Bool, 4 }, { 0x80000000 | 5, 6 },
        }}),
        // Slot 'onDataChanged'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'checkUser'
        QtMocHelpers::MethodData<int(const QString &, const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 11 }, { QMetaType::QString, 12 },
        }}),
        // Method 'getFoods'
        QtMocHelpers::MethodData<int()>(13, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'getProducts'
        QtMocHelpers::MethodData<int()>(14, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'updateProductQuantity'
        QtMocHelpers::MethodData<int(int, int)>(15, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 16 }, { QMetaType::Int, 17 },
        }}),
        // Method 'addProductQuantity'
        QtMocHelpers::MethodData<int(int, int)>(18, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 16 }, { QMetaType::Int, 19 },
        }}),
        // Method 'createOrder'
        QtMocHelpers::MethodData<int(int, int, const QString &, double, const QVariantList &)>(20, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 21 }, { QMetaType::Int, 22 }, { QMetaType::QString, 23 }, { QMetaType::Double, 24 },
            { 0x80000000 | 25, 26 },
        }}),
        // Method 'addOrderItem'
        QtMocHelpers::MethodData<int(int, int, int, double)>(27, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 28 }, { QMetaType::Int, 29 }, { QMetaType::Int, 30 }, { QMetaType::Double, 31 },
        }}),
        // Method 'checkAndConsume'
        QtMocHelpers::MethodData<int(int, int)>(32, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 29 }, { QMetaType::Int, 30 },
        }}),
        // Method 'returnIngredients'
        QtMocHelpers::MethodData<int(int, int)>(33, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 29 }, { QMetaType::Int, 30 },
        }}),
        // Method 'getLowStockWarning'
        QtMocHelpers::MethodData<int(int, int)>(34, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 29 }, { QMetaType::Int, 30 },
        }}),
        // Method 'getReportRevenue'
        QtMocHelpers::MethodData<int(const QString &, const QString &, int, int, int)>(35, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 36 }, { QMetaType::QString, 37 }, { QMetaType::Int, 38 }, { QMetaType::Int, 21 },
            { QMetaType::Int, 39 },
        }}),
        // Method 'getRevenueByDay'
        QtMocHelpers::MethodData<int(const QString &, const QString &, int, int, int)>(40, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 36 }, { QMetaType::QString, 37 }, { QMetaType::Int, 38 }, { QMetaType::Int, 21 },
            { QMetaType::Int, 39 },
        }}),
        // Method 'getFoodPopularity'
        QtMocHelpers::MethodData<int(const QString &, const QString &)>(41, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 36 }, { QMetaType::QString, 37 },
        }}),
        // Method 'getReportLowStock'
        QtMocHelpers::MethodData<int()>(42, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'getDishesList'
        QtMocHelpers::MethodData<int()>(43, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'getWaitersList'
        QtMocHelpers::MethodData<int()>(44, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'getUserName'
        QtMocHelpers::MethodData<QString() const>(45, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'getUserRole'
        QtMocHelpers::MethodData<QString() const>(46, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'getUserId'
        QtMocHelpers::MethodData<int() const>(47, 2, QMC::AccessPublic, QMetaType::Int),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Api, qt_meta_tag_ZN3ApiE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Api::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN3ApiE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN3ApiE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN3ApiE_t>.metaTypes,
    nullptr
} };

void Api::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Api *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->response((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[3]))); break;
        case 1: _t->dataRefreshRequested(); break;
        case 2: _t->onResponse((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[3]))); break;
        case 3: _t->onDataChanged(); break;
        case 4: { int _r = _t->checkUser((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 5: { int _r = _t->getFoods();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 6: { int _r = _t->getProducts();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 7: { int _r = _t->updateProductQuantity((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 8: { int _r = _t->addProductQuantity((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: { int _r = _t->createOrder((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QVariantList>>(_a[5])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 10: { int _r = _t->addOrderItem((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 11: { int _r = _t->checkAndConsume((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->returnIngredients((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 13: { int _r = _t->getLowStockWarning((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 14: { int _r = _t->getReportRevenue((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[5])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 15: { int _r = _t->getRevenueByDay((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[5])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 16: { int _r = _t->getFoodPopularity((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 17: { int _r = _t->getReportLowStock();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 18: { int _r = _t->getDishesList();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 19: { int _r = _t->getWaitersList();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 20: { QString _r = _t->getUserName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 21: { QString _r = _t->getUserRole();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 22: { int _r = _t->getUserId();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Api::*)(int , bool , const QVariantMap & )>(_a, &Api::response, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Api::*)()>(_a, &Api::dataRefreshRequested, 1))
            return;
    }
}

const QMetaObject *Api::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Api::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN3ApiE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Api::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void Api::response(int _t1, bool _t2, const QVariantMap & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3);
}

// SIGNAL 1
void Api::dataRefreshRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
