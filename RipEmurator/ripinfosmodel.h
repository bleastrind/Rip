/*
 * RipInfoTable.h
 *
 *  Created on: 2010-3-14
 *      Author: Bleastrind
 */

#ifndef RIPINFOSMODEL_H_
#define RIPINFOSMODEL_H_
#include <QtCore/QtCore>

class RowInfo;

class RipInfosModel : public QAbstractTableModel
 {
     Q_OBJECT

 public:
     RipInfosModel(QObject *parent=0);
     RipInfosModel(QList< RowInfo > listofRows, QObject *parent=0);

     void setModel(QList<RowInfo> listofrows);

     int rowCount(const QModelIndex &parent) const;
     int columnCount(const QModelIndex &parent) const;
     QVariant data(const QModelIndex &index, int role) const;
     QVariant headerData(int section, Qt::Orientation orientation, int role) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
     bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
     bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
     QList< RowInfo > getList();



 private:
     QList< RowInfo > listOfRows;
 };

class RowInfo
{
public:
	static const int MEMBERCOUNT = 6;
public:
	QString Interface;
	QString Target;
	QString Next;
	int Metric;
};

#endif
