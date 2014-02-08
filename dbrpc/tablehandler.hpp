#pragma once

#include <qk/rpc/handler.hpp>
#include <qk/rpc/context.hpp>
#include <qk/db/table.hpp>
#include <qk/db/field.hpp>

using namespace Qk::Rpc;
using namespace Qk::Db;

namespace Qk {
namespace DbRpc {
/*
// exposes public api for data table.
// working on path /tableName/methodName

template<class TTable>
class TableHandler : public Handler
{
public:
    TableHandler(Server* pParent)
        : Handler(pParent)
    {
        mSchema = &TTable::TableSchema;
        mPath = QRegExp("/" + TTable::TableSchema.name() + "/*", Qt::CaseSensitive, QRegExp::WildcardUnix);
    }

    virtual QRegExp path() override { return mPath; }

    QKRPC virtual void get(Context* pCtx);
/*    virtual void save(Context* pCtx);
    virtual void insert(Context* pCtx);
    virtual void update(Context* pCtx);
    virtual void remove(Context* pCtx);*/
  /*  QKRPC virtual void schema(Context* pCtx);

private:
    QRegExp mPath;
    ITable* mSchema;
};

template<class TTable>
void TableHandler<TTable>::get(Context* pCtx)
{
    doAsync(pCtx, [=] (Context* pCtx) {
        pCtx->out()->startObject("list");
/*        quint64 id = pCtx->param("id", 0);
        quint64 from = pCtx->param("from", 0);
        quint64 count = pCtx->param("count", 100);
        QString order = pCtx->param("sort", "");
        QString filter = pCtx->param("filter", "").trim();
        QString dir = pCtx->param("dir", "asc").trim();
        if (count > 1000) count = 1000;
        QList<IField*> fields = getFields(pCtx);
        if (id != 0)
        {
            TTable::select(fields).where(*mSchema->primaryField() == id).one(pCtx->out());
        }
        else
        {
            TTable::select(fields).where().limit(from, count).order(order, dir == "desc" ? ES).list(pCtx->out());
        }*/
    /*});
}

template<class TTable>
void TableHandler<TTable>::schema(Context* pCtx)
{
    pCtx->out()->startObject("schema")
            .write("name", mSchema->name())
            .write("description", mSchema->description());
    pCtx->out()->startObject("fields");
    foreach (IField* fld, mSchema->fields())
    {
        pCtx->out()->startObject(fld->name())
                .write("name", fld->name())
                .write("description", fld->description())
                .write("type", QMetaType::typeName(fld->type()))
                .write("title", fld->title());
        pCtx->out()->endObject();
    }
}
*/
}
}

