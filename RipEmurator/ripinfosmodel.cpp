/*
 * RipInfosModel.cpp
 *
 *  Created on: 2010-3-14
 *      Author: Bleastrind
 */

#include "ripinfosmodel.h"

RipInfosModel::RipInfosModel(QObject * parent)
	: QAbstractTableModel(parent)
{

}

RipInfosModel::RipInfosModel(QList< RowInfo > listofRows, QObject *parent)
	: QAbstractTableModel(parent)
	,listOfRows(listofRows){
}

int RipInfosModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return listOfRows.size();
}
int RipInfosModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return RowInfo::MEMBERCOUNT;
}
QVariant RipInfosModel::data(const QModelIndex &index, int role) const
{

	if (!index.isValid())
		return QVariant();

	if (index.row() >= listOfRows.size() || index.row() < 0)
		return QVariant();

	if (role == Qt::DisplayRole) {
		RowInfo info = listOfRows.at(index.row());

		switch(index.column())
		{
		case 0:
			return info.Target;
		case 1:
			return info.Next;
		case 2:
			return QVariant(info.Metric);
		case 3:
			return (info.Metric == 0)? QVariant("Direct"):QVariant("RIP");
		case 4:
			return (info.Metric == 0)? QVariant(0):QVariant(100);
		case 5:
			return info.Interface;
		default:
			return QVariant();
		}
	}
	return QVariant();
}
QVariant RipInfosModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal) {
		switch (section) {
		case 0:
			return tr("Target");
		case 1:
			return tr("Next");
		case 2:
			return tr("Cost");
		case 3:
			return tr("Protocol");
		case 4:
			return tr("Preference");
		case 5:
			return tr("Interface");
		default:
			return QVariant();
		}
	}
	return QVariant();
}
Qt::ItemFlags RipInfosModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool RipInfosModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
		int row = index.row();

		RowInfo info = listOfRows.value(row);

		switch (index.column()) {
		case 0:
			info.Target = value.toString();
			break;
		case 1:
			info.Next = value.toString();
			break;
		case 2:
			info.Metric = value.toInt();
			break;
		default:
			return false;
		}

		listOfRows.replace(row, info);
		emit
		(dataChanged(index, index));

		return true;
	}

	return false;
}

bool RipInfosModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; row++) {
        RowInfo info;
        listOfRows.insert(position,info);
    }

    endInsertRows();
    return true;
}

bool RipInfosModel::removeRows(int position, int rows, const QModelIndex &index)
{
	Q_UNUSED(index);
	beginRemoveRows(QModelIndex(), position, position + rows - 1);

	for (int row = 0; row < rows; ++row) {
		listOfRows.removeAt(position);
	}

	endRemoveRows();
	return true;
}

QList< RowInfo > RipInfosModel::getList()
{
	return this->listOfRows;
}



void RipInfosModel::setModel(QList<RowInfo> listofrows )
{
	this->listOfRows = listofrows;
	reset();
}


