#ifndef CSERIALPORTLISTMODEL_H
#define CSERIALPORTLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QSerialPortInfo>

class CSerialPortListModel : public QAbstractListModel
{
public:
    CSerialPortListModel();
    virtual ~CSerialPortListModel();

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QSerialPortInfo getSerialPortInfo(int row) const;

private:
    QList<QSerialPortInfo> mPortsList;
};

#endif // CSERIALPORTLISTMODEL_H
