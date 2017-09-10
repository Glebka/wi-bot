#include "CSerialPortListModel.h"

CSerialPortListModel::CSerialPortListModel()
    : mPortsList(QSerialPortInfo::availablePorts())
{

}

CSerialPortListModel::~CSerialPortListModel()
{

}

QVariant CSerialPortListModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if (index.isValid()) {
        switch(role) {
        case Qt::ToolTipRole:
            result = mPortsList.at(index.row()).manufacturer();
            break;
        case Qt::DisplayRole:
        case Qt::WhatsThisRole:
            result = mPortsList.at(index.row()).portName();
            break;
        default:
            break;
        }
    }
    return result;
}

int CSerialPortListModel::rowCount(const QModelIndex &parent) const
{
    return mPortsList.size();
}

Qt::ItemFlags CSerialPortListModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QSerialPortInfo CSerialPortListModel::getSerialPortInfo(int row) const
{
    return mPortsList.at(row);
}

