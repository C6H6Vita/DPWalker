/***************************************************************************
 *   Copyright (C) 2007 by Bernhard Beschow                                *
 *   bbeschow (.at) cs.tu-berlin.de                                        *
 *   Copyright (C) 2020 by C6H6Vita                                        *
 *   c6h6 (.at) outlook.kr                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef IMPORTSEXPORTSJOB_H
#define IMPORTSEXPORTSJOB_H

#include <QMap>
#include <QProcess>
#include <QTextStream>

class QTreeWidget;
class QTreeWidgetItem;


class ImportsJob : public QObject
{
	Q_OBJECT
public:
	ImportsJob( const QString &file, QTreeWidget *pImports, const QMap<QString, QString> *pMap );

public slots:
	void start();

signals:
	void finished();

protected slots:
	void readLineStdout();

protected:
	QString m_file;
	QTreeWidget *m_pImports;
	const QMap<QString, QString> *m_pImportsMap;
	QProcess m_proc;
	QTextStream m_stream;
};

class ExportsJob : public QObject
{
	Q_OBJECT
public:
	ExportsJob( const QString &file, QTreeWidget *pExports, QMap<QString, QString> *pMap );

public slots:
	void start();

signals:
	void finished();

protected slots:
	void readLineStdout();

protected:
	QString m_file;
	QTreeWidget *m_pExports;
	QMap<QString, QString> *m_pExportsMap;
	QProcess m_proc;
	QTextStream m_stream;
};

/**
 * @author Bernhard Beschow <bbeschow (.at) cs.tu-berlin.de>
 */
class ImportsExportsJob : public QObject
{
	Q_OBJECT

public:
	ImportsExportsJob( QTreeWidgetItem *pItem, QTreeWidget *pImports, QTreeWidget *pExports );

signals:
	void finished();

protected:
	QTreeWidgetItem *m_pItem;
	QMap<QString, QString> m_map;
	ExportsJob m_exportsJob;
	ImportsJob m_importsJob;
};

#endif
