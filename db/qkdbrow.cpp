#include "qkdbrow.hpp"

QkDbRow::QkDbRow(const QkDbTableBase *pTable)
    : mData(new RowData(pTable, QkDbRowState::New))
{
}

QkDbRow::~QkDbRow()
{
}
